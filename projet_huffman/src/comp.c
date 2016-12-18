#include "main.h"

#include <ctype.h>
#include <stdlib.h>

#include "comp.h"
#include "lex.h"
#include "word.h"

/* Lit un code de Huffman à partir de la racine d'un arbre. */
static HNode_t *
read_code (BFILE *file, HNode_t *head)
{
	unsigned char c;
	
	while (head->left && bfread_bit (file, &c))
	{
		if (c)
			head = head->right;
		else
			head = head->left;
	}
	
	return head;
}

/* Dans le cas d'un nouveau mot, cette fonction extrait les lettres de
 * ce mot à partir de leurs codes de Huffman. */
static int
uncompress_letters (BFILE *file, Alpha_t *al, unsigned char *word, size_t *len)
{
	unsigned char c;
	int i = 0;
	HNode_t *node;
	
	if (!(node = read_code (file, al->tree->head)))
		return ECOMP_READ;
	
	while (node != al->end)
	{
		if (node == al->tree->null)
			return ECOMP_READ;
		
		if (node == al->tree->new)
		{
			bfread_char (file, &c);
			node = alpha_add (al, c);
			word[i] = c;
		}
		else
		{
			alpha_update (al, node);
			word[i] = LETTER (node->data)->letter;
		}
		
		node = read_code (file, al->tree->head);
		i++;
	}
	
	word[i] = '\0';
	*len = i;
	
	return 0;
}

/* Lit un code de Huffman et trouve le mot qui lui correspond. */
static int
uncompress_word (BFILE *file, Lex_t *lex, unsigned char **word, size_t *len)
{
	unsigned char buf[MAX_WORD_LENGHT];
	int ret;
	HNode_t *node;
	
	if (!(node = read_code (file, lex->tree->head)))
		return ECOMP_END;
	
	if (node->left)
		return ECOMP_END;
	
	if (node == lex->tree->null)
		return ECOMP_END;
	
	if (node == lex->tree->new)
	{
		if (!(ret = uncompress_letters (file, lex->alpha, buf, len)))
		{
			if (!(node = lex_add (lex, buf, *len)))
				return ECOMP_MEM;
			*word = WORD (node->data)->word;
		}
		else
			return ret;
	}
	else
	{
		lex_update (lex, node);
		*word = WORD (node->data)->word;
		*len = WORD (node->data)->lenght;
	}
	
	return 0;
}

/* Décompression une passe. */
int
uncompress (FILE *out, BFILE *in)
{
	unsigned char *word;
	int ret;
	size_t len;
	Lex_t *lex1, *lex2, *lex_cur;
	
	if (!(lex1 = lex_new ()))
	{
		bfclose (in);
		fclose (out);
		
		return ECOMP_MEM;
	}
	if (!(lex2 = lex_new ()))
	{
		bfclose (in);
		fclose (out);
		
		return ECOMP_MEM;
	}
	lex_cur = lex1;
	
	while (!(ret = uncompress_word (in, lex_cur, &word, &len)))
	{
		if (len)
			fwrite (word, len, 1, out);
		
		if (lex_cur == lex1)
			lex_cur = lex2;
		else
			lex_cur = lex1;
	}
	
	lex_free (lex1);
	lex_free (lex2);
	
	if (ret == ECOMP_END)
		return 0;
	
	return ret;
}

/* */

/* Ecrit un code de Huffman. */
static int
write_code (BFILE *out, HNode_t *node)
{
	int i = 0;
	unsigned int plop;
	
	while (node->parent)
	{
		if (node->parent->right == node)
			SETBIT (plop, i, 1);
		else
			SETBIT (plop, i, 0);
		node = node->parent;
		i++;
	}
	
	i--;
	for (; i >= 0 ; i--)
		bfwrite_bit (out, (unsigned char) GETBIT (plop, i));
	
	return 0;
}

/* Remplace les lettres d'un mot par leur codes de Huffman. */
static int
compress_letters (BFILE *out, Alpha_t *al, const unsigned char *word, size_t len)
{
	size_t i = 0;
	HNode_t *node;
	
	while (i < len)
	{
		if (!(node = alpha_search (al, word[i])))
		{
			write_code (out, al->tree->new);
			
			if (!bfwrite_char (out, word[i]))
				return ECOMP_WRITE;
			
			if (!alpha_add (al, word[i]))
				return ECOMP_MEM;
		}
		else
		{
			write_code (out, node);
			alpha_update (al, node);
		}
		i++;
	}
	write_code (out, al->end);
	
	return 0;
}

/* Remplace un mot par son code de Huffman. */
static int
compress_word (BFILE *out, Lex_t *lex, const char *word, size_t len)
{
	int ret;
	HNode_t *node;
	
	if (word)
	{
		if (!(node = lex_search (lex, word, len)))
		{
			write_code (out, lex->tree->new);
			
			if ((ret = compress_letters (out, lex->alpha, word, len)))
				return ret;
			
			if (!(node = lex_add (lex, word, len)))
				return ECOMP_MEM;
		}
		else
		{
			write_code (out, node);
			lex_update (lex, node);
		}
	}
	else
		write_code (out, lex->tree->null);
	
	return 0;
}

/* Compression 1 passe : on fabrique l'arbre en lisant le
 * texte. L'arbre contient une feuille qui sert à indiquer la
 * présence d'un nouveau mot. */
int
compress (BFILE *out, FILE *in)
{
	int c, ret;
	size_t i;
	unsigned char buf[MAX_WORD_LENGHT];
	Lex_t *lex_word, *lex_seps, *lex_cur;
	
	if (!(lex_word = lex_new ()))
	{
		fclose (in);
		bfclose (out);
		
		return ECOMP_MEM;
	}
	if (!(lex_seps = lex_new ()))
	{
		fclose (in);
		bfclose (out);
		
		return ECOMP_MEM;
	}
	lex_cur = lex_word;
	
	while (!feof (in))
	{
		i = 0;
		
		if (lex_cur == lex_seps)
		{
			lex_cur = lex_word;
			
			while ((c = getc (in)) != EOF &&
			       (i < (MAX_WORD_LENGHT - 1) &&
				isalpha (c)))
				buf[i++] = c;
		}
		else
		{
			lex_cur = lex_seps;
			
			while ((c = getc (in)) != EOF &&
			       (i < (MAX_WORD_LENGHT - 1) &&
				!isalpha (c)))
				buf[i++] = c;
		}
		if (!feof (in))
			ungetc (c, in);
		buf[i] = '\0';
		
		if ((ret = compress_word (out, lex_cur, buf, i)))
			return ret;
	}
	
	/* Ecriture de la feuille null de l'arbre de Huffman qui nous
	 * permet de savoir si on a atteint la fin d'un fichier
	 * compressé. */
	if (lex_cur == lex_seps)
		compress_word (out, lex_word, NULL, 0U);
	else
		compress_word (out, lex_seps, NULL, 0U);
	
	lex_free (lex_word);
	lex_free (lex_seps);
	
	return EXIT_SUCCESS;
}
