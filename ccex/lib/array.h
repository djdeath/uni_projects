#ifndef __ARRAY_H__
#define __ARRAY_H__

/**
 * @file   array.h
 * @author [ATR]Dj-Death
 * @date   Wed Mar 24 17:47:39 2004
 * 
 * @brief  
 * 
 * 
 */


/* NB_dim:dim_1:dim_2:...:dim_n:matrice */

typedef int * Array_t;

/** 
 * 
 * 
 * @param dim1 
 * 
 * @return 
 */
Array_t __array_allocate (int dim1, ...);

/** 
 * 
 * 
 * @param array 
 */
void __array_free (Array_t array);

/** 
 * 
 * 
 * @param array 
 * @param dim1 
 * 
 * @return 
 */
int __array_get (Array_t array, int dim1, ...);

/** 
 * 
 * 
 * @param array 
 * @param index 
 * 
 * @return 
 */
int __array_get_a (Array_t array, Array_t index);

void __array_set (Array_t array, int val, int dim1, ...);

void __array_set_a (Array_t array, int val, Array_t index);

/** 
 * 
 * 
 * @param array 
 * 
 * @return 
 */
int __array_dimension (Array_t array);

/** 
 * 
 * 
 * @param array 
 * @param n 
 * 
 * @return 
 */
int __array_length (Array_t array, int n);

#endif
