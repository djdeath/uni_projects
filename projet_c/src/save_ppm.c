#include "main.h"

#include <stdio.h>
#include <limits.h>
#include <time.h>

#include "save_ppm.h"
#include "util.h"

static int
save_picture_ascii (FILE *file, const Picture_t *pic)
{
	unsigned char *s;
	int i, j, k;
	
	s = pic->data;
	k = pic->width * pic->depth;
	
	j = 0;
	while (j < pic->height)
	{
		i = 0;
		while (i < k)
		{
			fprintf (file, "%i ", (int) (*(s++)));
			i++;
		}
		fprintf (file, "\n");
		j++;
	}
	
	return 1;
}

static int
save_picture_raw (FILE *file, const Picture_t *pic)
{
	if (!fwrite (pic->data, (size_t) (pic->width * pic->height * pic->depth), 1, file))
		return 0;
	
	return 1;
}

int
save_ppm (const char *file, FileType_t type, const Picture_t *pic)
{
	int ret = 0, max_color, mul;
	FILE *f;
	time_t date;
	
	if (!file || !pic)
		return ret;
	
	if ((f = fopen (file, "wb")))
	{
		date = time (NULL);
		
		switch (type)
		{
		case FILE_PPM:
			switch (pic->type)
			{
			case GREYSCALE:
				mul = pic->depth;
				fprintf (f, "P2\n");
				break;
				
			case RGB:
				mul = pic->depth / 3;
				fprintf (f, "P3\n");
				break;
				
			case RGBA:
				mul = pic->depth / 4;
				fprintf (f, "P4\n");
				break;
				
			default:
				fclose (f);
				return ret;
			}
			max_color = 0;
			SETBIT (max_color, mul * CHAR_BIT, 1);
			max_color--;
			fprintf (f, "%i %i\n# %s%i\n", pic->width, pic->height, ctime (&date), max_color);
			ret = save_picture_ascii (f, pic);
			break;
			
		case FILE_PGM:
			switch (pic->type)
			{
			case GREYSCALE:
				mul = pic->depth;
				fprintf (f, "P5\n");
				break;
				
			case RGB:
				mul = pic->depth / 3;
				fprintf (f, "P6\n");
				break;
				
			case RGBA:
				mul = pic->depth / 4;
				fprintf (f, "P7\n");
				break;
				
			default:
				fclose (f);
				return ret;
			}
			max_color = 0;
			SETBIT (max_color, mul * CHAR_BIT, 1);
			max_color--;
			fprintf (f, "%i %i\n# %s%i\n", pic->width, pic->height, ctime (&date), max_color);
			ret = save_picture_raw (f, pic);
			break;
			
		default:
			fclose (f);
			return ret;
		}
		fclose (f);
	}
	
	return ret;
}
