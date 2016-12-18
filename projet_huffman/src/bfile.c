#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>

#include "bfile.h"

BFILE *
bfimport (FILE *file, BFMode mode)
{
	BFILE *f;
	
	if (!file)
		return NULL;
	
	f = (BFILE *) calloc (1, sizeof (BFILE));
	
	f->p = file;
	f->mode = mode;
	
	return f;
}

BFILE *
bfopen (const char *file, BFMode mode)
{
	BFILE *f;
	
	if (!file)
		return NULL;
	
	if (!(f = (BFILE *) calloc (1, sizeof (BFILE))))
		return f;
	
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
	if (f->mode == BFMODE_WRITE && f->pos)
	{
		/* Il faut réaliser le decalage final avant
		 * d'ecrire. */
		f->data <<= (CHAR_BIT - f->pos);
		fwrite (&f->data, sizeof (f->data), 1, f->p);
	}
	
	if (!f->imported)
		fclose (f->p);
	free (f);
	
	return;
}

int
bfflush (BFILE *f)
{
	if (!f)
		return EOF;
	
	f->pos = 0;
	
	return 0;
}

int
bfeof (BFILE *f)
{
	if (!f || f->mode != BFMODE_READ)
		return EOF;
	
	if ((f->pos < 1) && feof (f->p))
		return 1;
	
	return 0;
}

/**/

int
bfwrite_bit (BFILE *f, unsigned char bit)
{
	if (!f || f->mode != BFMODE_WRITE)
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
	
	if (!f || f->mode != BFMODE_WRITE)
		return 0;
	
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
	
	if (!f || !c || f->mode != BFMODE_READ)
		return 0;
	
	for (i = 0 ; i < CHAR_BIT ; i++)
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
bfputs (const char *str, BFILE *f)
{
	int ret = 0;
	
	if (!f || f->mode != BFMODE_WRITE || !str)
		return 0;
	
	while (*str != '\0')
		ret += bfwrite_char (f, *(str++));
	ret += bfwrite_char (f, *str);
	
	return ret;
}

int
bfgets (char *str, int size, BFILE *f)
{
	char c;
	int ret = 0;
	
	if (!f || f->mode != BFMODE_READ || !str)
		return 0;
	
	while ((ret < (size - 1)) && (bfread_char (f, &c) == CHAR_BIT) && (c != '\0'))
	{
		*(str++) = c;
		ret++;
	}
	*str = '\0';
	
	return ret * CHAR_BIT;
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
bfwrite_int (BFILE *f, unsigned int val)
{
        int i, ret = 0, pos;
        
        if (!f || f->mode != BFMODE_WRITE)
                return ret;
        
	if (!val)
                return bfwrite_bit (f, 1);
        
        pos = CHAR_BIT * sizeof (val);
	
        /* On se débarasse des bits à 0 au début du nombre que l'on
         * doit écrire. */
        while (!GETBIT (val, pos - 1))
                pos--;
        
	for (i = 0 ; i < pos ; i++)
        {
                if (!bfwrite_bit (f, 0))
                        return 0;
                ret++;
        }
        
        /* Enfin on écrit le nombre. */
        while (pos > 0)
        {
                if (!bfwrite_bit (f, (unsigned char) GETBIT (val, --pos)))
                        return 0;
                ret++;
        }
        
	return ret;
}

int
bfread_int (BFILE *f, unsigned int *val)
{
	unsigned char bit;
        int i, ret = 0, pos;
	
        if (!f || !val || f->mode != BFMODE_READ)
                return ret;
        
        while ((i = bfread_bit (f, &bit)) && !bit)
                ret++;
        
        /* Si l'on est arrivé sur la fin du fichier que celui
           a été correctement écrit, il devrait lire ue suite de
           bits (inférieur à 8) à 0, dans ce cas il faut
           retourner 0 même si l'on a lu des bits. */
        if (!i)
		return 0;
	
        /* Cas ou on lis la valeur 0. */
        if (!ret)
	{
                *val = 0;
                
                return 1;
        }
        
        *val = 1;
        pos = ret - 1;
        for (i = 0 ; i < pos ; i++)
        {
                if (!bfread_bit (f, &bit))
                        return 0;
                *val <<= 1;
                if (bit)
                        *val |= 1;
                ret++;
        }
        
	return ret;
}
