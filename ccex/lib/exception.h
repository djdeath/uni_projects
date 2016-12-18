#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

/**
 * @file   exception.h
 * @author [ATR]Dj-Death
 * @date   Wed Mar 24 17:48:24 2004
 * 
 * @brief  
 * 
 * 
 */


/** 
 * Throw.
 * 
 * @param except 
 */
void __throw (unsigned int except);

/** 
 * Début de bloc try.
 * 
 * @param rescue 
 * @param ebp 
 * @param esp 
 */
void __try (void *rescue, void *ebp, void *esp);

/** 
 * 
 * 
 * @param rescue 
 */
void __try_end (void *rescue);

/** 
 * Début de bloc finally.
 * 
 */
void __finally (void);

/** 
 * Fin de bloc finally.
 * 
 */
void __finally_end (void);

void __exception_init (void);

#endif
