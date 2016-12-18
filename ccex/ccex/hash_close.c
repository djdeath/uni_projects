#include <string.h>
#include <errno.h>
#include <limits.h>

#include "list.h"
#include "hash_close.h"
#include "hash_func.h"

static int hash_table_close_is_full (const HashTable_t *ht);

/**/

int
hash_table_close_insert_linear (HashTable_t *ht, const void *key, void *data)
{
	unsigned int i, j = 0;
	
	if (hash_table_close_is_full (ht))
		return EHASH_FULL;
	
	i = ht->hash1 (key);
	
	while (ht->table[(i + j) % ht->size] != NULL && j < ht->size)
		j++;
	
	if (j >= ht->size)
		return EHASH_FULL;
	
	ht->table[(i + j) % ht->size] = data;
	
        return 0;
}

void *
hash_table_close_search_linear (HashTable_t *ht, const void *key, const void *ldata)
{
	unsigned int i;
	static unsigned int j = UINT_MAX, k = 0U;
	static void *data = NULL;
	
        i = ht->hash1 (key);
	
	if (ldata && (data == ldata) && (i == j))
	{
		if (ht->table[(i + k) % ht->size])
		{
			k++;
			i = (i + k) % ht->size;
			
			if ((k > ht->size) || !ht->table[i])
			{
				k = 0U;
				j = UINT_MAX;
				return (data = NULL);
			}
			
			return (data = ht->table[i]);
		}
	}
	j = i;
	k = 0;
	
	return (data = ht->table[i % ht->size]);
}

/**/

int
hash_table_close_insert_quadra (HashTable_t *ht, const void *key, void *data)
{
	unsigned int i, j = 0;
        
	if (hash_table_close_is_full (ht))
		return EHASH_FULL;
	
        i = ht->hash1 (key) % ht->size;
        
	while ((ht->table[(i + j * j) % ht->size] != NULL) && (j < ht->size))
		j++;
	
        if (j >= ht->size)
                return EHASH_FULL;
	
        ht->table[(i + j * j) % ht->size] = data;
	
        return 0;
}

void *
hash_table_close_search_quadra (HashTable_t *ht, const void *key, const void *ldata)
{
	unsigned int i;
        static unsigned j = UINT_MAX, k = 0U;
	static void *data = NULL;
	
	i = ht->hash1 (key);
	
	if (ldata && (data == ldata) && (i == j))
	{
		if (ht->table[(i + k * k) % ht->size])
		{
			k++;
			i = (i + k * k) % ht->size;
			
			if ((k > ht->size) || !ht->table[i])
			{
				k = 0U;
				j = UINT_MAX;
				
				return (data = NULL);
			}
			
			return (data = ht->table[i]);
		}
	}
	j = i;
	k = 0U;
	
        return (data = ht->table[i % ht->size]);
}

/**/

int
hash_table_close_insert_double (HashTable_t *ht, const void *key, void *data)
{
	unsigned int i, h, j = 0U;
	
	if (hash_table_close_is_full (ht))
		return EHASH_FULL;
	
        i = ht->hash1 (key);
        h = ht->hash2 (key);
	
        while ((ht->table[(i + j * h) % ht->size]) && (j < ht->size))
		j++;
	
        if (j >= ht->size)
                return EHASH_FULL;
        
	ht->table[(i + j * h) % ht->size] = data;
        
        return 0;
}

void *
hash_table_close_search_double (HashTable_t *ht, const void *key, const void *ldata)
{
	unsigned int i;
        static unsigned j = UINT_MAX, k = 0U;
	static void *data = NULL;
	
	i = ht->hash1 (key);
	
	if (ldata && (data == ldata) && (i == j))
	{
		k++;
		i = (j + k * ht->hash2 (key)) % ht->size;
		
		if ((k > ht->size) || !ht->table[i])
		{
			k = 0U;
			j = UINT_MAX;
			
			return (data = NULL);
		}
		
		return (data = ht->table[i]);
	}
	j = i;
	k = 0U;
	
        return (data = ht->table[i % ht->size]);
}

/**/

void
hash_table_close_free (HashTable_t *ht)
{
	memset (ht->table, 0, ht->size);
}

void
hash_table_close_foreach (HashTable_t *ht, Foreach_t func, void *udata)
{
	unsigned int i;
       	
	for (i = 0U ; i < ht->size ; i++)
		if (ht->table[i])
			func (ht->table[i], udata);
}

static int
hash_table_close_is_full (const HashTable_t *ht)
{
	if (((ht->nb * 100) / ht->size) > 80)
		return 1;
	
	return 0;
}
