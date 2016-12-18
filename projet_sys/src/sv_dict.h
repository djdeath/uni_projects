#ifndef __SV_DICT_H__
#define __SV_DICT_H__

#include "hash_table.h"
#include "sv_word.h"

#define DICT(arg) ((Dict_t *)(arg))

typedef struct _Dict_t Dict_t;

struct _Dict_t
{
	size_t num_words, max_words;
	Word_t **words;
};

/** 
 * 
 * 
 * @param file 
 * @param max_words 
 * 
 * @return 
 */
Dict_t *dict_create (const char *file, size_t max_words);

/** 
 * 
 * 
 * @param max_words 
 * 
 * @return 
 */
Dict_t *dict_new (size_t max_words);

/** 
 * 
 * 
 * @param dict 
 */
void dict_free (Dict_t *dict);

/** 
 * 
 * 
 * @param dict 
 * 
 * @return 
 */
Word_t *dict_rand (Dict_t *dict);

#endif
