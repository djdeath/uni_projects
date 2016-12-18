#ifndef __SV_GAME_H__
#define __SV_GAME_H__

/**
 * @file   sv_game.h
 * @author [ATR]Dj-Death
 * @date   Sun Mar 21 04:52:28 2004
 * 
 * @brief  Gestion d'une partie.
 * 
 * 
 */

#define GAME(arg) ((Game_t *)(arg))

typedef struct _Game_t Game_t;

#include "sv_server.h"
#include "sv_client.h"
#include "sv_dict.h"

typedef enum
{
	GAME_STATIC = 0, /* On attend que le client ait tap� un mot pour
			  * en renvoyer un autre. */
	GAME_DYNAMIC /* On envoit des mots sans se soucier de la
		      * r�ponse du client. */
} GameType_t;

struct _Game_t
{
	/* Serveur associ� � la partie. */
	Server_t *sv;
	
	/* Type de la partie. */
	GameType_t type;
	
	size_t width, height;
	
	int started : 1; /* Le jeux a �t� lanc� ? */
	int paused : 1;  /* Le jeux est en pause ? */
	
	/* Ensemble des mots dans lesquels utilisable. */
	Dict_t *dict;
	
	/* Nombre maximum de mots � envoyer et mots d�j� envoy�s. */
	int words_sent;
	int words_sent_limit;
	int words_recv;
	
	double speed;
	double speed_start;
	double accel;
	double accel_timeout;
	
	/* Mots utilis� */
	HashTable_t *ht;
	char **map;
	
	TimerID_t acceler;
	TimerID_t sender;
};

/** 
 * Cr�er une partie.
 * 
 * @param sv 
 * 
 * @return 
 */
Game_t *game_create (Server_t *sv);

/** 
 * 
 * 
 * @param game 
 */
void game_destroy (Game_t *game);

/** 
 * D�marrer une partie
 * 
 * @param game 
 * 
 * @return 
 */
int game_start (Game_t *game);

/** 
 * Arr�ter une partie.
 * 
 * @param game 
 * 
 * @return 
 */
int game_stop (Game_t *game);

/** 
 * Mettre en pause une partie.
 * 
 * @param game 
 * 
 * @return 
 */
int game_pause (Game_t *game);

/** 
 * Reprendre la partie en pause.
 * 
 * @param game 
 * 
 * @return 
 */
int game_unpause (Game_t *game);

/** 
 * Envoyer � un client les informations sur le jeux en cours.
 * 
 * @param game 
 * @param cl 
 */
void game_send_client_infos (Game_t *game, Client_t *cl);

/** 
 * Fonction de rappel pour les mots tap�s par les clients.
 * 
 * @param game 
 * @param cl 
 * @param word 
 */
void game_client_response (Game_t *game, Client_t *cl, const char *word);

#endif
