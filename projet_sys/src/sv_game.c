#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

#include "sv_game.h"

#define MAP_IS_WORD(map,i,j) ((map)[i][j])

/**/

static void game_insert_word (Game_t *game);
static void game_remove_word (Game_t *game, Word_t *w);
static void game_accel (Game_t *game);

/**/

static void
game_map_insert (Game_t *game, int x, int y, int width)
{
	if ((x >= 0) && (y >= 0) && (width > 0))
		memset (&game->map[y][x], 1, (size_t) width);
}

static void
game_map_remove (Game_t *game, int x, int y, int width)
{
	if ((x >= 0) && (y >= 0) && (width > 0))
		memset (&game->map[y][x], 0, (size_t) width);
}

static int
game_map_alloc (Game_t *game, int width, int height)
{
	int i;
	
	if (!(game->map = (char **) calloc (height, sizeof (char *))))
		return 0;
	
	for (i = 0U ; i < height ; i++)
		if (!(game->map[i] = (char *) calloc (width, sizeof (char))))
			return 0;
	
	return 1;
}

static void
game_map_free (Game_t *game)
{
	if (game->map)
	{
		size_t i;
		
		for (i = 0U ; i < game->height ; i++)
			if (game->map[i])
				free (game->map[i]);
		free (game->map);
		game->map = NULL;
	}
}

/**/

void
game_destroy (Game_t *game)
{
	if (game)
	{
		timer_remove (game->acceler);
		
		switch (game->type)
		{
		case GAME_DYNAMIC:
			break;
			
		case GAME_STATIC:
			timer_remove (game->sender);
			break;
		}
		
		if (game->map)
			game_map_free (game);
		if (game->ht)
			hash_table_free (game->ht);
		if (game->dict)
			dict_free (game->dict);
		free (game);
	}
}

Game_t *
game_create (Server_t *sv)
{
	Game_t *game;
	
	if (!sv)
		return NULL;
	
	if (!(game = GAME (calloc (1, sizeof (Game_t)))))
		return game;
	
	/* Type de jeu */
	game->type = config_get_int (sv->config, "game_type");
	
	if ((game->type != GAME_DYNAMIC) &&
	    (game->type != GAME_STATIC))
	{
		free (game);
		
		return NULL;
	}
	
	/* Taille de la map */
	game->width = config_get_int (sv->config, "game_width");
	game->height = config_get_int (sv->config, "game_height");
	
	if ((game->width < 1) || (game->height < 1))
	{
		free (game);
		
		return NULL;
	}
	
	/* Allocation de la map */
	if (!game_map_alloc (game, game->width, game->height))
	{
		game_destroy (game);
		
		return NULL;
	}
	
	/* Autres données du jeu */
	game->speed_start = config_get_double (sv->config, "game_speed");
	game->accel = config_get_double (sv->config, "game_accel");
	game->accel_timeout = config_get_double (sv->config, "game_accel_timeout");
	
	if (!(game->dict = dict_create (config_get_str (sv->config, "dict_file"),
					config_get_int (sv->config, "dict_num"))))
	{
		game_destroy (game);
		
		return NULL;
	}
	
	if (!(game->ht = hash_table_new (HASH_TABLE_CLOSE_DEFAULT_SIZE,
					 HASH_MODE_OPEN, HASH_FUNC_GLIB)))
	{
		game_destroy (game);
		
		return NULL;
	}
	
	game->sv = sv;
	game->started = 0;
	
	return game;
}

/**/

static void
_game_send_client_word (Word_t *w, Client_t *cl)
{
	client_send (cl, "+\e[0;%im%s:%i:%i\n", w->color, w->word, w->x, w->y);
}

void
game_send_client_infos (Game_t *game, Client_t *cl)
{
	if (!game || !cl)
		return;
	
	client_send (cl, "=%u:%u\n", game->width, game->height);
	
	if (game->started)
		hash_table_foreach (game->ht, FOREACH (_game_send_client_word), cl);
}

/**/

int
game_start (Game_t *game)
{
	double i;
	
	if (!game || game->started)
		return 0;
	
	game->type = config_get_int (game->sv->config, "game_type");
	game->speed_start = config_get_double (game->sv->config, "game_speed");
	game->accel = config_get_double (game->sv->config, "game_accel");
	game->accel_timeout = config_get_double (game->sv->config, "game_accel_timeout");
	
	game->speed = game->speed_start;
	game->words_sent = 0;
	game->words_recv = 0;
	
	switch (game->type)
	{
	case GAME_STATIC:
		game->sender = timer_add ((unsigned int) (1000 / game->speed),
					  CALLBACK (game_insert_word), game);
		game_insert_word (game);
		break;
		
	case GAME_DYNAMIC:
		i = floor ((double) game->speed);
		while ((i--) > 0.0)
			game_insert_word (game);
		break;
		
	default:
		return 0;
	}
	
	game->acceler = timer_add ((unsigned int) (1000 * game->accel_timeout),
				   CALLBACK (game_accel), game);
	
	server_send (game->sv, "# Game starts\n");
	
	game->started = 1;
	
	return 1;
}

static void
_game_stop (Word_t *w, Server_t *sv)
{
	server_send (sv, "-\e[0;%im%s:%i:%i\n", w->color, w->word, w->x, w->y);
}

int
game_stop (Game_t *game)
{
	if (!game | !game->started)
		return 0;
	
	game->started = 0;
	
	timer_remove (game->acceler);
	game->acceler = 0U;
	
	switch (game->type)
	{
	case GAME_STATIC:
		timer_remove (game->sender);
		game->sender = 0U;
		break;
		
	case GAME_DYNAMIC:
		break;
	}
	
	hash_table_foreach (game->ht, FOREACH (_game_stop), game->sv);
	hash_table_clear (game->ht);
	
	server_send (game->sv, "# Game stopped\n");
	
	return 1;
}

