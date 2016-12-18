#include "sv_main.h"

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#ifdef HAVE_LOCALE_H
# include <locale.h>
#endif

#include "event.h"
#include "sv_console.h"

static void
main_exit (int sig_recv)
{
	event_loop_quit ();
}

static void
main_pipe (int sig)
{
	
}

static void
get_stop (void)
{
#ifdef HAVE_SIGNAL
	signal (SIGINT, main_exit);
	signal (SIGQUIT, main_exit);
	signal (SIGPIPE, main_pipe);
#endif
}

void
init (void)
{
	srand ((unsigned int) time (NULL));
#ifdef HAVE_SETLOCALE
	setlocale (LC_ALL, NULL);
#endif
	event_init ();
}

int
main (int argc, char *argv[])
{
	Console_t *cons;
	
	/* Choppage de signaux */
	get_stop ();
	
	/* Initialisation des différents composants du serveur (boucle
	 * d'attente des évènements, etc..) */
	init ();
	
	cons = console_create ();
	
	/* Boucle d'attente */
	event_loop ();
	
	console_destroy (cons);
	
	event_finish ();
	
	return EXIT_SUCCESS;
}
