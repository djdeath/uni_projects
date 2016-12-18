#include <string.h>

#include "context.h"
#include "hash_table.h"

static Function_t* current_func;
static HashTable_t* hash;

Function_t*
context_get_func (const char* name)
{
	Function_t* func = NULL;
	
	while ((func = FUNCTION (hash_table_search (hash, name, func))))
	{
		if (!strcmp (func->name, name))
			return func;
	}
	
	func = function_new (name);
	hash_table_insert (hash, func->name, func);
	
	return func;
}

Function_t*
context_get_current (void)
{
	return current_func;
}

Variable_t*
context_add_arg (const char* name)
{
	Variable_t* var = function_get_var (current_func, name);
	
	if (!var)
	{
		var = variable_new_symbol (name);
		function_add_arg (current_func, var);
	}
	
	return var;
}

Variable_t*
context_get_var (const char* name)
{
	Variable_t* var = function_get_var (current_func, name);
	
	if (!var)
	{
		var = variable_new_symbol (name);
		function_add_var (current_func, var);
	}
	
	return var;
}

/**/

void
context_add (Function_t* func)
{
	if (func)
	{
		if (!context_get_func (func->name))
			hash_table_insert (hash, func->name, func);
	}
}

void
context_set (Function_t* func)
{
	if (func)
		current_func = func;
}

/**/

void
context_init (void)
{
	current_func = NULL;
	hash = hash_table_new (HASH_TABLE_CLOSE_DEFAULT_SIZE, HASH_MODE_OPEN, HASH_FUNC_GLIB);
}

void
context_end (void)
{
	if (hash)
	{
		hash_table_foreach (hash, FOREACH (function_free), NULL);
		hash_table_free (hash);
	}
}
