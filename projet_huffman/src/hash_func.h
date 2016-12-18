#ifndef __HASH_FUNC_H__
#define __HASH_FUNC_H__

#include "hash_table.h"

HashFuncCB_t hash_num_get_func (HashFunc_t func);
HashFunc_t hash_func_get_num (HashFuncCB_t func);

#endif
