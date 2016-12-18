#include "main.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <locale.h>
#include <unistd.h>

#include "comp.h"

void
index_print_err (const char *fmt, ...)
{
	va_list args;
	
	va_start (args, fmt);
	
	fprintf (stderr, "** Erreur : ");
	vfprintf (stderr, fmt, args);
	fprintf (stderr, "\n");
	
	va_end (args);
}

void
index_print_info (const char *fmt, ...)
{
	va_list args;
	
	va_start (args, fmt);
	
	fprintf (stdout, "** Info : ");
	vfprintf (stdout, fmt, args);
	fprintf (stdout, "\n");
	
	va_end (args);
}

/**/

static void
print_help (const char *name)
{
	printf ("usage : %s [-cdhl] [fichier ...]\n", name);
	printf ("      -c           ecrire sur la sortie standard\n");
	printf ("      -d           decompresser\n");
	printf ("      -h           afficher cette aide\n");
}

/**/

char *
file_out_name (const char *name, int comp)
{
	char *file;
	size_t len;
	
	if (!name)
		return NULL;
	
	len = strlen (name);
	if (comp)
	{
		if ((file = malloc (sizeof (char) * (len + strlen (COMPRESSED_EXTENSION) + 1))))
		{
			strcpy (file, name);
			strcpy (file + len, COMPRESSED_EXTENSION);
		}
	}
	else
	{
		if ((file = malloc (sizeof (char) * (len + strlen (UNCOMPRESSED_EXTENSION) + 1))))
		{
			strcpy (file, name);
			strcpy (file + len, UNCOMPRESSED_EXTENSION);
		}
	}
	
	return file;
}

int
main (int argc, char *argv[])
{
	int c, i, st = 0, comp = 1, ret = EXIT_FAILURE;
	char *out;
	FILE *asc;
	BFILE *bin;
	
	/* Initialisation des locales selon les variables
	 * d'environnement LANG et LC_* . */
	if (!setlocale (LC_ALL, ""))
	{
		PRINT_ERR (("locale non supportee par la librairie C."));
		PRINT_INFO (("Utilisation de la locale C/POSIX par defaut."));
	}
	
	while ((c != EOF) && ((c = getopt (argc, argv, "cdhl")) != EOF))
	{
		switch (c)
		{
		case 'c':
			st = 1;
			break;
			
		case 'd':
			comp = 0;
			break;
			
		case 'h':
			print_help (argv[0]);
			return EXIT_SUCCESS;
			
		default:
			/* Mauvaise option. */
			print_help (argv[0]);
			return EXIT_FAILURE;
		}
	}
	
	/* Pas de noms de fichiers sur la ligne de commande, la
	 * programme va prendre les données provenant de l'entrée
	 * standard, Puisqu'il n'y a aucun nom de fichier précisé, on
	 * écrit sur la sortie standard. */
	if (optind == argc)
	{
		if (!comp)
		{
			/* Décompression */
			if (!(bin = bfimport (stdin, BFMODE_READ)))
			{
				
				return EXIT_FAILURE;
			}
			
			while (!bfeof (bin))
			{
				if ((ret = uncompress (stdout, bin)))
					return ret;
				bfflush (bin);
			}
			
			bfclose (bin);
		}
		else
		{
			/* Compression */
			if (!(bin = bfimport (stdout, BFMODE_WRITE)))
			{
				
				return EXIT_FAILURE;
			}
			
			if ((ret = compress (bin, stdin)))
				return ret;
			
			bfclose (bin);
		}
	}
	else
	{
		/* Cas ou l'utilisateur demande d'utiliser la sortie
		 * standard. */
		if (st)
		{
			for (i = optind ; i < argc ; i++)
			{
				if (!comp)
				{
					/* Décompression */
					if (!(bin = bfopen (argv[i], BFMODE_READ)))
					{
						PRINT_ERR (("Impossible d'ouvrir %s.", argv[i]));
						
						return EXIT_FAILURE;
					}
					
					while (!bfeof (bin))
					{
						if ((ret = uncompress (stdout, bin)))
						{
							PRINT_ERR (("Erreur lors du traitement de %s, code %i.", argv[i], ret));
							
							return ret;
						}
						bfflush (bin);
					}
					
					bfclose (bin);
				}
				else
				{
					/* Compression */
					if (!(asc = fopen (argv[i], "rb")))
					{
						PRINT_ERR (("Impossible d'ouvrir %s.", out));
						
						return EXIT_FAILURE;
					}
					
					if (!(bin = bfimport (stdout, BFMODE_WRITE)))
					{
						
						return EXIT_FAILURE;
					}
					
					if ((ret = compress (bin, asc)))
					{
						PRINT_ERR (("Erreur lors du traitement de %s, code %i.", argv[i], ret));
						
						return ret;
					}
					
					bfclose (bin);
				}
			}
		}
		else
		{
			/* Cas par défaut. */
			for (i = optind ; i < argc ; i++)
			{
				if ((out = file_out_name (argv[i], comp)))
				{
					if (!comp)
					{
						if (!(bin = bfopen (argv[i], BFMODE_READ)))
						{
							PRINT_ERR (("Impossible d'ouvrir %s.", argv[i]));
							free (out);
							
							return EXIT_FAILURE;
						}
						
						if (!(asc = fopen (out, "wb")))
						{
							PRINT_ERR (("Impossible d'ouvrir %s.", out));
							free (out);
							
							return EXIT_FAILURE;
						}
						
						while (!bfeof (bin))
						{
							if ((ret = uncompress (asc, bin)))
							{
								PRINT_ERR (("Erreur lors du traitement de %s, code %i.", argv[i], ret));
								
								return ret;
							}
							bfflush (bin);
						}
					}
					else
					{
						if (!(bin = bfopen (out, BFMODE_WRITE)))
						{
							PRINT_ERR (("Impossible d'ouvrir %s.", out));
							free (out);
							
							return EXIT_FAILURE;
						}
						
						if (!(asc = fopen (argv[i], "r")))
						{
							PRINT_ERR (("Impossible d'ouvrir %s.", argv[i]));
							free (out);
							
							return EXIT_FAILURE;
						}
						
						if ((ret = compress (bin, asc)))
						{
							PRINT_ERR (("Erreur lors du traitement de %s, code %i.", argv[i], ret));
							
							return ret;
						}
					}
					
					bfclose (bin);
					fclose (asc);
					
					free (out);
				}
				else
					return EXIT_FAILURE;
			}
		}
	}
	
	return EXIT_SUCCESS;
}
