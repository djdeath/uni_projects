#include <string.h>
#include <stdlib.h>

#include "util.h"

#include "sv_team.h"

/**/

#define TEAM_MAX_NAME (64)

/**/

void
team_destroy (Team_t *team)
{
	if (team)
	{
		if (team->clients)
			list_free_all (team->clients);
		if (team->name)
			free (team->name);
		free (team);
	}
}

Team_t *
team_create (Server_t *sv, const char *name)
{
	Team_t *team = TEAM (calloc (1, sizeof (Team_t)));
	
	if (team)
	{
		team->sv = sv;
		
		if (!(team->name = strndup (name, TEAM_MAX_NAME, NULL)))
		{
			team_destroy (team);
			
			return NULL;
		}
	}
	
	return team;
}

/**/

void
team_inc_score (Team_t *team, int points)
{
	if (team)
	{
		team->score += points;
		server_send (team->sv, "=%s:%i\n", team->name, team->score);
	}
}

/**/

int
team_add_client (Team_t *team, Client_t *cl)
{
	List_t *l = NULL;
	
	if (!team || !cl)
		return 0;
	
	if (list_find_d (team->clients, cl, &l) >= 0)
		return 0;
	
	if (!list_insert_first (&team->clients, cl))
		return 0;
	
	team->score += cl->score;
	team->nb_clients++;
	server_send (team->sv, "=%s:%i\n", team->name, team->score);
	
	return 1;
}

int
team_remove_client (Team_t *team, Client_t *cl)
{
	List_t *l;
	
	if (!team || !cl)
		return 0;
	
	if (list_find_d (team->clients, cl, &l) == -1)
		return 0;
	
	if (!list_delete (&team->clients, l))
		return 0;
	
	team->score -= cl->score;
	team->nb_clients--;
	
	if (team->nb_clients < 1)
		server_remove_team (team->sv, team);
	else
		server_send (team->sv, "=%s:%i\n", team->name, team->score);
	
	return 1;
}

int
team_name_cmp (const Team_t *team, const char *str)
{
	if (!team || !team->name || !str)
		return -1;
	
	return strcmp (team->name, str);
}
