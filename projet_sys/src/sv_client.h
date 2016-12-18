#ifndef __SV_CLIENT_H__
#define __SV_CLIENT_H__

/**
 * @file   client.h
 * @author [ATR]Dj-Death
 * @date   Sun Mar  7 21:41:46 2004
 * 
 * @brief  Gestion des clients connectés au serveur.
 * 
 * 
 */

#define CLIENT(arg) ((Client_t *)(arg))

typedef struct _Client_t Client_t;

#include "sv_server.h"
#include "sv_team.h"

/* Taille de base du buffer, si jmais on dépasse cette taille, on
 * double la taille du buffer. */
#define CLIENT_BUF_BLOCK (1024)

struct _Client_t
{
	/* Le serveur sur lequel le client est connecté. */
	Server_t *sv;
	
	/* Le point de communication avec le client ainsi que les
	 * identifants des surveillants du descripteur. */
	int fd;
	InputID_t fd_read, fd_close;
	
	/* Un buffer de données. */
	size_t ubuf_len;
	char buf[CLIENT_BUF_BLOCK];
	
	char *name;
	int score;
	Team_t *team;
};

/**/

/** 
 * Creer une nouveau client.
 * 
 * @param sv Un pointeur sur la structure de client associé.
 * @param fd Le descripteur associé au client.
 * 
 * @return Un pointeur sur une nouvelle structure de client allouée
 * dynnmiquement, NULL en cas d'erreur.
 */
Client_t *client_create (Server_t *sv, int fd);


/** 
 * Détruire un client (c'est à dire toute gestion du descripteur.
 * 
 * @param cl Un pointeur sur une structure client.
 */
void client_destroy (Client_t *cl);

/** 
 * Changer le nom d'un client.
 * 
 * @param cl   Un pointeur sur une structure de client.
 * @param name Un pointeur sur une chaîne de caractère ou NULL pour
 *             mettre le client en pause.
 * 
 * @return 1 si l'opération réussie, 0 sinon.
 */
int client_set_name (Client_t *cl, const char *name);

/** 
 * Changer l'équipe d'un client.
 * 
 * @param cl   Un pointeur sur une structure de client.
 * @param name Un pointeur sur une chaîne de caractère ou NULL pour
 *             que le client n'appartienne à aucune équipe.
 * 
 * @return 1 si l'opération réussie, 0 sinon.
 */
int client_set_team (Client_t *cl, const char *name);

/** 
 * Incrémenter le score d'un client.
 * 
 * @param cl Un pointeur sur une structure de client.
 */
void client_inc_score (Client_t *cl, int points);

/** 
 * Comparer le nom d'un client.
 * 
 * @param cl 
 * @param str 
 * 
 * @return 
 */
int client_name_cmp (const Client_t *cl, const char *str);

/** 
 * Envoyer un message à un client.
 * 
 * @param cl  Un pointeur sur une structure de client.
 * @param fmt Un descripteur de format (identique à la
 *            fonction printf).
 * 
 * @return Le nombre caractères effectivement envoyés.
 */
int client_send (Client_t *cl, const char *fmt, ...);

#endif
