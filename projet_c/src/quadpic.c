#include "main.h"

#include <stdlib.h>

#include "quadpic.h"

struct QuadFill
{
	const Picture_t *pic;
	int depth;
	int cur_level;
	int max_level;
};

static void
picture_to_quadtree_ (struct QuadFill *fill, Quad_t *quad, int i, int j, int size)
{
	if (quad->tl)
	{
		/* Calcul des sous niveaux */
		size /= 2;
		picture_to_quadtree_ (fill, quad->tl, i, j, size);
		picture_to_quadtree_ (fill, quad->tr, i + size, j, size);
		picture_to_quadtree_ (fill, quad->br, i + size, j + size, size);
		picture_to_quadtree_ (fill, quad->bl, i, j + size, size);
		
		/* Calcul du pixel courant (moyenne des sous pixels,
		 * etc...) */
		j = size = (int) quad->tl->pix;
		size += (int) quad->tr->pix;
		size += (int) quad->br->pix;
		size += (int) quad->bl->pix;
		
		quad->uniform = 0;
		quad->pix = (unsigned char) (size / 4);
		quad->error = (unsigned char) (size % 4);
	}
	else
	{
		quad->uniform = 1;
		quad->pix = *(PICTURE_PIX_COLOR (fill->pic, i, j, fill->depth));
	}
}

int
picture_to_quadtree (QuadTree_t *tree, const Picture_t *pic)
{
	int p;
	struct QuadFill fill;
	
	if (!tree || !pic || (tree->depth != pic->depth) || (pic->width != tree->width))
		return 0;
	
	fill.cur_level = 0;
	fill.max_level = 0;
	fill.pic = pic;
	for (p = 0 ; p < pic->depth ; p++)
	{
		fill.depth = p;
		picture_to_quadtree_ (&fill, &QUAD (tree, 0, 0, p), 0, 0, tree->width);
	}
	
	return 1;
}

QuadTree_t *
picture_to_quadtree_new (const Picture_t *pic)
{
	QuadTree_t *tree;
	
	if (!pic)
		return NULL;
	
	if ((tree = quadtree_new (pic->width, pic->height, pic->type, pic->depth)))
		picture_to_quadtree (tree, pic);
	
	return tree;
}

/**/

static void
quadtree_to_picture_ (struct QuadFill *fill, Quad_t *quad, int i, int j, int size)
{
	fill->cur_level++;
	
	if (quad->uniform || (fill->cur_level > fill->max_level))
	{
		/* Si l'on rencontre un noeud de l'arbre uniforme, on
		 * peut alors dessiner tous les sous noeuds de ce
		 * dernier. */
		int k, l;
		
		for (k = i ; k < (i + size) ; k++)
			for (l = j ; l < (j + size) ; l++)
				*(PICTURE_PIX_COLOR (fill->pic, k, l, fill->depth)) = quad->pix;
	}
	else if (quad->tl)
	{
		/* Calcul des sous niveaux */
		size /= 2;
		quadtree_to_picture_ (fill, quad->tl, i, j, size);
		quadtree_to_picture_ (fill, quad->tr, i + size, j, size);
	        quadtree_to_picture_ (fill, quad->br, i + size, j + size, size);
		quadtree_to_picture_ (fill, quad->bl, i, j + size, size);
	}
	else
		*(PICTURE_PIX_COLOR (fill->pic, i, j, fill->depth)) = quad->pix;
	
	fill->cur_level--;
}

int
quadtree_to_picture (Picture_t *pic, const QuadTree_t *tree, int level)
{
	int p;
	struct QuadFill fill;
	
	if (!tree || !pic || (tree->depth != pic->depth) || (pic->width != tree->width))
		return 0;
	
	fill.cur_level = 0;
	fill.max_level = level;
	fill.pic = pic;
	for (p = 0 ; p < tree->depth ; p++)
	{
		fill.depth = p;
		quadtree_to_picture_ (&fill, &QUAD (tree, 0, 0, p), 0, 0, tree->width);
	}
	
	return 1;
}

Picture_t *
quadtree_to_picture_new (const QuadTree_t *tree, int level)
{
	Picture_t *pic;
	
	if (!tree)
		return NULL;
	
	if ((pic = picture_new (tree->width, tree->height, tree->type, tree->depth)))
		quadtree_to_picture (pic, tree, level);
	
	return pic;
}
