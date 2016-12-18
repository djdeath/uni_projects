#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "context.h"
#include "temporary.h"

static Context_t* current;

static Context_t*
context_new (ContextType_t type, Context_t* parent)
{
	Context_t* new = CONTEXT (calloc (1, sizeof (Context_t)));
	
	if (new)
	{
		if ((new->symbols = hash_table_new (HASH_TABLE_CLOSE_DEFAULT_SIZE,
						    HASH_MODE_OPEN, HASH_FUNC_GLIB)))
		{
			new->parent = parent;
			new->type = type;
			new->returned = 0;
		}
		else
		{
			free (new);
			new = NULL;
		}
	}
	
	return new;
}

static void
context_free (Context_t* con)
{
	if (con)
	{
		if (con->symbols)
		{
			hash_table_foreach (con->symbols, FOREACH (symbol_free), NULL);
			hash_table_free (con->symbols);
		}
		free (con);
	}
}

/**/

void
context_init (void)
{
	current = context_new (CONTEXT_MAIN, NULL);
}

void
context_end (void)
{
	Context_t* tmp;
	
	while (current)
	{
		tmp = current;
		current = current->parent;
		context_free (tmp);
	}
}

/**/

void
context_up (ContextType_t type)
{
	current = context_new (type, current);
	/*printf ("upping context 0x%x\n", current);*/
}

void
context_up_func (Function_t* func)
{
	current = context_new (CONTEXT_FUNC, current);
	current->data.func = func;
}

void
context_up_except (Expression_t* expr)
{
	current = context_new (CONTEXT_EXCEPT, current);
	current->data.expr = expr;
}

void
context_down (void)
{
	Context_t* tmp;
	
	/*printf ("downing context 0x%x\n", current);*/
	tmp = current;
	current = current->parent;
	current->returned = tmp->returned;
	context_free (tmp);
}

/**/

Context_t*
context_get_current (void)
{
	return current;
}

Function_t*
context_get_current_func (void)
{
	Context_t* ctmp = current;
	
	while (ctmp)
	{
		if (ctmp->type & CONTEXT_FUNC)
			return ctmp->data.func;
		
		ctmp = ctmp->parent;
	}
	
	return NULL;
}

Expression_t*
context_get_current_except (void)
{
	Context_t* ctmp = current;
	
	while (ctmp)
	{
		if (ctmp->type & CONTEXT_EXCEPT)
			return ctmp->data.expr;
		
		ctmp = ctmp->parent;
	}
	
	return NULL;
}

int
context_in_expect (void)
{
	Context_t* ctmp = current;
	
	while (ctmp)
	{
		if (ctmp->type & CONTEXT_TRY)
			return 1;
		ctmp = ctmp->parent;
	}
	
	return 0;
}

int
context_in_func (void)
{
	Context_t* ctmp = current;
	
	while (ctmp)
	{
		if (ctmp->type & CONTEXT_FUNC)
			return 1;
		ctmp = ctmp->parent;
	}
	
	return 0;
}

int
context_in_try (void)
{
	Context_t* ctmp = current;
	
	while (ctmp)
	{
		if (ctmp->type & CONTEXT_TRY)
			return 1;
		ctmp = ctmp->parent;
	}
	
	return 0;
}

int
context_in_catch (void)
{
	Context_t* ctmp = current;
	
	while (ctmp)
	{
		if (ctmp->type & CONTEXT_CATCH)
			return 1;
		ctmp = ctmp->parent;
	}
	
	return 0;
}

int
context_in_finally (void)
{
	Context_t* ctmp = current;
	
	while (ctmp)
	{
		if (ctmp->type & CONTEXT_FINALLY)
			return 1;
		ctmp = ctmp->parent;
	}
	
	return 0;
}

int
context_in_test (void)
{
	if (current && (current->type == CONTEXT_TEST))
		return 1;
	return 0;
}

int
context_returned (void)
{
	if (current)
	{
		if (current->returned >= 2)
			return 1;
	}
	
	return 0;
}

/**/

void
context_set_returned (int val)
{
	if (current)
	{
		if (val == 1)
		{
			Context_t* con = current;
			
			while (con)
			{
				if (con->type == CONTEXT_TEST)
					break;
				con = con->parent;
			}
			con->returned++;
		}
		else
			current->returned += val;
	}
}

