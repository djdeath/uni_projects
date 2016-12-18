#ifndef __CL_UI_MAIN_H__
#define __CL_UI_MAIN_H__

/**
 * @file   cl_ui_main.h
 * @author [ATR]Dj-Death
 * @date   Sun Mar 21 05:11:11 2004
 * 
 * @brief  
 * 
 * 
 */

#include "cl_main.h"

#include <stdlib.h>
#include <gtk/gtk.h>

#include "config.h"

typedef struct _WindowMain_t WindowMain_t;

#include "cl_ui_play.h"
#include "cl_ui_chat.h"
#include "cl_ui_list.h"

#define SERVER_READ_BLOCK (1024)

struct _WindowMain_t
{
	GtkWidget *window;
	
	GtkWidget *play_box;
	GtkWidget *status;

	WindowPlay_t *play;
	WindowChat_t *chat;
	WindowList_t *list;
	
	/* Configuration */
	Config_t *config;
	
	/* Joueur */
	gchar *name, *team;
	
	/* Serveur */
	gchar *host;
	gint port;
	
	int fd;
	guint timeout;
	
	size_t ubuf_len;
	char buf[SERVER_READ_BLOCK];
};

/** 
 * 
 * 
 * 
 * @return 
 */
WindowMain_t *window_main_create (void);

/** 
 * 
 * 
 * @param win 
 */
void window_main_destroy (WindowMain_t *win);

/** 
 * 
 * 
 * @param win 
 * @param host 
 * @param port 
 */
void window_main_connect (WindowMain_t *win, const gchar *host, gint port);

/** 
 * 
 * 
 * @param win 
 */
void window_main_disconnect (WindowMain_t *win);

/** 
 * 
 * 
 * @param win 
 * @param fmt 
 */
void window_main_status_message (WindowMain_t *win, const gchar *fmt, ...);

/** 
 * 
 * 
 * @param win 
 * @param name 
 */
void window_main_set_name (WindowMain_t *win, const gchar *name);

/** 
 * 
 * 
 * @param win 
 * @param team 
 */
void window_main_set_team (WindowMain_t *win, const gchar *team);

#endif
