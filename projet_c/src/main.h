#ifndef __MAIN_H__
#define __MAIN_H__

#include "config.h"

#ifdef ENDIAN_BIG
# define MAGIC_ONE 
# define DECAL_R <<
#else
# define DECAL_L <<
# define DECAL_R >>
#endif 

#define PRINT_ERR(arg) ui_print_error arg

void ui_print_error (const char *fmt, ...);
void compress_print_err (const char *fmt, ...);

#endif
