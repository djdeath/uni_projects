#ifndef __MAIN_H__
#define __MAIN_H__

#include "config.h"

/* Nécessaire pour utiliser la fonction getopt */
#define _GNU_SOURCE

/* Définition générales */

#define HASH_TABLE_OPEN_DEFAULT_SIZE (50023)
#define HASH_TABLE_CLOSE_DEFAULT_SIZE (5023)

/* Longueur maximum d'un mot */

#define MAX_WORD_LENGHT (64) /* Modifier cette longueur maximale peut
			      * changer le taux de compression. */

#define COMPRESSED_EXTENSION ".H"
#define UNCOMPRESSED_EXTENSION ".D"

/* Codes d'erreur relatifs aux tables de hachage. */

#define EHASH_TABLE 125 /* Table invalide */
#define EHASH_KEY   126 /* Clée invalide */
#define EHASH_DATA  127 /* Donnée invalide */
/* Erreurs relatives aux tables fermées */
#define EHASH_FULL  128 /* La table de hachage est pleine. */
/* Erreurs relatives aux tables ouvertes */
#define EHASH_OMEM  129 /* Impossible d'allouer la place nécessaire
			 * pour ajouter un nouvel élément. */

/* Codes d'erreur relatifs au compresseur/décompresseur */
#define ECOMP_READ  130 /* Impossible de lire un code de huffman
			 * correct (provoqué par la lecture du
			 * caractère EOF sur le flux d'entrée ou un
			 * fichier de type incorrect). */
#define ECOMP_LEN   131 /* La longueur d'un mot n'est pas lisible (fin
			 * de fichier prématurée) ou la valeur lue est
			 * incorrecte. */
#define ECOMP_END   132 /* Signale la fin de la partie compressée du
			 * fichier (pas vraiment une erreur). */
#define ECOMP_MEM   133 /* Impossible d'allouer l'espace nécessaire au
			 * stockage des données. */
#define ECOMP_WRITE 134 /* Erreur lors de l'écriture du fichier de
			 * sortie (la variable errno devrait préciser
			 * l'erreur). */


#define PRINT_ERR(args) index_print_err args
#define PRINT_INFO(args) index_print_info args

void index_print_err (const char *fmt, ...);
void index_print_info (const char *fmt, ...);

#endif
