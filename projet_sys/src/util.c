#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>

#include "util.h"

/**/

char *
strnchr (char *str, char c, size_t len)
{
	size_t i = 0U;
	
	while ((i < len) && (*str != c))
	{
		str++;
		i++;
	}
	
	if (i >= len)
		return NULL;
	
	return str;
}

/**/

char *
strndup (const char *str, size_t len, size_t *rlen)
{
	char *ret;
	size_t slen;
	
	if (!str)
		return NULL;
	
	slen = strlen (str);
	if (rlen)
		*rlen = slen;
	
	if (slen > (len - 1))
		return NULL;
	
	if ((ret = (char *) malloc (sizeof (char) * (slen + 1))))
		strcpy (ret, str);
	
	return ret;
}

/**/

int
fdvnprintf (int fd, size_t size, const char *fmt, va_list args)
{
	int len, ret;
	char *buf;
	
	if ((fd < 0) || !fmt || !size)
		return -1;
	
	if (!(buf = malloc (sizeof (char) * size)))
		return -1;
	
	len = vsnprintf (buf, size, fmt, args);
	
	if (len < 1)
		return -1;
	
	if (len > size)
		return -1;
	      
	ret = write (fd, buf, len);
	
	free (buf);
	
	return ret;
}

int
fdnprintf (int fd, size_t size, const char *fmt, ...)
{
	int ret;
	
	va_list list;
	
	va_start (list, fmt);
	ret = fdvnprintf (fd, size, fmt, list);
	va_end (list);
	
	return ret;
}
