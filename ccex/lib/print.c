#include <stdio.h>

void
__print_expr (int expr)
{
	printf ("%i\n", expr);
}

void
__print_string (const char* str)
{
	if (str)
		printf ("%s", str);
}

void
__print_array (const int* array)
{
	int i ;
	int dimension = *array;
	unsigned int len = 1;
	
        for (i = 1 ; i <= dimension ; i++)
                len *= array[i];

        for (i = 1 + dimension ; i < (1 + len + dimension) ; i++)
                printf ("%i ", array[i]);
        printf ("\n");
}

int
__input (void)
{
	int s;
	
	scanf ("%i", &s);
	
	return s;
}
