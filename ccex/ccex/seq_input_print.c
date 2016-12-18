#include "seq.h"
#include "output.h"

/* INPUT/PRINT */

Expression_t*
do_input (void)
{
	Expression_t* expr = expression_new_temporary (TYPE_INT);
	
	if (expr)
		output ("\t%s = :CALL __input ;\n", expr->data.symbol->name);
	
	return expr;
}

void
do_print_expr (Expression_t* expr)
{
	if (!expr)
		return;
	
	if (expr->type == EXPR_CONST)
	{
		if (expr->data.constant.ctype & TYPE_INT)
			output ("\t:CALL __print_expr $%i ;\n", expr->data.constant.val);
		else
			output ("\t:CALL __print_array $%i ;\n", expr->data.constant.val);
	}
	else
	{
		if (expr->data.symbol->ctype & TYPE_INT)
			output ("\t:CALL __print_expr %s ;\n", expr->data.symbol->name);
		else
			output ("\t:CALL __print_array %s ;\n", expr->data.symbol->name);
	}
	
	expression_free (expr);
}

void
do_print_string (unsigned int st, const char* str)
{
	if (!str)
		return;
	
	output (":STRING .LC%u %s ;\n", st, str);
	output ("\t:CALL __print_string .LC%u ;\n", st);
}