int
context_add_symbol (Symbol_t* sym)
{
	Symbol_t* tmp = NULL;
	
	if (!current || !sym)
		return 0;
	
	sym->context = current;
	
	/* On vérifie si le symbol n'est pas déjà présent. */
	while ((tmp = hash_table_search (current->symbols, sym->name, tmp)))
	{
		if (!strcmp (sym->name, tmp->name))
		{
			yyerror ("Redeclaration of %s.", sym->name);
			
			return 0;
		}
	}
	
	if (hash_table_insert (current->symbols, sym->name, sym))
		return 0;
	
	return 1;
}

/**/

Function_t*
context_add_func (CExType_t ret_type, const char *name)
{
	Symbol_t *s = symbol_new (SYMBOL_FUNC, ret_type, name);
	
	if (!context_add_symbol (s))
	{
		symbol_free (s);
		
		return NULL;
	}
	
	return FUNCTION (s);
}

Function_t*
context_add_func_defined (CExType_t ret_type, const char *name)
{
	Symbol_t *s = context_get_symbol (name);
	
	if (s)
	{
		if (s->type != SYMBOL_FUNC)
		{
			yyerror ("Redefinition of function %s.", name);
			
			return NULL;
		}
	}
	else
		return context_add_func (ret_type, name);
	
	return FUNCTION (s);
}

Variable_t*
context_add_var (CExType_t ret_type, const char *name)
{
	Symbol_t *s = symbol_new (SYMBOL_VAR, ret_type, name);
	
	if (!context_add_symbol (s))
	{
		symbol_free (s);
		
		return NULL;
	}
	
	return VARIABLE (s);
}

Variable_t*
context_add_var_arg (CExType_t ret_type, const char *name)
{
	Symbol_t *s = symbol_new (SYMBOL_VAR, ret_type, name);
	
	s->is_arg = 1;
	
	if (!context_add_symbol (s))
	{
		symbol_free (s);
		
		return NULL;
	}
	
	return VARIABLE (s);
}

Exception_t*
context_add_except (const char *name)
{
	Symbol_t *s = symbol_new (SYMBOL_EXCEPT, TYPE_INT, name);
	
	if (!context_add_symbol (s))
	{
		symbol_free (s);
		
		return NULL;
	}
	
	return EXCEPTION (s);
 }

int
context_add_temp (void)
{
	const char *var;
	Symbol_t *s;
	
	do
	{
		var = temporary_var_new ();
	}
	while (context_get_var (var));
	
	s = symbol_new (SYMBOL_TMP, TYPE_INT, var);
	
	if (!context_add_symbol (s))
	{
		symbol_free (s);
		
		return 0;
	}
	
	return 1;
}

/* */

Symbol_t*
context_get_symbol (const char* name)
{
	Symbol_t* sym;
	Context_t* ctmp;
	
	if (!current || !name)
		return NULL;
	
	ctmp = current;
	
	while (ctmp)
	{
		if (ctmp->symbols)
		{
			sym = NULL;
			while ((sym = hash_table_search (ctmp->symbols, name, sym)))
			{
				if (!strcmp (name, sym->name))
					return sym;
			}
		}
		ctmp = ctmp->parent;
	}
	
	return NULL;
}

Function_t*
context_get_func (const char* name)
{
	Symbol_t *s = context_get_symbol (name);
	
	if (!s)
	{
		yyerror ("Undeclared function '%s'.", name);
		
		return NULL;
	}
	
	if (s->type != SYMBOL_FUNC)
	{
		yyerror ("Invalid use of '%s'.", name);
		
		return NULL;
	}
	
	return FUNCTION (s);
}

Variable_t*
context_get_var (const char* name)
{
	Symbol_t *s = context_get_symbol (name);
	
	if (!s)
	{
		yyerror ("Undeclared variable '%s'.", name);
		
		return NULL;
	}
	
	if (s->type != SYMBOL_VAR)
	{
		yyerror ("2Invalid use of '%s'.", name);
		
		return NULL;
	}
	
	return VARIABLE (s);
}

Exception_t*
context_get_except (const char* name)
{
	Symbol_t *s = context_get_symbol (name);
	
	if (!s)
	{
		yyerror ("Undeclared exception '%s'.", name);
		
		return NULL;
	}
	
	if (s->type != SYMBOL_EXCEPT)
	{
		yyerror ("Invalid use of '%s'.", name);
		
		return NULL;
	}
	
	return EXCEPTION (s);
}
