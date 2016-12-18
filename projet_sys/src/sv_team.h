#ifndef __SV_TEAM_H__
#define __SV_TEAM_H__

/**
 * @file   sv_team.h
 * @author [ATR]Dj-Death
 * @date   Sun Mar 21 04:52:01 2004
 * 
 * @brief  Gestion des équipes.
 * 
 * 
 */

#define TEAM(arg) ((Team_t *)(arg))

typedef struct _Team_t Team_t;

#include "sv_server.h"
#include "sv_client.h"

struct _Team_t
{
	/* Le serveur sur lequel joue l'équipe. */
	Server_t *sv;
	
	/* Nom de l'équipe & score */
       	char *name;
	int score;
	
	/* Les clients */
	int nb_clients;
	List_t *clients;
};

/**/

/** 
 * Créer une nouvelle structure d'équipe.
 * 
 * @param sv   Le serveur associé à l'équipe
 * @param name Le nom de l'équipe
 * 
 * @return Un pointeur sur une nouvelle structure d'équipe allouée dynamiquement.
 */
Team_t *team_create (Server_t *sv, const char *name);

/** 
 * Liberer une structure d'équipe.
 * 
 * @param team Une pointeur sur une structure d'équipe.
 */
void team_destroy (Team_t *team);

/** 
 * Augmenter le score d'une équipe.
 * 
 * @param team   L'équipe
 * @param points Le nombre de point à ajouter à son score
 */
void team_inc_score (Team_t *team, int points);

/** 
 * Ajouter un client à une équipe.
 * 
 * @param team 
 * @param cl 
 * 
 * @return 
 */
int team_add_client (Team_t *team, Client_t *cl);

/** 
 * Retirer un client à une équipe.
 * 
 * @param team 
 * @param cl 
 * 
 * @return 
 */
int team_remove_client (Team_t *team, Client_t *cl);

/** 
 * Comparer le nom d'une équipe.
 * 
 * @param team 
 * @param str 
 * 
 * @return 
 */
int team_name_cmp (const Team_t *team, const char *str);

#endif
