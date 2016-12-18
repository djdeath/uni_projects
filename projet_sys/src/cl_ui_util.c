#include <string.h>

#include "cl_ui_util.h"

GtkWidget *
gtk_menu_from_factoryentry (GtkItemFactoryEntry *entrys, GType type, gint nb,
			    GtkWidget *window, gpointer data)
{
	GtkWidget *menu;
	GtkItemFactory *item_factory;
	GtkAccelGroup *accel_group = NULL;
	
	g_return_val_if_fail (entrys != NULL, NULL);
	g_return_val_if_fail (nb > 0, NULL);
	
	if (window)
		accel_group = gtk_accel_group_new ();
	item_factory = gtk_item_factory_new (type, "<main>", accel_group);
	gtk_item_factory_create_items (item_factory, nb, entrys, data);
	if (window)
		gtk_window_add_accel_group (GTK_WINDOW (window), accel_group);
	
	menu = gtk_item_factory_get_widget (item_factory, "<main>");
	
	return menu;
}

GtkWidget *
widget_spin_add_int (GtkBox *box, const gchar *title,
		     gint value, gint max, gint min,
		     gint step)
{
	GtkWidget *hbox;
	GtkWidget *spin;
	GtkWidget *label;
	GtkObject *adj;
	
	g_return_val_if_fail (GTK_IS_BOX (box), NULL);
	
	adj = gtk_adjustment_new ((gfloat) value, (gfloat) min, (gfloat) max,
				  (gfloat) step, (gfloat) step, 0);
	
	hbox = gtk_hbox_new (FALSE, 0);
	gtk_box_pack_start (GTK_BOX (box), hbox, FALSE, FALSE, 0);
	
	if (title)
	{
		label = gtk_label_new (title);
		gtk_box_pack_start_defaults (GTK_BOX (hbox), label);
		gtk_label_set_justify (GTK_LABEL (label), GTK_JUSTIFY_LEFT);
	}
	
	spin = gtk_spin_button_new (GTK_ADJUSTMENT (adj), 0.2, 0);
	gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spin), TRUE);
	gtk_box_pack_start_defaults (GTK_BOX (hbox), spin);
		
	gtk_widget_show_all (hbox);
	
	return spin;
}

GtkWidget *
widget_entry_add_chars (GtkBox *box, const gchar *title,
			const gchar *value, gint max_char)
{
	GtkWidget *entry;
	GtkWidget *hbox;
	GtkWidget *label;
	
	g_return_val_if_fail (GTK_IS_BOX (box), NULL);
	
	hbox = gtk_hbox_new (FALSE, 0);
	gtk_box_pack_start (GTK_BOX (box), hbox, FALSE, FALSE, 0);
	
	if (title)
	{
		label = gtk_label_new (title);
		gtk_box_pack_start_defaults (GTK_BOX (hbox), label);
		gtk_label_set_justify (GTK_LABEL (label), GTK_JUSTIFY_LEFT);
	}
	
	entry = gtk_entry_new_with_max_length (max_char);
	if (value)
		gtk_entry_set_text (GTK_ENTRY (entry), value);
	gtk_box_pack_start_defaults (GTK_BOX (hbox), entry);
	
	gtk_widget_show_all (hbox);
	
	return entry;
}
