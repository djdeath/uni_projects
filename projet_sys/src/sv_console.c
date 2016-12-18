#include <stdio.h>
#include <string.h>

#include "event.h"
#include "list.h"
#include "sv_server.h"
#include "sv_console.h"

struct _Console_t
{
	int num, nb;
	Server_t *current;
	List_t *servers;
	
	InputID_t std_inp;
};

/**/

typedef struct _Command_t Command_t;

struct _Command_t
{
	char *name;
	CallBackM_t func;
};

static void
sv_set (Console_t *cons, int argc, const char *argv[])
{
	if (!cons->current)
		return;
	
	if (argc < 2)
	{
		fprintf (stdout, "---\nformat : sv_set option value\n");
		fflush (stdout);
		
		return;
	}
	
	config_set_str (cons->current->config, argv[0], argv[1]);
}

static void
sv_get (Console_t *cons, int argc, const char *argv[])
{
	if (!cons->servers)
		return;
	
	if (argc < 1)
		fprintf (stdout, "---\nformat : sv_get option\n");
	else
	{
		const char *tmp;
		
		if ((tmp = config_get_str (cons->current->config, argv[0])))
			fprintf (stdout, "---\n%s\n", tmp);
	}
	
	fflush (stdout);
}

static void
sv_next (Console_t *cons)
{
	if (!cons->servers)
		return;
	
	if (cons->servers->next)
	{
		cons->num++;
		cons->servers = cons->servers->next;
		cons->current = SERVER (cons->servers->data);
	}
	
	fprintf (stdout, "---\nNow on server %i\n", cons->num);
}

static void
sv_prev (Console_t *cons)
{
	if (!cons->servers)
		return;
	
	if (cons->servers->prev)
	{
		cons->num--;
		cons->servers = cons->servers->prev;
		cons->current = SERVER (cons->servers->data);
	}
	
	fprintf (stdout, "---\nNow on server %i\n", cons->num);
}

static void
sv_create (Console_t *cons, int argc, const char *argv[])
{
	Server_t *sv;
	
	if (argc < 1)
	{
		fprintf (stdout, "---\nformat : sv_create port\n");
		fflush (stdout);
		
		return;
	}
	
	if ((sv = server_create ("sv.cfg", atoi (argv[0]))))
	{
		fprintf (stdout, "---\nserveur cree sur le port %s\n", argv[0]);
		list_append (&cons->servers, sv);
		cons->servers = list_last (cons->servers);
		
		cons->num = cons->nb;
		cons->nb++;
		cons->current = sv;
	}
}

static void
sv_destroy (Console_t *cons)
{
	List_t *n, *tmp;
	
	if (!cons->current)
		return;
	
	tmp = cons->servers;
	
	if (!(n = tmp->prev))
		n = tmp->next;
	else
		cons->num--;
	list_delete (&cons->servers, tmp);
	server_destroy (cons->current);
	cons->nb--;
	
	cons->servers = n;
	if (cons->servers)
		cons->current = SERVER (cons->servers->data);
	else
		cons->current = NULL;
}

static void
sv_start (Console_t *cons)
{
	if (!cons->current)
		return;
	
	game_start (cons->current->game);
}

static void
sv_stop (Console_t *cons)
{
	if (!cons->current)
		return;
	
	game_stop (cons->current->game);
}

static void
sv_pause (Console_t *cons)
{
	if (!cons->current)
		return;
	
	game_pause (cons->current->game);
}

static void
sv_unpause (Console_t *cons)
{
	if (!cons->current)
		return;
	
	game_unpause (cons->current->game);
}

static void
sv_kick (Console_t *cons, int argc, const char *argv[])
{
	Client_t *cl;
	
	if (!cons->current)
		return;
	
	if (argc >= 1)
	{
		if ((cl = server_search_client (cons->current, argv[0])))
			server_remove_client (cons->current, cl);
		else
			fprintf (stdout, "---\nno client found : %s\n", argv[0]);
	}
	else
		fprintf (stdout, "---\nformat : sv_kick name\n");
	
	fflush (stdout);
}

static void
sv_list (Console_t *cons)
{
	List_t *l;
	
	if (!cons->current)
		return;
	
	if ((l = list_first (cons->current->playing_clients)))
	{
		fprintf (stdout, "---\n");
		while (l)
		{
			fprintf (stdout, "%s\n", CLIENT (l->data)->name);
			l = l->next;
		}
		fflush (stdout);
	}
}

