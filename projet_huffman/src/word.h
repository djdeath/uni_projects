#ifndef __WORD_H__
#define __WORD_H__

/* Permet de stocker des mots. Les mots sont des chaînes de caractères
 * quelconques et peuvent ne pas être terminées par le caractère '\0',
 * c'est pourquoi nous utilisons la longueur de cette chaîne pour
 * définir un mot. */

#define WORD(arg) ((Word_t *)(arg))

typedef struct _Word_t Word_t;

struct _Word_t
{
	char *word;
	size_t lenght;
};

/* Allocation/libération d'un mot. */
Word_t *word_new (const char *word, size_t len);
void word_free (Word_t *word);

#endif
