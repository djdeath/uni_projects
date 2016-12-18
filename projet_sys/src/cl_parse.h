#ifndef __CL_PARSE_H__
#define __CL_PARSE_H__

#include "cl_ui_main.h"

/** 
 * 
 * 
 * @param win 
 * 
 * @return 
 */
gint server_recv (WindowMain_t *win);

/** 
 * 
 * 
 * @param win 
 * @param fmt 
 * 
 * @return 
 */
gint server_send (WindowMain_t *win, const gchar *fmt, ...);

#endif
