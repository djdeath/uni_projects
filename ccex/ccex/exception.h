#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

/**
 * @file   exception.h
 * @author [ATR]Dj-Death
 * @date   Wed Mar 24 17:48:47 2004
 * 
 * @brief  
 * 
 * 
 */

#include "hash_table.h"

#define EXCEPTION_TREE_HEIGHT (8)
#define EXCEPTION_TREE_WIDTH (4)

#define EXCEPTION_TREE(arg) ((ExceptionTree_t *)(arg))
#define EXCEPTION_NODE(arg) ((ExceptionNode_t *)(arg))

#define EXCEPTION_EXTENDS(parent,child) (((parent) | (child)) == (parent) ? 1 : 0)

typedef struct _ExceptionTree_t ExceptionTree_t;
typedef struct _ExceptionNode_t ExceptionNode_t;

struct _ExceptionNode_t
{
	char *name;
	unsigned int number;
	
	ExceptionNode_t *parent;
	ExceptionNode_t *children[EXCEPTION_TREE_WIDTH];
};

struct _ExceptionTree_t
{
	ExceptionNode_t base;
	
	HashTable_t *exceptions;
};

/** 
 * Ajouter une exception à un arbre.
 * 
 * @param tree   L'arbre des exceptions.
 * @param parent Le node parent.
 * @param child  Le nom de l'enfant.
 * 
 * @return 1 si l'ajout réussit, 0 sinon.
 */
ExceptionNode_t*
exception_tree_extend (ExceptionNode_t* parent,
		       const char* child);

/** 
 * Ajouter une exception à un arbre.
 * 
 * @param tree   L'arbre auquel on doit ajouter l'exception.
 * @param parent Nom de l'exception parente de celle que l'on
 *               souhaite ajouter.
 * @param child  Nom de la nouvelle exception.
 * 
 * @return 1 si l'ajout réussit, 0 sinon.
 */
ExceptionNode_t*
exception_tree_extend_named (const char* parent,
			     const char* child);

/** 
 * Rechercher une exception dans l'arbre avec son nom.
 * 
 * @param name 
 * 
 * @return 
 */
ExceptionNode_t*
exception_tree_get_node (const char* name);

/** 
 * Initialisation du système d'attribution des nombres au exceptions.
 * 
 */
void
exception_init (void);

/** 
 * Libération.
 * 
 */
void
exception_end (void);

#endif
