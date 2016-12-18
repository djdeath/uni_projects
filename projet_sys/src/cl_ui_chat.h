#ifndef __CL_UI_CHAT_H__
#define __CL_UI_CHAT_H__

/**
 * @file   cl_ui_chat.h
 * @author [ATR]Dj-Death
 * @date   Sun Mar 21 05:12:32 2004
 * 
 * @brief  
 * 
 * 
 */

#define WINDOW_CHAT(arg) ((WindowChat_t *)(arg))

typedef struct _WindowChat_t WindowChat_t;

#include "cl_ui_main.h"

struct _WindowChat_t
{
	WindowMain_t *mwin;
	
	GtkWidget *window;
	
	GtkWidget *messages;
	GtkWidget *list;
	GtkWidget *entry;
};

/** 
 * 
 * 
 * @param mwin 
 * 
 * @return 
 */
WindowChat_t *window_chat_create (WindowMain_t *mwin);

/** 
 * 
 * 
 * @param win 
 */
void window_chat_destroy (WindowChat_t *win);

/** 
 * 
 * 
 * @param win 
 */
void window_chat_switch_hide_show (WindowChat_t *win);

/** 
 * 
 * 
 * @param win 
 * @param player 
 */
void window_chat_player_update (WindowChat_t *win, const char *player);

/** 
 * 
 * 
 * @param win 
 * @param player 
 */
void window_chat_player_del (WindowChat_t *win, const char *player);

/** 
 * 
 * 
 * @param win 
 * @param sender 
 * @param message 
 */
void window_chat_message_add (WindowChat_t *win, const char *sender, const char *message);

/** 
 * 
 * 
 * @param win 
 */
void window_chat_clear (WindowChat_t *win);

#endif
