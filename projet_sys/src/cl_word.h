#ifndef __CL_WORD_H__
#define __CL_WORD_H__

/**
 * @file   cl_word.h
 * @author [ATR]Dj-Death
 * @date   Sun Mar 21 05:09:32 2004
 * 
 * @brief  
 * 
 * 
 */

#include "cl_main.h"
#include <gtk/gtk.h>

#define WORD(arg) ((Word_t *)(arg))

typedef struct _Word_t Word_t;

struct _Word_t
{
	gchar *word;
	gint x, y;
	size_t len;
	
	gint layout_width, layout_height;
	PangoLayout *layout;
	GdkColor color;
};

/** 
 * 
 * 
 * @param str 
 * 
 * @return 
 */
Word_t *word_new (const char *str);

/** 
 * 
 * 
 * @param word 
 */
void word_free (Word_t *word);

#endif
