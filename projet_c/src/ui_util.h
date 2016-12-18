#ifndef __UI_UTIL_H__
#define __UI_UTIL_H__

#include <gtk/gtk.h>

gint widget_radio_get_selected (GtkWidget *radio);
GtkWidget *widget_radio_add (GtkWidget *box, GtkWidget *from, const gchar *title);

GtkWidget *gtk_menu_from_factoryentry (GtkItemFactoryEntry *entrys, GType type, gint nb,
				       GtkWidget *window, gpointer data);

#endif
