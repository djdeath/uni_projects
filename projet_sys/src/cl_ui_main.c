#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <unistd.h>

#include "network.h"
#include "util.h"
#include "cl_ui_main.h"
#include "cl_parse.h"
#include "cl_ui_menu.h"
#include "cl_util.h"

#define WINDOW_HEIGHT (400)
#define WINDOW_WIDTH (600)

#define CHECK_SOCKET_TIMEOUT (50)

/**/

void
window_main_destroy (WindowMain_t *win)
{
	if (win)
	{
		window_main_disconnect (win);
		
		config_save ("cl.cfg", win->config);
		config_free (win->config);
		
		/*window_play_destroy (win->play);*/
		window_chat_destroy (win->chat);
		window_list_destroy (win->list);
		
		g_free (win);
	}
}

#define MAX_STATUS (256)

void
window_main_status_message (WindowMain_t *win, const gchar *fmt, ...)
{
	gint cont;
	gchar buf[MAX_STATUS];
	gchar *ubuf;
	va_list list;
	
	va_start (list, fmt);
	vsnprintf (buf, MAX_STATUS, fmt, list);
	va_end (list);
	
	ubuf = str_convert_to_utf8 (buf);
	
	cont = gtk_statusbar_get_context_id (GTK_STATUSBAR (win->status), "status");
	gtk_statusbar_pop (GTK_STATUSBAR (win->status), cont);
	gtk_statusbar_push (GTK_STATUSBAR (win->status), cont, ubuf);
	g_free (ubuf);
}

static gboolean
window_main_destroyed (WindowMain_t *win)
{
	window_main_destroy (win);
	gtk_main_quit ();
	
	return TRUE;
}

WindowMain_t *
window_main_create (void)
{
	WindowMain_t *win = g_new0 (WindowMain_t, 1);
	
	if (win)
	{
		win->fd = -1;
		
		win->config = config_load ("cl.cfg");
		
		win->window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
		gtk_window_set_default_size (GTK_WINDOW (win->window),
					     WINDOW_WIDTH, WINDOW_HEIGHT);
		gtk_window_set_title (GTK_WINDOW (win->window), "Play");
		gtk_window_set_role (GTK_WINDOW (win->window), "play");
		gtk_widget_realize (win->window);
		g_signal_connect_swapped (win->window, "destroy",
					  G_CALLBACK (window_main_destroyed),
					  win);
		
		win->play_box = gtk_vbox_new (FALSE, 0);
		gtk_container_add (GTK_CONTAINER (win->window), win->play_box);
		
		gtk_box_pack_start (GTK_BOX (win->play_box), window_main_menu_create (win),
				    FALSE, FALSE, 0);
		
		if (!(win->play = window_play_create (win)) ||
		    !(win->chat = window_chat_create (win)) ||
		    !(win->list = window_list_create (win)))
		{
			window_main_destroy (win);
			win = NULL;
		}
		else
		{
			win->status = gtk_statusbar_new ();
			gtk_box_pack_start (GTK_BOX (win->play_box), win->status, FALSE, FALSE, 0);
			
			window_chat_switch_hide_show (win->chat);
			window_list_switch_hide_show (win->list);
			
			window_main_status_message (win, "Not connected");
			
			gtk_widget_show_all (win->window);
		}
		
		window_main_set_name (win, config_get_str (win->config, "name"));
		window_main_set_team (win, config_get_str (win->config, "team"));
	}
	
	return win;
}

void
window_main_set_name (WindowMain_t *win, const gchar *name)
{
	gchar *tmp;
	
	if (!win || !name)
		return;
	
	if (*name == '\0')
		return;
	
	if (!(tmp = strdup (name)))
		return;
	if (win->name)
		g_free (win->name);
	win->name = tmp;
	config_set_str (win->config, "name", win->name);
	
	server_send (win, "'%s\n", name);
}

void
window_main_set_team (WindowMain_t *win, const gchar *team)
{
	gchar *tmp;
	
	if (!win || !team)
		return;
	
	if (!(tmp = strdup (team)))
		return;
	if (win->team)
		g_free (win->team);
	win->team = tmp;
	config_set_str (win->config, "team", win->team);
	
	server_send (win, "\"%s\n", team);
}

void
window_main_connect (WindowMain_t *win, const gchar *host, gint port)
{
	if (!win || !host || (port < 1))
		return;
	
	window_main_disconnect (win);
	
	if (win->host)
		g_free (win->host);
	if (!(win->host = strdup (host)))
		return;
	win->port = port;
	
	if ((win->fd = net_connect (host, port)) != -1)
	{
		window_chat_clear (win->chat);
		window_list_clear (win->list);
		window_play_clear (win->play);
		
		win->timeout = g_timeout_add (CHECK_SOCKET_TIMEOUT,
					      (GSourceFunc) server_recv, win);
		if (win->name && (*win->name != '\0'))
		{
			if (win->team && (*win->team != '\0'))
				server_send (win, "'%s\n\"%s\n", win->name, win->team);
			else
				server_send (win, "'%s\n", win->name);
		}
		
		window_main_status_message (win, "Connection sur %s:%i", host, port);
	}
	else
		window_main_status_message (win, "Connection impossible sur %s:%i", host, port);
}

void
window_main_disconnect (WindowMain_t *win)
{
	if (win)
	{
		if (win->timeout > 0)
		{
			g_source_remove (win->timeout);
			win->timeout = 0U;
		}
		if (win->fd != -1)
		{
			net_close (win->fd);
			win->fd = -1;
		}
		window_main_status_message (win, "Not connected");
	}
}
