#ifndef __PALLOC_H__
#define __PALLOC_H__

#include <stdlib.h>

/* Fonctions de base d'un allocateur dynamique de mémoire. Cet
 * allocateur utilise des page de 5000 éléments (biensur cette valeur
 * est modifiable) de même taille, il devra donc être utilisé pour
 * allouer un grand nombre de zone mémoire de même taille (listes,
 * arbres, etc ...) */

/* Retourne un pointeur sur une zone mémoire de la taille demandée ou
 * NULL en cas d'erreur. */
void *pmalloc (size_t size);

/* Retourne un pointeur sur une zone mémoire remplie avec des zéros de
 * la taille demandée ou NULL en cas d'erreur. */
void *pcalloc (size_t size);

/* Libère une zone mémoire allouée par cet allocateur. */
void pfree (void *ptr);

#endif
