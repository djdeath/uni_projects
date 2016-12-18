#ifndef __SEQ_H__
#define __SEQ_H__

int yyerror (const char* fmt, ...);

#include "function.h"
#include "variable.h"

void do_funcstart (Function_t* func);
void do_funcend (Function_t* func);

void do_call (Function_t* func, List_t* args);
void do_cmp (Variable_t* var1, Variable_t* var2);

void do_affect (Variable_t* var);

void do_add (Variable_t* var1, Variable_t* var2);
void do_minus (Variable_t* var1, Variable_t* var2);
void do_mult (Variable_t* var1, Variable_t* var2);
void do_div (Variable_t* var1, Variable_t* var2);
void do_mod (Variable_t* var1, Variable_t* var2);
void do_dec_left (Variable_t* var1, Variable_t* var2);
void do_dec_right (Variable_t* var1, Variable_t* var2);

void do_unary_minus (Variable_t* var);
void do_unary_tilde (Variable_t* var);

void do_ret (Variable_t* var);
void do_nop (Variable_t* var);

void do_or (Variable_t* var1, Variable_t* var2);
void do_and (Variable_t* var1, Variable_t* var2);
void do_xor (Variable_t* var1, Variable_t* var2);

void do_try (const char* str);

void do_print_string (Function_t* func, const char* str);
void do_sv (const char* op);

#endif
