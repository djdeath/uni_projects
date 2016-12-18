#include "seq.h"
#include "cex.tab.h"
#include "label.h"
#include "output.h"

/* IF/ELSE */

void
do_if (Expression_t* expr, unsigned int endif)
{
	if (!expr)
		return;
	
	if (expr->type == EXPR_CONST)
	{
		if (!(expr->data.constant.val))
			output ("\t:JMP .LBL%u ;\n", endif);
	}
	else
		output ("\t:CMP %s $0 ;\n\t:je .LBL%u ;\n", expr->data.symbol->name, endif);
}

void
do_ifelse (unsigned int label)
{
	output ("\t:JMP .LBL%u ;\n", label);
}

void
do_ifend (unsigned int end)
{
	output (".LBL%u ;\n", end);
}

/* WHILE */

void
do_while_start (unsigned int label)
{
	output (".LBL%u ;\n", label);
}

void
do_while (Expression_t* expr, unsigned int label)
{
	if (!expr)
		return;
	
	if (expr->type == EXPR_CONST)
	{
		if (!(expr->data.constant.val))
			output ("\t:JMP .LBL%u ;\n", label);
	}
	else
		output ("\t:CMP %s $0 ;\n\t:je .LBL%u ;\n", expr->data.symbol->name, label);
}

void
do_whileend (unsigned int lbl_start, unsigned int lbl_stop)
{
	output ("\t:JMP .LBL%u ;\n.LBL%u ;\n", lbl_start, lbl_stop);
}

/* AND */
void
do_and (Expression_t* expr, unsigned int label)
{
	int val;
	unsigned int succ;
	Symbol_t* s;
	
	if (!expr)
		return;
	
	if (expr->type == EXPR_CONST)
	{
		val = expr->data.constant.val;
		
		expression_set_temporary (expr, TYPE_INT | TYPE_ARRAY);
		
		if (!val)
			output ("\t%s = $0 ;\n\t:JMP .LBL%u ;\n",
				expr->data.symbol->name, label);
		
		return;
	}
	
	s = expr->data.symbol;
	succ = label_new ();
	
	expression_set_temporary (expr, TYPE_INT | TYPE_ARRAY);
	
	output ("\t:CMP %s $0 ;\n\t:jne .LBL%u ;\n\t%s = $0 ;\n\t:JMP .LBL%u ;\n",
		s->name, succ, expr->data.symbol->name, label);
	output (".LBL%u ;\n", succ);
}

Expression_t*
do_andend (Expression_t* expr, Expression_t* ret, unsigned int label)
{
	if (!expr)
	{
		if (ret)
			expression_free (ret);
		return NULL;
	}
	else if (!ret)
	{
		if (expr)
			expression_free (expr);
		return NULL;
	}
	
	if (expr->type == EXPR_CONST)
	{
		if (!(expr->data.constant.val))
			output ("\t%s = $%i ;\n", ret->data.symbol->name,
				expr->data.constant.val);
	}
	else
		output ("\t%s = %s ;\n", ret->data.symbol->name,
			expr->data.symbol->name, label);
	output (".LBL%u ;\n", label);
	
	expression_free (expr);
	
	return ret;
}

/* OR */
void
do_or (Expression_t* expr, unsigned int label)
{
	int val;
	unsigned int succ;
	Symbol_t* s;
	
	if (!expr)
		return;
	
	if (expr->type == EXPR_CONST)
	{
		val = expr->data.constant.val;
		
		expression_set_temporary (expr, TYPE_INT | TYPE_ARRAY);
		
		if (val)
			output ("\t%s = $1 ;\n\t:JMP .LBL%u ;\n",
				expr->data.symbol->name, label);
		
		return;
	}
	
	s = expr->data.symbol;
	succ = label_new ();
	
	expression_set_temporary (expr, TYPE_INT | TYPE_ARRAY);
	
	output ("\t:CMP %s $0 ;\n\t:je .LBL%u ;\n\t%s = %s ;\n\t:JMP .LBL%u ;\n",
		s->name, succ, expr->data.symbol->name, s->name, label);
	output (".LBL%u ;\n", succ);
	
	/*output ("\t:CMP %s $0 ;\n\t:je .LBL%u ;\n\t%s = $0 ;\n\t:JMP .LBL%u ;\n",
	  s->name, succ, expr->data.symbol->name, end);
	  output (".LBL%u ;\n\t%s = $1 ;\n.LBL%u ;\n", succ, expr->data.symbol->name, end);*/
}

