#ifndef WCGEARPLATE_H
#define WCGEARPLATE_H

/* Includes */
#include "wcitem.h"

/*
	Draw a hero's gear at x and y.
	ix is the index of hero and s is the selected item
*/
extern void wcgearplate_draw(int x,int y,int ix,int s,int rm);

/*
	Draw an item's expanded summary to include attributes and effects
*/
extern void wcgearplate_draw_extra(int x,int y,item *it);

#endif