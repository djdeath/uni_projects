#ifndef __UTILS_H__
#define __UTILS_H__

#include "main.h"

/* #ifdef ENDIAN_BIG */
/* # define GETBIT(x, i) (((x) << (i)) & (MAGIC_ONE (x)) */
/* # define SETBIT(x, i, v) ((x) = ((v) == 0 ? ((x) & (~(MAGIC_ONE (x)))) : ((x) | (MAGIC_ONE (x))))) */
/* #else */
# define GETBIT(x, i) (((x) >> (i)) & 1)
# define SETBIT(x, i, v) ((x) = ((v) == 0 ? ((x) & (~(1 << (i)))) : ((x) | (1 << (i)))))
/* #endif */

/* Retourne le numéro du premier bit à l'etat 1 dans la valeur val. */
int first_bit_up (int val);

#endif
