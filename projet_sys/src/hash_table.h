#ifndef __HASH_TABLE_H__
#define __HASH_TABLE_H__

#include "sv_main.h"

#include <stdlib.h>
#include <stdarg.h>

#include "list.h"

#define HASH_TABLE(arg) ((HashTable_t *)(arg))
#define HASH_FUNC_CB(arg) ((HashFuncCB_t)(arg))

#define HASH_TABLE_OPEN_DEFAULT_SIZE (50023)
#define HASH_TABLE_CLOSE_DEFAULT_SIZE (5023)

#define EHASH_TABLE 125 /* Table invalide */
#define EHASH_KEY   126 /* Cl�e invalide */
#define EHASH_DATA  127 /* Donn�e invalide */
/* Erreurs relatives aux tables ferm�es */
#define EHASH_FULL  128 /* La table de hachage est pleine. */
/* Erreurs relatives aux tables ouvertes */
#define EHASH_OMEM  129 /* Impossible d'allouer la place n�cessaire
                         * pour ajouter un nouvel �l�ment. */
#define EHASH_FUNC  130 /* Fonctionnalit� non impl�ment�e. */

/* Modes de hachage disponibles */
typedef enum
{
	HASH_MODE_OPEN = 0,
	HASH_MODE_CLOSE_LINEAR,
	HASH_MODE_CLOSE_QUADRA,
	HASH_MODE_CLOSE_DOUBLE
} HashMode_t;

/* Fonctions de hachage disponibles */
typedef enum
{
	HASH_FUNC_NONE = 0,
	HASH_FUNC_POURRAVE,
	HASH_FUNC_GLIB,
	HASH_FUNC_OREILLY
} HashFunc_t;

typedef unsigned int (* HashFuncCB_t) (const void *);

typedef struct _HashTable_t HashTable_t;

struct _HashTable_t
{
	void **table;
	unsigned int size;
	unsigned int nb;
	
	void (* free) (struct _HashTable_t *ht);
	
	/* 2 fonctions de hachage (la deuxieme sert uniquement dans le
	 * cas d'une table de hachage fermee double) */
       	HashFuncCB_t hash1;
	HashFuncCB_t hash2;
	
	/* Fonction d'ajout */
	int (* insert) (struct _HashTable_t *ht, const void *key, void *data);
	int (* remove) (struct _HashTable_t *ht, const void *key, void *data);
	
	/* Fonction de recherche */
	void * (* search) (struct _HashTable_t *ht, const void *key, const void *ldata);
	
	/* Execution d'une fonction sur tous les elements de la
	 * table */
	void (* foreach) (struct _HashTable_t *ht, Foreach_t func, void *udata);
};

/* Allocation et lib�ration d'une table de hachage. */
HashTable_t *hash_table_new (unsigned int size, HashMode_t mode, ...);
HashTable_t *vhash_table_new (unsigned int size, HashMode_t mode, va_list args);
void hash_table_free (HashTable_t *ht);

/* Vider la table de ses informations. */
void hash_table_clear (HashTable_t *ht);

/* Nombre d'elements contenus dans une table. */
#define hash_table_nb(arg) ((arg) == NULL ? 0 : (arg)->nb)

/* Insertion d'un element dans une table de hachage. Retourne 0 si
 * l'insertion r�ussit, un code d'erreur sinon. */
int hash_table_insert (HashTable_t *ht, const void *key, void *data);

/* Retrait d'un �l�ment dans une table de hachage. Retourne 0 si
 * l'insertion r�ussit, un code d'erreur sinon. */
int hash_table_remove (HashTable_t *ht, const void *key, void *data);

/* Rechercher d'un element dans une table. Cette fonction ne renvoit
 * qu'un seul element a la fois, il convient donc de l'appeler
 * successivement jusqu'a ce qu'elle retourne NULL. */
void *hash_table_search (HashTable_t *ht, const void *key, const void *ldata);

/* Appelle la fonction func pour chaque element de la table avec
 * l'element et udata en argument. */
void hash_table_foreach (HashTable_t *ht, Foreach_t func, void *udata);

#endif
