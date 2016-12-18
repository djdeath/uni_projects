#include "main.h"

#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "picture.h"
#include "quadtree.h"
#include "file_bit.h"
#include "load_qtc.h"
#include "util.h"

#define BUF_SIZE 256

static void
load_qtc_data_quad_qual (BFILE *file, Quad_t *cur)
{
	unsigned char bit = 0;
	
	/* erreur */
	bfread_bit (file, &bit);
	cur->error = bit << 1;
	bfread_bit (file, &bit);
	cur->error |= bit;
	
	/* uniformite */
	if (!cur->error)
	{
		if (!bfread_bit (file, &bit))
			cur->uniform = 1;
		else
			cur->uniform = bit;
	}
	else
		cur->uniform = 0;
	
	return;
}

/* Calcul de la valeur du pixel manquant. */
static void
qtc_calculate (Quad_t *parent)
{
	int moy;
	
	moy = ((int) parent->pix) * 4 + (int) parent->error;
	moy -= (int) parent->tl->pix;
	moy -= (int) parent->tr->pix;
	moy -= (int) parent->br->pix;
	
	parent->bl->pix = (unsigned char) moy;
}

static int
load_qtc_data (BFILE *file, QuadTree_t *tree)
{
	int m, n, lev = 4, p;
	Quad_t *cur, *parent;
	
	/* Chargement de la racine. */
	for (p = 0 ; p < tree->depth ; p++)
	{
		cur = &QUAD (tree, 0, 0, p);
		if (!bfread_char (file, &(cur->pix)))
			return 0;
		load_qtc_data_quad_qual (file, cur);
	}
	
	/* Chargement des niveaux intermédiaires. */
	for (m = 1 , lev = 4 ; m < (tree->levels - 1) ; m++ , lev *= 4)
	{
		for (p = 0 ; p < tree->depth ; p++)
		{
			for (n = 0 ; n < lev ; n++)
			{
				cur = &QUAD (tree, m, n, p);
				parent = &QUAD_PARENT (tree, m, n, p);
				if (parent->uniform)
				{
					cur->pix = parent->pix;
					cur->uniform = 1;
				}
				else if ((n % 4) != 3)
				{
					if (!bfread_char (file, &(cur->pix)))
						cur->pix = parent->pix;
					load_qtc_data_quad_qual (file, cur);
				}
				else
				{
					qtc_calculate (parent);
					load_qtc_data_quad_qual (file, cur);
				}
			}
		}
	}
	
	/* Chargement du dernier niveau. */
	for (p = 0 ; p < tree->depth ; p++)
	{
		for (n = 0 ; n < lev ; n++)
		{
			cur = &QUAD (tree, m, n, p);
			parent = &QUAD_PARENT (tree, m, n, p);
			if (parent->uniform)
			{
				cur->pix = parent->pix;
			}
			else if ((n % 4) != 3)
			{
				if (!bfread_char (file, &(cur->pix)))
					cur->pix = parent->pix;
			}
			else
				qtc_calculate (parent);
		}
	}
	
	return 1;
}

static QuadTree_t *
load_qtc_headers (BFILE *file)
{
	unsigned char buf[BUF_SIZE];
	int complete = 0, width, depth;
	ImageType_t type;
	
	while (!bfeof (file) && (complete < 3))
	{
		if (bfread_char (file, buf) && (*buf != '#'))
		{
			switch (complete)
			{
			case 0:
				if (!bfgets (buf + 1, BUF_SIZE - 1, file))
					return NULL;
				
				/* Nombre magique */
				if (!strcmp (buf, "Q1"))
					type = GREYSCALE;
				else if (!strcmp (buf, "Q2"))
					type = RGB;
				else if (!strcmp (buf, "Q3"))
					type = RGBA;
				else
				{
					PRINT_ERR (("Not supported or wrong magic number."));
					
					return 0;
				}
				complete++;
				break;
			
			case 1:
				/* Largeur */
				width = 0;
				if (buf[0] < sizeof (int) * CHAR_BIT)
					SETBIT (width, (int) buf[0], 1);
				else
					return NULL;
				complete++;
				break;
				
			case 2:
				/* Profondeur */
				depth = (int) buf[0];
				
				depth *= type;
				depth /= CHAR_BIT;
				complete++;
				break;
				
			default:
				PRINT_ERR (("Internal error."));
				
				return NULL;
			}
		}
		else
			bfgets (buf, BUF_SIZE, file);
	}
	
	return quadtree_new (width, width, type, depth);
}

QuadTree_t *
load_qtc (const char *file)
{
	BFILE *f;
	QuadTree_t *q = NULL;
	
	if (!file)
		return q;
	
	if ((f = bfopen (file, BFMODE_READ)))
	{
		if (!(q = load_qtc_headers (f)))
		{
			bfclose (f);
			
			return q;
		}
		
		if (!load_qtc_data (f, q))
		{
			quadtree_free (q);
			bfclose (f);
			
			return NULL;
		}
		
		bfclose (f);
	}
	
	return q;
}