int
game_pause (Game_t *game)
{
	if (!game || game->paused || !game->started)
		return 0;
	
	game->paused = 1;
	
	timer_remove (game->acceler);
	game->acceler = 0U;
	
	switch (game->type)
	{
	case GAME_STATIC:
		timer_remove (game->sender);
		game->sender = 0U;
		break;
		
	case GAME_DYNAMIC:
		break;
	}
	
	server_send (game->sv, "# Game paused\n");
	
	return 1;
}

int
game_unpause (Game_t *game)
{
	if (!game || !game->paused || !game->started)
		return 0;
	
	game->paused = 0;
	
	server_send (game->sv, "# Attention REPRISE DU JEUX !!!!!\n");
	
	game->acceler = timer_add ((unsigned int) (1000 * game->accel_timeout),
				   CALLBACK (game_accel), game);
	
	switch (game->type)
	{
	case GAME_STATIC:
		game->sender = timer_add ((unsigned int) (1000 / game->speed),
					  CALLBACK (game_insert_word), game);
		game_insert_word (game);
		break;
		
	case GAME_DYNAMIC:
		break;
	}
	
	return 1;
}

void
game_client_response (Game_t *game, Client_t *cl, const char *word)
{
	Word_t *w = NULL;
	
	if (!game || !cl || !word)
		return;
	
	/* Si le serveur est en pause ou arrêté, on ne tient pas
	 * compte des words envoyés par les clients. */
	if (game->paused || !game->started)
		return;
	
	switch (game->type)
	{
	case GAME_STATIC:
		while ((w = WORD (hash_table_search (game->ht, word, w))))
		{
			if (!strcmp (w->word, word))
			{
				game->words_recv++;
				game_remove_word (game, w);
				client_inc_score (cl, w->len);
				break;
			}
		}
		break;
		
	case GAME_DYNAMIC:
		while ((w = WORD (hash_table_search (game->ht, word, w))))
		{
			if (!strcmp (w->word, word))
			{
				game->words_recv++;
				game_remove_word (game, w);
				client_inc_score (cl, w->len);
				game_insert_word (game);
				break;
			}
		}
		break;
	}
	
	if (!w)
		client_send (cl, "?%s\n", word);
}

/**/

static void
game_insert_word (Game_t *game)
{
	int i, j, k, w_space;
	Word_t *w, *wp;
	
	if (!game)
		return;
	
	if (game->words_sent >= config_get_int (game->sv->config, "game_limit"))
	{
		if (game->words_recv >= game->words_sent)
		{
			game_stop (game);
			game_start (game);
		}
		
		return;
	}
	
	for (i = 0 ; i < 3 ; i++)
	{
		for (j = 0 ; j < 3 ; j++)
		{
			if ((w = dict_rand (game->dict)) && (w->len <= game->width))
			{
				wp = NULL;
				while ((wp = WORD (hash_table_search (game->ht, w->word, wp))))
				{
					if (!strcmp (wp->word, w->word))
					{
						w = NULL;
						break;
					}
				}
			}
			else
				w = NULL;
		}
		
		/* On a fait 3 essais pour trouver un mot de longueur
		 * inférieure ou égal à la longueur de la map. Si
		 * jamais l'on a rien trouvé, on retentera plus
		 * tard. */
		if (!w)
			return;
		
		for (j = 0 ; j < 3 ; j++)
		{
			/* On commence par prendre une hauteur au
			 * hazard. */
			w->y = rand () % game->height;
			
			w->x = -1;
			w_space = 0;
			for (k = 0 ; k < game->width ; k++)
			{
				if (!MAP_IS_WORD (game->map, w->y, k))
				{
					if (w->x < 0)
						w->x = k;
					w_space++;
				}
				else
				{
					if ((w->x >= 0) && ((w_space - 2) > ((int) w->len)))
						break;
					w->x = -1;
					w_space = 0;
				}
			}
			
			if (w_space != (game->width - 1))
				w_space--;
			
			if (w->x > 1)
			{
				w->x++;
				w_space--;
			}
			
			if (w_space > ((int) w->len))
			{
				w->x += rand () % (w_space - w->len);
				break;
			}
		}
		
		if (j >= 3)
			return;
		
		if (w && !hash_table_insert (game->ht, w->word, w))
		{
			w->color = 30 + rand () % 8;
			
			game_map_insert (game, w->x, w->y, w->len);
			
			game->words_sent++;
			server_send (game->sv, "+\e[0;%im%s:%i:%i\n",
				     w->color, w->word, w->x, w->y);
			
			return;
		}
	}
}

static void
game_remove_word (Game_t *game, Word_t *w)
{
	hash_table_remove (game->ht, w->word, w);
	game_map_remove (game, w->x, w->y, w->len);
	
	server_send (game->sv, "-\e[0;%im%s:%i:%i\n",
		     w->color, w->word, w->x, w->y);
}

static void
game_accel (Game_t *game)
{
	double old_speed;
	
	if (!game)
		return;
	
	old_speed  = game->speed;
	game->speed += game->accel * game->accel_timeout;
	
	switch (game->type)
	{
	case GAME_STATIC:
		timer_remove (game->sender);
		game->sender = timer_add ((unsigned int) (1000 / game->speed),
					  CALLBACK (game_insert_word), game);
		break;
		
	case GAME_DYNAMIC:
		old_speed = floor ((double) game->speed) - floor ((double) old_speed);
		while (old_speed-- > 0)
			game_insert_word (game);
		break;
	}
}
