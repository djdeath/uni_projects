#include <stdio.h>
#include <stdarg.h>

#include "output.h"

static FILE* file;

void
output (const char* fmt, ...)
{
	va_list list;
	
	va_start (list, fmt);
	vfprintf (file, fmt, list);
	va_end (list);
}

void
output_init (FILE* out)
{
	file = out;
}
