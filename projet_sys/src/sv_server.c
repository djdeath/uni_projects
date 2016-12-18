#include "sv_main.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/types.h>
#include <unistd.h>

#include "list.h"
#include "network.h"
#include "util.h"

#include "sv_server.h"

/**/

#define SERVER_MAX_NAME (64)

static void server_read (Server_t *sv);
static void server_close (Server_t *sv);

/**/

Server_t *
server_create (const char *cfgfile, in_port_t port)
{
	Server_t *sv;
	
	if (port && (sv = SERVER (calloc (1, sizeof (Server_t)))))
	{
		sv->fd = -1;
		
		if (!(sv->config = config_load (cfgfile)))
		{
			server_destroy (sv);
			
			return NULL;
		}
		
		if (!(sv->game = game_create (sv)))
		{
			server_destroy (sv);
			
			return NULL;
		}
		
		/* On bind le port en dernier pour ne pas faire
		 * attendre les clients qui se connecterai rapidement
		 * (le chargement du dictionnaire peut être plus ou
		 * moins long). */
		if ((sv->fd = net_bind (port)) == -1)
		{
			server_destroy (sv);
			
			return NULL;
		}
		
		sv->fd_read = input_add (sv->fd, INPUT_DAT, CALLBACK (server_read), sv);
		sv->fd_close = input_add (sv->fd, INPUT_HUP | INPUT_ERR, CALLBACK (server_close), sv);
	}
		
	return sv;
}

void
server_destroy (Server_t *sv)
{
	if (sv)
	{
		/* Arrêt du jeux */
		if (sv->game)
			game_destroy (sv->game);
		
		/* Fermeture de toutes les connections clientes. */
		if (sv->playing_clients)
		{
			list_foreach (sv->playing_clients, FOREACH (client_destroy), NULL);
			list_free_all (sv->playing_clients);
		}
		if (sv->waiting_clients)
		{
			list_foreach (sv->waiting_clients, FOREACH (client_destroy), NULL);
			list_free_all (sv->waiting_clients);
		}
		if (sv->teams)
		{
			list_foreach (sv->teams, FOREACH (team_destroy), NULL);
			list_free_all (sv->teams);
		}
		
		if (sv->config)
			config_free (sv->config);
		
		/* IDEM pour le serveur */
		input_remove (sv->fd_read);
		input_remove (sv->fd_close);
		if (sv->fd != -1)
			net_close (sv->fd);
		
		free (sv);
	}
}

/**/

int
server_set_client_play (Server_t *sv, Client_t *cl)
{
	List_t *l = NULL;
	
	if (!sv || !cl)
		return 0;
	
	if (list_find_d (sv->waiting_clients, cl, &l) == -1)
		return 0;
	
	if (list_remove (&sv->waiting_clients, l))
		list_insert_first_e (&sv->playing_clients, l);
	
	server_send (sv, "_%s:%i\n", cl->name, cl->score);
	server_send (sv, "# %s has join the game !\n", cl->name);
	
	return 1;
}

int
server_set_client_wait (Server_t *sv, Client_t *cl)
{
	List_t *l = NULL;
	
	if (!sv || !cl)
		return 0;
	
	if (list_find_d (sv->playing_clients, cl, &l) == -1)
		return 0;
	
	if (list_remove (&sv->playing_clients, l))
		list_insert_first_e (&sv->waiting_clients, l);
	
	server_send (sv, "_%s:%i\n", cl->name, -1);
	server_send (sv, "# %s has left the game !\n", cl->name);
	
	return 1;
}

/* Search */

Client_t *
server_search_client (Server_t *sv, const char *name)
{
	List_t *l;
	
	if (!sv || !name)
		return NULL;
	
	if (list_find (sv->playing_clients, name, COMPARE (client_name_cmp), &l) != -1)
		return CLIENT (l->data);
	
	return NULL;
}

Team_t *
server_search_team (Server_t *sv, const char *name)
{
	List_t *l;
	
	if (!sv || !name)
		return NULL;
	
	if (list_find (sv->teams, name, COMPARE (team_name_cmp), &l) != -1)
		return TEAM (l->data);
	
	return NULL;
}


/* CLIENTS */

