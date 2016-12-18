#include "label.h"
#include "output.h"
#include "seq.h"
#include "cex.tab.h"

/* BINARY OPERATORS */

static Expression_t*
do_div_mod (char op,
	    Expression_t* expr1,
	    Expression_t* expr2)
{
	int val;
	unsigned int lbl = 0;
	Symbol_t *s;
	
	if (expr2->type == EXPR_CONST)
	{
		if (!(expr2->data.constant.val))
		{
			yyerror ("Division per 0 not permitted.");
			expression_free (expr1);
			expression_free (expr2);
			
			return NULL;
		}
	}
	else
	{
		ExceptionNode_t* node = exception_tree_get_node ("DivisionByZeroException");
		unsigned int except = 0U;
		
		if (node)
			except = node->number;
		
		lbl = label_new ();
		
		output ("\t:CMP %s $0 ;\n", expr2->data.symbol->name);
		output ("\t:jne .LBL%u ;\n\t:CALL __throw $%u ;\n.LBL%u ;\n", lbl, except, lbl);
	}
	
	if ((expr1->type == EXPR_CONST))
	{
		val = expr1->data.constant.val;
		s = expr2->data.symbol;
		
		if (!val)
			expression_set_constant (expr1, TYPE_INT, 0);
		else
		{
			expression_set_temporary (expr1, TYPE_INT);
			
			output ("\t%s = $%i %c %s ;\n", expr1->data.symbol->name, val, op, s->name);
		}
	}
	else
	{
		s = expr1->data.symbol;
		
		expression_set_temporary (expr1, TYPE_INT);
		
		
		output ("\t%s = %s %c ", expr1->data.symbol->name, s->name, op);
		
		if (expr2->type == EXPR_CONST)
			output ("$%i ;\n", expr2->data.constant.val);
		else
			output ("%s ;\n", expr2->data.symbol->name);
	}
	
	expression_free (expr2);
	
	return expr1;
}

Expression_t*
do_binary (char op,
	   Expression_t* expr1,
	   Expression_t* expr2)
{
	int val;
	Symbol_t* s;
	
	if (!expr1)
	{
		if (expr2)
			expression_free (expr2);
		return NULL;
	}
	else if (!expr2)
	{
		if (expr1)
			expression_free (expr1);
		return NULL;
	}
	
	if (!expression_check_int (expr1) ||
	    !expression_check_int (expr2))
	{
		yyerror ("Invalid type in expression.");
		
		expression_free (expr1);
		expression_free (expr2);
		
		return NULL;
	}
	
	if ((expr1->type == EXPR_CONST) && (expr2->type == EXPR_CONST))
	{
		switch (op)
		{
		case '+':
			expression_set_constant (expr1, TYPE_INT, expr1->data.constant.val + expr2->data.constant.val);
			break;
			
		case '-':
			expression_set_constant (expr1, TYPE_INT, expr1->data.constant.val - expr2->data.constant.val);
			break;
			
		case '/':
			if (expr2->data.constant.val == 0)
			{
				yyerror ("Division per 0 not permitted.");
				expression_free (expr1);
				expr1 = NULL;
			}
			else
				expression_set_constant (expr1, TYPE_INT, expr1->data.constant.val / expr2->data.constant.val);
			break;
			
		case '*':
			expression_set_constant (expr1, TYPE_INT, expr1->data.constant.val * expr2->data.constant.val);
			break;
			
		case '%':
			if (expr2->data.constant.val == 0)
			{
				yyerror ("Division per 0 not permitted.");
				expression_free (expr1);
				expr1 = NULL;
			}
			else
				expression_set_constant (expr1, TYPE_INT, expr1->data.constant.val % expr2->data.constant.val);
			break;
			
		case '&':
			expression_set_constant (expr1, TYPE_INT, expr1->data.constant.val & expr2->data.constant.val);
			break;
			
		case '^':
			expression_set_constant (expr1, TYPE_INT, expr1->data.constant.val ^ expr2->data.constant.val);
			break;
				
		case '|':
			expression_set_constant (expr1, TYPE_INT, expr1->data.constant.val | expr2->data.constant.val);
			break;
			
		default:
			yyerror ("Unknown operator '%c'.", op);
			expression_free (expr1);
			expr1 = NULL;
		}
		
		expression_free (expr2);
		
		return expr1;
	}
	
	if ((op == '/') || (op == '%'))
		return do_div_mod (op, expr1, expr2);
	
	if (expr1->type == EXPR_CONST)
	{
		val = expr1->data.constant.val;
		s = expr2->data.symbol;
		
		expression_set_temporary (expr1, TYPE_INT);
		
		output ("\t%s = $%i %c %s ;\n", expr1->data.symbol->name, val, op, s->name);
	}
	else
	{
		s = expr1->data.symbol;
		
		expression_set_temporary (expr1, TYPE_INT);
		
		output ("\t%s = %s %c ", expr1->data.symbol->name, s->name, op);
		
		if (expr2->type == EXPR_CONST)
			output ("$%i ;\n", expr2->data.constant.val);
		else
			output ("%s ;\n", expr2->data.symbol->name);
	}
	
	expression_free (expr2);
	
	return expr1;
}

