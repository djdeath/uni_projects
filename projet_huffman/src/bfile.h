#ifndef __BFILE_H__
#define __BFILE_H__

#include <stdio.h>

#define GETBIT(x, i) (((x) >> (i)) & 1)
#define SETBIT(x, i, v) ((x) = ((v) == 0 ? ((x) & (~(1 << (i)))) : ((x) | (1 << (i)))))

/* Mode d'ouvreture d'un fichier (le mode lecture et �criture n'est
 * pas implemente). */
typedef enum
{
	BFMODE_READ,
	BFMODE_WRITE
} BFMode;

/* L'ecriture bit � bit dans un fichier pose probleme car il est
 * impossible d'ecrire moins d'un octet a la fois. Notre structure de
 * donnee attend donc de remplir cet octet avant de l'ecrire. */
typedef struct _BFILE
{
	FILE *p;
	BFMode mode;
	
	unsigned char data;
	int pos;
	int imported;
} BFILE;

/* Permet d'utiliser un flux qqlconque avec les autres fonctions de ce
 * fichier (ATTENTION : l'utilisation de bfclose sur la structure
 * renvoy�e par cette fonction ne ferme pas le flux pass� en
 * argument). */
BFILE *bfimport (FILE *file, BFMode mode);

/* Ces fonctions ont le meme role que fopen et fclose. */
BFILE *bfopen (const char *file, BFMode mode);
void bfclose (BFILE *f);

/* Vide le `pseudo� buffer qui nous sert � �crire bit � bit. */
int bfflush (BFILE *f);

/* EOF ? */
int bfeof (BFILE *f);

/* Toutes les fonctions qui suivent renvoient le nombre de bits lu ou
 * ecrit si il n'y a eut aucun probleme, et 0 dans le cas
 * contraire. */

/* Ecrit un bit dans un fichier, le parametre bit doit �tre 0 pour
 * mettre le bit � 0 ou tout le reste pour mettre le bit � 1. La
 * valeur retourn�e est 1, 0 respectivement en fonction de la
 * r�ussite ou de l'echec de l'op�ration sur le fichier. */
int bfwrite_bit (BFILE *f, unsigned char bit);

/* Lit un bit dans un fichier et place le resultat dans la variable
 * pointee par bit. La valeur renvoyee est la meme que pour la
 * fonction bfwrite_bit. */
int bfread_bit (BFILE *f, unsigned char *bit);

/* Ecrire/lire un caractere. */
int bfwrite_char (BFILE *f, unsigned char c);
int bfread_char (BFILE *f, unsigned char *c);

/* Equivalent � fputs/fgets. */
int bfputs (const char *str, BFILE *f);
int bfgets (char *str, int size, BFILE *f);

/* Semblable � fprintf, � utiliser seulement en d�but de
 * fichier. Une fois que l'on a commenc� � �crire des
 * donn�es brutes, l'utilisation de cette fonction peut corrompre
 * les donn�es d�j� �critent. */
int bfprintf (BFILE *f, const char *fmt, ...);

int bfwrite_int (BFILE *f, unsigned int val);
int bfread_int (BFILE *f, unsigned int *val);

#endif
