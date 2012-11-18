#ifndef WCTYPE_H
#define WCTYPE_H

/* Defines */
#define WCINT_MAX_BYTES 5

/* Includes */
#include <stdio.h>

/*
	Returns true if the given byte indicates more bytes need to be read to complete the int
*/
extern int wcint_continue(unsigned char b);

/*
	Returns the actual value contributed to an int by a byte and level
*/
extern int wcint_get(unsigned char b,int lv);

/*
	Returns the length of the rpg int beginning at the pointer given
*/
extern int wcint_length(void *v);

/*
	Reads the rpg int starting at the base given
*/
extern int wcint_read(void *v);

/*
	Returns the pointer beginning at the data after this rpg int
*/
extern void *wcint_next(void *v);

/*
	Reads a string and returns it
	the returned string must be freed by the caller
*/
extern char *wcstr_read(void *v);

/*
	Reads a string and returns it
	This is when you already read the length number
	the returned string must be freed by the caller
*/
extern char *wcstr_read(void *v,int len);

/*
	Reads an rpg int through a file
*/
extern int wcint_read_file(FILE *f,int *len);

/*
	Reads a string through a file
	the returned string must be freed by the caller
*/
extern char *wcstr_read_file(FILE *f,int *len);

/*
	Reads a string through a file when you already know the length
	the returned string must be freed by the caller
*/
extern char *wcstr_read_file(FILE *f,int len);

#endif