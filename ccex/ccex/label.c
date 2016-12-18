static unsigned int label;

unsigned int
label_new (void)
{
	return label++;
}

void
label_init (void)
{
	label = 0U;
}
