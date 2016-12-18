#ifndef __PICTURE_H__
#define __PICTURE_H__

#include "image_common.h"

#define PICTURE_PIX(pic, x, y) ((pic)->data + ((pic)->width * (pic)->depth) * y + (x * (pic)->depth))
#define PICTURE_PIX_COLOR(pic, x, y, p) ((pic)->data + ((pic)->width * (pic)->depth) * y + (x * (pic)->depth) + (p))

typedef struct _Picture_t
{
	int width, height;
	int depth;
	ImageType_t type;
	
	unsigned char *data;
} Picture_t;

Picture_t *picture_new (int width, int height, ImageType_t type, int depth);
void picture_free (Picture_t *pic);

/* Fondu entre 2 images. */
void pictures_mix (Picture_t *p1, const Picture_t *p2, int blend);

/* Inversion des couleurs d'une image. */
void picture_invert (Picture_t *pic);

/* Création d'une image couleur à partir d'une image rgb. */
Picture_t *picture_convert_greyscale_to_rgb (const Picture_t *pic);

#endif
