#include "seq.h"

#include "output.h"

/* ARRAY GET */

Expression_t*
do_array_get (Expression_t* expr, List_t* args)
{
	Symbol_t* s;
	List_t* tmp = args;
	Expression_t* arg = NULL;
	
	if (!expr || !expression_check_array (expr))
	{
		if (expr)
			expression_free (expr);
		if (args)
		{
			list_foreach (args, FOREACH (expression_free), NULL);
			list_free_all (args);
		}
		
		return NULL;
	}
	
	s = expr->data.symbol;
	expression_set_temporary (expr, TYPE_INT);
	
	output ("\t%s = :CALL __array_get %s", expr->data.symbol->name, s->name);
	
	while (tmp)
	{
		if (!(arg = tmp->data))
			break;
		
		if (!expression_check_int (arg))
		{
			expression_free (expr);
			expr = NULL;
			break;
		}
		
		if (arg->type == EXPR_CONST)
			output (" $%i", arg->data.constant.val);
		else
			output (" %s", arg->data.symbol->name);
		expression_free (arg);
		
		tmp = tmp->next;
	}
	output (" $-1 ;\n");
	
	while (tmp)
	{
		expression_free (tmp->data);
		tmp = tmp->next;
	}
	list_free_all (args);
	
	return expr;
}

Expression_t*
do_array_get_a (Expression_t* expr, Expression_t* arg)
{
	Symbol_t* s;
	
	if (!expr || !arg
	    || !expression_check_array (expr)
	    || !expression_check_array (arg))
	{
		if (expr)
			expression_free (expr);
		if (arg)
			expression_free (arg);
		
		return NULL;
	}
	
	s = expr->data.symbol;
	expression_set_temporary (expr, TYPE_INT);
	
	output ("\t%s = :CALL __array_get_a %s %s ;\n", expr->data.symbol->name, s->name, arg->data.symbol->name);
	
	expression_free (arg);
	
	return expr;
}

Expression_t*
do_array_get_lenght (Expression_t* expr, Expression_t* dim)
{
	Symbol_t* s;
	
	if (!expr)
	{
		if (dim)
			expression_free (dim);
		return NULL;
	}
	else if (!dim)
	{
		if (!expr)
			expression_free (dim);
		return NULL;
	}
	
	s = expr->data.symbol;
	
	expression_set_temporary (expr, TYPE_INT);
	
	output ("\t%s = :CALL __array_length %s", expr->data.symbol->name, s->name);
	
	if (dim->type == EXPR_CONST)
		output (" $%i ;\n", dim->data.constant.val);
	else
		output (" %s ;\n", dim->data.symbol->name);
	
	expression_free (dim);
	
	return expr;
}

Expression_t*
do_array_get_dimension (Expression_t* expr)
{
	Symbol_t* s;
	
	if (!expr)
		return NULL;
	
	s = expr->data.symbol;
	
	expression_set_temporary (expr, TYPE_INT);
	
	output ("\t%s = :CALL __array_dimension %s ;\n", expr->data.symbol->name, s->name);
	
	return expr;
}
