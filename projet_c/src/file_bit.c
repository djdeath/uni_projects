#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>

#include "file_bit.h"
#include "util.h"

BFILE *
bfopen (const char *file, BFMode mode)
{
	BFILE *f;
	
	if (!file)
		return NULL;
	
	f = (BFILE *) malloc (sizeof (BFILE));
	
	f->pos = 0;
	f->mode = mode;
	
	switch (mode)
	{
	case BFMODE_READ:
		f->p = fopen (file, "rb");
		break;
	case BFMODE_WRITE:
		f->p = fopen (file, "wb");
		break;
	default:
		f->p = NULL;
		break;
	}
	
	if (f->p)
		return f;
	free (f);
	
	return NULL;
}

void
bfclose (BFILE *f)
{
	if (!f)
 		return;
	
	/* Si on est en mode écriture, reste il des bits a ecrire ? */
	if ((f->mode == BFMODE_WRITE) && f->pos)
	{
		/* Il faut réaliser le decalage final avant
		 * d'ecrire. */
		f->data <<= CHAR_BIT - f->pos;
		fwrite (&f->data, sizeof (f->data), 1, f->p);
	}
	
	fclose (f->p);
	free (f);
	
	return;
}

int
bfeof (BFILE *f)
{
	if (!f || (f->mode != BFMODE_READ))
		return 0;
	
	if ((f->pos < 1) && feof (f->p))
		return 1;
	
	return 0;
}

/**/

int
bfwrite_bit (BFILE *f, unsigned char bit)
{
	if (!f || (f->mode != BFMODE_WRITE))
		return 0;
	
	/* L'octet est plein, on écrit. */
	if (f->pos > (CHAR_BIT - 1))
	{
		fwrite (&f->data, sizeof (f->data), 1, f->p);
		f->data = 0;
		f->pos = 0;
	}
	
	f->data <<= 1;
	if (bit)
		f->data |= 1;
	f->pos++;

	return 1;
}

int
bfread_bit (BFILE *f, unsigned char *bit)
{
	if (!f || !bit || f->mode != BFMODE_READ)
		return 0;
	
	if (f->pos < 1)
	{
		if (fread (&f->data, sizeof (f->data), 1, f->p) != 1)
			return 0;
		f->pos = CHAR_BIT * sizeof (unsigned char);
	}
	
	*bit = GETBIT (f->data, f->pos - 1);
	f->pos--;
	
	return 1;
}

int
bfwrite_char (BFILE *f, unsigned char c)
{
	int i, ret = 0;
	
	if (!f || (f->mode != BFMODE_WRITE))
		return ret;
	
	for (i = (CHAR_BIT - 1) ; i >= 0 ; i--)
	{
		if (!bfwrite_bit (f, GETBIT (c, i)))
			return 0;
		ret++;
	}
	
	return ret;
}

int
bfread_char (BFILE *f, unsigned char *c)
{
	unsigned char bit;
	int b, ret = 0, i;
	
	if (!f || !c || (f->mode != BFMODE_READ))
		return ret;
	
	for (i = CHAR_BIT - 1 ; i >= 0 ; i--)
	{
		if (!(b = bfread_bit (f, &bit)))
			return 0;
		ret += b;
		*c <<= 1;
		if (bit)
			*c |= 1;
	}
	
	return ret;
}

int
bfprintf (BFILE *f, const char *fmt, ...)
{
	int ret;
	
	va_list args;
	
	va_start (args, fmt);
	
	ret = vfprintf (f->p, fmt, args);
	
	va_end (args);
	
	return ret * CHAR_BIT;
}

int
bfgets (char *buf, int size, BFILE *f)
{
	char c;
	int ret = 0;
	
	if (!f || (f->mode != BFMODE_READ))
		return 0;
	
	while (bfread_char (f, &c) && (ret < (size - 1)) && (c != '\n'))
	{
		*(buf++) = c;
		ret++;
	}
	*buf = '\0';
	
	return ret * CHAR_BIT;
}
