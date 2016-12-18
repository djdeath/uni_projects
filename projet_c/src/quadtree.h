#ifndef __QUADTREE_H__
#define __QUADTREE_H__

#include "image_common.h"

#define QUAD_PARENT(tree, m, n, p) ((tree)->quads[(m) - 1][((n) / 4) * (tree)->depth + (p)])

#define QUAD(tree, m, n, p) ((tree)->quads[m][(n) * (tree)->depth + (p)])

#define QUAD_CHILD_TL(tree, m, n, p) ((tree)->quads[m + 1][(4 * (n)) * (tree)->depth + (p)])
#define QUAD_CHILD_TR(tree, m, n, p) ((tree)->quads[m + 1][(4 * (n) + 1) * (tree)->depth + (p)])
#define QUAD_CHILD_BR(tree, m, n, p) ((tree)->quads[m + 1][(4 * (n) + 2) * (tree)->depth + (p)])
#define QUAD_CHILD_BL(tree, m, n, p) ((tree)->quads[m + 1][(4 * (n) + 3) * (tree)->depth + (p)])

#define QUAD_END_LEVEL(tree, m) ((m) >= (tree)->levels ? 1 : 0)

typedef struct _Quad_t
{
	unsigned char pix;          /* moyenne des sous pixels ou
				     * pixel (dans ce cas => error =
				     * 0) */
	unsigned char error : 2;    /* 2 bits d'erreur => 4 valeurs */
	unsigned char uniform : 1;  /* 1 bit d'uniformité des fils */
	struct _Quad_t *bl, *br, *tl, *tr;
} Quad_t;

typedef struct _QuadTree_t
{
	int width, height;
	int depth;
	ImageType_t type;
	
	int levels;
	Quad_t **quads;
} QuadTree_t;

QuadTree_t *quadtree_new (int width, int height, ImageType_t type, int depth);
void quadtree_free (QuadTree_t *tree);

void quadtree_init (QuadTree_t *tree);

/* Diverses opérations que l'on peut appliquer aux images
 * facilement lorsqu'elle sont sous forme quadtree. */

void quadtree_flip_hori (QuadTree_t *tree);
void quadtree_flip_vert (QuadTree_t *tree);

void quadtree_flip_diag_left (QuadTree_t *tree);
void quadtree_flip_diag_right (QuadTree_t *tree);

void quadtree_rotate_trigo (QuadTree_t *tree);
void quadtree_rotate_ntrigo (QuadTree_t *tree);

/* Positionne le bit uniform de tout un niveau d'un quadtree à 1 si
 * uniform est différent de 0, à 0 sinon. */
void quadtree_set_level_uniform (QuadTree_t *tree, int level, int uniform);

#endif
