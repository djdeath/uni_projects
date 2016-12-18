#include <stdlib.h>

#include "list.h"

/* ====================== DOUBLE LINKED LISTS ======================= */

List_t *
list_new (void *data)
{
	List_t *e;
	
	if ((e = LIST (calloc (1, sizeof (List_t)))))
		e->data = data;
	
	return e;
}

void
list_free (List_t *list)
{
	if (list)
		free (list);
}

void
list_free_all (List_t *list)
{
	List_t *t;
	
	while (list)
	{
		t = list->next;
		free (list);
		list = t;
	}
}

int
list_lenght (const List_t *list)
{
	int lenght = 0;
	
	while (list)
	{
		list = list->next;
		lenght++;
	}
	
	return lenght;
}

List_t *
list_first (List_t *list)
{
	if (!list)
		return list;
	
	while (list->prev)
		list = list->prev;
	
	return list;
}

List_t *
list_last (List_t *list)
{
	if (!list)
		return list;
	
	while (list->next)
		list = list->next;
	
	return list;
}

List_t *
list_nth (List_t *list, int nth)
{
	while (list && nth > 0)
	{
		list = list->next;
		nth--;
	}
	
	return list;
}

List_t *
list_copy (List_t *list)
{
	List_t *n, *f, *p;
	
	if (!list)
		return list;
	
	if ((n = list_new (list->data)))
	{
		f = n;
		list = list->next;
		p = NULL;
		
		while (list)
		{
			p = n;
			if (!(n = list_new (list->data)))
			{
				list_free_all (f);
				
				return NULL;
			}
			n->prev = p;
			p->next = n;
			
			list = list->next;
		}
		n = f;
	}
	
	return n;
}

int
list_insert_first_e (List_t **list, List_t *e)
{
	List_t *l;
	
	if (*list)
	{
		l = list_first (*list);
		e->next = l;
		l->prev = e;
	}
	*list = e;
	
	return 1;
}

int
list_insert_before_e (List_t **list, List_t *e)
{
	if (!(*list))
	{
		*list = e;
		
		return 1;
	}
	
	e->prev = (*list)->prev;
	e->next = *list;
	if (e->prev)
		e->prev->next = e;
	if (e->next)
		e->next->prev = e;
	
	*list = e;
	
	return 1;
}

int
list_insert_after_e (List_t **list, List_t *e)
{
	if (!(*list))
	{
		*list = e;
		
		return 1;
	}
	
	e->prev = *list;
	e->next = (*list)->next;
	if (e->prev)
		e->prev->next = e;
	if (e->next)
		e->next->prev = e;
	
	return 1;
}

int
list_append_e (List_t **list, List_t *e)
{
	List_t *l;
	
	if (!(*list))
	{
		*list = e;
		
		return 1;
	}
	
	l = list_last (*list);
	
	return list_insert_after_e (&l, e);
}

int
list_insert_sorted_e (List_t **list, Compare_t func, int way, List_t *e)
{
	int c;
	List_t *l, *p;
	
	if (!(*list))
	{
		*list = e;
		
		return 1;
	}
	
	p = NULL;
	l = list_first (*list);
	while (l != NULL && (c = (way * (func (e->data, l->data)))) < 0)
	{
		p = l;
		l = l->next;
	}
	
	if (c > 0)
		return list_insert_before_e (&l, e);
	
	if (l)
		return list_insert_after_e (&l, e);
	
	return list_insert_after_e (&p, e);
}

int
list_append (List_t **list, void *data)
{
	List_t *new = list_new (data);
	
	if (!new)
		return 0;
	
	return list_append_e (list, new);
}

int
list_insert_before (List_t **list, void *data)
{
	List_t *new = list_new (data);
	
	if (!new)
		return 0;
	
	return list_insert_before_e (list, new);
}

int
list_insert_first (List_t **list, void *data)
{
	List_t *new = list_new (data);
	
	if (!new)
		return 0;
	
	return list_insert_first_e (list, new);
}

int
list_insert_after (List_t **list, void *data)
{
	List_t *new = list_new (data);
	
	if (!new)
		return 0;
	
	return list_insert_after_e (list, new);
}

int
list_insert_sorted (List_t **list, Compare_t func, int way, void *data)
{
	List_t *new = list_new (data);
	
	if (!new)
		return 0;
	
	return list_insert_sorted_e (list, func, way, new);
}

int
list_remove (List_t **list, List_t *e)
{
	if (!(*list) || !e)
		return 0;
	
	if (e->next)
		e->next->prev = e->prev;
	if (e->prev)
		e->prev->next = e->next;
	if (*list == e)
	{
		if (e->prev)
			*list = e->prev;
		else
			*list = e->next;
	}
	e->next = NULL;
	e->prev = NULL;
	
	return 1;
}

int
list_delete (List_t **list, List_t *e)
{
	if (!list_remove (list, e))
		return 0;
	
	list_free (e);
	
	return 1;
}

int
list_find (List_t *list, const void *data, Compare_t func, List_t **l)
{
	int i = 0;
       	
	if (!list)
	{
		*l = NULL;
		
		return -1;
	}
	
	*l = list;
	while (*l)
	{
		if (!func ((*l)->data, data))
			return i;
		*l = (*l)->next;
		i++;
	}
	
	return -1;
}

