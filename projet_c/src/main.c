#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "picture.h"
#include "quadpic.h"
#include "quadcomp.h"

#include "load_ppm.h"
#include "load_qtc.h"
#include "save_ppm.h"
#include "save_qtc.h"

#include "ui_main.h"

/**/

void
ui_print_error (const char *fmt, ...)
{
	va_list args;
	
	gchar *mess;
	GtkWidget *window, *vbox, *hbox, *bbox;
	GtkWidget *icon, *label, *button;
	
	if (!fmt)
		return;
	
	va_start (args, fmt);
	
	mess = g_strdup_vprintf (fmt, args);
	
	va_end (args);
	
	if (!mess)
		return;
	
	window = gtk_dialog_new ();
	gtk_window_set_title (GTK_WINDOW (window), "Erreur");
	gtk_window_set_modal (GTK_WINDOW (window), TRUE);
	
	vbox = GTK_DIALOG (window)->vbox;
	gtk_container_set_border_width (GTK_CONTAINER (vbox), 5);
	
	hbox = gtk_hbox_new (FALSE, 5);
	gtk_box_pack_start_defaults (GTK_BOX (vbox), hbox);
	
	icon = gtk_image_new_from_stock (GTK_STOCK_STOP, GTK_ICON_SIZE_DIALOG);
	gtk_box_pack_start (GTK_BOX (hbox), icon, FALSE, FALSE, 5);
	
	label = gtk_label_new (mess);
	gtk_label_set_line_wrap (GTK_LABEL (label), TRUE);
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 5);
		
	/* Boutons */
	
	bbox = GTK_DIALOG (window)->action_area;
	gtk_container_set_border_width (GTK_CONTAINER (bbox), 5);

	button = gtk_button_new_from_stock (GTK_STOCK_OK);
	GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
	g_signal_connect_swapped (button, "clicked",
				  G_CALLBACK (gtk_widget_destroy),
				  window);
	gtk_box_pack_start_defaults (GTK_BOX (bbox), button);
	gtk_widget_grab_default (button);
	
	gtk_widget_show_all (window);
	
	g_free (mess);
}

void
compress_print_err (const char *fmt, ...)
{
	va_list list;
	
	fprintf (stderr, "error: ");
	
	va_start (list, fmt);
	vfprintf (stderr, fmt, list);
	va_end (list);
	
	fprintf (stderr, "\n");
}

/**/

int
main (int argc, char *argv[])
{
	gtk_init (&argc, &argv);
	
	if (!ui_main_create ())
		return EXIT_FAILURE;
	
	gtk_main ();
	
	return EXIT_SUCCESS;
}
