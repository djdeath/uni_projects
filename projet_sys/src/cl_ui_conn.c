#include "cl_util.h"

#include "cl_ui_conn.h"
#include "cl_ui_util.h"

typedef struct _WindowConn_t WindowConn_t;

struct _WindowConn_t
{
	WindowMain_t *mwin;
	
	GtkWidget *window;
	GtkWidget *host_entry;
	GtkWidget *port_spin;
};

static gboolean
window_connect_destroyed (WindowConn_t *conn)
{
	g_free (conn);
	
	return FALSE;
}

static void
window_connect_ok_clicked (WindowConn_t *win)
{
	const gchar *tmp;
	gchar *host;
	gint port;
	
	tmp = gtk_entry_get_text (GTK_ENTRY (win->host_entry));
	port = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (win->port_spin));
	
	/* utf-8 -> ascii */
	host = str_convert_to_ascii (tmp);
	
	config_set_str (win->mwin->config, "sv_host", host);
	config_set_int (win->mwin->config, "sv_port", port);
	
	window_main_connect (win->mwin, host, port);
	g_free (host);
	
	gtk_widget_destroy (win->window);
}

void
window_connect_create (WindowMain_t *mwin)
{
	WindowConn_t *win;
	GtkWidget *vbox, *hbox, *bbox, *image, *button;
	GtkWidget *label;
	
	if (!mwin || !(win = g_new0 (WindowConn_t, 1)))
		return;
	
	win->mwin = mwin;
	
	win->window = gtk_dialog_new ();
	gtk_window_set_title (GTK_WINDOW (win->window), "Connection");
	gtk_window_set_modal (GTK_WINDOW (win->window), TRUE);
	g_signal_connect_swapped (win->window, "destroy",
				  G_CALLBACK (window_connect_destroyed),
				  win);
	
	vbox = GTK_DIALOG (win->window)->vbox;
	gtk_container_set_border_width (GTK_CONTAINER (vbox), 5);
	
	bbox = GTK_DIALOG (win->window)->action_area;
	gtk_container_set_border_width (GTK_CONTAINER (bbox), 5);
	
	hbox = gtk_hbox_new (FALSE, 5);
	gtk_box_pack_start_defaults (GTK_BOX (vbox), hbox);
	
	image = gtk_image_new_from_stock (GTK_STOCK_EXECUTE, GTK_ICON_SIZE_DIALOG);
	gtk_box_pack_start (GTK_BOX (hbox), image, FALSE, FALSE, 5);
	
	label = gtk_label_new ("Connection au serveur");
	gtk_label_set_line_wrap (GTK_LABEL (label), TRUE);
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 5);
	
	win->host_entry = widget_entry_add_chars (GTK_BOX (vbox), "Adresse serveur",
						  config_get_str (mwin->config, "sv_host"), 256);
	win->port_spin = widget_spin_add_int (GTK_BOX (vbox), "Port serveur",
					      config_get_int (mwin->config, "sv_port"), 65535, 1, 1);
	
	button = gtk_button_new_from_stock (GTK_STOCK_EXECUTE);
	GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
	g_signal_connect_swapped (button, "clicked",
				  G_CALLBACK (window_connect_ok_clicked),
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
