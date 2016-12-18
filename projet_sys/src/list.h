#ifndef __LIST_H_
#define __LIST_H_

#include "data.h"

/* Fonctions de gestion des listes simplement et doublement
 * chaînées. */

#define LIST(arg) ((List_t *)(arg))

typedef struct _List_t List_t;

struct _List_t
{
	List_t *next;
	List_t *prev;
	
	void *data;
};

List_t *list_new (void *data);
void list_free (List_t *list);
void list_free_all (List_t *list);

int list_lenght (const List_t *list);
List_t *list_first (List_t *list);
List_t *list_last (List_t *list);
List_t *list_nth (List_t *list, int nth);
List_t *list_copy (List_t *list);

int list_find (List_t *list, const void *data, Compare_t func, List_t **l);
int list_find_d (List_t *list, const void *data, List_t **l);

void list_foreach (List_t *list, Foreach_t func, void *udata);

int list_append (List_t **list, void *data);
int list_insert_before (List_t **list, void *data);
int list_insert_after (List_t **list, void *data);
int list_insert_first (List_t **list, void *data);
int list_insert_sorted (List_t **list, Compare_t func, int way, void *data);
int list_remove (List_t **list, List_t *e);
int list_delete (List_t **list, List_t *e);

int list_append_e (List_t **list, List_t *e);
int list_insert_before_e (List_t **list, List_t *e);
int list_insert_after_e (List_t **list, List_t *e);
int list_insert_first_e (List_t **list, List_t *e);
int list_insert_sorted_e (List_t **list, Compare_t func, int way, List_t *e);

/**/

#define SLIST(arg) ((SList_t *)(arg))

typedef struct _SList_t SList_t;

struct _SList_t
{
	SList_t *next;
	
	void *data;
};

SList_t *slist_new (void *data);
void slist_free (SList_t *list);
void slist_free_all (SList_t *list);

int slist_lenght (const SList_t *list);
SList_t *slist_last (SList_t *list);
SList_t *slist_nth (SList_t *list, int nth);
SList_t *slist_copy (SList_t *list);

int slist_find (SList_t *list, const void *data, Compare_t func, SList_t **l);
int slist_find_d (SList_t *list, const void *data, SList_t **l);

void slist_foreach (SList_t *list, Foreach_t func, void *udata);

int slist_insert_first_e (SList_t **list, SList_t *e);
int slist_insert_after_e (SList_t **list, SList_t *e);
int slist_append_e (SList_t **list, SList_t *e);
int slist_insert_sorted_e (SList_t **list, Compare_t func, int way, SList_t *e);
int slist_remove (SList_t **list, SList_t *e);
int slist_delete (SList_t **list, SList_t *e);

int slist_append (SList_t **list, void *data);
int slist_insert_first (SList_t **list, void *data);
int slist_insert_after (SList_t **list, void *data);
int slist_insert_sorted (SList_t **list, Compare_t func, int way, void *data);


#endif
