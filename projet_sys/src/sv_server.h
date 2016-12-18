#ifndef __SV_SERVER_H__
#define __SV_SERVER_H__

/**
 * @file   sv_server.h
 * @author [ATR]Dj-Death
 * @date   Sun Mar 21 04:51:45 2004
 * 
 * @brief  Gestion d'un serveur.
 * 
 * 
 */

#include "config.h"
#include "event.h"
#include "list.h"
#include "network.h"

#define SERVER(arg) ((Server_t *)(arg))

#define MAX_SEND (1024)

typedef struct _Server_t Server_t;

#include "sv_team.h"
#include "sv_client.h"
#include "sv_game.h"

struct _Server_t
{
	/* Point de communication en attente de clients. Leur
	 * connection entraîne la création d'un autre point de
	 * communication qui gèrera les données envoyées par le
	 * client. */
	int fd;
	InputID_t fd_read, fd_close;
	
	/* Nombre de clients connectés, et maximum de clients
	 * autorisés (-1 signifie qu'il n'y a pas de limite). */
	int nb_clients;
	
	/* Liste des clients (play les clients peuvent jouer, wait les
	 * clients sont en attente car ils n'ont pas fournit assez
	 * d'informations pour faire partie du jeu). */
	List_t *playing_clients, *waiting_clients;
	List_t *teams;
	
	/* Configuration du serveur. */
	Config_t *config;
	
	/* Caractéristiques du jeu en cours. */
	Game_t *game;
};

/**/

/** 
 * Créer un nouveau serveur.
 * 
 * @param cfgfile chemin vers le fichier de configuration
 * @param port    port à binder
 * 
 * @return une nouvelle structure allouée dynamiquement, ou NULL en
 * cas d'échec.
 */
Server_t *server_create (const char *cfgfile, in_port_t port);

/** 
 * Détruire un serveur.
 * 
 * @param sv pointer sur une structure de serveur.
 */
void server_destroy (Server_t *sv);

/** 
 * Retirer un client d'un serveur.
 * 
 * @param sv 
 * @param cl 
 * 
 * @return 1, ou 0 en cas d'échec.
 */
int server_remove_client (Server_t *sv, Client_t *cl);

/** 
 * Mettre un joueur en mode actif.
 * 
 * @param sv 
 * @param cl 
 * 
 * @return 1, ou 0 en cas d'échec.
 */
int server_set_client_play (Server_t *sv, Client_t *cl);

/** 
 * Mettre un joueur en mode passif.
 * 
 * @param sv 
 * @param cl 
 * 
 * @return 1, ou 0 en cas d'échec.
 */
int server_set_client_wait (Server_t *sv, Client_t *cl);

/** 
 * Ajouter une équipe à un serveur.
 * 
 * @param sv 
 * @param team 
 * 
 * @return 1, ou 0 en cas d'échec.
 */
int server_add_team (Server_t *sv, Team_t *team);

/** 
 * Retirer une équipe à un serveur.
 * 
 * @param sv 
 * @param team 
 * 
 * @return 1, ou 0 en cas d'échec.
 */
int server_remove_team (Server_t *sv, Team_t *team);

/** 
 * Envoyer un message à l'ensemble des clients d'un serveur.
 * 
 * @param sv 
 * @param fmt 
 */
void server_send (Server_t *sv, const char *fmt, ...);

/** 
 * Rechercher une joueur sur un serveur.
 * 
 * @param sv 
 * @param name 
 * 
 * @return 
 */
Client_t *server_search_client (Server_t *sv, const char *name);

/** 
 * Rechercher une équipe sur un serveur.
 * 
 * @param sv 
 * @param name 
 * 
 * @return 
 */
Team_t *server_search_team (Server_t *sv, const char *name);


#endif
