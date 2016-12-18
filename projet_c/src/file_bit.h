#ifndef __FILE_BIT_H_
#define __FILE_BIT_H_

#include <stdio.h>

/* Mode d'ouvreture d'un fichier (le mode lecture et �criture n'est
   pas implemente). */
typedef enum
{
	BFMODE_READ,
	BFMODE_WRITE
} BFMode;

/* L'ecriture bit � bit dans un fichier pose probleme car il est
   impossible d'ecrire moins d'un octet a la fois. Notre structure de
   donnee attend donc de remplir cet octet avant de l'ecrire. */
typedef struct _BFILE
{
	FILE *p;
	BFMode mode;
	
	unsigned char data;
	int pos;
} BFILE;

/* Ces fonctions ont le meme role que fopen et fclose. */
BFILE *bfopen (const char *file, BFMode mode);
void bfclose (BFILE *f);

/* EOF ? */
int bfeof (BFILE *f);

/* Toutes les fonctions qui suivent renvoient le nombre de bits lu ou
   ecrit si il n'y a eut aucun probleme, et 0 dans le cas
   contraire. */

/* Ecrit un bit dans un fichier, le parametre bit doit �tre 0 pour
 * mettre le bit � 0 ou tout le reste pour mettre le bit � 1. La
 * valeur retourn�e est 1, 0 respectivement en fonction de la r�ussite
 * ou de l'echec de l'op�ration sur le fichier. */
int bfwrite_bit (BFILE *f, unsigned char bit);

/* Lit un bit dans un fichier et place le resultat dans la variable
 * pointee par bit. La valeur renvoyee est la meme que pour la
 * fonction bfwrite_bit. */
int bfread_bit (BFILE *f, unsigned char *bit);

/* Ecrire/lire un caractere. */
int bfwrite_char (BFILE *f, unsigned char c);
int bfread_char (BFILE *f, unsigned char *c);

/* Semblable � fprintf, � utiliser seulement en d�but de
 * fichier. Une fois que l'on a commenc� � �crire des
 * donn�es brutes, l'utilisation de cette fonction peut corrompre
 * les donn�es d�j� �critent. */
int bfprintf (BFILE *f, const char *fmt, ...);
int bfgets (char *buf, int size, BFILE *f);

#endif
