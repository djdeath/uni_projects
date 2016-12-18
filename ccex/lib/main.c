#include <stdlib.h>

#include "array.h"
#include "exception.h"

void sesame (Array_t args);

int
main (int argc, char* argv[])
{
	int i;
	Array_t args;
	
	args = __array_allocate (1, argc, -1);
	
	for (i = 1 ; i < argc ; i++)
		__array_set (args, atoi (argv[i]), i);
	
	__exception_init ();
	
	sesame (args);
	__array_free (args);
			     
	return 0;
}
