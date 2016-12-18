#include "main.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "word.h"
#include "list.h"
#include "palloc.h"

void
word_free (Word_t *word)
{
	if (word)
	{
		if (word->word)
			free (word->word);
		pfree (word);
	}
}

Word_t *
word_new (const char *word, size_t len)
{
	Word_t *w;
	
	if (!word)
		return NULL;
	
	if ((w = WORD (pcalloc (sizeof (Word_t)))))
	{
		w->lenght = len;
		
		if (len)
		{
			if (!(w->word = malloc (w->lenght)))
			{
				word_free (w);
				w = NULL;
			}
			else
				memcpy (w->word, word, len);
		}
	}
	
	return w;
}
