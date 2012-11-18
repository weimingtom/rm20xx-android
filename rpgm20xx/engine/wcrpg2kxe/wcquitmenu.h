#ifndef WCQUITMENU_H
#define WCQUITMENU_H

/* Require */
#include "wcmenu.h"

/*
	Open quit menu
*/
extern void wcquitmenu_open(menu *m);

/*
	Close quit menu
*/
extern void wcquitmenu_close();

/*
	Quit menu open?
*/
extern int wcquitmenu_active();

/*
	Handle quit menu
*/
extern void wcquitmenu_handle();

/*
	Draw quit menu
*/
extern void wcquitmenu_draw();

#endif