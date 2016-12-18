#ifndef __SV_TEAM_H__
#define __SV_TEAM_H__

/**
 * @file   sv_team.h
 * @author [ATR]Dj-Death
 * @date   Sun Mar 21 04:52:01 2004
 * 
 * @brief  Gestion des �quipes.
 * 
 * 
 */

#define TEAM(arg) ((Team_t *)(arg))

typedef struct _Team_t Team_t;

#include "sv_server.h"
#include "sv_client.h"

struct _Team_t
{
	/* Le serveur sur lequel joue l'�quipe. */
	Server_t *sv;
	
	/* Nom de l'�quipe & score */
       	char *name;
	int score;
	
	/* Les clients */
	int nb_clients;
	List_t *clients;
};

/**/

/** 
 * Cr�er une nouvelle structure d'�quipe.
 * 
 * @param sv   Le serveur associ� � l'�quipe
 * @param name Le nom de l'�quipe
 * 
 * @return Un pointeur sur une nouvelle structure d'�quipe allou�e dynamiquement.
 */
Team_t *team_create (Server_t *sv, const char *name);

/** 
 * Liberer une structure d'�quipe.
 * 
 * @param team Une pointeur sur une structure d'�quipe.
 */
void team_destroy (Team_t *team);

/** 
 * Augmenter le score d'une �quipe.
 * 
 * @param team   L'�quipe
 * @param points Le nombre de point � ajouter � son score
 */
void team_inc_score (Team_t *team, int points);

/** 
 * Ajouter un client � une �quipe.
 * 
 * @param team 
 * @param cl 
 * 
 * @return 
 */
int team_add_client (Team_t *team, Client_t *cl);

/** 
 * Retirer un client � une �quipe.
 * 
 * @param team 
 * @param cl 
 * 
 * @return 
 */
int team_remove_client (Team_t *team, Client_t *cl);

/** 
 * Comparer le nom d'une �quipe.
 * 
 * @param team 
 * @param str 
 * 
 * @return 
 */
int team_name_cmp (const Team_t *team, const char *str);

#endif
