#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "array.h"
#include "exception.h"

#include "defs_exceptions.h"

Array_t
__array_allocate (int dim1, ...)
{
	int nb_dim, tmp;
	Array_t ret;
	size_t len;
	va_list list;
	
	if (dim1 <= 0)
		__throw (BadDimensionException);
	
	nb_dim = 1;
	len = dim1;
	va_start (list, dim1);
	
	while ((tmp = va_arg (list, int)) != -1)
	{
		if (tmp <= 0)
		{
			va_end (list);
			__throw (BadDimensionException);
		}
		len *= tmp;
		nb_dim++;
	}
	
	va_end (list);
	
	if (!(ret = malloc (sizeof (int) * (nb_dim + 1 + len))))
		__throw (OutOfMemoryException);
	
	*ret = nb_dim;
	memcpy (ret + 1, &dim1, nb_dim * sizeof (int));
	memset (ret + 1 + nb_dim, 0, len * sizeof (int));
	
	return (Array_t) ret;
}

Array_t
__array_allocate_a (Array_t index)
{
	int i, nb_dim;
	size_t len;
	Array_t ret;
	
	if (!index)
		__throw (NullArrayException);
	
	/* index n'est pas un tableau unidimensionnel. */
	if (*index > 1)
		__throw (BadDimensionException);
	
	nb_dim = *index;
	len = 1;
	for (i = 0 , index++ ; i < nb_dim ; i++, index++)
	{
		if (*index < 0)
			__throw (BadDimensionException);
		len *= *index;
	}
	
	if (!(ret = malloc (sizeof (int) * (nb_dim + 1 + len))))
		__throw (OutOfMemoryException);
	
	*ret = nb_dim;
	memcpy (ret + 1, &nb_dim, nb_dim * sizeof (int));
	memset (ret + 1 + nb_dim, 0, len * sizeof (int));
	
	return ret;
}

void
__array_free (Array_t array)
{
	if (!array)
		__throw (NullArrayException);
	
	free (array);
	array = NULL;
}

int
__array_get (Array_t array, int dim1, ...)
{
	int i, v, *tmp;
	size_t len, tlen;
	va_list list;
	
	printf ("->get\n");
	
	if (!array)
		__throw (NullArrayException);
	
	va_start (list, dim1);
	
	for (i = 0, tmp = array + 1, tlen = 1; i < *array ; i++, tmp++)
		tlen *= *tmp;
	
	printf ("%u dim=%i\n", tlen, *array);

	tmp = array + 1;
	len = 0U;
	i = 0;
	v = dim1;
	do
	{
		tlen /= *tmp;
		
		if (i >= *array)
		{
			va_end (list);
			__throw (ArrayIndexOutOfBoundsException);
		}
		
		if (v >= *tmp)
		{
			va_end (list);
			__throw (ArrayIndexOutOfBoundsException);
		}
		
		if (v < 0)
		{
			va_end (list);
			__throw (BadDimensionException);
		}
		
		len += v * tlen;
		
		i++;
		tmp++;
	} while ((v = va_arg (list, int)) != -1);
	
	va_end (list);
	
	if (i != *array)
		__throw (BadDimensionException);
		
	printf ("getlen => %u\n", len);
	
	printf ("<-get\n");
	
	return *(array + *array + 1 + len);
}

int
__array_get_a (Array_t array, Array_t index)
{
	int i, *tmp;
	size_t len, tlen;
	
	if (!array || !index)
		__throw (NullArrayException);
	
	/* index n'est pas un tableau unidimensionnel. */
	if (*index > 1)
		__throw (BadDimensionException);
	
	/* Le nombre de dimensions de array est inférieur à la taille
	 * du tableau index */
	if (*(index + 1) < *array)
		__throw (BadDimensionException);
	
	for (i = 0, tmp = array + 2, tlen = 1; i < *array ; i++)
		tlen *= *(tmp++);
	
	len = 0U;
	tmp = array + 1;
	for (i = 0 , index += 2 ; i < *array ; i++, index++, tmp++)
	{
		if (*index < 0)
			__throw (BadDimensionException);
		if (*index >= *tmp)
			__throw (ArrayIndexOutOfBoundsException);
		
		len += (*index) * tlen;
		tlen /= *tmp;
	}
	
	return *(array + *array + 1 + len);
}

/**/

void
__array_set (Array_t array, int val, int dim1, ...)
{
	int i, v, *tmp;
	size_t len, tlen;
	va_list list;
	
	printf ("->set\n");
	
	if (!array)
		__throw (NullArrayException);
	
	va_start (list, dim1);
	
	for (i = 0, tmp = array + 1, tlen = 1; i < *array ; i++, tmp++)
		tlen *= *tmp;
	
	printf ("%u dim=%i\n", tlen, *array);
	
	tmp = array + 1;
	len = 0U;
	i = 0;
	v = dim1;
	do
	{
		tlen /= *tmp;
		
		if (i >= *array)
		{
			va_end (list);
			__throw (ArrayIndexOutOfBoundsException);
		}
		
		if (v >= *tmp)
		{
			va_end (list);
			__throw (ArrayIndexOutOfBoundsException);
		}
		
		if (v < 0)
		{
			va_end (list);
			__throw (BadDimensionException);
		}
		
		len += v * tlen;
		
		i++;
		tmp++;
	} while ((v = va_arg (list, int)) != -1);
	
	va_end (list);
	
	if (i != *array)
		__throw (BadDimensionException);
	
	printf ("setlen => %u\n", len);
	
	printf ("<-set\n");
	
	*(array + *array + 1 + len) = val;
}

void
__array_set_a (Array_t array, int val, Array_t index)
{
	int i, *tmp;
	size_t len, tlen;
	
	if (!array || !index)
		__throw (NullArrayException);
	
	/* index n'est pas un tableau unidimensionnel. */
	if (*index > 1)
		__throw (BadDimensionException);
	
	/* Le nombre de dimensions de array est inférieur à la taille
	 * du tableau index */
	if (*(index + 1) < *array)
		__throw (BadDimensionException);
	
	for (i = 0, tmp = array + 2, tlen = 1; i < *array ; i++)
		tlen *= *(tmp++);
	
	len = 0U;
	tmp = array + 1;
	for (i = 0 , index += 2 ; i < *array ; i++, index++, tmp++)
	{
		if (*index < 0)
			__throw (BadDimensionException);
		if (*index >= *tmp)
			__throw (ArrayIndexOutOfBoundsException);
		
		len += (*index) * tlen;
		tlen /= *tmp;
	}
	
	*(array + *array + 1 + len) = val;
}

/**/

int
__array_dimension (Array_t array)
{
	if (!array)
		__throw (NullArrayException);
	
	return *array;
}

int
__array_length (Array_t array, int n)
{
	if (!array)
		__throw (NullArrayException);
	
	return *(array + 1 + n);
}
