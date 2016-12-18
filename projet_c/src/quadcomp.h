#ifndef __QUADCOMP_H__
#define __QUADCOMP_H__

#include "picture.h"
#include "quadtree.h"

/* Application d'un paramètre loss, représentant la variance entre
 * sous carrés, pour le calcul des carrés uniformes. */
int quadtree_compress (QuadTree_t *tree, int loss);

#endif
