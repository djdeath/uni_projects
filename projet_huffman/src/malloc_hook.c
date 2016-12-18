/* Prototypes for __malloc_hook, __free_hook */
#include <sys/time.h>
#include <malloc.h>
#include <pthread.h>
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>

/* ***TEMPORARY*** */
#define LIBC_HAS_FUNCTIONNAL_BACKTRACE

#define STACK_DUMP_SIZE (5)
#define DUMP_BUFFER_SIZE (1024)

#define OUTPUT_FILE_NAME "MALLOC_HOOK_FILE"

/* Internal variables. */
static FILE *dump_stream;
static char dump_buffer[DUMP_BUFFER_SIZE];
static unsigned long first_time;
static pthread_mutex_t my_hook_mutex = PTHREAD_MUTEX_INITIALIZER;

/* Variables to store libc's hooks. */
static void *(*old_malloc_hook) (size_t, const void *);
static void *(*old_realloc_hook) (void*, size_t, const void *);
static void (*old_free_hook) (void*, const void *);

/* Prototypes for our hooks.  */
static void my_init_hook (void);
static void *my_malloc_hook (size_t, const void *);
static void *my_realloc_hook (void*, size_t, const void *);
static void my_free_hook (void*, const void *);

/* Override initializing hook from the C library. */
void (*__malloc_initialize_hook) (void) = my_init_hook;

/*
 * Get time in personnal format.
 */
static unsigned long
my_gettimeofday (void)
{
        unsigned long mytime;
        struct timeval time_sts;

        if (gettimeofday (&time_sts, NULL) == 0)
        {
                mytime = time_sts.tv_sec % 600000;
                mytime *= 1000;
                mytime += time_sts.tv_usec / 1000;
        }
        else
                mytime = 0;

        return mytime;
}


/*
 * Dump stack to file.
 */
static void
dump_stack (const char *cmd, const void *ptr, size_t size)
{
        int i, nb;
        unsigned long mytime;
        void *stack_dump[STACK_DUMP_SIZE];

#ifdef LIBC_HAS_FUNCTIONNAL_BACKTRACE
        if ((nb = backtrace (stack_dump, STACK_DUMP_SIZE)) > 0)
        {
                mytime = my_gettimeofday ();
                mytime -= first_time;

                fprintf (dump_stream, "%lu:%s:0x%x:%u", mytime, cmd, ptr, size);
                for (i = 0 ; i < nb ; i++)
                        fprintf (dump_stream, ":0x%x", stack_dump[i]);
                fprintf (dump_stream, "\n");
        }
#else

#endif
}


/*
 * Internal of malloc/free/realloc hooks.
 */
static void
my_hook_lock (void)
{
        pthread_mutex_lock (&my_hook_mutex);
}

static void
my_hook_unlock (void)
{
        pthread_mutex_unlock (&my_hook_mutex);
}

static void
switch_to_my_hook (void)
{
        /* Save current libc hooks */
        old_malloc_hook = __malloc_hook;
        old_realloc_hook = __realloc_hook;
        old_free_hook = __free_hook;

        __malloc_hook = my_malloc_hook;
        __realloc_hook = my_realloc_hook;
        __free_hook = my_free_hook;
}

static void
switch_to_old_hook (void)
{
        __malloc_hook = old_malloc_hook;
        __realloc_hook = old_realloc_hook;
        __free_hook = old_free_hook;
}

/*
 * Hooks of malloc/free/realloc and init.
 */
static void
my_exit_hook (void)
{
        if (dump_stream != NULL)
                fclose (dump_stream);
}

static void
my_init_hook (void)
{
        char *output_file;

        if ((output_file = getenv (OUTPUT_FILE_NAME)) == NULL)
                return;

        if ((dump_stream = fopen (output_file, "w+")) == NULL)
                return;

        setbuffer (dump_stream, dump_buffer, DUMP_BUFFER_SIZE);

        atexit (my_exit_hook);

        first_time = my_gettimeofday ();

        switch_to_my_hook ();
}

static void *
my_malloc_hook (size_t size, const void *caller)
{
        void *result;

        my_hook_lock ();

        /* Restore all old hooks */
        switch_to_old_hook ();

        /* Call recursively */
        if (old_malloc_hook != NULL)
                result = old_malloc_hook (size, caller);
        else
                result = malloc (size);

        dump_stack ("ALLOC", result, size);

        /* Restore our own hooks */
        switch_to_my_hook ();

        my_hook_unlock ();

        return result;
}

static void *
my_realloc_hook (void *ptr, size_t size, const void *caller)
{
        void *result;

        my_hook_lock ();

        /* Restore all old hooks */
        switch_to_old_hook ();

        dump_stack ("FREE", ptr, 0);

        /* Call recursively */
        if (old_realloc_hook != NULL)
                result = old_realloc_hook (ptr, size, caller);
        else
                result = realloc (ptr, size);

        dump_stack ("ALLOC", result, size);

        /* Restore our own hooks */
        switch_to_my_hook ();

        my_hook_unlock ();

        return result;
}


static void
my_free_hook (void *ptr, const void *caller)
{
        my_hook_lock ();

        /* Restore all old hooks */
        switch_to_old_hook ();

        dump_stack ("FREE", ptr, 0);

        /* Call recursively */
        if (old_free_hook != NULL)
                old_free_hook (ptr, caller);
        else
                free (ptr);

        /* Restore our own hooks */
        switch_to_my_hook ();

        my_hook_unlock ();
}

