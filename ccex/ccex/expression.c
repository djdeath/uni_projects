#include "output.h"
#include "label.h"
#include "temporary.h"
#include "expression.h"

#define expression_new() ((Expression_t*)calloc (1, sizeof (Expression_t)))

void
expression_free (Expression_t* expr)
{
	if (expr)
		free (expr);
}

/**/

Expression_t*
expression_new_constant (CExType_t ctype, int val)
{
	Expression_t* expr = expression_new ();
	
	if (expr)
	{
		expr->type = EXPR_CONST;
		expr->data.constant.ctype = ctype;
		expr->data.constant.val = val;
	}
	
	return expr;
}

Expression_t*
expression_new_symbol (Symbol_t* sym)
{
	Expression_t* expr = NULL;
	
	if (sym && (expr = expression_new ()))
	{
		expr->type = EXPR_SYMBOL;
		expr->data.symbol = sym;
	}
	
	return expr;
}

Expression_t*
expression_new_temporary (CExType_t ctype)
{
	Expression_t* expr = expression_new ();
	
	if (expr)
	{
		const char *tmpname;
		
		do
		{
			tmpname = temporary_var_new ();
			
		} while (context_get_symbol (tmpname));
		
		context_add_var (ctype, tmpname);
		
		expr->type = EXPR_SYMBOL;
		expr->data.symbol = SYMBOL (context_get_var (tmpname));
	}
	
	return expr;
}

/**/

void
expression_set_constant (Expression_t* expr,
			 CExType_t ctype, int val)
{
	if (expr)
	{
		expr->type = EXPR_CONST;
		expr->data.constant.ctype = ctype;
		expr->data.constant.val = val;
	}
}

void
expression_set_symbol (Expression_t* expr,
		       Symbol_t* sym)
{
	if (expr)
	{
		expr->type = EXPR_SYMBOL;
		expr->data.symbol = sym;
	}
}

void
expression_set_temporary (Expression_t* expr, CExType_t ctype)
{
	if (expr)
	{
		const char *tmpname;
		
		do
		{
			tmpname = temporary_var_new ();
			
		} while (context_get_symbol (tmpname));
		
		context_add_var (ctype, tmpname);
		
		expr->type = EXPR_SYMBOL;
		expr->data.symbol = SYMBOL (context_get_var (tmpname));
	}
}

/* TYPE CHECKING */

int
expression_check_int (Expression_t* expr)
{
	if (expr->type == EXPR_SYMBOL)
		if (!(expr->data.symbol->ctype & TYPE_INT))
			return 0;
	
	return 1;
}

int
expression_check_array (Expression_t* expr)
{
	if (expr->type == EXPR_SYMBOL)
		if (!(expr->data.symbol->ctype & TYPE_ARRAY))
			return 0;
	
	return 1;
}

int
expression_check_compat (Expression_t* expr1,
			 Expression_t* expr2)
{
	CExType_t t1, t2;
	
	if (expr1->type == EXPR_CONST)
		t1 = expr1->data.constant.ctype;
	else
		t1 = expr1->data.symbol->ctype;
	
	if (expr2->type == EXPR_CONST)
		t2 = expr2->data.constant.ctype;
	else
		t2 = expr2->data.symbol->ctype;
	
	return t1 & t2;
}
