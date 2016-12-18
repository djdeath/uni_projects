#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>

#include "util.h"
#include "cl_util.h"

#include "cl_ui_main.h"

#define MAX_SEND (1024)

static gint
player_score (WindowMain_t *win, gchar *str, size_t len)
{
	gchar *name, *uname;
	gchar *sscore;
	gint score;
	
	name = str;
	
	if ((sscore = strnchr (name, ':', len)))
	{
		*(sscore++) = '\0';
		score = atoi (sscore);
		
		if (!(uname = str_convert_to_utf8 (name)))
			return 0;
		
		if (score >= 0)
		{
			window_list_player_update (win->list, uname, score);
			window_chat_player_update (win->chat, uname);
		}
		else
		{
			window_list_player_del (win->list, uname);
			window_chat_player_del (win->chat, uname);
		}
		
		g_free (uname);
		
		return 1;
	}
	
	return 0;
}

static gint
team_score (WindowMain_t *win, gchar *str, size_t len)
{
	gchar *team, *uteam;
	gchar *sscore;
	gint score;
	
	team = str;
	
	if ((sscore = strnchr (team, ':', len)))
	{
		*(sscore++) = '\0';
		score = atoi (sscore);
		
		if (!(uteam = str_convert_to_utf8 (team)))
			return 0;
		
		if (score >= 0)
			window_list_team_update (win->list, team, score);
		else
			window_list_team_del (win->list, team);
		
		g_free (uteam);
		
		return 1;
	}
	
	return 0;
}

static gint
window_size (WindowMain_t *win, gchar *str, size_t len)
{
	gchar *width, *height;
	
	width = str;
	
	if ((height = strnchr (width, ':', len)))
	{
		*(height++) = '\0';
		
		window_play_set_size (win->play, atoi (width), atoi (height));
		
		return 1;
	}
	
	return 0;
}

static gint
word_add (WindowMain_t *win, gchar *str, size_t len)
{
	gchar *word, *x, *y;
	
	word = str;
	
	if ((x = strnchr (word, ':', len)))
	{
		*(x++) = '\0';
		
		if ((y = strnchr (x, ':', len - (x - word))))
		{
			*(y++) = '\0';
			
			window_play_word_add (win->play, word, atoi (x), atoi (y));
		}
		else
			return 0;
		
		return 1;
	}
	
	return 0;
}

static gint
word_del (WindowMain_t *win, gchar *str, size_t len)
{
	gchar *word, *x, *y;
	
	word = str;
	
	if ((x = strnchr (word, ':', len)))
	{
		*(x++) = '\0';
		
		if ((y = strnchr (x, ':', len - (x - word))))
		{
			*(y++) = '\0';
			
			window_play_word_del (win->play, word, atoi (x), atoi (y));
		}
		else
			return 0;
		
		return 1;
	}
	
	return 0;
}

static gint
message (WindowMain_t *win, gchar *str, size_t len)
{
	gchar *name, *message;
	
	name = str;
	
	if ((message = strnchr (name, ':', len)))
	{
		*(message++) = '\0';
		
		window_chat_message_add (win->chat, name, message);
		
		return 1;
	}
	
	return 0;
}

static gint
parse_buffer (WindowMain_t *win)
{
	gchar *s, *end, *tmp;
	gint stop = 0, ret;
	size_t len;
	
	s = win->buf;
	end = win->buf + win->ubuf_len;
	
	while (!stop && (s <= end))
	{
		len = (size_t) (end - s);
		
		switch (*s)
		{
		case '_':
			if ((tmp = strnchr (s, '\n', len)))
			{
				*tmp = '\0';
				player_score (win, s + 1, tmp - s - 1);
				s = tmp + 1;
			 }
			else
				stop = 1;
				
			break;
			
		case '=':
			/* Des informations sur un équipe ou sur la
			 * taille de la zone dans laquelle on affiche
			 * les mots. */
			if ((tmp = strnchr (s, '\n', len)))
			{
				*tmp = '\0';
				if (!isdigit (*(s + 1)))
					team_score (win, s + 1, tmp - s - 1);
				else
					window_size (win, s + 1, tmp - s - 1);
				s = tmp + 1;
			}
			else
				stop = 1;
			break;
			
		case '+':
			/* Un mot à ajouter */
			if ((tmp = strnchr (s, '\n', len)))
			{
				*tmp = '\0';
				word_add (win, s + 1, tmp - s - 1);
				s = tmp + 1;
			}
			else
				stop = 1;
			break;
			
		case '-':
			/* Un mot à retirer */
			if ((tmp = strnchr (s, '\n', len)))
			{
				*tmp = '\0';
				word_del (win, s + 1, tmp - s - 1);
				s = tmp + 1;
			}
			else
				stop = 1;
			break;
			
		case '<':
			/* Un message */
			if ((tmp = strnchr (s, '\n', len)))
			{
				*tmp = '\0';
				message (win, s + 1, tmp - s -1);
				s = tmp + 1;
			}
			else
				stop = 1;
			break;
			
		case '?':
			if ((tmp = strnchr (s, '\n', len)))
			{
				*tmp = '\0';
				window_main_status_message (win, "Mot %s invalide.", s + 1);
				s = tmp + 1;
			}
			else
				stop = 1;
			break;
			
		default:
			/* On zappe cette commande. */
			if ((tmp = strnchr (s, '\n', len)))
			{
				*tmp = '\0';
				s = tmp + 1;
			}
			else
				stop = 1;
			break;
		}
	}
	
	ret = end - s;
	
	if (stop)
	{
		if (s != win->buf)
		{
			if (s != end)
				memmove (win->buf, s, (size_t) (end - s));
			win->ubuf_len = (size_t) (end - s);
		}
		else
		{
			window_main_disconnect (win);
			ret = -1;
		}
	}
 	else
		win->ubuf_len = 0U;
	
	return ret;
}


gboolean
server_recv (WindowMain_t *win)
{
	ssize_t len;
	fd_set rd;
	struct timeval tm;
	
	if (!win)
		return FALSE;
	
	memset (&tm, 0, sizeof (tm));
	
	FD_ZERO (&rd);
	FD_SET (win->fd, &rd);
	
	if (select (win->fd + 1, &rd, NULL, NULL, &tm) > 0)
	{
		if ((len = read (win->fd, win->buf + win->ubuf_len, SERVER_READ_BLOCK - win->ubuf_len)) > 0)
		{
			win->ubuf_len += len;
			parse_buffer (win);
			
			return TRUE;
		}
		
		if (!len)
		{
			/* Connection coupée du côté serveur. */
			window_main_disconnect (win);
			
			return FALSE;
		}
		
		if (errno != EAGAIN)
		{
			/* Erreur sur le descripteur. */
			window_main_disconnect (win);
			
			return FALSE;
		}
	}
		
	return TRUE;
}

gint
server_send (WindowMain_t *win, const gchar *fmt, ...)
{
	gint ret;
	va_list list;
	
	if (!win || (win->fd == -1))
		return -1;
	
	va_start (list, fmt);
	ret = fdvnprintf (win->fd, MAX_SEND, fmt, list);
	va_end (list);
	
	return ret;
}
