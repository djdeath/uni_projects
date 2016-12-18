#include "cl_ui_menu.h"
#include "cl_ui_conn.h"
#include "cl_ui_setu.h"
#include "cl_ui_util.h"

static void
_menu_quit (WindowMain_t *win, guint action, GtkWidget *widget)
{
	gtk_widget_destroy (win->window);
	gtk_main_quit ();
}

static void
_menu_connect (WindowMain_t *win, guint action, GtkWidget *widget)
{
	window_connect_create (win);
}

static void
_menu_disconnect (WindowMain_t *win, guint action, GtkWidget *widget)
{
	window_main_disconnect (win);
}

static void
_menu_player (WindowMain_t *win, guint action, GtkWidget *widget)
{
	window_setup_create (win);
}

static void
_menu_list (WindowMain_t *win, guint action, GtkWidget *widget)
{
	window_list_switch_hide_show (win->list);
}

static void
_menu_chat (WindowMain_t *win, guint action, GtkWidget *widget)
{
	window_chat_switch_hide_show (win->chat);
}

static GtkItemFactoryEntry menu_items[] =
{
	
	{ "/_Fichier", "<ALT>F", NULL, 0, "<Branch>", NULL},
	
	{ "/_Fichier/tear1", NULL, NULL, 0, "<Tearoff>", NULL},
	{ "/_Fichier/Se _Connecter", "<CTRL>C", _menu_connect, 1, "<StockItem>", GTK_STOCK_EXECUTE},
	{ "/_Fichier/Se _Deconnecter", "<CTRL>D", _menu_disconnect, 1, "<StockItem>", GTK_STOCK_CLOSE},
	{ "/_Fichier/sep1", NULL, NULL, 0, "<Separator>", NULL},
	{ "/_Fichier/_Quitter", "<CTRL>Q", _menu_quit, 0, "<StockItem>", GTK_STOCK_QUIT},
	
	
	{ "/_Options", "<ALT>O", NULL, 0, "<Branch>", NULL},
	
	{ "/_Options/tear1", NULL, NULL, 0, "<Tearoff>", NULL},
	{ "/_Options/_Joueur", "<CTRL>J", _menu_player, 0, "<StockItem>", GTK_STOCK_PROPERTIES},
	
	
	{ "/_Autres", "<ALT>A", NULL, 0, "<Branch>", NULL},
	
	{ "/_Autres/tear1", NULL, NULL, 0, "<Tearoff>", NULL},
	{ "/_Autres/Liste des joueurs", NULL, _menu_list, 0, "<StockItem>", GTK_STOCK_GO_BACK},
	{ "/_Autres/Fenetre de discution", NULL, _menu_chat, 0, "<StockItem>", GTK_STOCK_GO_FORWARD}
};

GtkWidget *
window_main_menu_create (WindowMain_t *win)
{
	GtkWidget *menubar;
	
	menubar = gtk_menu_from_factoryentry (menu_items, GTK_TYPE_MENU_BAR,
					      sizeof (menu_items) / sizeof (menu_items[0]),
					      win->window, win);
	gtk_widget_show_all (menubar);
	
	return menubar;
}
