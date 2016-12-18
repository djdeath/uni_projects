#ifndef __HASH_CLOSE_H__
#define __HASH_CLOSE_H__

#include "hash_table.h"

/* Lineaire */

int hash_table_close_insert_linear (HashTable_t *ht, const void *key, void *data);
void *hash_table_close_search_linear (HashTable_t *ht, const void *key, const void *ldata);

/* Quadratique */

int hash_table_close_insert_quadra (HashTable_t *ht, const void *key, void *data);
void *hash_table_close_search_quadra (HashTable_t *ht, const void *key, const void *ldata);

/* Double */

int hash_table_close_insert_double (HashTable_t *ht, const void *key, void *data);
void *hash_table_close_search_double (HashTable_t *ht, const void *key, const void *ldata);

/* All */
void hash_table_close_foreach (HashTable_t *ht, ForeachCB_t func, void *udata);

#endif
