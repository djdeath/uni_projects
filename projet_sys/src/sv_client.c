#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <unistd.h>

#include "sv_client.h"
#include "util.h"

/**/

/* Au delà de cette limite, on arrête de remplir le buffer, et on
 * tente de traiter des données qui nous sont déjà parvenu déjà. */
#define CLIENT_BUF_LIMIT (4096)

#define CLIENT_MAX_NAME (64)

static void client_read (Client_t *cl);
static void client_close (Client_t *cl);

/**/

Client_t *
client_create (Server_t *sv, int fd)
{
	Client_t *cl;
	
	if (sv && (fd != -1) && (cl = CLIENT (calloc (1, sizeof (Client_t)))))
	{
		cl->sv = sv;
		cl->fd = fd;
		cl->fd_read = input_add (fd, INPUT_DAT, CALLBACK (client_read), cl);
		cl->fd_close = input_add (fd, INPUT_HUP | INPUT_ERR, CALLBACK (client_close), cl);
		/*printf ("new client fd=%i\n", fd);*/
	}
	
	return cl;
}

void
client_destroy (Client_t *cl)
{
	if (cl)
	{
		input_remove (cl->fd_read);
		input_remove (cl->fd_close);
		net_close (cl->fd);
		
		if (cl->team)
			team_remove_client (cl->team, cl);
		if (cl->name)
			free (cl->name);
		/*printf ("del client %i\n", cl->fd);*/
		free (cl);
	}
}

/**/

void
client_inc_score (Client_t *cl, int points)
{
	if (cl)
	{
		cl->score += points;
		if (cl->team)
			team_inc_score (cl->team, points);
		
		server_send (cl->sv, "_%s:%i\n", cl->name, cl->score);
	}
}

int
client_set_name (Client_t *cl, const char *name)
{
	char *tmp;
	
	if (!cl)
		return 0;
	
	if (!name)
	{
		if (cl->team)
		{
			team_remove_client (cl->team, cl);
			cl->team = NULL;
		}
		if (cl->name)
		{
			server_set_client_wait (cl->sv, cl);
			free (cl->name);
			cl->name = NULL;
		}
	}
	else
	{
		if (server_search_client (cl->sv, name))
		{
			client_send (cl, "# Name %s already in use\n", name);
			
			return 0;
		}
		
		if (!(tmp = strndup (name, CLIENT_MAX_NAME, NULL)))
			return 0;
		
		if (cl->name)
		{
			/* Ici il ne faut envoyer des informations aux
			 * autres clients que lorsque le joueur change
			 * son nom, les autres cas sont gérés dans
			 * sv_server.c. */
			server_send (cl->sv, "_%s:%i\n_%s:%i\n",
				     cl->name, -1,
				     tmp, cl->score);
			free (cl->name);
			cl->name = tmp;
		}
		else
		{
			cl->name = tmp;
			server_set_client_play (cl->sv, cl);
		}
	}
	
	return 1;
}

int
client_set_team (Client_t *cl, const char *name)
{
	Team_t *team;
	
	/* On ajoute pas à une équipe un joueur sans nom. */
	if (!cl || !cl->name)
		return 0;
	
	/* On retire le joueur de son équipe */
	if (!name)
	{
		if (!cl->team)
			return 0;
		
		team_remove_client (cl->team, cl);
		cl->team = NULL;
		
		return 1;
	}
	
	if (!(team = server_search_team (cl->sv, name)))
	{
		if ((team = team_create (cl->sv, name)))
		{
			if (cl->team)
				team_remove_client (cl->team, cl);
			
			if (team_add_client (team, cl))
				cl->team = team;
			else
			{
				team_destroy (team);
				
				return 0;
			}
			server_add_team (cl->sv, cl->team);
		}
		else
			return 0;
	}
	else
	{
		if (team != cl->team)
		{
			team_remove_client (cl->team, cl);
			
			if (team_add_client (team, cl))
				cl->team = team;
			else
			{
				cl->team = NULL;
				
				return 0;
			}
		}
		else
			return 0;
	}
	
	return 1;
}

