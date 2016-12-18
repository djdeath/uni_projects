#include "main.h"

#include <errno.h>
#include <string.h>
#include <limits.h>

#include "list.h"
#include "hash_open.h"
#include "hash_func.h"

void
hash_table_open_free (HashTable_t *ht)
{
	unsigned int i;
	
	for (i = 0U ; i < ht->size ; i++)
		if (ht->table[i])
			slist_free_all (ht->table[i]);
}

/**/

int
hash_table_open_insert (HashTable_t *ht, const void *key, void *data)
{
	unsigned int i;
	
	i = ht->hash1 (key) % ht->size;
	
	if (!slist_insert_first ((SList_t **) (&(ht->table[i])), data))
		return EHASH_OMEM;
	
	return 0;
}

void *
hash_table_open_search (HashTable_t *ht, const void *key, const void *ldata)
{
	unsigned int i;
	static unsigned int j = UINT_MAX;
	static void *data = NULL;
	static SList_t *pl = NULL;
	
	i = ht->hash1 (key) % ht->size;
	
	if (ldata && (data == ldata) && (i == j))
	{
		if (pl)
		{
			pl = pl->next;
			
			if (pl)
				return (data = pl->data);
			
			return (data = NULL);
		}
	}
	j = i;
	pl = ht->table[i];
	
	if (pl)
		return (data = pl->data);
	
	return (data = NULL);
}

/**/

void
hash_table_open_foreach (HashTable_t *ht, ForeachCB_t func, void *udata)
{
	unsigned int i;
	SList_t *l;
	
	for (i = 0U ; i < ht->size ; i++)
		if ((l = ht->table[i]))
			while (l)
			{
				func (l->data, udata);
				l = l->next;
			}
}
