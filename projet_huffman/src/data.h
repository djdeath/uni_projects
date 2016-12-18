#ifndef __DATA_H__
#define __DATA_H__

#define ASCENDING 1
#define DESCENDING -1

#define FOREACH_CB(arg) ((ForeachCB_t)(arg))

#define COMPARE_CB(arg) ((CompareCB_t)(arg))

typedef void (* ForeachCB_t) (void *, void *);

typedef int (* CompareCB_t) (const void *, const void *);

#endif
