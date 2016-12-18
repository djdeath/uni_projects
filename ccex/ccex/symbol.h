#ifndef __SYMBOL_H__
#define __SYMBOL_H__

#include "c-ex-types.h"

#define SYMBOL(arg) ((Symbol_t*)(arg))
#define FUNCTION(arg) ((Function_t*)(arg))
#define VARIABLE(arg) ((Variable_t*)(arg))
#define EXCEPTION(arg) ((Exception_t*)(arg))

typedef struct _Symbol_t Symbol_t;
typedef struct _Function_t Function_t;
typedef struct _Variable_t Variable_t;
typedef struct _Exception_t Exception_t;

typedef enum
{
	SYMBOL_EXCEPT, /* Exceptions du programmeur */
	SYMBOL_FUNC,   /* Fonctions du programmeur */
	SYMBOL_TMP,    /* Variables temporaire du compilateur */
	SYMBOL_VAR     /* Variables du programmeur */
} SymbolType_t;

#include "context.h"
#include "exception.h"

struct _Symbol_t
{
	char* name;
	
	int is_arg : 1;
	
	SymbolType_t type;
	CExType_t ctype;
	
	Context_t* context;
	
	void (* free) (Symbol_t* data);
};

struct _Variable_t
{
	Symbol_t sym;
	
	int is_arg : 1;
};

struct _Function_t
{
	Symbol_t sym;
	
	int is_set : 1;
	int nb_args;
	
	List_t* args;
};

struct _Exception_t
{
	Symbol_t sym;
	
	ExceptionNode_t* node;
};

Symbol_t* symbol_new (SymbolType_t type, CExType_t ctype, const char* name);
void symbol_free (Symbol_t* sym);

void function_add_argument (Function_t* sym, Symbol_t* symbol);
void function_set_arguments (Function_t* func, List_t* args);

ExceptionNode_t* exception_get_node (Exception_t* except);

int exception_set_parent (Exception_t* except, const char* name);

int function_check_args (Function_t* func, List_t* args);
int function_check_args_def (Function_t* func, List_t* args);

#endif
