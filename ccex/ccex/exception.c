#include <string.h>
#include <stdlib.h>

#include "exception.h"
#include "context.h"

#define GETBIT(x, i) (((x) >> (i)) & 1)
#define SETBIT(x, i, v) ((x) = ((v) == 0 ? ((x) & (~(1 << (i)))) : ((x) | (1 << (i)))))

static ExceptionTree_t *tree;

static ExceptionNode_t *
exception_node_new (const char *name)
{
	ExceptionNode_t *node;
	
	if (!name)
		return NULL;
	
	if (!(node = calloc (1, sizeof (ExceptionNode_t))))
		return node;
	
	if (!(node->name = strdup (name)))
	{
		free (node);
		return NULL;
	}
	
	return node;
}

static void
exception_node_free (ExceptionNode_t *node)
{
	if (node)
	{
		int i;
		
		for (i = 0 ; i < EXCEPTION_TREE_WIDTH ; i++)
			exception_node_free (node->children[i]);
		
		if (node->name)
			free (node->name);
		
		free (node);
	}
}

static int
exception_node_extend (ExceptionNode_t *parent, ExceptionNode_t *child)
{
	int i, j;
	
	if (!parent || !child)
		return 0;
	
	/* On cherche si il y a une place pour étendre l'exception
	 * parent à l'exception child. */
	for (i = 0 ; (i < EXCEPTION_TREE_WIDTH) && parent->children[i] ; i++);
	
	
	if (i >= EXCEPTION_TREE_WIDTH)
		return 0;
	
	/* Met à jour la structure d'arbre */
	parent->children[i] = child;
	child->parent = parent;
	
	/* Attribution du numéro d'exception. */
	child->number = parent->number;
	
	j = 0;
	parent = child;
	while (parent->parent)
	{
		parent = parent->parent;
		j++;
	}
	j--;
	
	SETBIT (child->number, i * EXCEPTION_TREE_HEIGHT + j, 1);
	
	return 1;
}

/**/

static void
exception_tree_free (ExceptionTree_t *tree)
{
	if (tree)
	{
		if (tree->exceptions)
			hash_table_free (tree->exceptions);
		
		exception_node_free (EXCEPTION_NODE (tree));
	}
}

static ExceptionTree_t *
exception_tree_new (const char *base)
{
	ExceptionTree_t *tr;
	
	if (!base)
		return NULL;
	
	if ((tr = calloc (1, sizeof (ExceptionTree_t))))
	{
		if (!(tr->exceptions = hash_table_new (HASH_TABLE_CLOSE_DEFAULT_SIZE,
							 HASH_MODE_OPEN, HASH_FUNC_GLIB)))
		{
			free (tr);
			return NULL;
		}
		
		if (!(EXCEPTION_NODE (tr)->name = strdup (base)))
		{
			exception_tree_free (tr);
			return NULL;
		}
		
		if (hash_table_insert (tr->exceptions, base, tr))
		{
			exception_tree_free (tr);
			return NULL;
		}
	}
	
	return tr;
}

ExceptionNode_t*
exception_tree_extend (ExceptionNode_t *parent,
		       const char *child)
{
	ExceptionNode_t *cnode;
	
	if (!tree || !parent || !child)
		return NULL;
	
	if (!(cnode = exception_node_new (child)))
		return NULL;
	
	if (!exception_node_extend (parent, cnode) ||
	    hash_table_insert (tree->exceptions, child, cnode))
	{
		exception_node_free (cnode);
		
		return NULL;
	}
	
	return cnode;
}

ExceptionNode_t*
exception_tree_extend_named (const char *parent,
			     const char *child)
{
	ExceptionNode_t *pnode;
	
	if (!tree || !parent || !child)
		return NULL;
	
	pnode = NULL;
	while ((pnode = hash_table_search (tree->exceptions, parent, pnode)))
	{
		if (!strcmp (pnode->name, parent))
			break;
	}
	
	return exception_tree_extend (pnode, child);
}

ExceptionNode_t*
exception_tree_get_node (const char* name)
{
	ExceptionNode_t *pnode;
	
	if (!tree)
		return NULL;
	
	pnode = NULL;
	while ((pnode = hash_table_search (tree->exceptions, name, pnode)))
	{
		if (!strcmp (pnode->name, name))
			return pnode;
	}
	
	return NULL;
}

/**/

void
exception_init (void)
{
	tree = exception_tree_new ("Exception");
	context_add_except ("Exception");
	
	exception_tree_extend_named ("Exception", "ArrayException");
	exception_tree_extend_named ("ArrayException", "ArrayIndexOutOfBoundsException");
	exception_tree_extend_named ("ArrayException", "BadDimensionException");
	exception_tree_extend_named ("ArrayException", "OutOfMemoryException");
	exception_tree_extend_named ("ArrayException", "NullArrayException");
	
	
	exception_tree_extend_named ("Exception", "ArithmeticException");
	exception_tree_extend_named ("ArithmeticException", "DivisionByZeroException");
	exception_tree_extend_named ("ArithmeticException", "OverflowException");
	exception_tree_extend_named ("ArithmeticException", "NumberFormatException");
	exception_tree_extend_named ("NumberFormatException", "SuckerException");
}

void
exception_end (void)
{
	exception_tree_free (tree);
	tree = NULL;
}

#if 0

#include <stdio.h>

static void
print_tree (FILE *f, ExceptionNode_t *node)
{
	int i;
	
	if (!node)
		return;
	
	fprintf (f, "#define %s (0x%x)\n", node->name, node->number);
	
	for (i = 0 ; i < EXCEPTION_TREE_WIDTH ; i++)
		print_tree (f, node->children[i]);
}

int
main (void)
{
	ExceptionTree_t *tree;
	FILE *out;
	
	if ((tree = exception_tree_new ("Exception")))
	{
		exception_tree_extend_named (tree, "Exception", "ArrayException");
		exception_tree_extend_named (tree, "ArrayException", "ArrayIndexOutOfBoundsException");
		exception_tree_extend_named (tree, "ArrayException", "BadDimensionException");
		exception_tree_extend_named (tree, "ArrayException", "OutOfMemoryException");
		exception_tree_extend_named (tree, "ArrayException", "NullArrayException");
		
		
		exception_tree_extend_named (tree, "Exception", "ArithmeticException");
		exception_tree_extend_named (tree, "ArithmeticException", "DivisionByZeroException");
		exception_tree_extend_named (tree, "ArithmeticException", "OverflowException");
		exception_tree_extend_named (tree, "ArithmeticException", "NumberFormatException");
		exception_tree_extend_named (tree, "NumberFormatException", "SuckerException");
		
		if ((out = fopen ("defs_exceptions.h", "w")))
		{
			print_tree (out, EXCEPTION_NODE (tree));
			fclose (out);
		}
		exception_tree_free (tree);
	}
	
	return EXIT_SUCCESS;
}

#endif
