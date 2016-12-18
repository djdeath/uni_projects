#ifndef __CONFIG_H__
#define __CONFIG_H__

/**
 * @file   sv_config.h
 * @author [ATR]Dj-Death
 * @date   Wed Mar 10 00:53:51 2004
 * 
 * @brief Gestion d'une structure de donn�e permettant la d'associer �
 * un ensemble de cl�es des valeur. Nous utilisons cette structure
 * afin d'acc�der facilement � des param�tres configurables sur notre
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
 * Cr�er une nouvelle structure de configuration.
 * 
 * 
 * @return Un pointeur sur nouvelle structure de configuration allou�e
 * dynamiquement, ou NULL si la fonction �choue.
 */
Config_t *config_new (void);

/** 
 * Lib�re l'espace m�moire utilis� par une structure de configuration.
 * 
 * @param conf Un pointeur sur la structure � lib�rer.
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
 * Acc�der � une entr�e de type entier dans un structure de
 * configuration.
 * 
 * @param conf Un pointeur sur une structure de configuration.
 * @param key  La cl�e permettant d'acc�der � la valeur.
 * 
 * @return Un entier.
 */
int config_get_int (Config_t *conf, const char *key);

/** 
 * Acc�der � une entr�e de type flottant dans une structure de
 * configuration.
 * 
 * @param conf Un pointeur sur une structure de configuration.
 * @param key  La cl�e permettant d'acc�der � la valeur.
 * 
 * @return Un flottant.
 */
double config_get_double (Config_t *conf, const char *key);

/** 
 * Acc�der � une entr�e de type cha�ne de caract�re dans une structure
 * de configuration.
 * 
 * @param conf Un pointeur sur une structure de configuration.
 * @param key  La cl�e permettant d'acc�der � la valeur.
 * 
 * @return Un pointeur sur une cha�ne de caract�re, ou NULL.
 */
const char *config_get_str (Config_t *conf, const char *key);

#endif
