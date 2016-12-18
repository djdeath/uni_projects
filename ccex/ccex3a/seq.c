#include "seq.h"
#include "output.h"
#include "context.h"

void
do_funcstart (Function_t* func)
{
	if (!func)
		return;
	
	output (".globl %s\n\t.type\t%s, @function\n%s:\n", func->name, func->name, func->name);
	output ("\tpushl\t%%ebp\n\tmovl\t%%esp, %%ebp\n");
	
	if (func->stack_vars || func->stack_call)
		output ("\tsubl\t$%u, %%esp\n", func->stack_vars * 4);
}

void
do_funcend (Function_t* func)
{
	if (!func)
		return;
	
	output ("\t.size\t%s, .-%s\n", func->name, func->name);
}

void
do_call (Function_t* func, List_t* args)
{
	unsigned int cal;
	Variable_t* var;
	Function_t* curfunc = context_get_current ();
	
	if (!func || !curfunc)
		return;
	
	cal = 0U;
	while (args)
	{
		var = VARIABLE (args->data);
		
		if (var->type == VARIABLE_CONST)
			output ("\tpushl\t$%i\n", var->data.val);
		else
			output ("\tpushl\t%i(%%ebp)\n", var->data.data.addr);
		
		cal++;
		args = args->next;
	}
	
	output ("\tcall\t%s\n", func->name);
	
	if (cal)
		output ("\taddl\t$%u, %%esp\n", cal * 4);
}

void
do_sv (const char* op)
{
	if (!op)
		return;
	
	output ("\t%s\t%%cl\n", op);
	output ("\tmovzbl\t%%cl, %%ecx\n");
	output ("\tmovl\t%%ecx, %%eax\n");
}

void
do_cmp (Variable_t* var1, Variable_t* var2)
{
	if (!var1 || !var2)
		return;
	
	if (var1->type == VARIABLE_CONST)
		output ("\tmovl\t$%i, %%eax\n", var1->data.val);
	else
		output ("\tmovl\t%i(%%ebp), %%eax\n", var1->data.data.addr);
	
	if (var2->type == VARIABLE_CONST)
		output ("\tmovl\t$%i, %%edx\n", var2->data.val);
	else
	{
		/*printf ("%s %i\n", var2->data.data.name, var2->data.data.addr);*/
		output ("\tmovl\t%i(%%ebp), %%edx\n", var2->data.data.addr);
	}
	
	output ("\tcmpl\t%%eax, %%edx\n");
}

void
do_affect (Variable_t* var)
{
	if (var && var->type == VARIABLE_SYM)
		output ("\tmovl\t%%eax, %i(%%ebp)\n", var->data.data.addr);
}

void
do_add (Variable_t* var1, Variable_t* var2)
{
	if(!var1 || !var2)
		return;
	
	if(var1->type == VARIABLE_CONST)	
		output("\tmovl\t$%i, %%eax\n", var1->data.val);
	else
		output("\tmovl\t%i(%%ebp), %%eax\n", var1->data.data.addr);
	
	if(var2->type == VARIABLE_CONST)
		output("\taddl\t$%i, %%eax\n", var2->data.val);
	else
		output("\taddl\t%i(%%ebp), %%eax\n", var2->data.data.addr);
}

void
do_minus (Variable_t* var1, Variable_t* var2)
{
	if(!var1 || !var2)
		return;
	
	if(var1->type == VARIABLE_CONST)
		output("\tmovl\t$%i, %%eax\n", var1->data.val);
	else
		output("\tmovl\t%i(%%ebp), %%eax\n", var1->data.data.addr);
	
	if(var2->type == VARIABLE_CONST)
		output("\tsubl\t$%i, %%eax\n", var2->data.val);
	else
		output("\tsubl\t%i(%%ebp), %%eax\n", var2->data.data.addr);
}

void
do_mult (Variable_t* var1, Variable_t* var2)
{
	if(!var1 || !var2)
		return;
	
	if(var1->type == VARIABLE_CONST)
		output("\tmovl\t$%i, %%eax\n", var1->data.val);
	else
		output("\tmovl\t%i(%%ebp), %%eax\n", var1->data.data.addr);
	
	if(var2->type == VARIABLE_CONST)
		output("\timull\t$%i, %%eax\n", var2->data.val);
	else
		output("\timull\t%i(%%ebp), %%eax\n", var2->data.data.addr);
}

void
do_div (Variable_t* var1, Variable_t* var2)
{
	
	if(var1->type == VARIABLE_CONST)
		output("\tmovl\t$%i, %%eax\n", var1->data.val);
	else
		output("\tmovl\t%i(%%ebp), %%eax\n", var1->data.data.addr);
	
	output("\tcltd\n");
	
	if(var2->type == VARIABLE_CONST)
		output("\tidivl\t$%i\n", var2->data.val);
	else
		output("\tidivl\t%i(%%ebp)\n", var2->data.data.addr);
}

