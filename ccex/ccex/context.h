#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "hash_table.h"

typedef enum
{
	CONTEXT_MAIN = 0x1,
	CONTEXT_FUNC = 0x2,
	CONTEXT_FUNC_ARG = 0x4,
	CONTEXT_BLOCK = 0x8,
	CONTEXT_TRY = 0x16,
	CONTEXT_CATCH = 0x32,
	CONTEXT_FINALLY = 0x64,
	CONTEXT_TEST = 0x128,
	CONTEXT_EXCEPT = 0x256
} ContextType_t;

#define CONTEXT(arg) ((Context_t *)(arg))

typedef struct _Context_t Context_t;

#include "symbol.h"
#include "expression.h"

struct _Context_t
{
	Context_t* parent;
	ContextType_t type;
	
	int returned;
	
	union
	{
		Expression_t* expr;
		Function_t* func;
		unsigned int label;
	} data;
	
	HashTable_t* symbols;
};

void context_init (void);
void context_end (void);

/**/

void context_up (ContextType_t type);
void context_up_func (Function_t* func);
void context_up_except (Expression_t* expr);
void context_down (void);

/**/

Context_t* context_get_current (void);
Function_t* context_get_current_func (void);
Expression_t* context_get_current_except (void);

/**/

int context_in_func (void);
int context_in_test (void);

/**/

int context_returned (void);
void context_set_returned (int val);

/**/
int context_add_symbol (Symbol_t* sym);
Function_t* context_add_func (CExType_t ret_type, const char* name);
Function_t* context_add_func_defined (CExType_t ret_type, const char *name);
Variable_t* context_add_var (CExType_t ret_type, const char* name);
Variable_t* context_add_var_arg (CExType_t ret_type, const char *name);
Exception_t* context_add_except (const char* name);

/**/

Symbol_t* context_get_symbol (const char* name);
Function_t* context_get_func (const char* name);
Variable_t* context_get_var (const char* name);
Exception_t* context_get_except (const char* name);

#endif
