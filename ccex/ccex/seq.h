#ifndef __SEQ_H__
#define __SEQ_H__

#include "expression.h"

/* IF/ELSE */

void do_if (Expression_t* expr, unsigned int endif);
void do_ifelse (unsigned int label);
void do_ifend (unsigned int end);

/* WHILE */

void do_while_start (unsigned int label);
void do_while (Expression_t* expr, unsigned int label);
void do_whileend (unsigned int lbl_start, unsigned int lbl_stop);

/* TRY/CATCH/FINALLY */

void do_try (unsigned int lbl_catchs);
void do_tryend (unsigned int lbl_catchs, unsigned int lbl_finally,
		Expression_t* expr);
void do_catch (const char *except, unsigned int label);
void do_catchend (unsigned int label);
void do_finally (unsigned int label);
void do_finallystart (void);
void do_finallyend (void);

/* THROW */
void do_throw (const char* except);

/* AFFECT */
void do_affect_var (Expression_t* ret, Expression_t* expr);
void do_affect_array (Expression_t* ret, List_t *args, Expression_t* expr);
void do_affect_array_a (Expression_t* ret, Expression_t* arg, Expression_t *expr);

Expression_t* do_array_get (Expression_t* expr, List_t* args);
Expression_t* do_array_get_a (Expression_t* expr, Expression_t* arg);

Expression_t* do_array_get_lenght (Expression_t* expr, Expression_t* dim);
Expression_t* do_array_get_dimension (Expression_t* expr);

/* RETURN */

void do_return (Expression_t* expr);
void do_returned (Function_t* func);

/* ALLOCATE */
Expression_t* do_allocate (List_t* params);
Expression_t* do_allocate_a (Expression_t* expr);

/* FREE */
void do_free (Expression_t* expr);

/* AND/OR */
void do_and (Expression_t* expr, unsigned int label);
Expression_t* do_andend (Expression_t* expr, Expression_t* ret, unsigned int label);
void do_or (Expression_t* expr, unsigned int label);
Expression_t* do_orend (Expression_t* expr, Expression_t* ret, unsigned int label);

/* GT/LT/GTEQ/LTEQ */
Expression_t* do_cmp (char op, int sop, Expression_t* expr1, Expression_t* expr2);

/* INPUT/PRINT */
Expression_t* do_input (void);
void do_print_expr (Expression_t* expr);
void do_print_string (unsigned int st, const char* str);

/* CALL */
void do_func_start (Function_t* func);
void do_func_end (Function_t* func);
Expression_t* do_call (Function_t* func, List_t* args);

/* OPS */
Expression_t* do_binary (char op, Expression_t* expr1, Expression_t* expr2);
Expression_t* do_dec (int sop, Expression_t* expr1, Expression_t* expr2);
Expression_t* do_unary (char op, Expression_t* expr);

#endif
