#include "main.h"

#include <string.h>

#include "quadpic.h"
#include "quadcomp.h"

#include "util.h"

#include "load_ppm.h"
#include "load_qtc.h"
#include "save_ppm.h"
#include "save_qtc.h"

#include "ui_main.h"
#include "ui_util.h"

/**/

typedef struct UIMainWindow
{
	GtkWidget *window;
	GtkWidget *menu;
	
	GtkWidget *image;
	GdkPixbuf *image_pixbuf;
	Picture_t *image_render;
	
	Picture_t *pic;
	QuadTree_t *tree;
	
	int level;
} UIMainWindow_t;

/**/

void
ui_main_view_update (UIMainWindow_t *mwin, Picture_t *pic, QuadTree_t *tree)
{
	if (!mwin)
		return;
	
	gtk_image_set_from_pixbuf (GTK_IMAGE (mwin->image), NULL);
	
	if (mwin->image_pixbuf)
	{
		g_object_unref (G_OBJECT (mwin->image_pixbuf));
		mwin->image_pixbuf = NULL;
	}
	if (mwin->image_render && mwin->image_render != mwin->pic && mwin->image_render != pic)
	{
		picture_free (mwin->image_render);
		mwin->image_render = NULL;
	}
	if (mwin->pic)
	{
		picture_free (mwin->pic);
		mwin->pic = NULL;
	}
	if (mwin->tree)
	{
		quadtree_free (mwin->tree);
		mwin->tree = NULL;
	}
	
	if (pic)
	{
		if (!tree)
		{
			if ((mwin->tree = picture_to_quadtree_new (pic)))
				mwin->pic = pic;
			else
				picture_free (pic);
		}
		else
		{
			mwin->pic = pic;
			mwin->tree = tree;
		}
	}
	else if (tree)
	{
		if (mwin->level < 0)
			mwin->level = tree->levels;
		
		if ((mwin->pic = quadtree_to_picture_new (tree, mwin->level)))
			mwin->tree = tree;
		else
			quadtree_free (tree);
	}
	
	if (mwin->pic && (mwin->pic->type == GREYSCALE))
	{
		if ((mwin->image_render = picture_convert_greyscale_to_rgb (mwin->pic)))
			mwin->image_pixbuf = gdk_pixbuf_new_from_data (mwin->image_render->data,
								       GDK_COLORSPACE_RGB, FALSE, 8,
								       mwin->image_render->width,
								       mwin->image_render->height,
								       mwin->image_render->width * mwin->image_render->depth,
								       NULL, NULL);
	}
	else
	{
		mwin->image_pixbuf = gdk_pixbuf_new_from_data (mwin->pic->data,
							       GDK_COLORSPACE_RGB, FALSE, 8,
							       mwin->pic->width, mwin->pic->height,
							       mwin->pic->width * mwin->pic->depth,
							       NULL, NULL);
	}
	
	gtk_image_set_from_pixbuf (GTK_IMAGE (mwin->image), mwin->image_pixbuf);
}

/**/

void
ui_main_opened (GtkWidget *filesel)
{
	gint len;
	const gchar *ext, *filename;
	Picture_t *ptmp;
	QuadTree_t *qtmp;
	UIMainWindow_t *mwin;
	
	if ((mwin = g_object_get_data (G_OBJECT (filesel), "mwindow")) &&
	    (filename = gtk_file_selection_get_filename (GTK_FILE_SELECTION (filesel))))
	{
		
		/* Nettoyage de l'ancienne image */
		
		len = strlen (filename);
		ext = filename + (len - 3);
		
		/* */
		if (!strcmp (ext, "ppm") || !strcmp (ext, "pgm") ||
		    !strcmp (ext, "PPM") || !strcmp (ext, "PGM"))
		{
			if ((ptmp = load_ppm (filename)))
			{
				if ((ptmp->width == ptmp->height) &&
				    ((1 << first_bit_up (ptmp->width)) == ptmp->width))
				{
					mwin->level = -1;
					ui_main_view_update (mwin, ptmp, NULL);
				}
				else
				{
					PRINT_ERR (("L'image n'est pas de taille 2^n"));
					picture_free (ptmp);
				}
			}
		}
		else if (!strcmp (ext, "qtc") || !strcmp (ext, "QTC"))
		{
			if ((qtmp = load_qtc (filename)))
			{
				mwin->level = qtmp->levels;
				ui_main_view_update (mwin, NULL, qtmp);
			}
		}
		else
			return;
		
		gtk_widget_destroy (filesel);
	}
	
	return;
}

