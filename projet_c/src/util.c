#include "main.h"

#include <limits.h>

#include "util.h"

int
first_bit_up (int val)
{
	int i;
	
	for (i = (CHAR_BIT * sizeof (int) - 1) ; i >= 0 ; i--)
		if (GETBIT (val, i))
			return i;
	
	return -1;
}
