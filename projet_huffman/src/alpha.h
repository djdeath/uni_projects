#ifndef __ALPHA_H__
#define __ALPHA_H__

#include "hash_table.h"
#include "huff.h"

/* Cette structure permet de garder un arbre de Huffman associé aux
 * fréquences des lettres. Nous utilisons un codage des lettres par
 * dessus le codage des mots. Cela permet d'avoir un meilleur taux de
 * compression en évitant d'écrire toutes les lettres des nouveaux
 * mots que l'on lit. */

#define ALPHA(arg) ((Alpha_t *)(arg))
#define LETTER(arg) ((Letter_t *)(arg))

typedef struct _Alpha_t Alpha_t;
typedef struct _Letter_t Letter_t;

struct _Letter_t
{
	unsigned char letter;
	HNode_t *node;
};

struct _Alpha_t
{
	HTree_t *tree;
	HNode_t *end; /* Feuille qui nous permet d'avoir un code
		       * permettant d'identifier la fin d'un mot (au
		       * lieu d'ecrire le nombre de lettre avant le
		       * mot. */
	Letter_t *letters; /* Alphabet associé à un lexique. */
};

Alpha_t *alpha_new (void);
void alpha_free (Alpha_t *al);

HNode_t *alpha_add (Alpha_t *al, unsigned char c);
void alpha_update (Alpha_t *al, HNode_t *node);

HNode_t *alpha_search (Alpha_t *al, unsigned char c);

#endif
