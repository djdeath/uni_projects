#ifndef __CL_UI_PLAY_H__
#define __CL_UI_PLAY_H__

/**
 * @file   cl_ui_play.h
 * @author [ATR]Dj-Death
 * @date   Sun Mar 21 05:10:30 2004
 * 
 * @brief  
 * 
 * 
 */

#include "hash_table.h"

#define WINDOW_PLAY(arg) ((WindowPlay_t *)(arg))

typedef struct _WindowPlay_t WindowPlay_t;

#include "cl_ui_main.h"

struct _WindowPlay_t
{
	WindowMain_t *mwin;
	
	GtkWidget *play_area;
	GtkWidget *entry;
	
	gboolean configured : 1;
	
	gint width, height;
	gint c_width, c_height;
	
	HashTable_t *ht;
	GdkGC *gc;
	PangoLayout *font;
	GdkColor background;
};

/** 
 * 
 * 
 * @param mwin 
 * 
 * @return 
 */
WindowPlay_t *window_play_create (WindowMain_t *mwin);

/** 
 * 
 * 
 * @param win 
 */
void window_play_destroy (WindowPlay_t *win);

/** 
 * 
 * 
 * @param win 
 * @param word 
 * @param x 
 * @param y 
 */
void window_play_word_add (WindowPlay_t *win,
			   const gchar *word,
			   gint x, gint y);

/** 
 * 
 * 
 * @param win 
 * @param word 
 * @param x 
 * @param y 
 */
void window_play_word_del (WindowPlay_t *win,
			   const gchar *word,
			   gint x, gint y);

/** 
 * 
 * 
 * @param win 
 * @param width 
 * @param height 
 */
void window_play_set_size (WindowPlay_t *win,
			   gint width,
			   gint height);

/** 
 * 
 * 
 * @param win 
 */
void window_play_clear (WindowPlay_t *win);

#endif
