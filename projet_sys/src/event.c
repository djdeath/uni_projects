#include <stdlib.h>
#include <string.h>

#include <sys/time.h>
#include <sys/types.h>

#include "event.h"

#ifdef HAVE_WINSOCK_H
# include <winsock.h>
#endif

/**/

#define WAIT_POLL_TIME (10)

#define INPUT_BLOCK (5000)
#define TIMER_BLOCK (50)

#define MAX(a,b) ((a) > (b) ? (a) : (b))

#define TIMER(arg) ((Timer_t *)(arg))
#define INPUT(arg) ((Input_t *)(arg))

#define TIME_SET(t,m) {(t)->tv_sec = (m) / 1000; (t)->tv_usec = ((m) % 1000) * 1000; }
#define TIME_ADD(t1,t2) {(t1)->tv_sec += (t2)->tv_sec; (t1)->tv_usec += (t2)->tv_usec;}
#define TIME_CPY(t1,t2) {(t1)->tv_sec = (t2)->tv_sec; (t1)->tv_usec = (t2)->tv_usec;}
#define TIME_CMP_GT(t1,t2) (((t1)->tv_sec == (t2)->tv_sec) ? ((t1)->tv_usec > (t2)->tv_usec) : ((t1)->tv_sec > (t2)->tv_sec))
#define TIME_CMP_LT(t1,t2) (((t1)->tv_sec == (t2)->tv_sec) ? ((t1)->tv_usec < (t2)->tv_usec) : ((t1)->tv_sec < (t2)->tv_sec))

/**/

#if ! HAVE_SYS_POLL_H
struct pollfd
{
	int fd;
	short events;
	short revents;
};
#endif

typedef struct _Timer_t Timer_t;
typedef struct _Input_t Input_t;

struct _Timer_t
{
	int used;
	struct timeval expiration;
	struct timeval timeout;

	CallBack_t handler;
	void *data;
};

struct _Input_t
{
	int used;
	int fd;
	short mask;

	CallBack_t handler;
	void *data;
};

/**/

static int polling;

static TimerID_t timer_num, timer_max;
static TimerID_t timer_min_free, timer_max_used;
static Timer_t *timer_events;

static struct pollfd polls[INPUT_BLOCK];
static InputID_t input_num, input_max;
static InputID_t input_min_free, input_max_used;
static Input_t *input_events;

/**/

static void
timer_init (void)
{
	timer_num = 0U;
	timer_max = 0U;
	timer_min_free = 0U;
	timer_events = NULL;
}

static void
timer_finish (void)
{
	if (timer_events)
		free (timer_events);
}

static int
timer_check (TimerID_t id)
{
	if (timer_num && (id <= timer_max_used))
	{
		Timer_t *ev = &timer_events[id];

		if (ev->used)
		{
			struct timeval cur;

			if (gettimeofday (&cur, NULL) != -1)
			{
				if (TIME_CMP_GT (&cur, &ev->expiration))
				{
					TIME_ADD (&cur, &ev->timeout);
					TIME_CPY (&ev->expiration, &cur);

					ev->handler (ev->data);
				}
			}
		}

		return 1;
	}

	return 0;
}

static TimerID_t
timer_add_list (Timer_t **ev)
{
	TimerID_t i, registring;

	if (!ev)
		return 0;

	if (!timer_events)
	{
		if (!(timer_events = malloc (TIMER_BLOCK * sizeof (Timer_t))))
			return 0;
		timer_max = TIMER_BLOCK;
		memset (timer_events, 0, timer_max * sizeof (Timer_t));
	}
	else if ((timer_num - 1) >= timer_max)
	{
		Timer_t *new;

		if (!(new = TIMER (realloc (timer_events, 2 * timer_max * sizeof (Timer_t)))))
			return 0;

		memset (&new[timer_max], 0, timer_max * sizeof (Timer_t));
		timer_events = new;
		timer_max *= 2;
	}

	/* On utilise le 1er libre */
	i = registring = timer_min_free;
	*ev = &timer_events[registring];
	timer_events[registring].used = 1;

	/* On cherche un nouveau libre */
	while (timer_events[i].used)
		i++;
	timer_min_free = i;

	if (registring > timer_max_used)
		timer_max_used = registring;
	timer_num++;

	return registring + 1;
}

TimerID_t
timer_add (time_t timeout, CallBack_t handler, void *data)
{
	TimerID_t ret;
	Timer_t *timer;

	if ((timeout < WAIT_POLL_TIME) || !handler)
		return 0;

	if ((ret = timer_add_list (&timer)) > 0)
	{
		timer->handler = handler;
		timer->data = data;

		TIME_SET (&timer->timeout, timeout);
		gettimeofday (&timer->expiration, NULL);
		TIME_ADD (&timer->expiration, &timer->timeout);

		return ret;
	}

	return ret;
}

