#include "seq.h"
#include "output.h"

/* AFFECT */

void
do_affect_var (Expression_t* ret, Expression_t* expr)
{
	if (!ret || !expr)
		return;
	
	if (!expression_check_compat (ret, expr))
	{
		yyerror ("incompatible type in assignment.");
		
		expression_free (ret);
		expression_free (expr);
		
		return;
	}
	
	output ("\t%s = ", ret->data.symbol->name);
	if (expr->type == EXPR_CONST)
		output ("$%i ;\n", expr->data.constant.val);
	else
		output ("%s ;\n", expr->data.symbol->name);
	
	expression_free (ret);
	expression_free (expr);
}

void
do_affect_array (Expression_t* ret, List_t *args, Expression_t* expr)
{
	List_t* tmp = args;
	Expression_t* arg;
	
	if (!ret)
	{
		if (expr)
			expression_free (expr);
		if (args)
		{
			list_foreach (args, FOREACH (expression_free), NULL);
			list_free_all (args);
		}
		return;
	}
	else if (!args)
	{
		if (ret)
			expression_free (ret);
		if (args)
		{
			list_foreach (args, FOREACH (expression_free), NULL);
			list_free_all (args);
		}
		return;
	}
	
	if (!args)
	{
		expression_free (ret);
		expression_free (expr);
		
		return;
	}
	
	if (!expression_check_array (ret) ||
	    !expression_check_int (expr))
	{
		yyerror ("incompatible type in assignment.");
		
		expression_free (ret);
		expression_free (expr);
		if (args)
		{
			list_foreach (args, FOREACH (expression_free), NULL);
			list_free_all (args);
		}
		
		return;
	}
	
	output ("\t:CALL __array_set %s", ret->data.symbol->name);
	
	if (expr->type == EXPR_CONST)
		output (" $%i", expr->data.constant.val);
	else
		output (" %s", expr->data.symbol->name);
	
	while (tmp)
	{
		if (!(arg = tmp->data))
			break;
		
		if (arg->type == EXPR_CONST)
		{
			if (!(arg->data.constant.ctype & TYPE_INT))
			{
				expression_free (arg);
				expr = NULL;
				break;
			}
			output (" $%i", arg->data.constant.val);
		}
		else
		{
			if (!(arg->data.symbol->type & TYPE_INT))
			{
				expression_free (arg);
				expr = NULL;
				break;
			}
			output (" %s", arg->data.symbol->name);
		}
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
	expression_free (ret);
	expression_free (expr);
}

void
do_affect_array_a (Expression_t* ret, Expression_t* arg, Expression_t *expr)
{
	if (!arg || !ret || !expr ||
	    !expression_check_array (ret) ||
	    !expression_check_array (arg) ||
	    !expression_check_int (expr))
		yyerror ("incompatible type in assignment.");
	
	else
	{
		output ("\t:CALL __array_set_a %s ", ret->data.symbol->name);
		if (expr->type == EXPR_CONST)
			output ("$%i %s ;\n", expr->data.constant.val, arg->data.symbol->name);
		else
			output ("%s %s ;\n", expr->data.symbol->name, arg->data.symbol->name);
	}
	
	expression_free (ret);
	expression_free (arg);
	expression_free (expr);
}
