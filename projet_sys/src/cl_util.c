#include <string.h>

#include "cl_util.h"

gchar *
str_convert_to_utf8 (const gchar *str)
{
	return g_convert (str, strlen (str), "UTF-8", "ISO-8859-1", NULL, NULL, NULL);
}

gchar *
str_convert_to_ascii (const gchar *str)
{
	g_return_val_if_fail (str != NULL, NULL);
        
	return g_locale_from_utf8 (str, -1, NULL, NULL, NULL);
}
