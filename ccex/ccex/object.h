#ifndef __OBJECT_H__
#define __OBJECT_H__

typedef struct _Object_t Object_t;

struct _Object_t
{
	unsigned int count;
};

void object_init (Object_t *obj);

Object_t object_ref (Object_t *obj);
void oject_unref (Object_t *obj);

#endif
