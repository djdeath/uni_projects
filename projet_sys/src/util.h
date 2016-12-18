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
 * Rechercher la 1ère occurence d'un caractère dans une chaîne.
 * 
 * @param str La chaîne de caractère dans laquelle on cherche
 * @param c   Le caractère à chercher
 * @param len La longueur de la chaîne
 * 
 * @return Un pointeur sur la 1er caractère égal à c.
 */
char *strnchr (char *str, char c, size_t len);

/** 
 * Copier une chaîne de caractère. 
 * 
 * @param str  La chaîne à copier
 * @param len  La longueur maximale à copier
 * @param rlen La longueur copiée
 * 
 * @return Une nouvelle chaîne de caractère allouée dynamiquement.
 */
char *strndup (const char *str, size_t len, size_t *rlen);

/** 
 * Ecrire sur un descipteur de manière formatée.
 * 
 * @param fd   Le descipteur sur lequel on va écrire
 * @param size La taille maximale à écrire
 * @param fmt  Un descripteur de format
 * @param args Une liste d'arguments
 * 
 * @return Le nombre d'octets écrits.
 */
int fdvnprintf (int fd, size_t size, const char *fmt, va_list args);

/** 
 * Ecrire sur un descipteur de manière formatée.
 * 
 * @param fd   Le descipteur sur lequel on va écrire
 * @param size La taille maximale à écrire
 * @param fmt  Un descripteur de format
 * 
 * @return Le nombre d'octets écrits.
 */
int fdnprintf (int fd, size_t size, const char *fmt, ...);

#endif
