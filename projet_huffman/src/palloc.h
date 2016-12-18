#ifndef __PALLOC_H__
#define __PALLOC_H__

#include <stdlib.h>

/* Fonctions de base d'un allocateur dynamique de m�moire. Cet
 * allocateur utilise des page de 5000 �l�ments (biensur cette valeur
 * est modifiable) de m�me taille, il devra donc �tre utilis� pour
 * allouer un grand nombre de zone m�moire de m�me taille (listes,
 * arbres, etc ...) */

/* Retourne un pointeur sur une zone m�moire de la taille demand�e ou
 * NULL en cas d'erreur. */
void *pmalloc (size_t size);

/* Retourne un pointeur sur une zone m�moire remplie avec des z�ros de
 * la taille demand�e ou NULL en cas d'erreur. */
void *pcalloc (size_t size);

/* Lib�re une zone m�moire allou�e par cet allocateur. */
void pfree (void *ptr);

#endif
