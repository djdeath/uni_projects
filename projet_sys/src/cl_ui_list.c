#include <string.h>

#include "cl_ui_list.h"
#include "cl_ui_util.h"

#define WINDOW_HEIGHT (250)
#define WINDOW_WIDTH (150)

enum
{
	LIST_NAME = 0,
	LIST_SCORE,
	LIST_COLUMNS
};

void
window_list_destroy (WindowList_t *win)
{
	if (win)
	{
		gtk_widget_destroy (win->window);
		g_free (win);
	}
}

static gboolean
window_list_hided (WindowList_t *win)
{
	gtk_widget_hide (win->window);
	
	return TRUE;
}

WindowList_t *
window_list_create (WindowMain_t *mwin)
{
	WindowList_t *win;
	
	if (!mwin)
		return NULL;
	
	if ((win = g_new0 (WindowList_t, 1)))
	{
		GType store_types[LIST_COLUMNS] = { G_TYPE_STRING, G_TYPE_INT };
		GtkWidget *vbox, *scrolled;
		GtkListStore *store;
		GtkTreeViewColumn *column;
		
		win->mwin = mwin;
		
		win->window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
		gtk_window_set_default_size (GTK_WINDOW (win->window),
					     WINDOW_WIDTH, WINDOW_HEIGHT);
		gtk_window_set_title (GTK_WINDOW (win->window), "Liste");
		gtk_window_set_role (GTK_WINDOW (win->window), "list");
		g_signal_connect_swapped (win->window, "delete-event",
					  G_CALLBACK (window_list_hided),
					  win);
		
		vbox = gtk_vbox_new (FALSE, 5);
		gtk_container_add (GTK_CONTAINER (win->window), vbox);
		
		/* Liste des joueurs */
		scrolled = gtk_scrolled_window_new (NULL, NULL);
		gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled),
						GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
		gtk_scrolled_window_set_placement (GTK_SCROLLED_WINDOW (scrolled),
						   GTK_CORNER_TOP_RIGHT);
		gtk_box_pack_start (GTK_BOX (vbox), scrolled, TRUE, TRUE, 5);
		
		store = gtk_list_store_newv (LIST_COLUMNS, store_types);
		win->player_list = gtk_tree_view_new_with_model (GTK_TREE_MODEL (store));
		g_object_unref (G_OBJECT (store));
		gtk_container_add (GTK_CONTAINER (scrolled), win->player_list);
		
		column = gtk_tree_view_column_new_with_attributes ("Joueurs", gtk_cell_renderer_text_new (),
								   "text", LIST_NAME, NULL);
		gtk_tree_view_append_column (GTK_TREE_VIEW (win->player_list), column);
		
		column = gtk_tree_view_column_new_with_attributes ("Scores", gtk_cell_renderer_text_new (),
								   "text", LIST_SCORE, NULL);
		gtk_tree_view_append_column (GTK_TREE_VIEW (win->player_list), column);
		
		/* Liste des équipes */
		scrolled = gtk_scrolled_window_new (NULL, NULL);
		gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled),
						GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
		gtk_scrolled_window_set_placement (GTK_SCROLLED_WINDOW (scrolled),
						   GTK_CORNER_TOP_RIGHT);
		gtk_box_pack_start (GTK_BOX (vbox), scrolled, TRUE, TRUE, 5);
		
		store = gtk_list_store_newv (LIST_COLUMNS, store_types);
		win->team_list = gtk_tree_view_new_with_model (GTK_TREE_MODEL (store));
		g_object_unref (G_OBJECT (store));
		gtk_container_add (GTK_CONTAINER (scrolled), win->team_list);
		
		column = gtk_tree_view_column_new_with_attributes ("Equipes", gtk_cell_renderer_text_new (),
								   "text", LIST_NAME, NULL);
		gtk_tree_view_append_column (GTK_TREE_VIEW (win->team_list), column);
		
		column = gtk_tree_view_column_new_with_attributes ("Scores", gtk_cell_renderer_text_new (),
								   "text", LIST_SCORE, NULL);
		gtk_tree_view_append_column (GTK_TREE_VIEW (win->team_list), column);
		
		gtk_widget_show_all (vbox);
	}
	
	return win;
}

void
window_list_switch_hide_show (WindowList_t *win)
{
	if (!win)
		return;
	
	if (GTK_WIDGET_VISIBLE (win->window))
		gtk_widget_hide (win->window);
	else
		gtk_widget_show (win->window);
}

/**/

static gboolean
_window_list_search (GtkTreeModel *model,
		     GtkTreeIter *iter,
		     const gchar *name)
{
	gboolean ret;
	gchar *in_name;
        
        ret = gtk_tree_model_get_iter_first (model, iter);
        
        while (ret)
        {
                gtk_tree_model_get (model, iter, LIST_NAME, &in_name, -1);
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

static void
window_list_update (GtkTreeModel *model,
		    const gchar *name,
		    int score)
{
	GtkTreeIter iter;
	
	if (_window_list_search (model, &iter, name))
	{
		gtk_list_store_set (GTK_LIST_STORE (model), &iter,
				    LIST_SCORE, score, -1);
	}
	else
	{
		gtk_list_store_append (GTK_LIST_STORE (model), &iter);
		gtk_list_store_set (GTK_LIST_STORE (model), &iter,
				    LIST_NAME, name, LIST_SCORE, score, -1);
	}
}

static void
window_list_del (GtkTreeModel *model,
		 const gchar *name)
{
	GtkTreeIter iter;
	
	if (!_window_list_search (model, &iter, name))
		return;
	
	gtk_list_store_remove (GTK_LIST_STORE (model), &iter);
}

/**/

void
window_list_player_del (WindowList_t *win,
			const gchar *player)
{
	if (win && player)
		window_list_del (gtk_tree_view_get_model (GTK_TREE_VIEW (win->player_list)),
				 player);
}

void
window_list_player_update (WindowList_t *win,
			   const gchar *player,
			   int score)
{
	if (win && player)
		window_list_update (gtk_tree_view_get_model (GTK_TREE_VIEW (win->player_list)),
				    player, score);
}

void
window_list_team_del (WindowList_t *win,
		      const gchar *team)
{
	if (win && team)
		window_list_del (gtk_tree_view_get_model (GTK_TREE_VIEW (win->team_list)),
				 team);
}

void
window_list_team_update (WindowList_t *win,
			 const gchar *team,
			 int score)
{
	if (win && team)
		window_list_update (gtk_tree_view_get_model (GTK_TREE_VIEW (win->team_list)),
				    team, score);
}

void
window_list_clear (WindowList_t *win)
{
	if (win)
	{
		GtkListStore *store;
		
		store = GTK_LIST_STORE (gtk_tree_view_get_model (GTK_TREE_VIEW (win->player_list)));
		gtk_list_store_clear (store);
		
		store = GTK_LIST_STORE (gtk_tree_view_get_model (GTK_TREE_VIEW (win->team_list)));
		gtk_list_store_clear (store);
	}
}
