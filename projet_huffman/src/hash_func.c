#include "main.h"

#include "hash_func.h"

static unsigned int hash_oreilly (const void *key);
static unsigned int hash_glib (const void *key);
static unsigned int hash_pourrave (const void *key);

/**/

HashFuncCB_t
hash_num_get_func (HashFunc_t func)
{
	switch (func)
	{
	case HASH_FUNC_POURRAVE:
		return HASH_FUNC_CB (hash_pourrave);
		
	case HASH_FUNC_GLIB:
		return HASH_FUNC_CB (hash_glib);
		
	case HASH_FUNC_OREILLY:
		return HASH_FUNC_CB (hash_oreilly);
		
	case HASH_FUNC_NONE:
		return NULL;
	}
	
	return NULL;
}

HashFunc_t
hash_func_get_num (HashFuncCB_t func)
{
	if (func == hash_pourrave)
		return HASH_FUNC_POURRAVE;
	else if (func == hash_glib)
		return HASH_FUNC_GLIB;
	else if (func == hash_oreilly)
		return HASH_FUNC_OREILLY;
	
	return HASH_FUNC_NONE;
}

/**/

static unsigned int
hash_oreilly (const void *key)
{
	const char *ptr;
	int val;
	int tmp;
	
	val = 0;
	ptr = key;
	while (*ptr != '\0')
	{
		val = (val << 4) + (*ptr);
		
		if ((tmp = (val & 0xf0000000)))
		{
			val = val ^ (tmp >> 24);
			val = val ^ tmp;
		}
		
		ptr++;
	}
	
	return val;
}

static unsigned int
hash_glib (const void *key)
{
	const char *p = key;
	unsigned int h = (unsigned int) *p;
	
	if (h)
		for (p += 1 ; *p != '\0' ; p++)
			h = (h << 5) - h + *p;
	
	return h;
}

static unsigned int
hash_pourrave (const void *key)
{
	const char *p = key;
	unsigned int i, h = (unsigned int) *p;
	
	for (i = 1 ; *p != '\0' ; i++, p++)
		h += i * ((unsigned int) (*p));
	
	return h;
}
