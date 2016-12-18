#ifndef __SAVE_PPM_H__
#define __SAVE_PPM_H__

#include "picture.h"

typedef enum
{
	FILE_PPM,
	FILE_PGM
} FileType_t;

int save_ppm (const char *file, FileType_t type, const Picture_t *pic);

#endif
