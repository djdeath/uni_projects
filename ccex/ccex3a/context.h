#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "function.h"
#include "variable.h"

Function_t* context_get_func (const char* name);
Function_t* context_get_current (void);

Variable_t* context_get_var (const char* name);
Variable_t* context_add_arg (const char* name);

void context_add (Function_t* func);
void context_set (Function_t* func);

void context_init (void);
void context_end (void);

#endif

