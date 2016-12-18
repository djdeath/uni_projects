#include <stdlib.h>
#include <string.h>

#include "symbol.h"
#include "exception.h"
#include "expression.h"

/**/

void
symbol_free_arg (Symbol_t* sym)
{
	if (sym)
	{
		if (sym->free)
			sym->free (sym);
		
		if (sym->name)
			free (sym->name);
		free (sym);
	}
}

void
symbol_free (Symbol_t* sym)
{
	if (sym && !sym->is_arg)
	{
		if (sym->free)
			sym->free (sym);
		
		if (sym->name)
			free (sym->name);
		free (sym);
	}
}

/**/

static void
symbol_func_free (Symbol_t* func)
{
	if (FUNCTION (func)->args)
	{
		list_foreach (FUNCTION (func)->args, FOREACH (symbol_free_arg), NULL);
		list_free_all (FUNCTION (func)->args);
	}
}

/**/

Symbol_t*
symbol_new (SymbolType_t type,
	    CExType_t ctype,
	    const char* name)
{
	Symbol_t* sym;
	
	switch (type)
	{
	case SYMBOL_EXCEPT:
		sym = SYMBOL (calloc (1, sizeof (Exception_t)));
		break;
		
	case SYMBOL_VAR:
		sym = SYMBOL (calloc (1, sizeof (Variable_t)));
		break;
		
	case SYMBOL_TMP:
		sym = SYMBOL (calloc (1, sizeof (Symbol_t)));
		break;
		
	case SYMBOL_FUNC:
		sym = SYMBOL (calloc (1, sizeof (Function_t)));
		sym->free = symbol_func_free;
		break;
		
	default:
		return NULL;
	}
	
	sym->type = type;
	sym->ctype = ctype;
	if (name)
		sym->name = strdup (name);
		
	return sym;
}

/**/

void
function_add_argument (Function_t* func,
		       Symbol_t* symbol)
{
	if (!func)
		return;
	
	list_append (&func->args, (void*) symbol);
}

void
function_set_arguments (Function_t* func,
			List_t* args)
{
	List_t* tmp;
	Symbol_t* sym;
	
	if (!func)
		return;
	
	if (func->is_set)
	{
		function_check_args (func, args);
		
		return;
	}
	
	func->is_set = 1;
	
	tmp = args;
	while (tmp)
	{
		if (!(sym = tmp->data))
		{
			list_free_all (args);
			func->nb_args = 0;
			
			return;
		}
		func->nb_args++;
		
		tmp = tmp->next;
	}
	
	func->args = args;
}

ExceptionNode_t*
exception_get_node (Exception_t* except)
{
	return except->node;
}

int
exception_set_parent (Exception_t* except, const char* name)
{
	if (!except || !name)
		return 0;
	
	if ((except->node = exception_tree_extend_named (name, SYMBOL (except)->name)))
		return 1;
	
	return 0;
}

int
function_check_args (Function_t* func, List_t* args)
{
	int callnb, nb;
	List_t* dargs, * cargs;
	Expression_t* expr;
	Symbol_t* sym;
	
	if (!func)
		return 0;
	
	callnb = list_length (args);
	
	if (callnb < func->nb_args)
	{
		yyerror ("too few arguments to function %s.", SYMBOL (func)->name);
		
		return 0;
	}
	else if (callnb > func->nb_args)
	{
		yyerror ("too many arguments to function %s.", SYMBOL (func)->name);
		
		return 0;
	}
	
	dargs = func->args;
	cargs = args;
	
	nb = 1;
	while (cargs)
	{
		sym = dargs->data;
		expr = cargs->data;
		
		if (!expr)
		{
			yyerror ("error with argument %i to function %s.", nb, SYMBOL (func)->name);
			
			return 0;
		}
		
		if (expr->type == EXPR_CONST)
		{
			if (!(expr->data.constant.ctype & sym->ctype))
			{
				yyerror ("invalid type of argument %i to function %s.", nb, SYMBOL (func)->name);
				
				return 0;
			}
		}
		else
		{
			if (!(expr->data.symbol->ctype & sym->ctype))
			{
				yyerror ("invalid type of argument %i to function %s.", nb, SYMBOL (func)->name);
				
				return 0;
			}
		}
		
		nb++;
		cargs = cargs->next;
		dargs = dargs->next;
	}
	
	return 1;
}

int
function_check_args_def (Function_t* func, List_t* args)
{
	int callnb, nb;
	List_t* dargs, * cargs;
	Symbol_t* sym, *sym2;
	
	if (!func)
		return 0;
	
	if (!func->is_set)
	{
		function_set_arguments (func, args);
		
		return 1;
	}
	
	callnb = list_length (args);
	
	if (callnb < func->nb_args)
	{
		yyerror ("too few arguments to function %s.", SYMBOL (func)->name);
		
		return 0;
	}
	else if (callnb > func->nb_args)
	{
		yyerror ("too many arguments to function %s.", SYMBOL (func)->name);
		
		return 0;
	}
	
	dargs = func->args;
	cargs = args;
	
	nb = 1;
	while (cargs)
	{
		sym = dargs->data;
		sym2 = cargs->data;
		
		if (!sym2)
		{
			yyerror ("error with argument %i to function %s.", nb, SYMBOL (func)->name);
			
			return 0;
		}
		
		if (sym2->type == EXPR_CONST)
		{
			if (!(sym2->ctype & sym->ctype))
			{
				yyerror ("invalid type of argument %i to function %s.", nb, SYMBOL (func)->name);
				
				return 0;
			}
		}
		else
		{
			if (!(sym2->ctype & sym->ctype))
			{
				yyerror ("invalid type of argument %i to function %s.", nb, SYMBOL (func)->name);
				
				return 0;
			}
		}
		
		nb++;
		cargs = cargs->next;
		dargs = dargs->next;
	}
	
	list_free_all (args);
	
	return 1;
}
