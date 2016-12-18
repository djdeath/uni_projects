#include <string.h>

#include "identifier.h"

static char id[1024];

void
set_identifier (const char *str)
{
	strncpy (id, str, 1024);
}


const char *
get_identifier (void)
{
	return id;
}
