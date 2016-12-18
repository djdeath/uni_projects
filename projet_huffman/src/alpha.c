#include "main.h"

#include <limits.h>
#include <stdlib.h>

#include "alpha.h"

Alpha_t *
alpha_new (void)
{
	Alpha_t *al = ALPHA (calloc (1, sizeof (Alpha_t)));
	
	if (al)
	{
		if ((al->tree = htree_new ()))
		{
			if (!(al->letters = calloc (UCHAR_MAX + 1,  sizeof (Letter_t))))
			{
				alpha_free (al);
				
				return NULL;
			}
			if (!(al->end = htree_add (al->tree, NULL)))
			{
				alpha_free (al);
				
				return NULL;
			}
		}
		else
		{
			free (al);
			al = NULL;
		}
	}
	
	return al;
}

void
alpha_free (Alpha_t *al)
{
	if (al)
	{
		if (al->tree)
			htree_free (al->tree);
		if (al->letters)
			free (al->letters);
		free (al);
	}
}

HNode_t *
alpha_add (Alpha_t *al, unsigned char c)
{
	unsigned int i;
	
	if (!al)
		return NULL;
	
	i = (unsigned int) c;
	
	if (al->letters[i].node)
		return al->letters[i].node;
	
	al->letters[i].letter = c;
	
	if (!(al->letters[i].node = htree_add (al->tree, &(al->letters[i]))))
		return NULL;
	
	htree_increment (al->tree, al->tree->new);
	
	return al->letters[i].node;
}

void
alpha_update (Alpha_t *al, HNode_t *node)
{
	if (!al || !node)
		return;
	
	htree_increment (al->tree, node);
}

HNode_t *
alpha_search (Alpha_t *al, unsigned char c)
{
	if (!al)
		return NULL;
	
	return al->letters[((unsigned int) c)].node;
}