void
ui_main_open (UIMainWindow_t *mwin, guint action, GtkWidget *w)
{
	GtkWidget *filesel;
	
	filesel = gtk_file_selection_new ("Ouvrir le fichier");
	g_object_set_data (G_OBJECT (filesel), "mwindow", mwin);
	g_object_set (G_OBJECT (filesel), "select-multiple", FALSE, NULL);
	g_signal_connect_swapped (GTK_FILE_SELECTION (filesel)->cancel_button,
				  "clicked",
				  G_CALLBACK (gtk_widget_destroy),
				  filesel);
	g_signal_connect_swapped (GTK_FILE_SELECTION (filesel)->ok_button,
				  "clicked",
				  G_CALLBACK (ui_main_opened),
				  filesel);
	
	gtk_widget_show (filesel);
}

/**/

void
ui_main_saved_ppm (GtkWidget *window, gint response, GtkWidget *filesel)
{
	const gchar *filename;
	GtkWidget *radio;
	UIMainWindow_t *mwin;
	
	if (response == GTK_RESPONSE_ACCEPT)
	{
		mwin = g_object_get_data (G_OBJECT (filesel), "mwindow");
		radio = g_object_get_data (G_OBJECT (window), "radio");
		filename = gtk_file_selection_get_filename (GTK_FILE_SELECTION (filesel));
		
		if (!widget_radio_get_selected (radio))
			save_ppm (filename, FILE_PPM, mwin->pic);
		else
			save_ppm (filename, FILE_PGM, mwin->pic);
		
		gtk_widget_destroy (filesel);
	}
	gtk_widget_destroy (window);
}

void
ui_main_save_ppm (GtkWidget *filesel)
{
	GtkWidget *window;
	GtkWidget *radio;
	
	window = gtk_dialog_new_with_buttons ("Format du fichier pgm/ppm",
					      NULL,
					      GTK_DIALOG_MODAL,
					      GTK_STOCK_CANCEL,
					      GTK_RESPONSE_REJECT,
					      GTK_STOCK_OK,
					      GTK_RESPONSE_ACCEPT,
					      NULL);
	gtk_window_set_modal (GTK_WINDOW (window), TRUE);
	g_signal_connect (window, "response", G_CALLBACK (ui_main_saved_ppm),
			  filesel);
	
	radio = widget_radio_add (GTK_DIALOG (window)->vbox, NULL, "brut");
	widget_radio_add (GTK_DIALOG (window)->vbox, radio, "ascii");
	g_object_set_data (G_OBJECT (window), "radio", radio);
		
	gtk_widget_show_all (window);
}

void
ui_main_saved_qtc (GtkWidget *window, gint response, GtkWidget *filesel)
{
	const gchar *filename;
	int lev;
	GtkWidget *level;
	UIMainWindow_t *mwin;
	
	if (response == GTK_RESPONSE_ACCEPT)
	{
		mwin = g_object_get_data (G_OBJECT (filesel), "mwindow");
		level = g_object_get_data (G_OBJECT (window), "level");
		filename = gtk_file_selection_get_filename (GTK_FILE_SELECTION (filesel));
		
		lev = (int) gtk_range_get_value (GTK_RANGE (level));
		quadtree_compress (mwin->tree, lev);
		save_qtc (filename, mwin->tree);
		
		gtk_widget_destroy (filesel);
	}
	gtk_widget_destroy (window);
}


