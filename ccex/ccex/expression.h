#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

#define EXPRESSION(arg) ((Expression_t*)(arg))
#define EXPRESSION_CST(arg) ((ExpressionCst_t*)(arg))

typedef struct _Expression_t Expression_t;
typedef struct _ExpressionCst_t ExpressionCst_t;

typedef enum
{
	EXPR_CONST,
	EXPR_SYMBOL
} ExpressionType_t;

#include "c-ex-types.h"
#include "symbol.h"

struct _ExpressionCst_t
{
	CExType_t ctype;
	int       val;
};

struct _Expression_t
{
	ExpressionType_t type;
	
	union
	{
		ExpressionCst_t constant;
		Symbol_t*       symbol;
	} data;
};

Expression_t* expression_new_constant (CExType_t ctype, int val);
Expression_t* expression_new_symbol (Symbol_t* sym);
Expression_t* expression_new_temporary (CExType_t ctype);

void expression_free (Expression_t* expr);

void expression_set_constant (Expression_t* expr,
			      CExType_t ctype, int val);
void expression_set_symbol (Expression_t* expr, Symbol_t* sym);
void expression_set_temporary (Expression_t* expr, CExType_t ctype);

int expression_check_int (Expression_t* expr);
int expression_check_array (Expression_t* expr);
int expression_check_compat (Expression_t* expr1, 
			     Expression_t* expr2);

#endif
