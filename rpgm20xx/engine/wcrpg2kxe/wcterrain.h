#ifndef WCTERRAIN_H
#define WCTERRAIN_H

/* Includes */
#include <stdio.h>
#include "wctable.h"

/* Class */
class terrain
{
private:
	char *name;
	char *file;
public:
	/*
		Create terrain from entry
	*/
	terrain(entry *en);
	~terrain();
	/*
		Sets this terrain as current for backgrounds and stuff
	*/
	void set();
	/*
		Draws the terrain, setting it if it is not current
	*/
	void draw();
};

/*
	Load all terrains
*/
extern void wcterrain_load_all(FILE *f,int len);

/*
	Free all terrains
*/
extern void wcterrain_free_all();

/*
	Get terrain
*/
extern terrain *wcterrain_get(int id);

#endif