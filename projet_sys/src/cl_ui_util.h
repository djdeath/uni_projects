#ifndef __CL_UI_UTIL_H__
#define __CL_UI_UTIL_H__

#include <gtk/gtk.h>

/** 
 * 
 * 
 * @param entrys 
 * @param type 
 * @param nb 
 * @param window 
 * @param data 
 * 
 * @return 
 */
GtkWidget *gtk_menu_from_factoryentry (GtkItemFactoryEntry *entrys,
					GType type,
				       gint nb,
				       GtkWidget *window,
				       gpointer data);

/** 
 * 
 * 
 * @param box 
 * @param title 
 * @param value 
 * @param max 
 * @param min 
 * @param step 
 * 
 * @return 
 */
GtkWidget *widget_spin_add_int (GtkBox *box, const gchar *title,
				gint value, gint max, gint min,
				gint step);

/** 
 * 
 * 
 * @param box 
 * @param title 
 * @param value 
 * @param max_char 
 * 
 * @return 
 */
GtkWidget *widget_entry_add_chars (GtkBox *box, const gchar *title,
				   const gchar *value, gint max_char);

#endif
