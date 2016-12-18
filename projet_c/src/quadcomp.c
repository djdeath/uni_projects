#include "main.h"

#include <stdlib.h>

#include "quadcomp.h"

/* @loss doit être compris entre 0 et 100, il définit la
 * l'inverse variance. Lorsque @loss vaut 100, alors la variance des
 * sous pixels pour obtenir un parent uniform est de 0. Lorsque @loss
 * vaut 0 alors la variance est de 100. */

int
quadtree_compress (QuadTree_t *tree, int loss)
{
	int m, n, p, lev, var;
	Quad_t *cur;
	
	if (!tree || (loss > 100) || (loss < 0))
		return 0;
	
	/* Calcul de la variance à partir de loss. */
	var = 100 - loss;
	
	quadtree_set_level_uniform (tree, tree->levels - 1, 1);
	
	for (m = 0 , lev = 1 ; m < (tree->levels - 2) ; m++ , lev *= 4);
	
	for (m = tree->levels - 2 ; m > 0 ; m-- , lev /= 4)
	{
		for (n = 0 ; n < lev ; n++)
		{
			for (p = 0 ; p < tree->depth ; p++)
			{
				cur = &QUAD (tree, m, n, p);
				if ((abs ((int) cur->pix - (int) cur->tl->pix) <= var) &&
				    (abs ((int) cur->pix - (int) cur->tr->pix) <= var) &&
				    (abs ((int) cur->pix - (int) cur->bl->pix) <= var) &&
				    (abs ((int) cur->pix - (int) cur->br->pix) <= var) &&
				    cur->tl->uniform && cur->tr->uniform &&
				    cur->bl->uniform && cur->br->uniform)
					cur->uniform = 1;
				else
					cur->uniform = 0;
			}
		}
	}
	
	return 1;
}
