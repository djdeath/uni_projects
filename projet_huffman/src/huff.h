#ifndef __HUFF_H__
#define __HUFF_H__

#include "data.h"

#define HUFF_TREE(arg) ((HTree_t *)(arg))
#define HUFF_NODE(arg) ((HNode_t *)(arg))

typedef struct _HTree_t HTree_t;
typedef struct _HNode_t HNode_t;
typedef struct _HBlock_t HBlock_t;

/* La structure block sert à regrouper les noeuds de même poids, et
 * garde un pointeur sur le haut noeud droit de même poids dans
 * l'arbre. C'est ce noeud qui sert à mettre à jour l'arbre de
 * Huffman. */
struct _HBlock_t {
	unsigned int count;
	
	unsigned int weight;
	
	HNode_t *leader;
};

/* Structure d'un noeud de l'arbre. parent, left et right sont
 * respectivement des pointeurs sur le noeud parent, fils gauche et
 * fils droit. prev et next sont des pointeurs sur d'autres noeud et
 * permettent de former une liste de tous les noeuds par ordre
 * croissant. block est un pointeur sur la structure ci dessus. */
struct _HNode_t
{
	HNode_t *parent;
	HNode_t *left;
	HNode_t *right;
	
	HNode_t *prev;
	HNode_t *next;
	HBlock_t *block;
	
	void *data;
};

/* L'arbre */
struct _HTree_t
{
	HNode_t *head; /* Pointeur sur la racine. */
	
	HNode_t *null; /* Pointeur sur la feuille nulle qui est
			* l'endroit ou l'on insère une nouvelle
			* feuile. */
	HNode_t *new; /* Pointeur sur la feuille new qui permet de
		       * déterminer que l'on doit lire un mot ne
		       * figurant pas encore dans l'arbre. */
	
	int width; /* Nombre de feuilles. */
};

HTree_t *htree_new (void);
void htree_free (HTree_t *tree);

/* Ajoute une feuille de poids 1 et maintient l'arbre minimal. */
HNode_t *htree_add (HTree_t *tree, void *data);

/* Incrémente le poids d'une feuille de 1 et maintient l'arbre
 * minimal. */
void htree_increment (HTree_t *tree, HNode_t *node);

/* Appelle la fonction func sur chaque élément "accroché" à une feuille de l'arbre. */
void htree_foreach (HTree_t *tree, ForeachCB_t func, void *udata);

#endif

