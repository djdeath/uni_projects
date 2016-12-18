#include <stdlib.h>
#include <string.h>

#include "function.h"

Function_t*
function_new (const char* name)
{
	Function_t* func;
	
	if (name && (func = FUNCTION (calloc (1, sizeof (Function_t)))))
	{
		func->name = strdup (name);
		func->hash = hash_table_new (HASH_TABLE_CLOSE_DEFAULT_SIZE, HASH_MODE_OPEN, HASH_FUNC_GLIB);
	}
	
	return func;
}

void
function_free (Function_t* func)
{
	if (func)
	{
		if (func->name)
			free (func->name);
		if (func->hash)
		{
			hash_table_foreach (func->hash, FOREACH (variable_free), NULL);
			hash_table_free (func->hash);
		}
		free (func);
	}
}

unsigned int
function_get_stack_vars (Function_t* func)
{
	if (func)
		return func->stack_vars;
	
	return 0U;
}

void
function_inc_stack_vars (Function_t* func)
{
	if (func)
		func->stack_vars++;
}

unsigned int
function_get_stack_args (Function_t* func)
{
	if (func)
		return func->stack_args;
	
	return 0U;
}

void
function_inc_stack_args (Function_t* func)
{
	if (func)
		func->stack_args++;
}

void
function_set_stack_call (Function_t* func, unsigned int val)
{
	if (func && (val > func->stack_call))
		func->stack_call = val;
}

Variable_t*
function_get_var (Function_t* func, const char* name)
{
	Variable_t* var = NULL;
	
	while ((var = VARIABLE (hash_table_search (func->hash, name, var))))
	{
		if (!strcmp (var->data.data.name, name))
			return var;
	}
	
	return NULL;
}

void
function_add_arg (Function_t* func, Variable_t* var)
{
	if (!func || !var)
		return;
	
	variable_set_addr (var, 4 * ((int) function_get_stack_args (func)) + 8);
	function_inc_stack_args (func);
	hash_table_insert (func->hash, var->data.data.name, var);
}

void
function_add_var (Function_t* func, Variable_t* var)
{
	if (!func || !var)
		return;
	
	variable_set_addr (var, -4 * ((int) ++func->stack_vars));
	hash_table_insert (func->hash, var->data.data.name, var);
}
