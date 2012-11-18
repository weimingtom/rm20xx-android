#ifndef WCTABLE_H
#define WCTABLE_H

/* Includes */
#include <stdio.h>
#include "wcarray.h"

/* Classes */
class entry
{
private:
	int id; /* ID of this entry */
	int size; /* Byte size of this entry (in the database) */
	arraylist codes; /* Codes */
	arraylist lens; /* Length of section belonging to code */
	arraylist datas; /* Actual data for code */
	/*
		Turns code into id
	*/
	int get_id_from_code(int code);
public:
	/*
		Creates a new entry from file stream
	*/
	entry(FILE *f);
	/*
		Creates a new entry from raw data
	*/
	entry(void *v);
	~entry();
	/*
		Gets a length of the section asked for
	*/
	int get_length(int code);
	/*
		Gets the data for a section asked for
	*/
	unsigned char *get_data(int code);
	/*
		Gets id 
	*/
	int get_id();
	/*
		Contains data for code given?
	*/
	int has_data(int code);
	/*
		Gets the size of the source data from the database
	*/
	int get_size();
};

class table
{
private:
	int count; /* Number of entryes */
	int size; /* Byte size of the entire table */
	arraylist entries; /* Actual entries */
public:
	/*
		Creates new table
	*/
	table(FILE *f);
	/*
		Creates a new table from memory data
	*/
	table(void *v);
	~table();
	/*
		Gets an entry
	*/
	entry *get_entry(int id);
	/*
		Gets number of entries
	*/
	int get_count();
	/*
		Gets size
	*/
	int get_size();
};

#endif