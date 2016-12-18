#include "seq.h"
#include "output.h"

/* CALL */
Expression_t*
do_call (Function_t* func, List_t* args)
{
	List_t* tmp = args;
	Expression_t* expr, * ex;
	
	if (!func)
	{
		list_foreach (args, FOREACH (expression_free), NULL);
		list_free_all (args);
		
		return NULL;
	}
	
	if (!function_check_args (func, args))
	{
		list_foreach (args, FOREACH (expression_free), NULL);
		list_free_all (args);
		
		return NULL;
	}
	
	if (!(expr = expression_new_temporary (SYMBOL (func)->ctype)))
		return NULL;
	
	output ("\t%s = :CALL %s", expr->data.symbol->name, SYMBOL (func)->name);
	
	while (tmp)
	{
		ex = tmp->data;
		
		if (ex->type == EXPR_CONST)
			output (" $%i", ex->data.constant.val);
		else
			output (" %s", ex->data.symbol->name);
		expression_free (ex);
		
		tmp = tmp->next;
	}
	
	output (" ;\n");
	
	list_free_all (args);
	
	return expr;
}

void
do_func_start (Function_t* func)
{
	List_t* args;
	Symbol_t* sym;
	
	if (!func)
		return;
	
	output (":FUNC_START %s", SYMBOL (func)->name);
	
	args = func->args;
	while (args)
	{
		if ((sym = args->data))
			output (" %s", sym->name);
		else
			return;
		args = args->next;
	}
	
	output (" ;\n");
}

void
do_func_end (Function_t* func)
{
	if (!func)
		return;
	
	output (":FUNC_END %s ;\n", SYMBOL (func)->name);
}
