#ifndef WCINI_H
#define WCINI_H

/* Includes */
#include "wcarray.h"
#include "wclog.h"

/* Classes */
class ini
{
private:
	char *source; /* Source filename */
	int lines; /* Line count */
	arraylist data; /* Actual data as a list of lines */
	arraylist fields; /* List of fields */
	arraylist values; /* List of values set for fields */
public:
	/*
		Creates a new ini from file
	*/
	ini(char *file);
	~ini();
	/*
		Returns the number of entries.
	*/
	int entries();
	/*
		Reports the summary of this object to the log.
	*/
	void report(elog l);
	/*
		Returns the value for the given field.
	*/
	char *get_value(char *field);
};

#endif