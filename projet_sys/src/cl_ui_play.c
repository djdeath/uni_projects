#include <string.h>

#include <gdk/gdkkeysyms.h>

#include "cl_parse.h"
#include "cl_word.h"
#include "cl_ui_play.h"
#include "cl_util.h"

struct WindowPlayRedraw
{
	WindowPlay_t *win;
	GdkEventExpose *ev;
};

static void
window_play_draw_word (WindowPlay_t *win, Word_t *word)
{
	gdk_draw_layout_with_colors (win->play_area->window,
				     win->gc,
				     word->x * win->c_width, word->y * win->c_height,
				     word->layout, &word->color, NULL);
}

static void
window_play_erase_word (WindowPlay_t *win, Word_t *word)
{
	gdk_draw_rectangle (win->play_area->window,
			    win->gc,
			    TRUE,
			    word->x * win->c_width, word->y * win->c_height,
			    word->layout_width, word->layout_height);
}

static void
window_play_redraw_words (Word_t *word, struct WindowPlayRedraw *re)
{
	int w_x, w_y;
	
	w_x = word->x * re->win->c_width;
	w_y = word->y * re->win->c_height;
	
	if (((w_x >= re->ev->area.x) && (w_x <= (re->ev->area.x + re->ev->area.width))) ||
	    (((w_x + word->layout_width) >= re->ev->area.x) && (w_x <= (re->ev->area.x + re->ev->area.width))) ||
	    ((w_y >= re->ev->area.y) && (w_y <= (re->ev->area.y + re->ev->area.height))) ||
	    (((w_y + word->layout_height) >= re->ev->area.y) && (w_x <= (re->ev->area.y + re->ev->area.height))))
		gdk_draw_layout_with_colors (re->win->play_area->window,
					     re->win->gc,
					     word->x * re->win->c_width, word->y * re->win->c_height,
					     word->layout, &word->color, NULL);
}

static gboolean
window_play_exposed (GtkWidget *widget, GdkEventExpose *event, WindowPlay_t *win)
{
	struct WindowPlayRedraw re;
	
	re.win = win;
	re.ev = event;
	
	gdk_gc_set_rgb_fg_color (win->gc, &win->background);
	
	gdk_draw_rectangle (widget->window,
			    win->gc,
			    TRUE,
			    0, 0,
			    widget->allocation.width, widget->allocation.height);
	
	hash_table_foreach (win->ht, FOREACH (window_play_redraw_words), &re);
	
	return TRUE;
}

void
window_play_destroy (WindowPlay_t *win)
{
	if (win)
	{
		if (win->ht)
		{
			hash_table_foreach (win->ht, FOREACH (word_free), NULL);
			hash_table_free (win->ht);
		}
		
		if (win->gc)
			g_object_unref (G_OBJECT (win->gc));
		
		g_free (win);
	}
}

static gboolean
window_play_entry_pressed (GtkWidget *entry, GdkEventKey *event, WindowPlay_t *win)
{
	if (event->keyval == GDK_Return)
	{
		const gchar *uword;
		
		if ((uword = gtk_entry_get_text (GTK_ENTRY (win->entry))))
		{
			gchar *word;
			
			if ((word = str_convert_to_ascii (uword)))
			{
				server_send (win->mwin, "!%s\n", word);
				
				g_free (word);
				
				gtk_entry_set_text (GTK_ENTRY (win->entry), "");
			}
		}
	}
	
	return FALSE;
}