void
ui_main_save_qtc (GtkWidget *filesel)
{
	GtkWidget *window;
	GtkWidget *level;
	
	window = gtk_dialog_new_with_buttons ("Niveau de compression",
					      NULL,
					      GTK_DIALOG_MODAL,
					      GTK_STOCK_CANCEL,
					      GTK_RESPONSE_REJECT,
					      GTK_STOCK_OK,
					      GTK_RESPONSE_ACCEPT,
					      NULL);
	gtk_window_set_modal (GTK_WINDOW (window), TRUE);
	g_signal_connect (window, "response", G_CALLBACK (ui_main_saved_qtc),
			  filesel);
	
	level = gtk_hscale_new_with_range (0.0, 100.0, 1.0);
	gtk_scale_set_digits (GTK_SCALE (level), 0);
	gtk_box_pack_start_defaults (GTK_BOX (GTK_DIALOG (window)->vbox), level);
	g_object_set_data (G_OBJECT (window), "level", level);
		
	gtk_widget_show_all (window);
}

void
ui_main_saved (GtkWidget *filesel)
{
	gint len;
	const gchar *ext, *filename;
	
	if ((filename = gtk_file_selection_get_filename (GTK_FILE_SELECTION (filesel))))
	{
		len = strlen (filename);
		ext = filename + (len - 3);
		
		if (!strcmp (ext, "ppm") || !strcmp (ext, "pgm") ||
		    !strcmp (ext, "PPM") || !strcmp (ext, "PGM"))
			ui_main_save_ppm (filesel);
		else if (!strcmp (ext, "qtc") || !strcmp (ext, "QTC"))
			ui_main_save_qtc (filesel);
		else
			return;
	}
	
	return;
}

void
ui_main_save (UIMainWindow_t *mwin, guint action, GtkWidget *w)
{
	GtkWidget *filesel;
	
	/* Aucune image chargée. */
	if (!mwin->image_pixbuf)
		return;
	
	filesel = gtk_file_selection_new ("Enregister le fichier");
	gtk_window_set_modal (GTK_WINDOW (filesel), TRUE);
	g_object_set_data (G_OBJECT (filesel), "mwindow", mwin);
	g_object_set (G_OBJECT (filesel), "select-multiple", FALSE, NULL);
	g_signal_connect_swapped (GTK_FILE_SELECTION (filesel)->cancel_button,
				  "clicked",
				  G_CALLBACK (gtk_widget_destroy),
				  (gpointer) filesel);
	g_signal_connect_swapped (GTK_FILE_SELECTION (filesel)->ok_button,
				  "clicked",
				  G_CALLBACK (ui_main_saved),
				  (gpointer) filesel);
	
	gtk_widget_show (filesel);
}

/**/

/* Rotations des images. */
void
ui_main_rotate (UIMainWindow_t *mwin, guint action, GtkWidget *w)
{
	Picture_t *ptmp;
	QuadTree_t *qtmp;
	
	if (!mwin->tree)
		return;
	
	qtmp = mwin->tree;
	mwin->tree = NULL;
	ptmp = mwin->pic;
	mwin->pic = NULL;
	
	if (!action)
		quadtree_rotate_ntrigo (qtmp);
	else
		quadtree_rotate_trigo (qtmp);
	quadtree_to_picture (ptmp, qtmp, qtmp->levels);
	quadtree_init (qtmp);
	picture_to_quadtree (qtmp, ptmp);
	ui_main_view_update (mwin, ptmp, qtmp);
}

void
ui_main_flip (UIMainWindow_t *mwin, guint action, GtkWidget *w)
{
	Picture_t *ptmp;
	QuadTree_t *qtmp;
	
	if (!mwin->tree)
		return;
	
	qtmp = mwin->tree;
	mwin->tree = NULL;
	ptmp = mwin->pic;
	mwin->pic = NULL;
	
	switch (action)
	{
	case 0:
		quadtree_flip_hori (qtmp);
		break;
		
	case 1:
		quadtree_flip_vert (qtmp);
		break;
	}
	
	quadtree_to_picture (ptmp, qtmp, qtmp->levels);
	quadtree_init (qtmp);
	picture_to_quadtree (qtmp, ptmp);
	ui_main_view_update (mwin, ptmp, qtmp);
}

