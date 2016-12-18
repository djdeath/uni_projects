#include "main.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "hash_table.h"
#include "hash_func.h"
#include "hash_close.h"
#include "hash_open.h"

void
hash_table_free (HashTable_t *ht)
{
	if (ht)
	{
		if (ht->table)
		{
			if (ht->free)
				ht->free (ht);
			free (ht->table);
		}
		free (ht);
	}
}

HashTable_t *
vhash_table_new (unsigned int size, HashMode_t mode, va_list args)
{
	HashFunc_t func;
       	HashTable_t *ht = HASH_TABLE (calloc (1, sizeof (HashTable_t)));
	
	if (ht)
	{
		if (!(ht->table = calloc (size, sizeof (void *))))
		{
			free (ht);
			
			return NULL;
		}
		ht->size = size;
		ht->nb = 0U;
		
		func = va_arg (args, HashFunc_t);
		
		if (!(ht->hash1 = hash_num_get_func (func)))
		{
			hash_table_free (ht);
			
			return NULL;
		}
		
		switch (mode)
		{
		case HASH_MODE_OPEN:
			ht->insert = hash_table_open_insert;
			ht->search = hash_table_open_search;
			ht->foreach = hash_table_open_foreach;
			ht->free = hash_table_open_free;
			break;
			
		case HASH_MODE_CLOSE_LINEAR:
			ht->insert = hash_table_close_insert_linear;
			ht->search = hash_table_close_search_linear;
			ht->foreach = hash_table_close_foreach;
			break;
			
		case HASH_MODE_CLOSE_QUADRA:
			ht->insert = hash_table_close_insert_quadra;
			ht->search = hash_table_close_search_quadra;
			ht->foreach = hash_table_close_foreach;
			break;
			
		case HASH_MODE_CLOSE_DOUBLE:
			ht->insert = hash_table_close_insert_double;
			ht->search = hash_table_close_search_double;
			ht->foreach = hash_table_close_foreach;
			
			func = va_arg (args, HashFunc_t);
			if ((ht->hash2 = hash_num_get_func (func)))
				break;
			
		default:
			hash_table_free (ht);
			return NULL;
		}
	}
	
	return ht;
}

HashTable_t *
hash_table_new (unsigned int size, HashMode_t mode, ...)
{ 
       	va_list args;
	HashTable_t *ht;
	
	va_start (args, mode);
	
	ht = vhash_table_new (size, mode, args);
	
	va_end (args);
	
	return ht;
}

/**/

int
hash_table_insert (HashTable_t *ht, const void *key, void *data)
{
	int ret;
	
	/* L'insertion de data NULL est interdite, car la valeur est
	 * la valeur retournee par la fonction de recherche lorsque la
	 * recherche est terminee pour une valeur. */
	if (!ht)
		return EHASH_TABLE;
	if (!data)
		return EHASH_DATA;
	
	if (!(ret = ht->insert (ht, key, data)))
		ht->nb++;
	
	return ret;
}

void *
hash_table_search (HashTable_t *ht, const void *key, const void *ldata)
{
	if (!ht)
		return NULL;
	
	return ht->search (ht, key, ldata);
}

/**/

void
hash_table_foreach (HashTable_t *ht, ForeachCB_t func, void *udata)
{
	if (!ht || !func)
		return;
	
	ht->foreach (ht, func, udata);
}