void
timer_remove (TimerID_t id)
{
	if (id == 0U)
		return;

	id--;
	if (id <= timer_max_used)
	{
		if (id < timer_min_free)
			timer_min_free = id;
		if ((id == timer_max_used) && (timer_max_used > 0))
			timer_max_used--;

		timer_events[id].used = 0;
		timer_num--;
	}
}

/**/

static void
input_init (void)
{
	input_num = 0U;
	input_max = 0U;
	input_min_free = 0U;
	input_max_used = 0U;
	input_events = NULL;

	memset (polls, 0, sizeof (polls)/*struct pollfd) * INPUT_BLOCK*/);
}

static void
input_finish (void)
{
	if (input_events)
		free (input_events);
}

static int
input_check (InputID_t id)
{
	Input_t *ev;

	if (input_num && (id <= input_max_used))
	{
		ev = &input_events[id];

		if (ev->used)
		{
			if (polls[id].revents & polls[id].events)
			{
				polls[id].revents = 0;
				ev->handler (ev->data);
			}
		}

		return 1;
	}

	return 0;
}

static InputID_t
input_add_list (Input_t **ev)
{
	InputID_t i, registring;

	if (!ev)
		return 0;

	if (!input_events)
	{
		if (!(input_events = calloc (TIMER_BLOCK, sizeof (Input_t))))
			return 0;
		input_max = INPUT_BLOCK;
	}
	else if (input_num >= input_max)
		return 0;

	/* On utilise le 1er libre */
	i = registring = input_min_free;
	*ev = &input_events[registring];
	input_events[registring].used = 1;

	/* On cherche un nouveau libre */
	while ((i < input_max) && input_events[i].used)
		i++;
	input_min_free = i;

	if (registring > input_max_used)
		input_max_used = registring;
	input_num++;

	return registring + 1;
}

InputID_t
input_add (int fd, short mask, CallBack_t handler, void *data)
{
	InputID_t ret;
	Input_t *input;

	if (!handler)
		return 0;

	if ((ret = input_add_list (&input)) > 0)
	{
		input->handler = handler;
		input->data = data;

		input->fd = fd;
		input->mask = mask;

		polls[ret - 1].fd = fd;
		polls[ret - 1].events = mask;

		return ret;
	}

	return ret;
}

void
input_remove (InputID_t id)
{
	if (id == 0U)
		return;

	id--;
	if (id <= input_max_used)
	{
		if (id < input_min_free)
			input_min_free = id;
		if (id == input_max_used)
			input_max_used--;

		input_events[id].used = 0;
		polls[id].fd = 0;
		polls[id].events = 0;
		input_num--;
	}
}

/**/

void
event_init (void)
{
	timer_init ();
	input_init ();
}

void
event_finish (void)
{
	timer_finish ();
	input_finish ();
}

void
event_loop_quit (void)
{
	polling = 0;
}

static int
poll_events (void)
{
#ifdef HAVE_POLL
	return poll (polls, MAX (input_num, input_max_used), WAIT_POLL_TIME);
#else
	int i, max_fd;
	struct timeval tm;
	fd_set rd, er, wr;

	tm.tv_sec = 0U;
	tm.tv_usec = WAIT_POLL_TIME * 1000;

	FD_ZERO (&rd);
	FD_ZERO (&er);
	FD_ZERO (&wr);

	max_fd = 0;
	for (i = 0 ; i < input_max_used ; i++)
	{
		if (input_events[i].used)
		{
			max_fd = MAX (polls[i].fd, max_fd);
			if (polls[i].events & INPUT_DAT)
				FD_SET (polls[i].fd, &rd);
			if (polls[i].events & (INPUT_HUP | INPUT_ERR))
				FD_SET (polls[i].fd, &er);
		}
	}

	if (select (max_fd + 1, &rd, &wr, &er, &tm) != -1)
	{
		for (i = 0 ; i < input_max_used ; i++)
		{
			if (input_events[i].used)
			{
				if (FD_ISSET (polls[i].fd, &rd))
					polls[i].revents = INPUT_DAT;
				if (FD_ISSET (polls[i].fd, &er))
					polls[i].revents = INPUT_DAT;
			}
		}

		return 1;
	}

	return -1;
#endif
}

void
event_loop (void)
{
	int r, p;
	unsigned int id;

	polling = 1;

	while (polling)
	{
		id = 0U;
		if (!input_num && !timer_num)
			break;

		if ((p = poll_events ()) < 0)
			break;

		do
		{
			r = 0;
			if (p)
				r |= input_check (id);
			r |= timer_check (id);

			id++;
		} while (r);
	}
}
