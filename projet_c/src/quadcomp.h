#ifndef __QUADCOMP_H__
#define __QUADCOMP_H__

#include "picture.h"
#include "quadtree.h"

/* Application d'un param�tre loss, repr�sentant la variance entre
 * sous carr�s, pour le calcul des carr�s uniformes. */
int quadtree_compress (QuadTree_t *tree, int loss);

#endif
