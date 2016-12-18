#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "sv_dict.h"

#define EDICT_FULL (200)
#define EDICT_WORD (201)

void
dict_free (Dict_t *dict)
{
	if (dict)
	{
		if (dict->words)
		{
			size_t i;
			
			for (i = 0U ; i < dict->num_words ; i++)
				word_free (dict->words[i]);
			free (dict->words);
		}
		free (dict);
	}
}

Dict_t *
dict_new (size_t max_words)
{
	Dict_t *dict = NULL;
	
	if (max_words && (dict = DICT (calloc (1, sizeof (Dict_t)))))
	{
		if ((dict->words = (Word_t **) calloc (max_words, sizeof (Word_t *))))
			dict->max_words = max_words;
		else
		{
			free (dict);
			dict = NULL;
		}
	}
	
	return dict;
}

static int
dict_add (Dict_t *dict, const char *str)
{
	if (!dict || !str || (*str == '\0'))
		return EDICT_WORD;
	
	if (dict->num_words >= dict->max_words)
		return EDICT_FULL;
	
	if (!(dict->words[dict->num_words] = word_new (str)))
		return EDICT_WORD;
	dict->num_words++;
	
	return 0;
}


#define BUF_READ_SIZE (512)

Dict_t *
dict_create (const char *file, size_t max_words)
{
	int fd;
	Dict_t *dict = NULL;
	
	if (file && (fd = open (file, O_RDONLY)) != -1)
	{
		if ((dict = dict_new (max_words)))
		{
			char buf[BUF_READ_SIZE], *s, *e;
			ssize_t len;
			
			s = buf;
			while ((len = read (fd, s, BUF_READ_SIZE - (s - buf))) > 0)
			{
				s = buf;
				e = s;
				
				do
				{
					e = s;
					while ((e < (buf + len)) && isalpha (*e))
						e++;
					
					if (e < (buf + len))
					{
						if (e < (buf + len))
						{
							*e = '\0';
							if (dict_add (dict, s) == EDICT_FULL)
							{
								close (fd);
								
								return dict;
							}
						}
						s = e + 1;
					}
				} while (e < (buf + len));
				
				if (s != e)
				{
					memmove (buf, s, e - s);
					s = buf + (e - s);
				}
			}
		}
		close (fd);
	}
	
	return dict;
}

Word_t *
dict_rand (Dict_t *dict)
{
	size_t u;
	
	if (!dict)
		return NULL;
	
	u = (size_t) rand ();
	u %= dict->num_words;
	
	return dict->words[u];
}
