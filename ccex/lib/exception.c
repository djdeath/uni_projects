#include <stdlib.h>
#include <stdio.h>

#include "defs_exceptions.h"

#define MAX_LIST_EXCEPTION (30)

typedef struct _Catcher_t Catcher_t;

struct _Catcher_t
{
	int has_throw : 2;
	int has_return : 1;
	void* except;
	
	void* rescue;
	void* ebp;
	void* esp;
	void* ebx;
	void* esi;
	void* edi;
	
	Catcher_t *prev;
};

static Catcher_t *catch_list = NULL, *old_catch = NULL;

void
__throw (void* except)
{
	void *rescue, *ebp, *esp, *ebx, *esi, *edi;
	
	/* Aucun bloc try/catch => on termine le programme. */
	if (!catch_list)
		exit ((int) except);
	
	catch_list->has_throw++;
	catch_list->except = except;
	
	rescue = catch_list->rescue;
	ebp = catch_list->ebp;
	esp = catch_list->esp;
	ebx = catch_list->ebx;
	esi = catch_list->esi;
	edi = catch_list->edi;
	
	printf ("throw rescue=0x%x ebp=0x%x esp=0x%x ebx=0x%x esi=0x%x edi=0x%x\n",
		rescue, ebp, esp, ebx, esi, edi);
	
	/* Restaurer %ebp et %esp, mettre la valeur de l'exception
	 * dans %eax puis éxécuter le code à l'adresse rescue. */
	__asm__ ("movl %0, %%eax\n\tmovl %1, %%esp\n\tmovl %2, %%ebp\n"
		 :
		 : "g" (rescue), "g" (esp), "g" (ebp)
		 : "%eax", "%esp", "%ebp");
	__asm__ ("movl %0, %%ebx\n\tmovl %1, %%esi\n\tmovl %2, %%edi\n"
		 :
		 : "g" (ebx), "g" (esi), "g" (edi)
		 : "%ebx", "%esi", "%edi");
	__asm__ ("jmp *%%eax" ::: "%eax");
	
}

void
__try (void *rescue, void *ebp,
       void *esp, void *ebx,
       void *esi, void *edi)
{
	Catcher_t* l;
	
	if (!(l = calloc (1, sizeof (Catcher_t))))
		__throw ((void *)OutOfMemoryException);
	
	l->prev = catch_list;
	catch_list = l;
	
	catch_list->rescue = rescue;
	catch_list->ebp = ebp;
	catch_list->esp = esp;
	catch_list->ebx = ebx;
	catch_list->esi = esi;
	catch_list->edi = edi;
	
	printf ("try rescue=0x%x ebp=0x%x esp=0x%x ebx=0x%x esi=0x%x edi=0x%x\n",
		rescue, ebp, esp, ebx, esi, edi);
}

void*
__try_end (void* rescue)
{
	printf ("try_end\n");
	
	if (!catch_list)
		return NULL;
	
	catch_list->rescue = rescue;
	
	return catch_list->except;
}

void
__return (void* val)
{
	if (!catch_list)
		return;
	printf ("return\n");
	
	catch_list->except = val;
	catch_list->has_return = 1;
}

void
__finally (void)
{
	if (!catch_list)
		return;
	
	printf ("finally\n");
	
	if (old_catch)
	{
		printf ("finally free\n");
		free (old_catch);
		old_catch = NULL;
	}
	
	old_catch = catch_list;
	catch_list = catch_list->prev;
}

int
__finally_exists (void)
{
	printf ("finally_exists 0x%x\n", old_catch);
	
	if (old_catch)
		return old_catch->has_return || old_catch->has_throw;
	
	return 0;
}

int
__finally_returns (void)
{
	printf ("finally_returns 0x%x\n", old_catch);
	
	if (old_catch)
		return old_catch->has_return;
	
	return 0;
}

void*
__finally_end (void)
{
	void* val;
	
	printf ("finally_end\n");
	
	if (old_catch)
	{
		free (old_catch);
		val = old_catch->except;
		old_catch = NULL;
	}
	else
		val = NULL;
	
	return val;
}

void
__exception_init (void)
{
	catch_list = NULL;
	old_catch = NULL;
}
