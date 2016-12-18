#include "main.h"

#include <stdlib.h>
#include <limits.h>

#include "picture.h"

Picture_t *
picture_new (int width, int height, ImageType_t type, int depth)
{
	Picture_t *pic = (Picture_t *) (calloc (1, sizeof (Picture_t)));
	
	if (pic)
	{
		if (!(pic->data = (unsigned char *) malloc ((size_t) (width * height * depth * sizeof (unsigned char)))))
		{
			free (pic);
			
			return NULL;
		}
		pic->width = width;
		pic->height = height;
		pic->type = type;
		pic->depth = depth;
	}
	
	return pic;
}

void
picture_free (Picture_t *pic)
{
	if (pic)
	{
		if (pic->data)
			free (pic->data);
		free (pic);
	}		
}

void
pictures_mix (Picture_t *p1, const Picture_t *p2, int blend)
{
	unsigned char *d1, *d2;
	int i, j, k;
	
	if (!p1 || !p2)
		return;
	
	d1 = p1->data;
	d2 = p2->data;
	
	for (j = 0 ; j < p1->height ; j++)
		for (i = 0 ; i < p1->width ; i++)
			for (k = 0 ; k < p1->depth ; k++, d1++, d2++)
				*d1 = (*d1 * (256 - blend) + *d2 * blend) >> 8;
	
}

void
picture_invert (Picture_t *pic)
{
	unsigned char *d;
	int i, j, k;
	
	if (!pic);
	
	d = pic->data;
	
	for (j = 0 ; j < pic->height ; j++)
		for (i = 0 ; i < pic->width ; i++)
			for (k = 0 ; k < pic->depth ; k++, d++)
				*d = UCHAR_MAX - *d;
}

Picture_t *
picture_convert_greyscale_to_rgb (const Picture_t *pic)
{
	int i, j, k;
	Picture_t *new;
	
	if (!pic)
		return NULL;
	
	if (!(new = picture_new (pic->width, pic->height, RGB, 3)))
		return new;
	
	for (j = 0 ; j < pic->height ; j++)
		for (i = 0 ; i < pic->width ; i++)
			for (k = 0 ; k < RGB ; k++)
				*PICTURE_PIX_COLOR (new, i, j, k) = *PICTURE_PIX (pic, i, j);
	
	return new;
}
