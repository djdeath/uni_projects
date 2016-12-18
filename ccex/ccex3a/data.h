#ifndef __DATA_H__
#define __DATA_H__

#define ASCENDING 1
#define DESCENDING -1

#define CALLBACK(arg) ((CallBack_t)(arg))
#define CALLBACKM(arg) ((CallBackM_t)(arg))
#define COMPARE(arg) ((Compare_t)(arg))
#define FOREACH(arg) ((Foreach_t)(arg))

typedef void (* CallBack_t) (void*);
typedef void (* CallBackM_t) (void*, ...);
typedef int (* Compare_t) (const void*, const void*);
typedef void (* Foreach_t) (void*, void*);

#endif
