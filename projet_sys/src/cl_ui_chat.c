#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gdk/gdkkeysyms.h>

#include "cl_ui_chat.h"
#include "cl_parse.h"
#include "cl_util.h"

#define WINDOW_HEIGHT (300)
#define WINDOW_WIDTH (450)

enum
{
	CHAT_NAME = 0,
	CHAT_COLUMNS
};

void
window_chat_destroy (WindowChat_t *win)
{
	if (win)
	{
		gtk_widget_destroy (win->window);
		g_free (win);
	}
}

static gboolean
window_chat_hided (WindowChat_t *win)
{
	gtk_widget_hide (win->window);
	
	return TRUE;
}

static void
window_chat_send (GtkTreeModel *model,
		  GtkTreePath *path,
		  GtkTreeIter *selected,
		  WindowChat_t *win)
{
	const gchar *umessage;
	gchar *player, *uplayer, *message;
	
	umessage = gtk_entry_get_text (GTK_ENTRY (win->entry));
	gtk_tree_model_get (model, selected, CHAT_NAME, &uplayer, -1);
	
	message = str_convert_to_ascii (umessage);
	player = str_convert_to_ascii (uplayer);
	
	server_send (win->mwin, ">%s:%s\n", player, message);
	
	g_free (uplayer);
	g_free (player);
	g_free (message);
}

static gboolean
window_chat_entry_pressed (GtkWidget *entry, GdkEventKey *event, WindowChat_t *win)
{
	if (event->keyval == GDK_Return)
	{
		GtkTreeSelection *selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (win->list));
		
		if (gtk_tree_selection_count_selected_rows (selection) < 1)
		{
			const gchar *umessage;
			
			if ((umessage = gtk_entry_get_text (GTK_ENTRY (win->entry))) && (*umessage != '\0'))
			{
				gchar *message;
				
				message = str_convert_to_ascii (umessage);
				
				server_send (win->mwin, ">:%s\n", message);
				
				g_free (message);
			}
		}
		else
			gtk_tree_selection_selected_foreach (selection,
							     (GtkTreeSelectionForeachFunc) window_chat_send,
							     win);
		gtk_entry_set_text (GTK_ENTRY (win->entry), "");
	}
	
	return FALSE;
}

WindowChat_t *
window_chat_create (WindowMain_t *mwin)
{
	WindowChat_t *win;
	
	if (!mwin)
		return NULL;
	
	if ((win = g_new0 (WindowChat_t, 1)))
	{
		GType store_types[CHAT_COLUMNS] = { G_TYPE_STRING };
		GtkWidget *vbox, *paned, *scrolled;
		GtkListStore *store;
		GtkTreeViewColumn *column;
		GtkTreeSelection *selection;
		
		win->mwin = mwin;
		
		win->window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
		gtk_window_set_default_size (GTK_WINDOW (win->window),
					     WINDOW_WIDTH, WINDOW_HEIGHT);
		gtk_window_set_title (GTK_WINDOW (win->window), "Chat");
		gtk_window_set_role (GTK_WINDOW (win->window), "chat");
		g_signal_connect_swapped (win->window, "delete-event",
					  G_CALLBACK (window_chat_hided),
					  win);
		
		paned = gtk_hpaned_new ();
		gtk_container_add (GTK_CONTAINER (win->window), paned);
		
		vbox = gtk_vbox_new (FALSE, 0);
		gtk_paned_pack1 (GTK_PANED (paned), vbox, TRUE, FALSE);
		
		/* Zone de texte déjà tapé */
		scrolled = gtk_scrolled_window_new (NULL, NULL);
		gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled),
						GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
		gtk_scrolled_window_set_placement (GTK_SCROLLED_WINDOW (scrolled),
						   GTK_CORNER_TOP_RIGHT);
		gtk_box_pack_start_defaults (GTK_BOX (vbox), scrolled);
		
		win->messages = gtk_text_view_new ();
		gtk_text_view_set_editable (GTK_TEXT_VIEW (win->messages), FALSE);
		gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (win->messages), GTK_WRAP_CHAR);
		gtk_text_view_set_cursor_visible (GTK_TEXT_VIEW (win->messages), FALSE);
		gtk_container_add (GTK_CONTAINER (scrolled), win->messages);
		
		/* Entree de texte */
		win->entry = gtk_entry_new_with_max_length (1000);
		gtk_box_pack_start (GTK_BOX (vbox), win->entry, FALSE, FALSE, 5);
		g_signal_connect (win->entry, "key-press-event",
				  G_CALLBACK (window_chat_entry_pressed),
				  win);
		
		/* Liste des joueurs */
		scrolled = gtk_scrolled_window_new (NULL, NULL);
		gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled),
						GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
		gtk_scrolled_window_set_placement (GTK_SCROLLED_WINDOW (scrolled),
						   GTK_CORNER_TOP_LEFT);
		gtk_paned_pack2 (GTK_PANED (paned), scrolled, TRUE, TRUE);
		
		store = gtk_list_store_newv (CHAT_COLUMNS, store_types);
		win->list = gtk_tree_view_new_with_model (GTK_TREE_MODEL (store));
		g_object_unref (G_OBJECT (store));
		
		column = gtk_tree_view_column_new_with_attributes ("Joueurs", gtk_cell_renderer_text_new (),
								   "text", CHAT_NAME, NULL);
		gtk_tree_view_column_set_sort_column_id (column, CHAT_NAME);
		gtk_tree_view_append_column (GTK_TREE_VIEW (win->list), column);
		gtk_container_add (GTK_CONTAINER (scrolled), win->list);
		
		selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (win->list));
		gtk_tree_selection_set_mode (selection, GTK_SELECTION_MULTIPLE);
		
		gtk_widget_show_all (paned);
	}
	
	return win;
}