int
server_add_client (Server_t *sv, Client_t *cl)
{
	const char *entry;
	List_t *l;
	Team_t *tms;
	Client_t *cls;
	
	if (!sv || !cl)
		return 0;
	
	if (!list_insert_first (&(sv->waiting_clients), cl))
		return 0;
	
	sv->nb_clients++;
	
	if ((entry = config_get_str (sv->config, "sv_entry_msg")))
		client_send (cl, "# %s\n", entry);
	
	/* Les informations sur le jeu en cours. */
	game_send_client_infos (sv->game, cl);
	
	/* Affichage de la liste des clients déjà connectés. */
	l = sv->playing_clients;
	while (l)
	{
		cls = CLIENT (l->data);
		client_send (cl, "_%s:%i\n", cls->name, cls->score);
		l = l->next;
	}
	
	/* IDEM pour les équipes */
	l = sv->teams;
	while (l)
	{
		tms = TEAM (l->data);
		client_send (cl, "=%s:%i\n", tms->name, tms->score);
		l = l->next;
	}
	
	return 1;
}

int
server_remove_client (Server_t *sv, Client_t *cl)
{
	List_t *l = NULL;
	
	if (!sv || !cl)
		return 0;
	
	/* Il y a 2 listes à traiter. Le client doit être dans l'une
	 * des deux. */
	if (list_find_d (sv->playing_clients, cl, &l) == -1)
	{
		if (list_find_d (sv->waiting_clients, cl, &l) == -1)
			return 0;
		if (!list_delete (&sv->waiting_clients, l))
			return 0;
	}
	else
	{
		if (!list_delete (&sv->playing_clients, l))
			return 0;
	}
	
	if (cl->name)
	{
		server_send (sv, "_%s:%i\n", cl->name, -1);
		server_send (sv, "# %s has left the game !\n", cl->name);
	}
	
	/* Le serveur ne contient plus aucune référence au client, on
	 * peut donc le détruire. */
	client_destroy (cl);
	sv->nb_clients--;
	
	return 1;
}

/* TEAMS */

int
server_add_team (Server_t *sv, Team_t *team)
{
	List_t *l;
	
	if (!sv || !team)
		return 0;
	
	/* La team fait déjà partie du serveur. */
	if (list_find_d (sv->teams, team, &l) != -1)
		return 0;
	
	/* L'insertion a CHIÉE DU CUL !! */
	if (!list_insert_first (&sv->teams, team))
		return 0;
	
	/*server_send (sv, "\"%s:%i\n", team->name, team->score);*/
	
	return 1;
}

int
server_remove_team (Server_t *sv, Team_t *team)
{
	List_t *l;
	
	if (!sv || !team)
		return 0;
	
	/* Cette team ne fait déjà plus partie du serveur. */
	if (list_find_d (sv->teams, team, &l) == -1)
		return 0;
	
	if (!list_delete (&sv->teams, l))
		return 0;
	
	server_send (sv, "=%s:%i\n", team->name, -1);
	team_destroy (team);
	
	return 1;
}

void
server_send (Server_t *sv, const char *fmt, ...)
{
	int len;
	char buf[MAX_SEND];
	va_list args;
	List_t *l;
	Client_t *cl;
	
	if (!sv || !fmt)
		return;
	
	va_start (args, fmt);
	len = vsnprintf (buf, MAX_SEND, fmt, args);
	va_end (args);
	
	if (len < 1)
		return;
	
	if (len > MAX_SEND)
		len = MAX_SEND;
	
	l = sv->playing_clients;
	while (l)
	{
		cl = CLIENT (l->data);
		write (cl->fd, buf, len);
		l = l->next;
	}
	
	l = sv->waiting_clients;
	while (l)
	{
		cl = CLIENT (l->data);
		write (cl->fd, buf, len);
		l = l->next;
	}
}

/* Fonctions de rappel sur les différents surveillants. */

static void
server_read (Server_t *sv)
{
	int fd;
	
	if ((fd = net_accept (sv->fd)) != -1)
	{
		Client_t *cl;
		
		/* Si il n'y a pas assez de place, on envoit un petit message
		 * au client. */
		if (config_get_int (sv->config, "sv_max_clients") <= sv->nb_clients)
		{
			fdnprintf (fd, MAX_SEND, "Server is full.\n");
			net_close (fd);
			
			return;
		}
		
		if (!(cl = client_create (sv, fd)))
		{
			net_close (fd);
			
			return;
		}
		
		if (!server_add_client (sv, cl))
		{
			client_destroy (cl);
		}
	}
}

static void
server_close (Server_t *sv)
{
	server_destroy (sv);
}
