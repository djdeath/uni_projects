#ifndef __CONFIG_H__
#define __CONFIG_H__

/**
 * @file   sv_config.h
 * @author [ATR]Dj-Death
 * @date   Wed Mar 10 00:53:51 2004
 * 
 * @brief Gestion d'une structure de donnée permettant la d'associer à
 * un ensemble de clées des valeur. Nous utilisons cette structure
 * afin d'accéder facilement à des paramètres configurables sur notre
 * serveur.
 * 
 * 
 */

#include "hash_table.h"

#define CONFIG(arg) ((Config_t *)(arg))

typedef enum
{
	VALUE_INT,
	VALUE_DOUBLE,
	VALUE_STR
} ConfigValueType_t;

typedef struct HashTable_t Config_t;

/** 
 * Créer une nouvelle structure de configuration.
 * 
 * 
 * @return Un pointeur sur nouvelle structure de configuration allouée
 * dynamiquement, ou NULL si la fonction échoue.
 */
Config_t *config_new (void);

/** 
 * Libére l'espace mémoire utilisé par une structure de configuration.
 * 
 * @param conf Un pointeur sur la structure à libérer.
 */
void config_free (Config_t *conf);

/** 
 * 
 * 
 * @param file 
 * 
 * @return 
 */
Config_t *config_load (const char *file);

/** 
 * 
 * 
 * @param file 
 * @param conf 
 * 
 * @return 
 */
int config_save (const char *file, Config_t *conf);

/** 
 * 
 * 
 * @param conf 
 * @param key 
 * @param val 
 * 
 * @return 
 */
int config_set_int (Config_t *conf, const char *key, int val);

/** 
 * 
 * 
 * @param conf 
 * @param key 
 * @param val 
 * 
 * @return 
 */
int config_set_double (Config_t *conf, const char *key, double val);

/** 
 * 
 * 
 * @param conf 
 * @param key 
 * @param str 
 * 
 * @return 
 */
int config_set_str (Config_t *conf, const char *key, const char *str);

/** 
 * Accéder à une entrée de type entier dans un structure de
 * configuration.
 * 
 * @param conf Un pointeur sur une structure de configuration.
 * @param key  La clée permettant d'accéder à la valeur.
 * 
 * @return Un entier.
 */
int config_get_int (Config_t *conf, const char *key);

/** 
 * Accéder à une entrée de type flottant dans une structure de
 * configuration.
 * 
 * @param conf Un pointeur sur une structure de configuration.
 * @param key  La clée permettant d'accéder à la valeur.
 * 
 * @return Un flottant.
 */
double config_get_double (Config_t *conf, const char *key);

/** 
 * Accéder à une entrée de type chaîne de caractère dans une structure
 * de configuration.
 * 
 * @param conf Un pointeur sur une structure de configuration.
 * @param key  La clée permettant d'accéder à la valeur.
 * 
 * @return Un pointeur sur une chaîne de caractère, ou NULL.
 */
const char *config_get_str (Config_t *conf, const char *key);

#endif