Expression_t*
do_dec (int sop,
	Expression_t* expr1,
	Expression_t* expr2)
{
	char op;
	int val;
	Symbol_t* s;
	
	if (!expr1)
	{
		if (expr2)
			expression_free (expr2);
		return NULL;
	}
	else if (!expr2)
	{
		if (expr1)
			expression_free (expr1);
		return NULL;
	}
	
	if (!expression_check_int (expr1) ||
	    !expression_check_int (expr2))
	{
		yyerror ("Invalid type in expression.");
		
		expression_free (expr1);
		expression_free (expr2);
		
		return NULL;
	}
	
	if (sop == KEY_DEC_LEFT)
		op = '<' ;
	else
		op = '>' ;
	
	if ((expr1->type == EXPR_CONST) && (expr2->type == EXPR_CONST))
	{
		switch (op)
		{
		case '<':
			expression_set_constant (expr1, TYPE_INT,
						 expr1->data.constant.val << expr2->data.constant.val);
			break;
			
		case '>':
			expression_set_constant (expr1, TYPE_INT,
						 expr1->data.constant.val >> expr2->data.constant.val);
			break;
			
		default:
			yyerror ("Unknown operator code '%i'.", sop);
			expression_free (expr1);
			expression_free (expr2);
			return NULL;
		}
		
		expression_free (expr2);
		
		return expr1;
	}
	
	if (expr1->type == EXPR_CONST)
	{
		val = expr1->data.constant.val;
		s = expr2->data.symbol;
		
		expression_set_temporary (expr1, TYPE_INT);
		
		
		output ("\t%s = $%i %c %s ;\n", expr1->data.symbol->name, val, op, s->name);
	}
	else
	{
		s = expr1->data.symbol;
		
		expression_set_temporary (expr1, TYPE_INT);
		
		output ("\t%s = %s %c ", expr1->data.symbol->name, s->name, op);
		
		if (expr2->type == EXPR_CONST)
			output ("$%i ;\n", expr2->data.constant.val);
		else
			output ("%s ;\n", expr2->data.symbol->name);
	}
	
	expression_free (expr2);
	
	return expr1;
}

/* UNARY OPERATOR */

Expression_t*
do_unary (char op,
	  Expression_t* expr)
{
	Symbol_t* s;
	
	if (!expr)
		return NULL;
	
	if (!expression_check_int (expr))
	{
		yyerror ("Require expression of type int.");
		expression_free (expr);
		
		return NULL;
	}
	
	if (expr->type == EXPR_CONST)
	{
		switch (op)
		{
		case '-':
			expression_set_constant (expr, TYPE_INT, -(expr->data.constant.val));
			break;
			
		case '~':
			expression_set_constant (expr, TYPE_INT, ~(expr->data.constant.val));
			break;
			
		default:
			yyerror ("Unknown operator '%c'.", op);
			expression_free (expr);
			return NULL;
		}
		
		return expr;
	}
	
	s = expr->data.symbol;
	
	expression_set_temporary (expr, TYPE_INT);
	
	output ("\t%s = %c %s ;\n", expr->data.symbol->name, op, s->name);
	
	return expr;
}
