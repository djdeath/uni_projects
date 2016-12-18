#ifndef __SV_WORD_H__
#define __SV_WORD_H__

/**
 * @file   sv_word.h
 * @author [ATR]Dj-Death
 * @date   Sun Mar 21 04:43:39 2004
 * 
 * @brief  Une structure permettant d'associer une mot à un couple position,
 *         couleur.
 * 
 */


#include "sv_main.h"

#include <stdlib.h>

#define WORD(arg) ((Word_t *)(arg))

typedef struct _Word_t Word_t;

struct _Word_t
{
	char *word;
	size_t len;
	
	int color;
	int x, y;
};

/** 
 * Allouer une nouveau mot.
 * 
 * @param str Une chaîne de caractère décrivant ce mot.
 * 
 * @return Un pointeur sur une structure de mot allouée dynamiquement
 */
Word_t *word_new (const char *str);

/** 
 * Libérer une structrure de mot.
 * 
 * @param word Un pointeur sur une sctructure de mot.
 */
void word_free (Word_t *word);

#endif