static void quit (Console_t *cons);
static void cmd_list (Console_t *cons);

static Command_t sv_cmds[] =
{
	{ "quit", CALLBACKM (quit) }, /* Quitter le programme */
	
	{ "help", CALLBACKM (cmd_list) }, /* Afficher la liste des commandes */
	{ "list", CALLBACKM (cmd_list) }, /* Afficher la liste des commandes */
	
	{ "sv_next", CALLBACKM (sv_next) }, /* Passer au serveur suivant */
	{ "sv_prev", CALLBACKM (sv_prev) }, /* Passer au serveur précédent */
	
	{ "sv_create", CALLBACKM (sv_create) }, /* Créer un nouveau serveur */
	{ "sv_destroy", CALLBACKM (sv_destroy) }, /* Détruire un serveur */
	
	{ "sv_start", CALLBACKM (sv_start) }, /* Démarrer un serveur (si il ne l'est pas) */
	{ "sv_stop", CALLBACKM (sv_stop) }, /* Stopper un serveur */
	{ "sv_pause", CALLBACKM (sv_pause) }, /* Mettre/Retirer un serveur en mode pause */
	{ "sv_unpause", CALLBACKM (sv_unpause) }, /* Mettre/Retirer un serveur en mode pause */

	{ "sv_list", CALLBACKM (sv_list) }, /* liste des clients du serveur x*/
	{ "sv_kick", CALLBACKM (sv_kick) }, /* Virer un client du serveur */
	
	{ "sv_set", CALLBACKM (sv_set) }, /* positionner une variable */
	{ "sv_get", CALLBACKM (sv_get) }, /* afficher le contenue d'une variable */
	
	{ NULL, NULL }
};

static void
cmd_list (Console_t *cons)
{
	int i = 0;
	
	fprintf (stdout, "---\nCommands :\n");
	while (sv_cmds[i].name)
		fprintf (stdout, "%s\n", sv_cmds[i++].name);
	fflush (stdout);
}

static void
quit (Console_t *cons)
{
	if (cons->servers)
	{
		cons->servers = list_first (cons->servers);
		list_foreach (cons->servers, FOREACH (server_destroy), NULL);
		list_free_all (cons->servers);
		cons->servers = NULL;
	}
	event_loop_quit ();
}

/**/

#define CMD_LEN (1024)
#define CMD_MAX_ARGS (10)

static void
console_command (Console_t *cons)
{
	char *s, buf[CMD_LEN], *args[CMD_MAX_ARGS];
	int i, j;
	ssize_t len;
	
	if ((len = read (0, buf, CMD_LEN - 1)) <= 0)
		return;
	
	buf[len - 1] = '\0';
	
	if ((s = strchr (buf, ' ')))
		*s = '\0';
	
	i = 0;
	while (sv_cmds[i].name && strcmp (sv_cmds[i].name, buf))
		i++;
	
	if (sv_cmds[i].name)
	{
		j = 0;
		if (s)
		{
			do
			{
				*s = '\0';
				s++;
				while (*s == ' ')
					s++;
				args[j++] = s;
			} while (((j < CMD_MAX_ARGS - 1)) && (s = strchr (s, ' ')));
		}
		args[j] = NULL;
		
		sv_cmds[i].func (cons, j, args);
	}
	else
	{
		fprintf (stdout, "---\nUnknown command : %s\n", buf);
		fflush (stdout);
	}
}

Console_t *
console_create (void)
{
	Console_t *cons = CONSOLE (calloc (1, sizeof (Console_t)));
	
	if (cons)
	{
		if (!(cons->std_inp = input_add (0, INPUT_DAT, CALLBACK (console_command), cons)))
		{
			free (cons);
			
			return NULL;
		}
	}
	
	return cons;
}

void
console_destroy (Console_t *cons)
{
	if (cons)
	{
		if (cons->servers)
		{
			cons->servers = list_first (cons->servers);
			list_foreach (cons->servers, FOREACH (server_destroy), NULL);
			list_free_all (cons->servers);
		}
		
		if (cons->std_inp)
			input_remove (cons->std_inp);
		free (cons);
	}
}