int
list_find_d (List_t *list, const void *data, List_t **l)
{
	int i = 0;
	
	if (!list)
	{
		*l = NULL;
		
		return -1;
	}
	
	*l = list;
	while (*l)
	{
		if ((*l)->data == data)
			return i;
		*l = (*l)->next;
		i++;
	}
	
	return -1;
}

void
list_foreach (List_t *list, Foreach_t func, void *udata)
{
	while (list)
	{
		func (list->data, udata);
		list = list->next;
	}
}


/* ====================== SINGLE LINKED LISTS ======================= */


SList_t *
slist_new (void *data)
{
	SList_t *e;
	
	if ((e = SLIST (calloc (1, sizeof (SList_t)))))
		e->data = data;
       	
	return e;
}

void
slist_free (SList_t *list)
{
	if (list)
		free (list);
}

void
slist_free_all (SList_t *list)
{
	SList_t *t;
	
	while (list)
	{
		t = list->next;
		free (list);
		list = t;
	}
}

int
slist_lenght (const SList_t *list)
{
	int lenght = 0;
	
	while (list)
	{
		list = list->next;
		lenght++;
	}
	
	return lenght;
}

SList_t *
slist_last (SList_t *list)
{
	if (!list)
		return list;
	
	while (list->next)
		list = list->next;
	
	return list;
}

SList_t *
slist_nth (SList_t *list, int nth)
{
	while (list && nth > 0)
	{
		list = list->next;
		nth--;
	}
	
	return list;
}

SList_t *
slist_copy (SList_t *list)
{
	SList_t *n, *f, *p;
	
	if (!list)
		return list;
	
	if ((n = slist_new (list->data)))
	{
		f = n;
		list = list->next;
		p = NULL;
		
		while (list)
		{
			p = n;
			if (!(n = slist_new (list->data)))
			{
				slist_free_all (f);
				
				return NULL;
			}
			p->next = n;
			
			list = list->next;
		}
		n = f;
	}
	
	return n;
}

int
slist_insert_first_e (SList_t **list, SList_t *e)
{
	if (*list)
		e->next = *list;
	*list = e;
	
	return 1;
}

int
slist_insert_after_e (SList_t **list, SList_t *e)
{
	if (!(*list))
	{
		*list = e;
		
		return 1;
	}
	
	e->next = (*list)->next;
	(*list)->next = e;
	
	return 1;
}

int
slist_append_e (SList_t **list, SList_t *e)
{
	SList_t *l;
	
	if (!(*list))
	{
		*list = e;
		
		return 1;
	}
	
	l = slist_last (*list);
	
	if (l != *list)
		return slist_insert_after_e (&l, e);
	
	return slist_insert_after_e (list, e);
}

int
slist_insert_sorted_e (SList_t **list, Compare_t func, int way, SList_t *e)
{
	int c;
	SList_t *l, *p;
	
	if (!(*list))
	{
		*list = e;
		
		return 1;
	}
	
	p = NULL;
	l = *list;
	while (l != NULL && (c = (way * (func (e->data, l->data)))) < 0)
	{
		p = l;
		l = l->next;
	}
	
	if (c > 0)
		return slist_insert_first_e (list, e);
	
	if (l)
		return slist_insert_after_e (&l, e);
	
	return slist_insert_after_e (&p, e);
}

/**/

int
slist_append (SList_t **list, void *data)
{
	SList_t *new = slist_new (data);
	
	if (!new)
		return 0;
	
	return slist_append_e (list, new);
}

int
slist_insert_first (SList_t **list, void *data)
{
	SList_t *new = slist_new (data);
	
	if (!new)
		return 0;
	
	return slist_insert_first_e (list, new);
}

int
slist_insert_after (SList_t **list, void *data)
{
	SList_t *new = slist_new (data);
	
	if (!new)
		return 0;
	
	return slist_insert_after_e (list, new);
}

int
slist_insert_sorted (SList_t **list, Compare_t func, int way, void *data)
{
	SList_t *new = slist_new (data);
	
	if (!new)
		return 0;
	
	return slist_insert_sorted_e (list, func, way, new);
}

int
slist_remove (SList_t **list, SList_t *e)
{
	if (!(*list) || !e)
		return 0;
	
	if (*list == e)
		*list = e->next;
	else
	{
		SList_t *tmp = *list;
		
		while (tmp && (tmp->next != e))
			tmp = tmp->next;
		
		if (!tmp)
			return 0;
		
		tmp = e->next;
	}
	e->next = NULL;
	
	return 1;
}

int
slist_delete (SList_t **list, SList_t *e)
{
	if (!slist_remove (list, e))
		return 0;
	
	slist_free (e);
	
	return 1;
}

int
slist_find (SList_t *list, const void *data, Compare_t func, SList_t **l)
{
	int i = 0;
       	
	if (!list)
	{
		*l = NULL;
		
		return -1;
	}
	
	*l = list;
	while (*l)
	{
		if (!func ((*l)->data, data))
			return i;
		*l = (*l)->next;
		i++;
	}
	
	return -1;
}

int
slist_find_d (SList_t *list, const void *data, SList_t **l)
{
	int i = 0;
	
	if (!list)
	{
		*l = NULL;
		
		return -1;
	}
	
	*l = list;
	while (*l)
	{
		if ((*l)->data == data)
			return i;
		*l = (*l)->next;
		i++;
	}
	
	return -1;
}

void
slist_foreach (SList_t *list, Foreach_t func, void *udata)
{
	while (list)
	{
		func (list->data, udata);
		list = list->next;
	}
}
