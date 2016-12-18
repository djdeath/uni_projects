#ifndef __COMP_H__
#define __COMP_H__

#include "bfile.h"

int compress (BFILE *out, FILE *in);
int uncompress (FILE *out, BFILE *in);

#endif
