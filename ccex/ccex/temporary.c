#include <stdio.h>

#include "temporary.h"

#define MAX_LENGTH (256)

static unsigned int nb_var;
static char name[MAX_LENGTH];

void
temporary_var_init (void)
{
	nb_var = 0U;
}

const char*
temporary_var_new (void)
{
	snprintf (name, MAX_LENGTH, "temp%u", nb_var++);
	
	return name;
}
