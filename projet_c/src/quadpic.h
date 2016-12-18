#ifndef __QUADPIC_H__
#define __QUADPIC_H__

#include "picture.h"
#include "quadtree.h"


/* Transformation d'une image sous forme linéaire à une image sous
 * forme de quadtree. */
int picture_to_quadtree (QuadTree_t *tree, const Picture_t *pic);
/* Transformation d'une image sous forme quadtree à une image sous
 * forme linéraire. */
int quadtree_to_picture (Picture_t *pic, const QuadTree_t *tree, int level);

/* Même chose avec allocation de mémoire. */
QuadTree_t *picture_to_quadtree_new (const Picture_t *pic);
Picture_t *quadtree_to_picture_new (const QuadTree_t *tree, int level);

#endif
