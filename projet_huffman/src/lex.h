#ifndef __LEX_H__
#define __LEX_H__

#include "hash_table.h"
#include "huff.h"
#include "alpha.h"

/* Cette structure désigne un lexique (ensemble de mot) et maintient à
 * jour un arbre de Huffman ou chaque feuille représente un mot
 * pondéré par son nombre d'occurrence. */

#define LEX(arg) ((Lex_t *)(arg))

typedef struct _Lex_t Lex_t;

struct _Lex_t
{
	HashTable_t *hash;
	HTree_t *tree;
	Alpha_t *alpha;
};

Lex_t *lex_new (void);
void lex_free (Lex_t *lex);

/* Ajoute un mot au lexique et retourne la feuille de l'arbre associée
 * à ce mot. */
HNode_t *lex_add (Lex_t *lex, const unsigned char *word, size_t len);

/* Incrémente le poids d'une feuille de l'arbre (et donc le nombre
 * d'occurence d'une feuille). */
void lex_update (Lex_t *lex, HNode_t *node);

/* Retourne la feuille de l'arbre associé à un mot. */
HNode_t *lex_search (Lex_t *lex, const unsigned char *word, size_t len);

#endif
