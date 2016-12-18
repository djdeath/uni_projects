#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#include "hash_table.h"
#include "variable.h"

#define FUNCTION(arg) ((Function_t*)(arg))

typedef struct _Function_t Function_t;

struct _Function_t
{
	char* name;
	
	unsigned int stack_vars;
	unsigned int stack_args;
	unsigned int stack_call;
	
	HashTable_t* hash;
	List_t* args;
};

Function_t* function_new (const char* name);
void function_free (Function_t* func);

unsigned int function_get_stack_vars (Function_t* func);
void function_inc_stack_vars (Function_t* func);

unsigned int function_get_stack_args (Function_t* func);
void function_inc_stack_args (Function_t* func);

void function_set_stack_call (Function_t* func, unsigned int val);

Variable_t* function_get_var (Function_t* func, const char* name);
void function_add_arg (Function_t* func, Variable_t* var);
void function_add_var (Function_t* func, Variable_t* var);

#endif
