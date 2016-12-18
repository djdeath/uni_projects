#ifndef __SV_CMD_H__
#define __SV_CMD_H__

/**
 * @file   sv_console.h
 * @author [ATR]Dj-Death
 * @date   Wed Apr 28 17:25:15 2004
 * 
 * @brief La console gère un ensemble de serveur de permet d'agir sur
 * certains de leurs paramètres ainsi que de les contrôler.
 * 
 * 
 */


#include "event.h"

#define CONSOLE(arg) ((Console_t *)(arg))

typedef struct _Console_t Console_t;

/** 
 * Créer une console.
 * 
 * 
 * @return 
 */
Console_t *console_create (void);

/** 
 * Détruire une console.
 * 
 * @param cons 
 */
void console_destroy (Console_t *cons);

#endif
