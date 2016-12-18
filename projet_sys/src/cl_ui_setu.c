#include "cl_util.h"

#include "cl_ui_setu.h"
#include "cl_ui_util.h"

typedef struct _WindowSetup_t WindowSetup_t;

struct _WindowSetup_t
{
	WindowMain_t *mwin;
	
	GtkWidget *window;
	GtkWidget *name_entry;
	GtkWidget *team_entry;
};

static gboolean
window_setup_destroyed (WindowSetup_t *win)
{
	g_free (win);
	
	return FALSE;
}

static void
window_setup_ok_clicked (WindowSetup_t *win)
{
	const gchar *uname, *uteam;
	gchar *name, *team;
	
	uname = gtk_entry_get_text (GTK_ENTRY (win->name_entry));
	uteam = gtk_entry_get_text (GTK_ENTRY (win->team_entry));
	
	/* utf-8 -> ascii */
	name = str_convert_to_ascii (uname);
	team = str_convert_to_ascii (uteam);
	
	window_main_set_name (win->mwin, name);
	window_main_set_team (win->mwin, team);
	
	g_free (name);
	g_free (team);
	
	gtk_widget_destroy (win->window);
}

void
window_setup_create (WindowMain_t *mwin)
{
	GtkWidget *vbox, *bbox, *button;
	WindowSetup_t *win;
	
	if (!mwin || !(win = g_new0 (WindowSetup_t, 1)))
		return;
	
	win->mwin = mwin;
	
	win->window = gtk_dialog_new ();
	gtk_window_set_title (GTK_WINDOW (win->window), "Setup");
	gtk_window_set_modal (GTK_WINDOW (win->window), TRUE);
	g_signal_connect_swapped (win->window, "destroy",
				  G_CALLBACK (window_setup_destroyed),
				  win);
	
	vbox = GTK_DIALOG (win->window)->vbox;
	gtk_container_set_border_width (GTK_CONTAINER (vbox), 5);
	
	win->name_entry = widget_entry_add_chars (GTK_BOX (vbox), "Nom", mwin->name, 64);
	win->team_entry = widget_entry_add_chars (GTK_BOX (vbox), "Equipe", mwin->team, 64);
	
	bbox = GTK_DIALOG (win->window)->action_area;
	gtk_container_set_border_width (GTK_CONTAINER (bbox), 5);
	
	button = gtk_button_new_from_stock (GTK_STOCK_EXECUTE);
	GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
	g_signal_connect_swapped (button, "clicked",
				  G_CALLBACK (window_setup_ok_clicked),
				  win);
	gtk_box_pack_start_defaults (GTK_BOX (bbox), button);
	gtk_widget_grab_default (button);
	
	button = gtk_button_new_from_stock (GTK_STOCK_CANCEL);
	GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
	g_signal_connect_swapped (button, "clicked",
				  G_CALLBACK (gtk_widget_destroy),
				  win->window);
	gtk_box_pack_start_defaults (GTK_BOX (bbox), button);
	
	gtk_widget_show_all (win->window);
}
