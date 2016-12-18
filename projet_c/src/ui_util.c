#include "main.h"

#include "ui_util.h"

gint
widget_radio_get_selected (GtkWidget *radio)
{
	gint i;
	GSList *list;
	
	g_return_val_if_fail (GTK_IS_RADIO_BUTTON (radio), -1);
	
	if (!(list = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radio))))
		return -1;
	
	i = 0;
	while (list && !gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (list->data)))
	{
		list = list->next;
		i++;
	}
	
	return i;
}

GtkWidget *
widget_radio_add (GtkWidget *box, GtkWidget *from, const gchar *title)
{
	GtkWidget *radio;
	
	if (from != NULL)
		radio = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (from),
								     title);
	else
		radio = gtk_radio_button_new_with_label_from_widget (NULL, title);
	gtk_box_pack_start (GTK_BOX (box), radio, FALSE, FALSE, 0);
	gtk_widget_show (radio);
	
	return radio;
}

/* If window != NULL we use accels. */
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
