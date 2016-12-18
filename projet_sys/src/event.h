#ifndef __EVENT_H__
#define __EVENT_H__

/**
 * @file   event.h
 * @author [ATR]Dj-Death
 * @date   Sun Mar 21 04:55:01 2004
 * 
 * @brief  Syst�me de gestion des �venements.
 * 
 * Ce syst�me se base sur l'appel syst�me select ou poll suivant la
 * disponibilit� du syst�me d'exploitation sur lequel il est install�. Ce
 * syst�me permet d'�tre pr�venu si des donn�es sont disponibles en lecture
 * sur un descripteur de fichier ou d'�tre pr�venu lorsqu'un certain labs de
 * temps est �coul� et ceci gr�ce � des fonctions de rappel.
 *
 */

#include "sv_main.h"

#include <time.h>
#include <unistd.h>
#ifdef HAVE_SYS_POLL_H
# include <sys/poll.h>
#else
enum
{
	POLLIN = 0x0001,
	POLLPRI = 0x0002,
	POLLOUT = 0x0004,
	POLLERR = 0x0008,
	POLLHUP = 0x0010,
	POLLNVAL = 0x0020
};
#endif

#define INPUT_DAT (POLLIN | POLLPRI)
#define INPUT_HUP (POLLHUP)
#define INPUT_ERR (POLLERR)

#include "data.h"

typedef unsigned int TimerID_t;
typedef unsigned int InputID_t;

/** 
 * 
 * 
 * @param timeout 
 * @param handler 
 * @param data 
 * 
 * @return 
 */
TimerID_t timer_add (time_t timeout, CallBack_t handler, void *data);

/** 
 * 
 * 
 * @param id 
 */
void timer_remove (TimerID_t id);

/** 
 * 
 * 
 * @param fd 
 * @param mask 
 * @param handler 
 * @param data 
 * 
 * @return 
 */
InputID_t input_add (int fd, short mask, CallBack_t handler, void *data);

/** 
 * 
 * 
 * @param id 
 */
void input_remove (InputID_t id);

/** 
 * Initialiser le syst�me d'�venements (ceci est n�cessaireavant tout
 * appel d'autres fonction d�crites dans ce fichier).
 * 
 */
void event_init (void);

/** 
 * D�sinitialisation du syst�me d'�venements.
 * 
 */
void event_finish (void);

/** 
 * Entrer dans la boucle d'�venement (qui sortira automatiquement si
 * il n'y a rien � surveiller).
 * 
 */
void event_loop (void);

/** 
 * Quitter le boucle d'�venements.
 * 
 */
void event_loop_quit (void);

#endif
