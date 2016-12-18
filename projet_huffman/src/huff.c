#include "main.h"

#include <stdlib.h>

#include "palloc.h"
#include "huff.h"

#define HUFF_BLOCK(arg) ((HBlock_t *)(arg))

/**/

static HBlock_t *
hblock_new (HNode_t *leader, unsigned int weight)
{
	HBlock_t *block = HUFF_BLOCK (pmalloc (sizeof (HBlock_t)));
	
	if (block)
	{
		block->count = 1;
		
		block->weight = weight;
		block->leader = leader;
	}
	
	return block;
}

static HBlock_t *
hblock_ref (HBlock_t *block)
{
	if (block)
		block->count++;
	
	return block;
}

static void
hblock_unref (HBlock_t *block)
{
	if (block)
	{
		block->count--;
		if (!block->count)
			pfree (block);
	}
}

/**/

static HNode_t *
hnode_new (void *data)
{
	HNode_t *node = HUFF_NODE (pcalloc (sizeof (HNode_t)));
	
	if (node)
		node->data = data;
	
	return node;
}

static void
hnode_free (HNode_t *node)
{
	HNode_t *tmp;
	
	while (node)
	{
		tmp = node;
		node = node->prev;
		hblock_unref (tmp->block);
		pfree (tmp);
	}
}

HTree_t *
htree_new (void)
{
	HTree_t *tree = HUFF_TREE (calloc (1, sizeof (HTree_t)));
	
	if (tree)
	{
		if ((tree->head = hnode_new (NULL)))
		{
			tree->null = tree->head;
			tree->null->block = hblock_new (tree->null, 0U);
			tree->width = 1;
			
			if (!(tree->new = htree_add (tree, NULL)))
			{
				htree_free (tree);
				tree = NULL;
			}
		}
		else
		{
			htree_free (tree);
			tree = NULL;
		}
	}
	
	return tree;
}

void
htree_free (HTree_t *tree)
{
	if (tree)
	{	
		if (tree->head)
			hnode_free (tree->head);
		free (tree);
	}
}

/**/

/* Echange 2 node de même poids. */
static void
swap_nodes (HNode_t *node1, HNode_t *node2)
{
	HNode_t *prev, *next;
	
	/* Yeah, I won't do that stupid thing... */
	if ((node1 == node2) || (node1->parent == node2))
		return;
	
	/* Liste */
	prev = node1->prev;
	next = node1->next;
	
	if (node1->next == node2)
	{
		node1->next = node2->next;
		node1->next->prev = node1;
		node2->prev = node1->prev;
		node2->prev->next = node2;
		node2->next = node1;
		node1->prev = node2;
	}
	else
	{
		node1->prev = node2->prev;
		node1->prev->next = node1;
		node1->next = node2->next;
		node1->next->prev = node1;
		node2->next = next;
		next->prev = node2;
		node2->prev = prev;
		prev->next = node2;
	}
	node1->block->leader = node1;
	
	
	/* Arbre */
	next = node1->parent;
	
	node1->parent = node2->parent;
	if (node1->parent->left == node2)
		node1->parent->left = node1;
	else
		node1->parent->right = node1;
	
	node2->parent = next;
	if (next->left == node1)
		next->left = node2;
	else
		next->right = node2;
}

static void
update_block (HNode_t *node)
{
	HNode_t *next, *prev;
	
	prev = node->prev;
	next = node->next;
	
	if (next && (node->block->weight == (next->block->weight - 1)))
	{
		if (prev->block->leader == node)
			prev->block->leader = prev;
		hblock_unref (node->block);
		node->block = hblock_ref (next->block);
	}
	else
	{
		if (prev->block->leader == node)
		{
			prev->block->leader = prev;
			hblock_unref (node->block);
			node->block = hblock_new (node, prev->block->weight + 1);
		}
		else
			node->block->weight++;
	}
}

void
htree_increment (HTree_t *tree, HNode_t *node)
{
	HNode_t *forward;
	
	if (!tree || !node)
		return;
	
	while (node->parent)
	{
		/* On échange le node courant avec le plus grand
		 * node de même poids dans la liste. */
		forward = node->block->leader;
	
		swap_nodes (node, forward);
		update_block (node);
		
		/* On passe au père et on fait de même. */
		node = node->parent;
	}
	update_block (node);
	
	tree->head = node;
}

/**/

static void
insert_next (HNode_t *node, HNode_t *next)
{
	node->next = next;
	next->prev = node;
}

HNode_t *
htree_add (HTree_t *tree, void *data)
{
	HNode_t *new;
	
	if (!tree)
		return NULL;
	
	if ((new = hnode_new (data)))
	{
		HNode_t *parent;
		
		if ((parent = hnode_new (NULL)))
		{
			/* Reconstruction de l'arbre avec deux
			 * nouveaux nodes à la place de la feuille
			 * null. */
			if (tree->null->next)
			{
				insert_next (parent, tree->null->next);
				parent->parent = tree->null->parent;
				if (parent->parent->left == tree->null)
					parent->parent->left = parent;
				else
					parent->parent->right = parent;
			}
			insert_next (new, parent);
			insert_next (tree->null, new);
			parent->left = tree->null;
			parent->right = new;
			tree->null->parent = parent;
			new->parent = parent;
			
			if (parent->next && (parent->next->block->weight == 1))
				parent->block = hblock_ref (parent->next->block);
			else
				parent->block = hblock_new (parent, 1);
			new->block = hblock_ref (parent->block);
			
			if (parent->parent)
				htree_increment (tree, parent->parent);
			else
				tree->head = parent;
			
			tree->width++;
		}
		else
		{
			hnode_free (new);
			new = NULL;
		}
	}
	
	return new;
}

/**/

void
htree_foreach (HTree_t *tree, ForeachCB_t func, void *udata)
{
	HNode_t *list;
	
	if (!tree)
		return;
	
	list = tree->null;
	
	while (list)
	{
		if (list->data)
			func (list->data, udata);
		list = list->next;
	}
}
