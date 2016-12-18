#include "seq.h"
#include "label.h"
#include "output.h"

/* TRY/CATCH/FINALLY */

void
do_try (unsigned int lbl_catchs)
{
	/* APPEL A try (); */
	output ("\t:TRY .LBL%u ;\n", lbl_catchs);
}

void
do_tryend (unsigned int lbl_catchs, unsigned int lbl_finally, Expression_t* expr)
{
	if (!expr)
		return;
	
	output ("\t:JMP .LBL%u ;\n.LBL%u ;\n\t%s = :CALL __try_end ;\n",
		lbl_finally, lbl_catchs, expr->data.symbol->name);
}

void
do_catch (const char *except, unsigned int label)
{
	ExceptionNode_t* node;
	Expression_t* expr;
	
	if (!except)
		return;
	
	if (!expr)
		printf ("sux\n");
	
	if (!(expr = context_get_current_except ()))
		return;
	
	if (!(node = exception_tree_get_node (except)))
	{
		yyerror ("exception %s undeclared.", except);
		
		return;
	}
	
	/* A & B */
	output ("\t:CMP $%i %s ;\n\t:jne .LBL%u ;\n",
		node->number, expr->data.symbol->name, label);
}

void
do_catchend (unsigned int label)
{
	output (".LBL%u ;\n", label);
}

void
do_finally (unsigned int label)
{
	output (".LBL%u ;\n", label);
}

void
do_finallystart (void)
{
	output ("\t:CALL __finally ;\n");
}

void
do_finallyend (void)
{
	int throw = label_new (), end = label_new ();
	Expression_t* expr = context_get_current_except ();
	
	if (!expr)
		return;
	
	output ("\t%s = :CALL __finally_exists ;\n", expr->data.symbol->name);
	output ("\t:CMP %s $0 ;\n\t:jne .LBL%u ;\n", expr->data.symbol->name, end);
	output ("\t%s = :CALL __finally_returns ;\n", expr->data.symbol->name);
	output ("\t:CMP %s $0 ;\n\t:je .LBL%u ;\n", expr->data.symbol->name, end);
	output ("\t%s = :CALL __finally_end ;\n\t:RET %s ;\n",
		expr->data.symbol->name, expr->data.symbol->name);
	output (".LBL%u ;\n", throw);
	output ("\t%s = :CALL __finally_end ;\n\t:CALL __throw %s ;\n",
		expr->data.symbol->name, expr->data.symbol->name);
	output (".LBL%u ;\n", end);
}

/* THROW */

void
do_throw (const char* except)
{
	ExceptionNode_t* node;
	
	if (!except)
		return;
	
	if (!(node = exception_tree_get_node (except)))
	{
		yyerror ("exception %s undeclared.", except);
		
		return;
	}
	
	output ("\t:CALL __throw $%u ;\n", node->number);
}
