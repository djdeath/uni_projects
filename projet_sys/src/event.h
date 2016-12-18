#ifndef __EVENT_H__
#define __EVENT_H__

/**
 * @file   event.h
 * @author [ATR]Dj-Death
 * @date   Sun Mar 21 04:55:01 2004
 * 
 * @brief  Système de gestion des évenements.
 * 
 * Ce système se base sur l'appel système select ou poll suivant la
 * disponibilité du système d'exploitation sur lequel il est installé. Ce
 * système permet d'être prévenu si des données sont disponibles en lecture
 * sur un descripteur de fichier ou d'être prévenu lorsqu'un certain labs de
 * temps est écoulé et ceci grâce à des fonctions de rappel.
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
 * Initialiser le système d'évenements (ceci est nécessaireavant tout
 * appel d'autres fonction décrites dans ce fichier).
 * 
 */
void event_init (void);

/** 
 * Désinitialisation du système d'évenements.
 * 
 */
void event_finish (void);

/** 
 * Entrer dans la boucle d'évenement (qui sortira automatiquement si
 * il n'y a rien à surveiller).
 * 
 */
void event_loop (void);

/** 
 * Quitter le boucle d'évenements.
 * 
 */
void event_loop_quit (void);

#endif
