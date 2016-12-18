#include "main.h"

#include <string.h>
#include <stdlib.h>

#include "lex.h"
#include "word.h"

void
lex_free (Lex_t *lex)
{
	if (lex)
	{
		if (lex->tree)
		{
			htree_foreach (lex->tree, FOREACH_CB (word_free), NULL);
			htree_free (lex->tree);
		}
		if (lex->hash)
			hash_table_free (lex->hash);
		if (lex->alpha)
			alpha_free (lex->alpha);
		free (lex);
	}
}

Lex_t *
lex_new (void)
{
	Lex_t *lex = LEX (calloc (1, sizeof (Lex_t)));
	
	if (lex)
	{
		if ((lex->tree = htree_new ()))
		{
			if ((lex->hash = hash_table_new (HASH_TABLE_OPEN_DEFAULT_SIZE,
							 HASH_MODE_OPEN, HASH_FUNC_GLIB)))
			{
				if (!(lex->alpha = alpha_new ()))
				{
					lex_free (lex);
					lex = NULL;
				}
			}
			else
			{
				lex_free (lex);
				lex = NULL;
			}
		}
		else
		{
			free (lex);
			lex = NULL;
		}
	}
	
	return lex;
}

HNode_t *
lex_add (Lex_t *lex, const unsigned char *word, size_t len)
{
	Word_t *w;
	HNode_t *node;
	
	if (!lex || !word)
		return NULL;
	
	if (!(w = word_new (word, len)))
		return NULL;
	
	if (!(node = htree_add (lex->tree, w)))
	{
		word_free (w);
		
		return NULL;
	}
	
	if (hash_table_insert (lex->hash, word, node))
		return NULL;
	
	htree_increment (lex->tree, lex->tree->new);
	htree_increment (lex->alpha->tree, lex->alpha->end);
	
	return node;
}

void
lex_update (Lex_t *lex, HNode_t *node)
{
	if (!lex || !node)
		return;
	
	htree_increment (lex->tree, node);
}

HNode_t *
lex_search (Lex_t *lex, const unsigned char *word, size_t len)
{
	HNode_t *node = NULL;
	
	if (!lex || !word)
		return node;
	
	while ((node = hash_table_search (lex->hash, word, node)))
		if ((WORD (node->data)->lenght == len) && !memcmp (WORD (node->data)->word, word, len))
			return node;
	
	return node;
}
