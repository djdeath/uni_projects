/*#include <string.h>*/
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
		{
			list_free_all (ht->table[i]);
			ht->table[i] = NULL;
		}
}

/**/

int
hash_table_open_insert (HashTable_t *ht, const void *key, void *data)
{
	unsigned int i;
	
	i = ht->hash1 (key) % ht->size;
	
	if (!list_insert_first ((List_t **) (&(ht->table[i])), data))
		return EHASH_OMEM;
	
	return 0;
}

int
hash_table_open_remove (HashTable_t *ht, const void *key, void *data)
{
	unsigned int i;
	List_t *l;
	
	i = ht->hash1 (key) % ht->size;
	
	if (list_find_d (LIST (ht->table[i]), data, &l) >= 0)
	{
		if (!list_delete ((List_t **) (&(ht->table[i])), l))
			return 0;
	}
	
	return EHASH_DATA;
}

void *
hash_table_open_search (HashTable_t *ht, const void *key, const void *ldata)
{
	unsigned int i;
	static unsigned int j = UINT_MAX;
	static void *data = NULL;
	static List_t *pl = NULL;
	
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
hash_table_open_foreach (HashTable_t *ht, Foreach_t func, void *udata)
{
	unsigned int i;
	List_t *l;
	
	for (i = 0U ; i < ht->size ; i++)
		if ((l = ht->table[i]))
			while (l)
			{
				func (l->data, udata);
				l = l->next;
			}
}
