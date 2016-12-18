#include "main.h"

#include <string.h>

#include "palloc.h"

#define PAGE(arg) ((Page_t *)(arg))
#define MEMUNIT(arg) ((MemUnit_t *)(arg))

#define PAGES_NUM (100)

#define PAGE_ELS_NUM (5000) /* MAX is USHORT_MAX */

/*#define MEMUNIT_NUM(page, el) ((MemUnit_t *)(((void *) ((page) + 1)) + el * (sizeof (MemUnit_t) + (page)->size)))*/

typedef struct _Page_t Page_t;
typedef struct _MemUnit_t MemUnit_t;

struct _MemUnit_t
{
	Page_t *page;
	
	MemUnit_t *prev;
	MemUnit_t *next;
};

struct _Page_t
{
	size_t size;
	
	Page_t *prev;
	Page_t *next;
	
	MemUnit_t *used;
	MemUnit_t *free;
	
	unsigned int nb_garbage;
	void *garbage;
};

/* Un tableau de listes contenant des pages mémoire */
static Page_t *palloc_pages = NULL;

static Page_t *
page_add (size_t size)
{
	Page_t *page = PAGE (malloc (sizeof (Page_t) + PAGE_ELS_NUM * (sizeof (MemUnit_t) + size)));
	
	if (page)
	{
		void *tmp = page + 1;
		
		/* clean headers */
		memset (page, 0, sizeof (Page_t));
		page->size = size;
		
		page->free = tmp;
		page->free->page = page;
		page->free->prev = page->free->next = NULL;
		
		page->nb_garbage = PAGE_ELS_NUM - 1;
		page->garbage = tmp + sizeof (MemUnit_t) + size;
		
		page->next = palloc_pages;
		if (page->next)
			page->next->prev = page;
		
		/* update list */
		palloc_pages = page;
	}
	
	return page;
}

static void
page_delete (Page_t *page)
{
	if (page)
	{
		if (palloc_pages == page)
			palloc_pages = page->next;
		if (page->prev)
			page->prev->next = page->next;
		if (page->next)
			page->next->prev = page->prev;
		
		free (page);
	}
}

static Page_t *
page_find (size_t size)
{
	Page_t *list = palloc_pages;
	
	while (list)
	{
		if ((list->size == size) && list->free)
			return list;
		
		list = list->next;
	}
	
	return list;
}

void *
pmalloc (size_t size)
{
	Page_t *f;
	MemUnit_t *unit;
	
	if (!palloc_pages)
	{
		if (!(f = page_add (size)))
			return NULL;
	}
	else
	{
		if (!(f = page_find (size)))
			if (!(f = page_add (size)))
				return NULL;
	}
	
	unit = f->free;
	if ((f->free = unit->next))
		f->free->prev = NULL;
	else if (f->nb_garbage)
	{
		f->free = f->garbage;
		f->free->page = f;
		f->free->prev = f->free->next = NULL;
		f->garbage += sizeof (MemUnit_t) + f->size;
		f->nb_garbage--;
	}
		
	unit->next = f->used;
	if (f->used)
		f->used->prev = unit;
	f->used = unit;
	
	return (unit + 1);
}

void *
pcalloc (size_t size)
{
	void *ptr;
	
	if ((ptr = pmalloc (size)))
		memset (ptr, 0, size);
	
	return ptr;
}

void
pfree (void *ptr)
{
	Page_t *page;
	MemUnit_t *unit;
	
	unit = ptr - sizeof (MemUnit_t);
	page = unit->page;
	
	if (unit == page->used)
		page->used = unit->next;
	
	if (unit->prev)
		unit->prev->next = unit->next;
	if (unit->next)
		unit->next->prev = unit->prev;
	
	unit->prev = NULL;
	if ((unit->next = page->free))
		unit->next->prev = unit;
	page->free = unit;
	
	if (!page->used)
		page_delete (page);
}
