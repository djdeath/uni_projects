#include "parse.h"

static int writing;

void
parse_set_writing (int val)
{
	writing = val;
}

int
parse_writing (void)
{
	return writing;
}
