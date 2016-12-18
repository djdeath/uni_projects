#ifndef __MAIN_H__
#define __MAIN_H__

#include "config.h"

/* N�cessaire pour utiliser la fonction getopt */
#define _GNU_SOURCE

/* D�finition g�n�rales */

#define HASH_TABLE_OPEN_DEFAULT_SIZE (50023)
#define HASH_TABLE_CLOSE_DEFAULT_SIZE (5023)

/* Longueur maximum d'un mot */

#define MAX_WORD_LENGHT (64) /* Modifier cette longueur maximale peut
			      * changer le taux de compression. */

#define COMPRESSED_EXTENSION ".H"
#define UNCOMPRESSED_EXTENSION ".D"

/* Codes d'erreur relatifs aux tables de hachage. */

#define EHASH_TABLE 125 /* Table invalide */
#define EHASH_KEY   126 /* Cl�e invalide */
#define EHASH_DATA  127 /* Donn�e invalide */
/* Erreurs relatives aux tables ferm�es */
#define EHASH_FULL  128 /* La table de hachage est pleine. */
/* Erreurs relatives aux tables ouvertes */
#define EHASH_OMEM  129 /* Impossible d'allouer la place n�cessaire
			 * pour ajouter un nouvel �l�ment. */

/* Codes d'erreur relatifs au compresseur/d�compresseur */
#define ECOMP_READ  130 /* Impossible de lire un code de huffman
			 * correct (provoqu� par la lecture du
			 * caract�re EOF sur le flux d'entr�e ou un
			 * fichier de type incorrect). */
#define ECOMP_LEN   131 /* La longueur d'un mot n'est pas lisible (fin
			 * de fichier pr�matur�e) ou la valeur lue est
			 * incorrecte. */
#define ECOMP_END   132 /* Signale la fin de la partie compress�e du
			 * fichier (pas vraiment une erreur). */
#define ECOMP_MEM   133 /* Impossible d'allouer l'espace n�cessaire au
			 * stockage des donn�es. */
#define ECOMP_WRITE 134 /* Erreur lors de l'�criture du fichier de
			 * sortie (la variable errno devrait pr�ciser
			 * l'erreur). */


#define PRINT_ERR(args) index_print_err args
#define PRINT_INFO(args) index_print_info args

void index_print_err (const char *fmt, ...);
void index_print_info (const char *fmt, ...);

#endif
