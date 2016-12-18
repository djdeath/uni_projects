#ifndef __HASH_OPEN_H__
#define __HASH_OPEN_H__

#include "hash_table.h"

/* Libere toutes les listes de la table de hachage. */
void hash_table_open_free (HashTable_t *ht);

int hash_table_open_insert (HashTable_t *ht, const void *key, void *data);
void *hash_table_open_search (HashTable_t *ht, const void *key, const void *ldata);

void hash_table_open_foreach (HashTable_t *ht, ForeachCB_t func, void *udata);

#endif
