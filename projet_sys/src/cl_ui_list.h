#ifndef __CL_UI_PLAYER_H__
#define __CL_UI_PLAYER_H__

/**
 * @file   cl_ui_list.h
 * @author [ATR]Dj-Death
 * @date   Sun Mar 21 05:11:46 2004
 * 
 * @brief  
 * 
 * 
 */

#define WINDOW_LIST(arg) ((WindowList_t *)(arg))

typedef struct _WindowList_t WindowList_t;

#include "cl_ui_main.h"

struct _WindowList_t
{
	WindowMain_t *mwin;
	
	GtkWidget *window;
	
	GtkWidget *player_list;
	GtkWidget *team_list;
};

/** 
 * 
 * 
 * @param mwin 
 * 
 * @return 
 */
WindowList_t *window_list_create (WindowMain_t *mwin);

/** 
 * 
 * 
 * @param win 
 */
void window_list_destroy (WindowList_t *win);

/** 
 * 
 * 
 * @param win 
 */
void window_list_switch_hide_show (WindowList_t *win);

/** 
 * 
 * 
 * @param win 
 * @param player 
 */
void window_list_player_del (WindowList_t *win,
			     const gchar *player);

/** 
 * 
 * 
 * @param win 
 * @param player 
 * @param score 
 */
void window_list_player_update (WindowList_t *win,
				const gchar *player,
				int score);

/** 
 * 
 * 
 * @param win 
 * @param team 
 */
void window_list_team_del (WindowList_t *win,
			   const gchar *team);

/** 
 * 
 * 
 * @param win 
 * @param team 
 * @param score 
 */
void window_list_team_update (WindowList_t *win,
			      const gchar *team,
			      int score);

/** 
 * 
 * 
 * @param win 
 */
void window_list_clear (WindowList_t *win);

#endif
