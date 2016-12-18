#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>

#include "load_ppm.h"
#include "util.h"

#define BUF_SIZE 256

static int
load_ppm_data_raw (FILE *file, Picture_t *pic, size_t size)
{
	if (fread (pic->data, size, 1, file) < 1)
	{
		PRINT_ERR (("Error in image data."));
		
		return 0;
	}
	
	return 1;
}

static int
load_ppm_data_ascii (FILE *file, Picture_t *pic, size_t size)
{
	unsigned char *s;
	int c, max_color;
	unsigned int i;
	
	s = pic->data;
	i = 0;
	
	max_color = 0;
	SETBIT (max_color, pic->depth * CHAR_BIT, 1);
	max_color--;
	
	while ((i < size) && (fscanf (file, "%i", &c) > 0))
	{
		if (c > max_color)
		{
			PRINT_ERR (("Wrong image data."));
		}
		*(s++) = (unsigned char) c;
		i++;
	}
	
	if (i < size)
	{
		PRINT_ERR (("Error in image data."));
		
		return 0;
	}
	
	return 1;
}

static int
load_ppm_data (FILE *file, Picture_t *pic, int ascii)
{
	size_t mem_space;
	
	if ((pic->width <= 0) || (pic->height <= 0) || (pic->depth <= 0))
	{
		PRINT_ERR (("WidthxHeightxMaxcolor : %ix%ix%i something wrong... no ?", pic->width, pic->height, pic->depth));
		
		return 0;
	}
	
	mem_space = pic->width * pic->height * pic->depth * sizeof (char);
	
	if (ascii)
	{
		if (!load_ppm_data_ascii (file, pic, mem_space))
			return 0;
	}
	else
		if (!load_ppm_data_raw (file, pic, mem_space))
			return 0;
	
	return 1;
}

/**/

static Picture_t *
load_ppm_header (FILE *file, int *ascii)
{
	char *c, *n, buf[BUF_SIZE];
	int complete = 0, width, height, depth, max_color, tmp;
	ImageType_t type;
	
	while (!feof (file) && (complete < 4))
	{
		c = n = fgets (buf, BUF_SIZE, file);
		
		while ((c != NULL) && (*c != '\0') && (*c != '#'))
		{
			while (isalnum((int) *n))
				n++;
			
			if (*n != '\0')
			{
				*n = '\0';
				n++;
			}
			else
				n = NULL;
			
			switch (complete)
			{
			case 0:
				/* Nombre magique */
				if (!strcmp (c, "P2"))
				{
					*ascii = 1;
					type = GREYSCALE;
				}
				else if (!strcmp (c, "P3"))
				{
					*ascii = 1;
					type = RGB;
				}
				else if (!strcmp (c, "P4"))
				{
					*ascii = 1;
					type = RGBA;
				}
				else if (!strcmp (c, "P5"))
					type = GREYSCALE;
				else if (!strcmp (c, "P6"))
					type = RGB;
				else if (!strcmp (c, "P7"))
					type = RGBA;
				else
				{
					PRINT_ERR (("Not supported or wrong magic number."));
					
					return NULL;
				}
				complete++;
				break;
				
			case 1:
				/* Largeur */
				width = (int) strtol (c, &c, 10);
				if ((width < INT_MAX) &&
				    (width > INT_MIN))
					complete++;
				break;
				
			case 2:
				/* Hauteur */
				height = (int) strtol (c, &c, 10);
				if ((height < INT_MAX) &&
				    (height > INT_MIN))
					complete++;
				break;
				
			case 3:
				/* Profondeur */
				max_color = (int) strtol (c, &c, 10);
				if ((max_color < INT_MAX) &&
				    (max_color > INT_MIN))
				{
					tmp = (first_bit_up (max_color) + 1) / CHAR_BIT;
					if (type == GREYSCALE)
						depth = tmp;
					else
						depth = 3 * tmp;
					complete++;
				}
				break;
				
			default:
				PRINT_ERR (("Internal error."));
				
				return NULL;
			}
			
			c = n;
		}
	}
	
	return picture_new (width, height, type, depth);
}

Picture_t *
load_ppm (const char *file)
{
	FILE *f;
	Picture_t *pic = NULL;
	
	if (!file)
		return pic;
	
	if ((f = fopen (file, "rb")))
	{
		int ascii = 0;
		
		/* Chargement des informations contenues dans l'entete
		 * du fichier.  Pour que le chargement soit fini, il
		 * faut avoir lu 4 informations sur l'image : nombre
		 * magique, largeur, hauteur et pronfondeur de
		 * l'image. */
		if (!(pic = load_ppm_header (f, &ascii)))
		{
			fclose (f);
			
			return NULL;
		}
		
		/* Une fois le chargement de l'entete effectue, on
		 * peut charger l'image. Une fois encore, si l'on a
		 * moins de données que prévut, c'est qu'il y a
		 * eut un problème. */
		if (!load_ppm_data (f, pic, ascii))
		{
			picture_free (pic);
			fclose (f);
			
			return NULL;
		}
		
		fclose (f);
	}
	
	return pic;
}
