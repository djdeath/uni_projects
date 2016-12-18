#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

#include "util.h"
#include "config.h"

/**/

#define MAX_VAL_STR (50)

#define CONFIG_VALUE(arg) ((ConfigValue_t *)(arg))

typedef struct _ConfigValue_t ConfigValue_t;

struct _ConfigValue_t
{
	char *key;
	char *val;
};

/**/

static ConfigValue_t *
config_value_new (const char *key)
{
	ConfigValue_t *val;
	
	if (!key)
		return NULL;
	
	if ((val = CONFIG_VALUE (calloc (1, sizeof (ConfigValue_t)))))
	{
		if (!(val->key = strndup (key, MAX_VAL_STR, NULL)))
		{
			free (val);
			val = NULL;
		}
	}
	
	return val;
}

static void
config_value_free (ConfigValue_t *val)
{
	if (val)
	{
		if (val->key)
			free (val->key);
		if (val->val)
			free (val->val);
		
		free (val);
	}
}

static int
config_value_set (ConfigValue_t *val, ConfigValueType_t type, ...)
{
	char *sval, *tmp;
	int ival, ret;
	double fval;
	va_list list;
	
	if (!val)
		return 0;
	
	ret = 1;
	va_start (list, type);
	switch (type)
	{
	case VALUE_STR:
		if (!(sval = va_arg (list, char *)))
		{
			ret = 0;
			break;
		}
		if (!(tmp = strndup (sval, MAX_VAL_STR, NULL)))
		{
			ret = 0;
			break;
		}
		break;
		
	case VALUE_DOUBLE:
		fval = va_arg (list, double);
		if (!(tmp = malloc (MAX_VAL_STR * sizeof (char))))
		{
			ret = 0;
			
			break;
		}
		snprintf (tmp, MAX_VAL_STR, "%f", fval);
		break;
		
	case VALUE_INT:
		ival = va_arg (list, int);
		if (!(tmp = malloc (MAX_VAL_STR * sizeof (char))))
		{
			ret = 0;
			
			break;
		}
		snprintf (tmp, MAX_VAL_STR, "%i", ival);
		break;
	}
	va_end (list);
	
	if (ret)
	{
		if (val->val)
			free (val->val);
		val->val = tmp;
	}
	
	return ret;
}

static int
config_value_get (ConfigValue_t *val, ConfigValueType_t type, ...)
{
	char **sval;
	int *ival, ret;
	double *fval;
	va_list list;
	
	if (!val)
		return 0;
	
	ret = 1;
	va_start (list, type);
	switch (type)
	{
	case VALUE_STR:
		if (!(sval = va_arg (list, char **)))
		{
			ret = 0;
			break;
		}
		*sval = val->val;
		break;
		
	case VALUE_DOUBLE:
		if (!(fval = va_arg (list, double *)))
		{
			ret = 0;
			break;
		}
		if (val->val)
			*fval = (double) strtod (val->val, NULL);
		else
			*fval = 0.0;
		break;
		
	case VALUE_INT:
		if (!(ival = va_arg (list, int *)))
		{
			ret = 0;
			break;
		}
		if (val->val)
			*ival = strtol (val->val, NULL, 10);
		else
			*ival = 0;
		break;
	}
	va_end (list);
	
	return ret;
}

/**/

Config_t *
config_new (void)
{
	Config_t *conf = CONFIG (hash_table_new (HASH_TABLE_CLOSE_DEFAULT_SIZE,
						 HASH_MODE_OPEN, HASH_FUNC_GLIB));
	
	return conf;
}

void
config_free (Config_t *conf)
{
	if (conf)
	{
		hash_table_foreach (HASH_TABLE (conf), FOREACH (config_value_free), NULL);
		hash_table_free (HASH_TABLE (conf));
	}
}

/**/

static int
config_add (Config_t *conf, const char *key, ConfigValue_t **val)
{
	ConfigValue_t *value = NULL;
	
	if (!conf || !key)
		return 0;
	
	while ((value = hash_table_search (HASH_TABLE (conf), key, value)))
	{
		if (!strcmp (key, value->key))
			return 0;
	}
	
	if (!(value = config_value_new (key)))
		return 0;
	
	if (hash_table_insert (HASH_TABLE (conf), key, value))
	{
		config_value_free (value);
		
		return 0;
	}
	
	*val = value;
	
	return 1;
}

/* Pas utilisé mais on sait jamais :) */
/*static void
config_remove (Config_t *conf, const char *key)
{
	ConfigValue_t *value = NULL;
	
	if (!conf || !key)
		return;
	
	while ((value = hash_table_search (HASH_TABLE (conf), key, value)))
	{
		if (!strcmp (key, value->key))
		{
			hash_table_remove (HASH_TABLE (conf), key, value);
			config_value_free (value);
			
			return;
		}
	}
}*/

/**/

/* On a essayé de faire propre, promis... */
#define BUF_READ_SIZE (512)

static int
read_key (char **start, char **next, char *end)
{
	while ((*start < end) && (!isalpha (**start)) && (**start != '#'))
		(*start)++;
	
	if ((*start >= end) || (**start == '#'))
		return 0;
	
	*next = *start;
	
	while ((*next < end) && (isalpha (**next) || (**next == '_')) && (**start != '#'))
		(*next)++;
	
	if (**next == '#')
		return 0;
	
	**next = '\0';
	(*next)++;
	
	return 1;
}

