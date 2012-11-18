#ifndef WCSAVEIO_H
#define WCSAVEIO_H

/* Requires */
#include <stdio.h>

/*
	Starts a save block
*/
extern void wcsaveio_start(FILE *f);

/*
	Ends a save block
*/
extern void wcsaveio_end();

/*
	Writes a single byte to save block
*/
extern void wcsaveio_write(unsigned char b);

/*
	Writes a number to save block
*/
extern void wcsaveio_write_num(int n);

/*
	Writes a float to save block
*/
extern void wcsaveio_write_float(float f);

/*
	Writes a string to save block
*/
extern void wcsaveio_write_str(char *str);

/*
	Starts reading from a save block
*/
extern void wcsaveio_read(FILE *f);

/*
	Reads a single byte
*/
extern unsigned char wcsaveio_read_byte();

/*
	Reads a number
*/
extern int wcsaveio_read_num();

/*
	Reads a float
*/
extern float wcsaveio_read_float();

/*
	Reads a string
*/
extern char *wcsaveio_read_str();

/*
	Finish reading and move onto next
*/
extern void wcsaveio_next();

#endif