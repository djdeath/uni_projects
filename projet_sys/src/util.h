#ifndef __UTIL_H__
#define __UTIL_H__

/**
 * @file   util.h
 * @author [ATR]Dj-Death
 * @date   Sun Mar 21 04:36:10 2004
 * 
 * @brief  Quelques fonctions utile un peu partout dans le programme.
 * 
 * 
 */

#include "sv_main.h"

#include <stdlib.h>
#include <stdarg.h>

/** 
 * Rechercher la 1�re occurence d'un caract�re dans une cha�ne.
 * 
 * @param str La cha�ne de caract�re dans laquelle on cherche
 * @param c   Le caract�re � chercher
 * @param len La longueur de la cha�ne
 * 
 * @return Un pointeur sur la 1er caract�re �gal � c.
 */
char *strnchr (char *str, char c, size_t len);

/** 
 * Copier une cha�ne de caract�re. 
 * 
 * @param str  La cha�ne � copier
 * @param len  La longueur maximale � copier
 * @param rlen La longueur copi�e
 * 
 * @return Une nouvelle cha�ne de caract�re allou�e dynamiquement.
 */
char *strndup (const char *str, size_t len, size_t *rlen);

/** 
 * Ecrire sur un descipteur de mani�re format�e.
 * 
 * @param fd   Le descipteur sur lequel on va �crire
 * @param size La taille maximale � �crire
 * @param fmt  Un descripteur de format
 * @param args Une liste d'arguments
 * 
 * @return Le nombre d'octets �crits.
 */
int fdvnprintf (int fd, size_t size, const char *fmt, va_list args);

/** 
 * Ecrire sur un descipteur de mani�re format�e.
 * 
 * @param fd   Le descipteur sur lequel on va �crire
 * @param size La taille maximale � �crire
 * @param fmt  Un descripteur de format
 * 
 * @return Le nombre d'octets �crits.
 */
int fdnprintf (int fd, size_t size, const char *fmt, ...);

#endif