static int
read_sep (char **start, char **next, char *end)
{
	while ((*start < end) && (**start == ' ') && (**start != '#'))
		(*start)++;
	
	if ((*start >= end) || (**start != '='))
		return 0;
	
	*next = *start;
	
	(*next)++;
	
	return 1;
}

static int
read_val (char **start, char **next, char *end)
{
	while ((*start < end) && (**start == ' ') && (**start != '#'))
		(*start)++;
	
	if ((*start >= end) || (**start == '#'))
		return 0;
	
	*next = *start;
	
	while ((*next < end) && (isgraph (**next) ||
	       isspace (**next)) && (**next != '#'))
		(*next)++;
	
	if (**next == '#')
		return 0;
	
	**next = '\0';
	(*next)++;
	
	return 1;
}

Config_t *
config_load (const char *file)
{
	Config_t *conf = NULL;
	int fd;
	
	if (file && (fd = open (file, O_RDONLY)) != -1)
	{
		if ((conf = config_new ()))
		{
			char buf[BUF_READ_SIZE], *s, *key, *next, *val, *end;
			ssize_t len;
			
			s = buf;
			while ((len = read (fd, s, BUF_READ_SIZE - (s - buf))) > 0)
			{
				s = buf;
				
				do
				{
					end = s;
					
					while ((end < (buf + len)) && (*end != '\n'))
						end++;
					
					if (*end == '\n')
					{
						*end = '\0';
						
						key = s;
						if (read_key (&key, &next, end))
						{
							if (read_sep (&next, &val, end))
							{
								if (read_val (&val, &end, end))
									config_set_str (conf, key, val);
								s = end;
							}
							else
								s = end;
						}
						else
							s = end;
					}
					else
						break;
				} while (s < (buf + len));
				
				if (s < (buf + len))
				{
					memmove (buf, s, end - s);
					s = buf + (end - s);
				}
			}
		}
		close (fd);
	}
	
	return conf;
}

static void
config_save_key (ConfigValue_t *val, FILE *f)
{
	fprintf (f, "%s = %s\n", val->key, val->val);
}

int
config_save (const char *file, Config_t *conf)
{
	FILE *f;
	
	if ((f = fopen (file, "w")))
	{
		fprintf (f, "# COMMENT\n");
		
		hash_table_foreach (HASH_TABLE (conf), FOREACH (config_save_key), f);
		
		fclose (f);
		
		return 1;
	}
	
	return 0;
}

/**/

int
config_set_int (Config_t *conf, const char *key, int val)
{
	ConfigValue_t *value = NULL;
	
	if (!conf || !key)
		return 0;
	
	while ((value = hash_table_search (HASH_TABLE (conf), key, value)))
	{
		if (!strcmp (key, value->key))
		{
			config_value_set (value, VALUE_INT, val);
			
			return 1;
		}
	}
	
	if (config_add (conf, key, &value))
	{
		config_value_set (value, VALUE_INT, val);
		
		return 1;
	}
	
	return 1;
}

int
config_set_double (Config_t *conf, const char *key, double val)
{
	ConfigValue_t *value = NULL;
	
	if (!conf || !key)
		return 0;
	
	while ((value = hash_table_search (HASH_TABLE (conf), key, value)))
	{
		if (!strcmp (key, value->key))
		{
			config_value_set (value, VALUE_DOUBLE, val);
			
			return 1;
		}
	}
	
	if (config_add (conf, key, &value))
	{
		config_value_set (value, VALUE_DOUBLE, val);
		
		return 1;
	}
	
	return 1;
}

int
config_set_str (Config_t *conf, const char *key, const char *str)
{
	ConfigValue_t *value = NULL;
	
	if (!conf || !key)
		return 0;
	
	while ((value = hash_table_search (HASH_TABLE (conf), key, value)))
	{
		if (!strcmp (key, value->key))
		{
			config_value_set (value, VALUE_STR, str);
			
			return 1;
		}
	}
	
	if (config_add (conf, key, &value))
	{
		config_value_set (value, VALUE_STR, str);
		
		return 1;
	}
	
	return 1;
}

/**/

int
config_get_int (Config_t *conf, const char *key)
{
	int ret;
	ConfigValue_t *value = NULL;
	
	if (!conf || !key)
		return 0;
	
	while ((value = hash_table_search (HASH_TABLE (conf), key, value)))
	{
		if (!strcmp (key, value->key))
		{
			config_value_get (value, VALUE_INT, &ret);
			
			return ret;
		}
	}
	
	return 0;
}

double
config_get_double (Config_t *conf, const char *key)
{
	double ret;
	ConfigValue_t *value = NULL;
	
	if (!conf || !key)
		return 0.0;
	
	while ((value = hash_table_search (HASH_TABLE (conf), key, value)))
	{
		if (!strcmp (key, value->key))
		{
			config_value_get (value, VALUE_DOUBLE, &ret);
			
			return ret;
		}
	}
	
	return 0.0;
}

const char *
config_get_str (Config_t *conf, const char *key)
{
	const char *ret;
	ConfigValue_t *value = NULL;
	
	if (!conf || !key)
		return NULL;
	
	while ((value = hash_table_search (HASH_TABLE (conf), key, value)))
	{
		if (!strcmp (key, value->key))
		{
			if (config_value_get (value, VALUE_STR, &ret))
				return ret;
			return NULL;
		}
	}
	
	return NULL;
}


