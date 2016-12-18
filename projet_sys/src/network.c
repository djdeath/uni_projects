#include "network.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#ifdef HAVE_FCNTL_H
# include <fcntl.h>
#endif /* HAVE_FCNTL_H */

#ifdef HAVE_NETDB_H
# include <netdb.h>
#endif /* HAVE_NETDB_H */
#ifdef HAVE_SYS_SOCKET_H
# include <sys/socket.h>
#endif /* HAVE_SYS_SOCKET_H */
#ifdef HAVE_IO_H
# include <io.h>
#endif /* HAVE_IO_H */
#ifdef HAVE_WINSOCK_H
# include <winsock.h>
#endif /* HAVE_WINSOCK_H */
#ifdef HAVE_WINSOCK2_H
# include <winsock2.h>
#endif /* HAVE_WINSOCK2_H */

#ifndef SHUT_RDWR
# define SHUT_RDWR 2
#endif /* SHUT_RDWR */

int
net_block (int fd, int val)
{
	int attr;
	
	attr = fcntl (fd, F_GETFL);
	if (val)
		attr &= ~O_NONBLOCK;
	else
		attr |= O_NONBLOCK;			
	
	if (fcntl (fd, F_SETFL, attr) < 0)
	{
		perror ("fcntl");
		return -1;
	}
	
	return 0;
}

int
net_close (int fd)
{
	if (fd < 0)
		return -1;
	
	shutdown (fd, SHUT_RDWR);
	close (fd);
	
	return 0;
}

int
net_connect (const char *addr, in_port_t port)
{
	int fd;
	struct sockaddr_in sv_addr;
	struct hostent *host;
	
	if (!addr)
		return -1;
	
	/* Résolution de l'adresse ou du nom de domaine. */
	if (!(host = gethostbyname (addr)))
		return -1;
	
	memset (&sv_addr, 0, sizeof (struct sockaddr_in));
	
	sv_addr.sin_family = AF_INET;
	sv_addr.sin_addr = *((struct in_addr *) host->h_addr);
	sv_addr.sin_port = htons (port);
	
	if ((fd = socket (AF_INET, SOCK_STREAM, IPPROTO_IP)) >= 0)
	{
		if (connect (fd, (struct sockaddr *) &sv_addr, sizeof (struct sockaddr)) < 0)
		{
			perror ("connect");
			net_close (fd);
			return -1;
		}
	}
	else
		perror ("socket");
	
	net_block (fd, 0);
	
	return fd;
}

int
net_bind (in_port_t port)
{
	int fd;
	ssize_t len;
	struct sockaddr_in sv_addr;
	
	if ((fd = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
		perror ("socket");
		return fd;
	}
	
	len = sizeof (struct sockaddr_in);
	
	memset (&sv_addr, 0, sizeof (struct sockaddr_in));
	
	sv_addr.sin_family = AF_INET;
	sv_addr.sin_port = htons (port);
	sv_addr.sin_addr.s_addr = htonl (INADDR_ANY);
	
	setsockopt (fd, SOL_SOCKET, SO_REUSEADDR, (void *) &len, sizeof (len));
	
	if (bind (fd, (struct sockaddr *) &sv_addr, sizeof (struct sockaddr)) < 0)
	{
		perror ("bind");
		net_close (fd);
		return -1;
	}
	
	listen (fd, 5);
	net_block (fd, 0);
	
	return fd;
}

int
net_accept (int sfd)
{
	int fd, len;
	struct sockaddr_in cl_addr;
	
	len = sizeof (cl_addr);
	
	fd = accept (sfd, (struct sockaddr *) &cl_addr, &len);
	net_block (fd, 0);
	
	return fd;
}