/* Inversion des couleurs. */
void
ui_main_invert (UIMainWindow_t *mwin, guint action, GtkWidget *w)
{
	Picture_t *tmp;
	
	if (!mwin->pic)
		return;
	
	tmp = mwin->pic;
	mwin->pic = NULL;
	picture_invert (tmp);
	ui_main_view_update (mwin, tmp, NULL);
}

/**/

/* 2 fonctions rappelé à interval régulier pour la mise à jour de
 * l'affichage progressif. */
gboolean
ui_main_unprog_timeout (UIMainWindow_t *mwin)
{
	QuadTree_t *qtmp;
	
	if (mwin->level <= 0)
	{
		qtmp = mwin->tree;
		mwin->level = qtmp->levels;
		mwin->tree = NULL;
		ui_main_view_update (mwin, NULL, qtmp);
		gtk_widget_set_sensitive (GTK_WIDGET (mwin->menu), TRUE);
		
		return FALSE;
	}
	
	mwin->level--;
	qtmp = mwin->tree;
	mwin->tree = NULL;
	ui_main_view_update (mwin, NULL, qtmp);
	
	return TRUE;
}

gboolean
ui_main_prog_timeout (UIMainWindow_t *mwin)
{
	QuadTree_t *qtmp;
	
	if (mwin->level >= mwin->tree->levels)
	{
		gtk_widget_set_sensitive (GTK_WIDGET (mwin->menu), TRUE);
		
		return FALSE;
	}
	
	mwin->level++;
	qtmp = mwin->tree;
	mwin->tree = NULL;
	ui_main_view_update (mwin, NULL, qtmp);
	
	return TRUE;
}

/* Fonction de rappel du menu appelé pour une simulation d'affichage
 * progressif. */
void
ui_main_prog (UIMainWindow_t *mwin, guint action, GtkWidget *w)
{
	QuadTree_t *qtmp;
	
	if (!mwin->tree)
		return;
	
	switch (action)
	{
	case 0:
		gtk_widget_set_sensitive (GTK_WIDGET (mwin->menu), FALSE);
		qtmp = mwin->tree;
		mwin->level = 0;
		mwin->tree = NULL;
		ui_main_view_update (mwin, NULL, qtmp);
		g_timeout_add (500, (GSourceFunc) ui_main_prog_timeout, mwin);
		break;
		
	case 1:
		gtk_widget_set_sensitive (GTK_WIDGET (mwin->menu), FALSE);
		qtmp = mwin->tree;
		mwin->level = qtmp->levels;
		mwin->tree = NULL;
		ui_main_view_update (mwin, NULL, qtmp);
		g_timeout_add (500, (GSourceFunc) ui_main_unprog_timeout, mwin);
		break;
	}
}

/**/

void
ui_main_quit (UIMainWindow_t *mwindow)
{
	gtk_widget_destroy (mwindow->window);
}