/* On effectue ici le véritable traitement des données. */
static void
client_read_data (Client_t *cl)
{
	char *s, *end, *tmp;
	int stop = 0;
	
	s = cl->buf;
	end = cl->buf + cl->ubuf_len;
	
	while (!stop && (s < end))
	{
		switch (*s)
		{
		case '\'':
			/* Pour le nom */
			s++;
			if ((tmp = strnchr (s, '\n', (size_t) (end - s))))
			{
				*tmp = '\0';
				if (s != tmp)
					client_set_name (cl, s);
				else
					client_set_name (cl, NULL);
				s = tmp + 1;
			}
			else
				stop = 1;
			break;
			
		case '\"':
			/* Pour la team */
			if (cl->name)
			{
				s++;
				if ((tmp = strnchr (s, '\n', (size_t) (end - s))))
				{
					*tmp = '\0';
					if (s != tmp)
						client_set_team (cl, s);
					else
						client_set_team (cl, NULL);
					s = tmp + 1;
				}
				else
					stop = 1;
				break;
			}
			
		case '!':
			/* Pour un mot */
			if (cl->name)
			{
				s++;
				if ((tmp = strnchr (s, '\n', (size_t) (end - s))))
				{
					*tmp = '\0';
					if (s != tmp)
						game_client_response (cl->sv->game, cl, s);
					s = tmp + 1;
				}
				else
					stop = 1;
				break;
			}
			
		case '>':
			/* Pour un message */
			if (cl->name)
			{
				s++;
				if ((tmp = strnchr (s, ':', (size_t) (end - s))))
				{
					char *mess;
					Client_t *scl;
					
					*(tmp++) = '\0';
					
					if ((mess = strnchr (tmp, '\n', (size_t) (end - tmp))))
					{
						*mess = '\0';
						
						if (s == (tmp - 1))
						{
							server_send (cl->sv, "<%s:%s\n", cl->name, tmp);
						}
						else
						{
							if ((scl = server_search_client (cl->sv, s)))
							{
								if (scl != cl)
									client_send (scl, "<%s:%s\n", cl->name, tmp);
								client_send (cl, "<%s:%s\n", cl->name, tmp);
							}
						}
						s = mess + 1;
					}
					else
					{
						s = cl->buf;
						stop = 1;
					}
				}
				else
					stop = 1;
				break;
			}
			
		default:
			/* On zappe cette commande. */
			if ((tmp = strnchr (s, '\n', (size_t) (end - s))))
				s = tmp + 1;
			else
				stop = 1;
		}
		
	}
	
	if (stop)
	{
		if (s != cl->buf)
		{
			memmove (cl->buf, s, (size_t) (end - s));
			cl->ubuf_len = (size_t) (end - s);
		}
		else
		{
			/* Allo ??!! Allo ??!! Y a d'la merde dans
			 * l'tuyau !!  Le client a envoyé 1Ko de
			 * données n'ayant aucun sens => TU DEGAGES
			 * ! */
			
			server_remove_client (cl->sv, cl);
		}
	}
	else
		cl->ubuf_len = 0U;
}

/* Tout ce que le client envera doit être récupéré ici. On va donc
 * traiter toutes les commandes et agir en conséquence. */
static void
client_read (Client_t *cl)
{
	ssize_t len;
	
	/* On sait jamais... des fois qu'un bolos ait modifié les
	 * sources ... */
	if (!cl)
		return;
	
	if ((len = read (cl->fd, cl->buf + cl->ubuf_len, CLIENT_BUF_BLOCK - cl->ubuf_len)) > 0)
		cl->ubuf_len += (size_t) len;
	
	/* Une erreur ? on dégage le client. Et oui, on fait pas dans
	 * le détail. */
	if ((len == -1) && (errno != EAGAIN))
	{
		server_remove_client (cl->sv, cl);
		
		return;
	}
	
	/* EOC/EOF */
	if (!len)
	{
		server_remove_client (cl->sv, cl);
		
		return;
	}
	
	client_read_data (cl);
}

int
client_send (Client_t *cl, const char *fmt, ...)
{
	int ret;
	va_list list;
		
	if (!cl)
		return -1;
	
	va_start (list, fmt);
	ret = fdvnprintf (cl->fd, MAX_SEND, fmt, list);
	va_end (list);
	
	/*if ((ret < 0) && (errno == EPIPE))
	  server_remove_client (cl->sv, cl);*/
	
	return ret;
}

/**/

int
client_name_cmp (const Client_t *cl, const char *str)
{
	if (!cl || !cl->name || !str)
		return -1;
	
	return strcmp (cl->name, str);
}

/**/

/* La fermeture du point de communication par le client ou une erreur
 * quelconque entraînera l'appel de cette fonction. */
static void
client_close (Client_t *cl)
{
	/* On retire ce client du serveur. */
	if (cl)
		server_remove_client (cl->sv, cl);
}
