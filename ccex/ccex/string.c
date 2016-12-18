static unsigned int str;

unsigned int
string_new (void)
{
	return str++;
}

void
string_init (void)
{
	str = 0U;
}
