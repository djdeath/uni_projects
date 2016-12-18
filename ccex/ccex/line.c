static int line;

void
line_init (void)
{
	line = 1;
}

void
inc_line (void)
{
	line++;
}

int
get_line (void)
{
	return line;
}
