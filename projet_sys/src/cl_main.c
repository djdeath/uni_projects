#include <stdlib.h>
#include <unistd.h>

#include "cl_ui_main.h"

int
main (int argc, char *argv[])
{
	WindowMain_t *p;
	
	gtk_init (&argc, &argv);
	
	if ((p = window_main_create ()))
		gtk_main ();
	
	return EXIT_SUCCESS;
}
