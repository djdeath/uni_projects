#include "seq.h"
#include "output.h"

/* RETURN */

void
do_return (Expression_t* expr)
{
	Function_t* func = context_get_current_func ();
	
	if (!func)
		return;
	
	if (expr)
	{
		if (expr->type == EXPR_CONST)
		{
			if (SYMBOL (func)->ctype & expr->data.constant.ctype)
				output ("\t:RET $%i ;\n", expr->data.constant.val);
			else
				yyerror ("return from incompatible type.");
		}
		else
		{
			if (SYMBOL (func)->ctype & expr->data.symbol->ctype)
				output ("\t:RET %s ;\n", expr->data.symbol->name);
			else
				yyerror ("return from incompatible type.");
		}
	}
	else
	{
		if (SYMBOL (func)->ctype & TYPE_VOID)
			output ("\t:RET ;\n");
		else
			yyerror ("return from incompatible type.");
	}
	
	if (context_in_test ())
		context_set_returned (1);
	else
		context_set_returned (2);
	
	expression_free (expr);
}

void
do_returned (Function_t* func)
{
	Context_t* context = context_get_current ();
	
	if (func && context)
	{
		if (!context->returned)
		{
			if (SYMBOL (func)->ctype & TYPE_VOID)
				output ("\t:RET ;\n");
			else
				yyerror ("function %s needs a return instruction.", SYMBOL (func)->name);
		}
	}
}
