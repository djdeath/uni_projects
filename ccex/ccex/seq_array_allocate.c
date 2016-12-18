#include "seq.h"
#include "output.h"

/* ALLOCATE */

Expression_t*
do_allocate (List_t* params)
{
	List_t* tmp = params;
	Expression_t *expr, *new;
	
	if (!params)
		return NULL;
	
	if (!(new = expression_new_temporary (TYPE_ARRAY)))
		return NULL;
	
	output ("\t%s = :CALL __array_allocate", new->data.symbol->name);
	
	while (tmp)
	{
		expr = tmp->data;
		
		if (!expr)
			break;
		
		if (expr->type == EXPR_CONST)
		{
			if (!(expr->data.constant.ctype & TYPE_INT))
			{
				expression_free (expr);
				expr = NULL;
				break;
			}
			output (" $%i", expr->data.constant.val);
		}
		else
		{
			if (!(expr->data.symbol->type & TYPE_INT))
			{
				expression_free (expr);
				expr = NULL;
				break;
			}
			output (" %s", expr->data.symbol->name);
		}
		expression_free (expr);
		
		tmp = tmp->next;
	}
	
	while (tmp)
	{
		expression_free (tmp->data);
		tmp = tmp->next;
	}
	
	list_free_all (params);
	
	output (" $-1 ;\n");
	
	return new;
}

Expression_t*
do_allocate_a (Expression_t* expr)
{
	Symbol_t* s;
	
	if (!expr)
		return NULL;
	
	if (expr->type == EXPR_CONST)
	{
		yyerror ("allocate expression error.");
		expression_free (expr);
		
		return NULL;
	}
	
	if (!expression_check_array (expr))
	{
		yyerror ("wrong type expression.");
		expression_free (expr);
		
		return NULL;
	}
	
	s = expr->data.symbol;
	expression_set_temporary (expr, TYPE_ARRAY);
	
	output ("\t%s = :CALL __array_allocate_a %s ;\n", s->name);
	
	return expr;
}

/* FREE */

void
do_free (Expression_t* expr)
{
	if (!expr)
		return;
	
	if (!expression_check_array (expr))
		yyerror ("incompatible type.");
	else
		output ("\t:CALL __array_free %s ;\n", expr->data.symbol->name);
	expression_free (expr);
}