Expression_t*
do_orend (Expression_t* expr, Expression_t* ret, unsigned int label)
{
	if (!expr)
	{
		if (ret)
			expression_free (ret);
		return NULL;
	}
	else if (!ret)
	{
		if (expr)
			expression_free (expr);
		return NULL;
	}
	
	if (expr->type == EXPR_CONST)
		output ("\t%s = $%i ;\n",
			ret->data.symbol->name,
			expr->data.constant.val);
	else
		output ("\t%s = %s ;\n", ret->data.symbol->name,
			expr->data.symbol->name, label);
	output (".LBL%u ;\n", label);
	
	expression_free (expr);
	
	return ret;
}

/* GT */
Expression_t *
do_cmp (char op, int sop, Expression_t* expr1, Expression_t* expr2)
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
	
	if (!expression_check_compat (expr1, expr2))
	{
		yyerror ("incompatible type in comparaison.\n");
		expression_free (expr1);
		expression_free (expr2);
		
		return NULL;
	}
	
	if ((expr1->type == EXPR_CONST) && (expr2->type == EXPR_CONST))
	{
		val = expr1->data.constant.val;
		
		if (op != '\0')
		{
			switch (op)
			{
			case '<':
				expression_set_constant (expr1, TYPE_INT, val < expr2->data.constant.val);
				break;
				
			case '>':
				expression_set_constant (expr1, TYPE_INT, val > expr2->data.constant.val);
				break;
				
			default:
				yyerror ("Unknown operator '%c'.", op);
				expression_free (expr1);
				expression_free (expr2);
				return NULL;
			}
		}
		else
		{
			switch (sop)
			{
			case KEY_LTEQ:
				expression_set_constant (expr1, TYPE_INT, val <= expr2->data.constant.val);
				break;
				
			case KEY_GTEQ:
				expression_set_constant (expr1, TYPE_INT, val >= expr2->data.constant.val);
				break;
				
			case KEY_NOTEQ:
				expression_set_constant (expr1, TYPE_INT, val != expr2->data.constant.val);
				break;
				
			case KEY_EQ:
				expression_set_constant (expr1, TYPE_INT, val == expr2->data.constant.val);
				break;
				
			default:
				yyerror ("1Unknown operator code '%i'.", sop);
				expression_free (expr1);
				expression_free (expr2);
				return NULL;
			}
		}
		expression_free (expr2);
		
		return expr1;
	}
	
	if (expr1->type == EXPR_CONST)
	{
		val = expr1->data.constant.val;
		
		expression_set_temporary (expr1, TYPE_INT | TYPE_ARRAY);
		
		output ("\t:CMP %s $%i;\n", expr2->data.symbol->name, val);
	}
	else
	{
		s = expr1->data.symbol;
		
		expression_set_temporary (expr1, TYPE_INT | TYPE_ARRAY);
		
		if (expr2->type == EXPR_CONST)
			output ("\t:CMP $%i %s ;\n", expr2->data.constant.val, s->name);
		else
			output ("\t:CMP %s %s ;\n", expr2->data.symbol->name, s->name);
	}
	
	if (op != '\0')
	{
		switch (op)
		{
		case '<':
			output ("\t%s = :setl ;\n", expr1->data.symbol->name);
			break;
			
		case '>':
			output ("\t%s = :setg ;\n", expr1->data.symbol->name);
			break;
			
		default:
			yyerror ("Unknown operator '%c'.", op);
			expression_free (expr1);
			expression_free (expr2);
			return NULL;
		}
	}
	else
	{
		switch (sop)
		{
		case KEY_LTEQ:
			output ("\t%s = :setle ;\n", expr1->data.symbol->name);
			break;
			
		case KEY_GTEQ:
			output ("\t%s = :setge ;\n", expr1->data.symbol->name);
			break;
			
		case KEY_NOTEQ:
			output ("\t%s = :setne ;\n", expr1->data.symbol->name);
			break;
				
		case KEY_EQ:
			output ("\t%s = :sete ;\n", expr1->data.symbol->name);
			break;
				
		default:
			yyerror ("2Unknown operator code '%i'.", sop);
			expression_free (expr1);
			expression_free (expr2);
			return NULL;
		}
	}
	
	expression_free (expr2);
	
	return expr1;
}