static GtkItemFactoryEntry menu_items[] = {
	{ "/_Fichier", "<ALT>F", NULL, 0, "<Branch>", NULL},
	{ "/_Fichier/tear1", NULL, NULL, 0, "<Tearoff>", NULL},
	{ "/_Fichier/_Ouvrir", "<CTRL>O", ui_main_open, 0, "<StockItem>", GTK_STOCK_OPEN},
	{ "/_Fichier/_Enregistrer", "<CTRL>S", ui_main_save, 0, "<StockItem>", GTK_STOCK_SAVE},
	{ "/_Fichier/sep1", NULL, NULL, 0, "<Separator>", NULL},
	{ "/_Fichier/_Quitter", "<CTRL>Q", ui_main_quit, 0, "<StockItem>", GTK_STOCK_QUIT},
	
	
	{ "/_Effets", "<ALT>E", NULL, 0, "<Branch>", NULL},
	{ "/_Effets/tear1", NULL, NULL, 0, "<Tearoff>", NULL},
	{ "/_Effets/Rotation sens horaire", "<CTRL>H", ui_main_rotate, 0, "<Item>", NULL},
	{ "/_Effets/Rotation sens trigo", "<CTRL>T", ui_main_rotate, 1, "<Item>", NULL},
	{ "/_Effets/sep1", NULL, NULL, 0, "<Separator>", NULL},
	{ "/_Effets/Flip horizontal", NULL, ui_main_flip, 0, "<Item>", NULL},
	{ "/_Effets/Flip vertical", NULL, ui_main_flip, 1, "<Item>", NULL},
	{ "/_Effets/sep2", NULL, NULL, 0, "<Separator>", NULL},
	{ "/_Effets/Inverser les couleurs", "<CTRL>I", ui_main_invert, 0, "<Item>", NULL},
	
	{ "/_Animations", "<ALT>A", NULL, 0, "<Branch>", NULL},
	{ "/_Animations/tear1", NULL, NULL, 0, "<Tearoff>", NULL},
	{ "/_Animations/Simuler l'affichage progressif", NULL, ui_main_prog, 0, "<Item>", NULL},
	{ "/_Animations/Simuler l'affichage improgressif", NULL, ui_main_prog, 1, "<Item>", NULL},
	
	{ "/_Aide", NULL, NULL, 0, "<LastBranch>"},
	{ "/_Aide/tear1", NULL, NULL, 0, "<Tearoff>", NULL},
	{ "/_Aide/A Propos", NULL, NULL, 0, "<StockItem>", GTK_STOCK_DIALOG_INFO}
};

/* Création de la barre de menu. */
static GtkWidget *
ui_main_create_menu (GtkWidget *window, gpointer data)
{
	GtkWidget *menubar;
	
	menubar = gtk_menu_from_factoryentry (menu_items, GTK_TYPE_MENU_BAR,
					      sizeof (menu_items) / sizeof (menu_items[0]),
					      window, data);
	gtk_widget_show_all (menubar);
	
	return menubar;
}

static gboolean
ui_main_free (UIMainWindow_t *mwindow)
{
	if (mwindow->image_render)
		picture_free (mwindow->image_render);
	if (mwindow->pic)
		picture_free (mwindow->pic);
	if (mwindow->tree)
		quadtree_free (mwindow->tree);
	g_free (mwindow);
	
	gtk_main_quit ();
	
	return FALSE;
}

/* Création de la fenêtre principale. */
gboolean
ui_main_create (void)
{
	GtkWidget *vbox, *imgbox;
	UIMainWindow_t *mwindow;
	
	if (!(mwindow = g_new0 (UIMainWindow_t, 1)))
		return FALSE;
	
	mwindow->window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	g_signal_connect_swapped (mwindow->window, "destroy", G_CALLBACK (ui_main_free), mwindow);
	gtk_window_set_title (GTK_WINDOW (mwindow->window), PACKAGE);
	
	vbox = gtk_vbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (mwindow->window), vbox);
	
	mwindow->menu = ui_main_create_menu (mwindow->window, mwindow);
	gtk_box_pack_start (GTK_BOX (vbox), mwindow->menu, FALSE, FALSE, 0);
	
	imgbox = gtk_vbox_new (FALSE, 0);
	gtk_box_pack_start_defaults (GTK_BOX (vbox), imgbox);
	gtk_container_set_border_width (GTK_CONTAINER (imgbox), 5);
	
	mwindow->image = gtk_image_new ();
	gtk_box_pack_start_defaults (GTK_BOX (vbox), mwindow->image);
	
	gtk_widget_show_all (mwindow->window);
	
	return TRUE;
}
