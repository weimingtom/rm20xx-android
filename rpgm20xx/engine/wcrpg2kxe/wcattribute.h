#ifndef WCATTRIBUTE_H
#define WCATTRIBUTE_H

/* Includes */
#include <stdio.h>
#include "wctable.h"

/* Defines */
#define WCATTRIBUTE_WEAPON 0
#define WCATTRIBUTE_MAGIC 1
#define WCATTRIBUTE_NUM_TIER 5
#define WCATTRIBUTE_A 0
#define WCATTRIBUTE_B 1
#define WCATTRIBUTE_C 2
#define WCATTRIBUTE_D 3
#define WCATTRIBUTE_E 4

/* Classes */
class attribute
{
private:
	char *name;
	int type;
	int levels[WCATTRIBUTE_NUM_TIER];
public:
	/*
		Create new attribute from entry
	*/
	attribute(entry *en);
	~attribute();
	/*
		Returns name
	*/
	char *get_name();
	/*
		Returns type
	*/
	int get_type();
	/*
		Returns effect power
	*/
	int get_effect(int ix);
};

/*
	Loads all attributes
*/
extern void wcattribute_load_all(FILE *f,int len);

/*
	Releases all attributes
*/
extern void wcattribute_free_all();

/*
	Get an attribute
*/
extern attribute *wcattribute_get(int id);

/*
	Get count
*/
extern int wcattribute_count();

#endif