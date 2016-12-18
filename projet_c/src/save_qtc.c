#include <limits.h>
#include <time.h>

#include "save_qtc.h"
#include "file_bit.h"
#include "util.h"

static int
save_quadtree_data (BFILE *file, const QuadTree_t *tree)
{
	int m, n, p, lev;
	Quad_t *parent, *cur;
	
	/* Ecriture de la racine. */
	for (p = 0 ; p < tree->depth ; p++)
	{
		cur = &QUAD (tree, 0, 0, p);
		bfwrite_char (file, cur->pix);
		bfwrite_bit (file, GETBIT (cur->error, 1));
		bfwrite_bit (file, GETBIT (cur->error, 0));
		if (!cur->error)
			bfwrite_bit (file, cur->uniform);
	}
	
	/* Ecriture des niveaux intermédiaires. */
	for (m = 1 , lev = 4 ; m < (tree->levels - 1) ; m++ , lev *= 4)
	{
		for (p = 0 ; p < tree->depth ; p++)
		{
			for (n = 0 ; n < lev ; n++)
			{
				parent = &QUAD_PARENT (tree, m, n, p);
				cur = &QUAD (tree, m, n, p);
				
				if (!parent->uniform)
				{
					if ((n % 4) != 3)
						bfwrite_char (file, cur->pix);
					if (cur->uniform)
					{
						bfwrite_bit (file, 0);
						bfwrite_bit (file, 0);
						bfwrite_bit (file, 1);
					}
					else
					{
						bfwrite_bit (file, GETBIT (cur->error, 1));
						bfwrite_bit (file, GETBIT (cur->error, 0));
						if (!cur->error)
							bfwrite_bit (file, cur->uniform);
					}
				}
			}
		}
	}
	
	/* Dernier niveau. */
	for (p = 0 ; p < tree->depth ; p++)
	{
		for (n = 0 ; n < lev ; n++)
		{
			parent = &QUAD_PARENT (tree, m, n, p);
			cur = &QUAD (tree, m, n, p);
			if (!parent->uniform && ((n % 4) != 3))
				bfwrite_char (file, cur->pix);
		}
	}
	
	return 1;
}

int
save_qtc (const char *file, const QuadTree_t *tree)
{
	int ret = 0;
	time_t date;
	BFILE *f;
	
	if (!file || !tree)
		return ret;
	
	if ((f = bfopen (file, BFMODE_WRITE)))
	{
		switch (tree->type)
		{ 
		case GREYSCALE:
			bfprintf (f, "Q1\n");
			break;
			
		case RGB:
			bfprintf (f, "Q2\n");
			break;
			
		case RGBA:
			bfprintf (f, "Q3\n");
			break;
			
		default:
			bfclose (f);
			return 0;
		}
		bfprintf (f, "# %s", ctime (&date));
		bfwrite_char (f, (unsigned char) (tree->levels - 1));
		bfwrite_char (f, (unsigned char) ((tree->depth / tree->type) * CHAR_BIT));
		ret = save_quadtree_data (f, tree);
		
		bfclose (f);
	}
	
	return ret;
}