WindowPlay_t *
window_play_create (WindowMain_t *mwin)
{
	WindowPlay_t *win;
	
	if ((win = g_new0 (WindowPlay_t, 1)))
	{
		win->mwin = mwin;
		
		if ((win->ht = hash_table_new (HASH_TABLE_CLOSE_DEFAULT_SIZE,
					       HASH_MODE_OPEN, HASH_FUNC_GLIB)))
		{
			PangoContext *cont;
			PangoFontMetrics *metrics;
			
			win->play_area = gtk_drawing_area_new ();
			gtk_widget_set_size_request (win->play_area, 100, 100);
			gtk_box_pack_start_defaults (GTK_BOX (mwin->play_box), win->play_area);
			g_signal_connect (G_OBJECT (win->play_area), "expose_event",
					  G_CALLBACK (window_play_exposed), win);
			gtk_widget_show_all (win->play_area);
			
			win->entry = gtk_entry_new_with_max_length (1000);
			gtk_box_pack_start (GTK_BOX (mwin->play_box), win->entry, FALSE, FALSE, 5);
			g_signal_connect (win->entry, "key-press-event",
					  G_CALLBACK (window_play_entry_pressed),
					  win);
			
			gtk_widget_show_all (win->entry);
			
			gdk_color_parse ("black", &win->background);
			win->gc = gdk_gc_new (GDK_DRAWABLE (mwin->window->window));
			
			cont = gtk_widget_get_pango_context (win->play_area);
			pango_font_description_set_family (pango_context_get_font_description (cont), "Monospace");
			if ((metrics = pango_context_get_metrics (cont,
								  pango_context_get_font_description (cont),
								  pango_context_get_language (cont))))
			{			
				win->c_width = PANGO_PIXELS (pango_font_metrics_get_approximate_char_width (metrics));
				win->c_height = PANGO_PIXELS (pango_font_metrics_get_ascent (metrics) + pango_font_metrics_get_descent (metrics));
				
				pango_font_metrics_unref (metrics);
			}
		}
		else
		{
			g_free (win);
			win = NULL;
		}
	}
	
	return win;
}

void
window_play_word_add (WindowPlay_t *win, const gchar *str, gint x, gint y)
{
	gint color = 0;
	gchar *uword;
	Word_t *w;
	
	if (!win || !str || !win->configured)
		return;
	
	if (*str == '\e')
	{
		if ((str = strchr (str, ';')))
			color = strtol (++str, NULL, 10);
		
		if ((str = strchr (str, 'm')))
			str++;
	}
	
	/*server_send (win->mwin, "!%s\n", str);*/
	
	if (!(w = word_new (str)))
		return;
	
	if (!(uword = str_convert_to_utf8 (str)))
	{
		word_free (w);
		return;
	}
	
	w->x = x;
	w->y = y;
	w->layout = gtk_widget_create_pango_layout (win->play_area, uword);
	/*pango_font_description_set_family (pango_context_get_font_description (pango_layout_get_context (w->layout)), "Monospace");*/
	g_free (uword);
	pango_layout_get_pixel_size (w->layout, &w->layout_width, &w->layout_height);
	win->c_width = w->layout_width / w->len;
	
	color -= 30;
	switch (color)
	{
	case 0:
		gdk_color_parse ("white", &w->color);
		break;
		
	case 1:
		gdk_color_parse ("red", &w->color);
		break;
		
	case 2:
		gdk_color_parse ("green", &w->color);
		break;
		
	case 3:
		gdk_color_parse ("yellow", &w->color);
		break;
		
	case 4:
		gdk_color_parse ("blue", &w->color);
		break;
		
	case 5:
		gdk_color_parse ("magenta", &w->color);
		break;
		
	case 6:
		gdk_color_parse ("cyan", &w->color);
		break;
		
	case 7:
		gdk_color_parse ("brown", &w->color);
		break;
		
	default:
		gdk_color_parse ("white", &w->color);
	}
	
	if (hash_table_insert (win->ht, str, w))
		word_free (w);
	
       	window_play_draw_word (win, w);
}

void
window_play_word_del (WindowPlay_t *win, const gchar *str, gint x, gint y)
{
	Word_t *w = NULL;
	
	if (!win || !str || !win->configured)
		return;
	
	if (*str == '\e')
		if ((str = strchr (str, 'm')))
			str++;
	
	while ((w = hash_table_search (win->ht, str, w)))
	{
		if (!strcmp (w->word, str))
			break;
	}
	
	if (w)
	{
		window_play_erase_word (win, w);
		hash_table_remove (win->ht, str, w);
		word_free (w);
	}
}

void
window_play_set_size (WindowPlay_t *win, gint width, gint height)
{
	if (!win || (width < 1) || (height < 1))
		return;
	
	window_play_clear (win);
	
	win->configured = 1;
	win->width = width;
	win->height = height;
	
	gtk_window_resize (GTK_WINDOW (win->mwin->window), 1, 1);
	gtk_widget_set_size_request (win->play_area, width * win->c_width, height * win->c_height);
}

void
window_play_clear (WindowPlay_t *win)
{
	if (win)
	{
		hash_table_foreach (win->ht, FOREACH (word_free), NULL);
		hash_table_clear (win->ht);
		
		window_play_exposed (win->play_area, NULL, win);
	}
}
