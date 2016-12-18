#ifndef __NETWORK_H__
#define __NETWORK_H__

/*****************************************************************************/

/**
 * @file   network.h
 * @author [ATR]Dj-Death
 * @date   Sun Mar  7 21:22:02 2004
 * 
 * @brief  Quelques routines simple pour g�rer les connections.
 * 
 * Contient un ensemble de fonction pour g�rer facilement les connections
 * clientes, serveur, etc... Ne supporte que les sockets TCP.
 *
 */

/*****************************************************************************/

#include "mptc_config.h"

#include <sys/types.h>

#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif

/*#ifndef HAVE_IN_PORT_T
typedef unsigned short in_port_t;
#endif*/

/**
 * Rendre un descripteur bloquant ou non.
 *
 * @param fd Descripteur que l'on souhaiter modifier.
 * @param val Si val 0 on rend le descripteur non bloquant, sinon on
 * le rend bloquant.
 *
 * @return 0
 *   La fonction a r�ussie.
 * @return -1
 *   Descripteur invalide.
 */
int net_block (int fd, int val);

/**
 * Fermer une connection.
 *
 * @param fd Descripteur que l'on souhaiter fermer.
 *
 * @return 0
 *   La fonction a r�ussie.
 * @return -1
 *   Descripteur invalide.
 */
int net_close (int fd);

/**
 * Se connecter � une Adresse ip:port.
 *
 * @param addr Adresse ip ou l'on souhaite effectuer la connection.
 * @param port Port
 *
 * @return >= 0
 *   Un descripteur r�f�ren�ant une socket cr��e en cas de r�ussite.
 * @return -1
 *   Erreur.
 */
int net_connect (const char *addr, in_port_t port);

/**
 * R�server un port pour attendre des connections clientes.
 *
 * @param port Port que l'on souhaite r�server.
 * 
 * @return >= 0
 *   Un descripteur r�f�ren�ant une socket cr��e en cas de r�ussite.
 * @return -1
 *   Erreur.
 */
int net_bind (in_port_t port);

/**
 * Accepter des connections clientes.
 *
 * @param sfd descripteur retourn� par la focntion net_bind.
 * 
 * @return >= 0
 *   Un descripteur r�f�ren�ant une socket cr��e en cas de r�ussite.
 * @return -1
 *   Erreur.
 */
int net_accept (int sfd);

#endif
