#ifndef __VARIABLE_H__
#define __VARIABLE_H__

#define VARIABLE(arg) ((Variable_t*)(arg))

typedef struct _Variable_t Variable_t;

typedef enum
{
	VARIABLE_CONST,
	VARIABLE_SYM
} VariableType_t;

struct _Variable_t
{
	VariableType_t type;
	
	union
	{
		struct
		{
			char* name;
			int addr;
		} data;
		int val;
	} data;
};

Variable_t* variable_new_constant (int val);
Variable_t* variable_new_symbol (const char* name);

void variable_free (Variable_t* var);
void variable_free_constant (Variable_t* var);

void variable_set_addr (Variable_t* var, int addr);

#endif
