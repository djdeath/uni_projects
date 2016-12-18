#include "util.h"
#include "cl_word.h"

#define MAX_WORD_LENGTH (32)

Word_t *
word_new (const char *str)
{
	Word_t *w = NULL;
	
       	if (!str || (w = WORD (calloc (1, sizeof (Word_t)))))
	{
		if (!(w->word = strndup (str, MAX_WORD_LENGTH, &w->len)))
		{
			free (w);
			w = NULL;
		}
	}
	
	return w;
}

void
word_free (Word_t *word)
{
	if (word)
	{
		if (word->word)
			free (word->word);
		free (word);
	}
}