void
window_chat_switch_hide_show (WindowChat_t *win)
{
	if (!win)
		return;
	
	if (GTK_WIDGET_VISIBLE (win->window))
		gtk_widget_hide (win->window);
	else
		gtk_widget_show (win->window);
}

static gboolean
_window_chat_search_player (GtkTreeModel *model,
			    GtkTreeIter *iter,
			    const gchar *name)
{
	gboolean ret;
	gchar *in_name;
        
        ret = gtk_tree_model_get_iter_first (model, iter);
        
        while (ret)
        {
                gtk_tree_model_get (model, iter, CHAT_NAME, &in_name, -1);
                if (in_name && !strcmp (in_name, name))
                {
			g_free (in_name);
			return TRUE;
		}
		g_free (in_name);
                ret = gtk_tree_model_iter_next (model, iter);
        }
        
        return FALSE;
}

void
window_chat_player_update (WindowChat_t *win,
			   const gchar *player)
{
	GtkTreeIter iter;
	GtkTreeModel *model;
	
	if (!win || !player)
		return;
	
	model = gtk_tree_view_get_model (GTK_TREE_VIEW (win->list));
	if (_window_chat_search_player (model, &iter, player))
		return;
	
	gtk_list_store_append (GTK_LIST_STORE (model), &iter);
	gtk_list_store_set (GTK_LIST_STORE (model), &iter,
			    CHAT_NAME, player, -1);
}

void
window_chat_player_del (WindowChat_t *win,
			const gchar *player)
{
	GtkTreeIter iter;
	GtkTreeModel *model;
	
	if (!win || !player)
		return;
	
	model = gtk_tree_view_get_model (GTK_TREE_VIEW (win->list));
	if (!_window_chat_search_player (model, &iter, player))
		return;
	
	gtk_list_store_remove (GTK_LIST_STORE (model), &iter);
}

void
window_chat_message_add (WindowChat_t *win,
			 const gchar *sender,
			 const gchar *message)
{
	GtkTextBuffer *buf;
	GtkTextIter iter;
	GtkTextMark *mark;
        gchar *utext, *text;
	
	if (!win || !sender || !message)
		return;
	
	text = g_strdup_printf ("\n<%s> %s", sender, message);
	utext = str_convert_to_utf8 (text);
	g_free (text);
	
        buf = gtk_text_view_get_buffer (GTK_TEXT_VIEW (win->messages));
        gtk_text_buffer_get_end_iter (buf, &iter);
	mark = gtk_text_buffer_create_mark (buf, NULL, &iter, FALSE);
	
        gtk_text_buffer_insert (buf, &iter, utext, -1);
	g_free (utext);
	
	gtk_text_view_scroll_to_mark (GTK_TEXT_VIEW (win->messages), mark, 0, TRUE, 0.0, 1.0);
	gtk_text_buffer_delete_mark (buf, mark);
}

void
window_chat_clear (WindowChat_t *win)
{
	if (win)
	{
		GtkListStore *store;
		GtkTextBuffer *buf;
		
		store = GTK_LIST_STORE (gtk_tree_view_get_model (GTK_TREE_VIEW (win->list)));
		gtk_list_store_clear (store);
		
		buf = gtk_text_view_get_buffer (GTK_TEXT_VIEW (win->messages));
		gtk_text_buffer_set_text (buf, "", 0);
	}
}
