#include "main.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "quadtree.h"
#include "util.h"

static void
quad_free (Quad_t *quad[])
{
	if (quad)
	{
		int i = 0;
		
		while (quad[i])
			free (quad[i++]);
		
		free (quad);
	}
}

static Quad_t **
quad_new (int levels, int depth)
{
	int i, n;
	Quad_t **q;
	
	if (!levels)
		return NULL;
	
	if (!(q = (Quad_t **) calloc ((size_t) (levels + 1), sizeof (Quad_t *))))
		return NULL;
	
	for (i = 0 , n = depth ; i < levels ; i++ , n *= 4)
	{
		if (!(q[i] = (Quad_t *) malloc ((size_t) (n * sizeof (Quad_t)))))
		{
			quad_free (q);
			
			return NULL;
		}
	}
	
	return q;
}

/**/

/* Initialisation des pointeurs de la structure d'arbre. */
void
quadtree_init (QuadTree_t *tree)
{
	int m, n, p, lev;
	Quad_t *parent, *f;
	
	for (m = 0 , lev = 1 ; m < (tree->levels - 1) ; m++ , lev *= 4)
	{
		for (p = 0 ; p < tree->depth ; p++)
		{
			for (n = 0 ; n < lev ; n++)
			{
				parent = &QUAD (tree, m, n, p);
				f = &QUAD_CHILD_TL (tree, m, n, p);
				parent->tl = f;
				parent->tr = (f += tree->depth);
				parent->br = (f += tree->depth);
				parent->bl = (f += tree->depth);
			}
		}
	}
	
	memset (tree->quads[m], 0, (size_t) (lev * tree->depth * sizeof (Quad_t)));
}

QuadTree_t *
quadtree_new (int width, int height, ImageType_t type, int depth)
{
	QuadTree_t *tree = (QuadTree_t *) calloc (1, sizeof (QuadTree_t));
	
	if (!tree)
		return tree;
	
	tree->width = width;
	tree->height = height;
	tree->type = type;
	tree->depth = depth;
	
	tree->levels = first_bit_up (tree->width) + 1;
	
	if (!(tree->quads = quad_new (tree->levels, depth)))
	{
		free (tree);
		
		return NULL;
	}
	
	quadtree_init (tree);
	
	return tree;
}

void
quadtree_free (QuadTree_t *tree)
{
	if (tree)
	{
		if (tree->quads)
			quad_free (tree->quads);
		free (tree);
	}
}

/**/

/* Rotation par rapport à l'axe horizontal. */
void
quadtree_flip_hori (QuadTree_t *tree)
{
	int m, n, d;
	Quad_t *cur, *tmp;
	
	if (!tree)
		return;
	
	n = tree->depth;
	for (m = 0 ; m < (tree->levels - 1) ; m++)
	{
		for (d = 0 ; d < n ; d++)
		{
			cur = &tree->quads[m][d];
			tmp = cur->tr;
			cur->tr = cur->br;
			cur->br = tmp;
			
			tmp = cur->tl;
			cur->tl = cur->bl;
			cur->bl = tmp;
		}
		n *= 4;
	}
}

/* Rotation par rapport à l'axe vertical. */
void
quadtree_flip_vert (QuadTree_t *tree)
{
	int m, n, d;
	Quad_t *cur, *tmp;
	
	if (!tree)
		return;
	
	n = tree->depth;
	for (m = 0 ; m < (tree->levels - 1) ; m++)
	{
		for (d = 0 ; d < n ; d++)
		{
			cur = &tree->quads[m][d];
			tmp = cur->tl;
			cur->tl = cur->tr;
			cur->tr = tmp;
			
			tmp = cur->bl;
			cur->bl = cur->br;
			cur->br = tmp;
		}
		n *= 4;
	}
}

/* Rotation d'une image dans le sens trigonométrique. */
void
quadtree_rotate_trigo (QuadTree_t *tree)
{
	int m, n, d;
	Quad_t *cur, *tmp;
	
	if (!tree)
		return;
	
	n = tree->depth;
	for (m = 0 ; m < (tree->levels - 1) ; m++)
	{
		for (d = 0 ; d < n ; d++)
		{
			cur = &tree->quads[m][d];
			tmp = cur->tl;
			cur->tl = cur->tr;
			cur->tr = cur->br;
			cur->br = cur->bl;
			cur->bl = tmp;
		}
		n *= 4;
	}
}

/* Rotation d'une image dans le sens anti trigonométrique. */
void
quadtree_rotate_ntrigo (QuadTree_t *tree)
{
	int m, n, d;
	Quad_t *cur, *tmp;
	
	if (!tree)
		return;
	
	n = tree->depth;
	for (m = 0 ; m < (tree->levels - 1) ; m++)
	{
		for (d = 0 ; d < n ; d++)
		{
			cur = &tree->quads[m][d];
			tmp = cur->tl;
			cur->tl = cur->bl;
			cur->bl = cur->br;
			cur->br = cur->tr;
			cur->tr = tmp;
		}
		n *= 4;
	}
}

/**/

/* Met tous les bits uniform d'un même niveau à 0 si @uniform vaut 0,
 * à 1 sinon. */
void
quadtree_set_level_uniform (QuadTree_t *tree, int level, int uniform)
{
	int i, n, p;
	
	if (!tree || (level < 0) || (level >= tree->levels))
		return;
	
	if (uniform)
		uniform = 1;
	else
		uniform = 0;
	
	for (i = 0 , n = 1 ; i < level ; i++ , n *= 4);
	
	for (p = 0 ; p < tree->depth ; p++)
		for (i = 0 ; i < n  ; i++)
			QUAD (tree, level, i, p).uniform = uniform;
}