void
do_mod (Variable_t* var1, Variable_t* var2)
{
	
	if(var1->type == VARIABLE_CONST)
		output("\tmovl\t$%i, %%eax\n", var1->data.val);
	else
		output("\tmovl\t%i(%%ebp), %%eax\n", var1->data.data.addr);
	
	output("\tcltd\n");
	
	if(var2->type == VARIABLE_CONST)
		output("\tidivl\t$%i\n", var2->data.val);
	else
		output("\tidivl\t%i(%%ebp)\n", var2->data.data.addr);

	output("\tmovl\t%%edx, %%eax\n");
}

void
do_dec_left (Variable_t* var1, Variable_t* var2)
{
	if (var1->type == VARIABLE_CONST)
		output("\tmovl\t$%i, %%eax\n", var1->data.val);
	else
		output("\tmovl\t%i(%%ebp), %%eax\n", var1->data.data.addr);
	if (var2->type == VARIABLE_CONST)
		output("\tmovl\t$%i, %%ecx\n", var2->data.val);
	else
		output("\tmovl\t%i(%%ebp), %%ecx\n", var2->data.data.addr);

	output("\tsall\t%%cl, %%eax\n");
}

void
do_dec_right (Variable_t* var1, Variable_t* var2)
{
	
	if (var1->type == VARIABLE_CONST)
		output("\tmovl\t$%i, %%eax\n", var1->data.val);
	else
		output("\tmovl\t%i(%%ebp), %%eax\n", var1->data.data.addr);
	if (var2->type == VARIABLE_CONST)
		output("\tmovl\t$%i, %%ecx\n", var2->data.val);
	else
		output("\tmovl\t%i(%%ebp), %%ecx\n", var2->data.data.addr);

	output("\tsarl\t%%cl, %%eax\n");
}

void
do_unary_minus (Variable_t* var)
{
	if(var->type == VARIABLE_CONST)
		output("\tmovl\t$%i, %%eax\n", var->data.val);
	else
		output("\tmovl\t%i(%%ebp), %%eax\n", var->data.data.addr);
	
	output("\tnegl\t%%eax\n");
}

void
do_unary_tilde (Variable_t* var)
{
	
	if(var->type == VARIABLE_CONST)
		output("\tmovl\t$%i, %%eax\n", var->data.val);
	else
		output("\tmovl\t%i(%%ebp), %%eax\n", var->data.data.addr);
	
	output("\tnotl\t%%eax\n");
}

void
do_ret (Variable_t* var)
{
	if (var)
	{
		if (var->type == VARIABLE_CONST)
			output("\tmovl\t$%i, %%eax\n", var->data.val);
		else
			output("\tmovl\t%i(%%ebp), %%eax\n", var->data.data.addr);
	}

	output("\tleave\n\tret\n");
}

void
do_or (Variable_t* var1, Variable_t* var2)
{

	if(!var1 || !var2)
		return;
	
	if(var1->type == VARIABLE_CONST)
		output("\tmovl\t$%i, %%eax\n", var1->data.val);
	else
		output("\tmovl\t%i(%%ebp), %%eax\n", var1->data.data.addr);

	if(var2->type == VARIABLE_CONST)
		output("\torl\t$%i, %%eax\n", var2->data.val);
	else
		output("\torl\t%i(%%ebp), %%eax\n", var2->data.data.addr);
}

void
do_and (Variable_t* var1, Variable_t* var2)
{

	if(!var1 || !var2)
		return;
	
	if(var1->type == VARIABLE_CONST)
		output("\tmovl\t$%i, %%eax\n", var1->data.val);
	else
		output("\tmovl\t%i(%%ebp), %%eax\n", var1->data.data.addr);

	if(var2->type == VARIABLE_CONST)
		output("\tandl\t$%i, %%eax\n", var2->data.val);
	else
		output("\tandl\t%i(%%ebp), %%eax\n", var2->data.data.addr);
}

void
do_xor (Variable_t* var1, Variable_t* var2)
{

	if (!var1 || !var2)
		return;
	
	if (var1->type == VARIABLE_CONST)
		output("\tmovl\t$%i, %%eax\n", var1->data.val);
	else
		output("\tmovl\t%i(%%ebp), %%eax\n", var1->data.data.addr);

	if (var2->type == VARIABLE_CONST)
		output("\txorl\t$%i, %%eax\n", var2->data.val);
	else
		output("\txorl\t%i(%%ebp), %%eax\n", var2->data.data.addr);
}

void
do_nop (Variable_t* var)
{
	if (var)
	{
		if (var->type == VARIABLE_CONST)
			output("\tmovl\t$%i, %%eax\n", var->data.val);
		else
			output("\tmovl\t%i(%%ebp), %%eax\n", var->data.data.addr);
	}
}

void
do_print_string (Function_t* func, const char* str)
{
	if (!func || !str)
		return;
	
	output ("\tpushl\t$%s\n\tcall\t%s\n\taddl\t$4, %%esp\n", str, func->name);
}

void
do_try (const char* str)
{
	output ("\tpushl\t%%edi\n\tpushl\t%%esi\n\tpushl\t%%ebx\n\tpushl\t%%esp\n\tpushl\t%%ebp\n");
	output ("\tpushl\t$%s\n", str);
	output ("\tcall\t__try\n");
	output ("\taddl\t$6, %%ebp\n");
}
