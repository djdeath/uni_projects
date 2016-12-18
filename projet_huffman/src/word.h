#ifndef __WORD_H__
#define __WORD_H__

/* Permet de stocker des mots. Les mots sont des cha�nes de caract�res
 * quelconques et peuvent ne pas �tre termin�es par le caract�re '\0',
 * c'est pourquoi nous utilisons la longueur de cette cha�ne pour
 * d�finir un mot. */

#define WORD(arg) ((Word_t *)(arg))

typedef struct _Word_t Word_t;

struct _Word_t
{
	char *word;
	size_t lenght;
};

/* Allocation/lib�ration d'un mot. */
Word_t *word_new (const char *word, size_t len);
void word_free (Word_t *word);

#endif
