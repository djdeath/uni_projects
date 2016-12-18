#include <stdlib.h>
#include <string.h>

#include "variable.h"

#define variable_new() (VARIABLE (calloc (1, sizeof (Variable_t))))

Variable_t*
variable_new_constant (int val)
{
	Variable_t* var = variable_new ();
	
	if (var)
	{
		var->type = VARIABLE_CONST;
		var->data.val = val;
	}
	
	return var;
}

Variable_t*
variable_new_symbol (const char* name)
{
	Variable_t* var = NULL;
	
	if (name && (var = variable_new ()))
	{
		var->type = VARIABLE_SYM;
		var->data.data.name = strdup (name);
	}
	
	return var;
}

void
variable_free (Variable_t* var)
{
	if (var)
	{
		if (var->type == VARIABLE_SYM)
			if (var->data.data.name)
				free (var->data.data.name);
		free (var);
	}
}

void
variable_free_constant (Variable_t* var)
{
	if (var && (var->type == VARIABLE_CONST))
		free (var);
}

void
variable_set_addr (Variable_t* var, int addr)
{
	if (var && (var->type == VARIABLE_SYM))
		var->data.data.addr = addr;
}
