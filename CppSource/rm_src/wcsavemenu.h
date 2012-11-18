#ifndef WCSAVEMENU_H
#define WCSAVEMENU_H

/* Defines */
#define WCSAVEMENU_WIDTH 320
#define WCSAVEMENU_HEIGHT 80

/* Require */
#include "wcmenu.h"

/*
	Initialize the save menu
*/
extern void wcsavemenu_init();

/*
	Refresh and update the menu
*/
extern void wcsavemenu_refresh(int loadin);

/*
	Open save menu
*/
extern void wcsavemenu_open(menu *m,int loadin);

/*
	Close save menu
*/
extern void wcsavemenu_close();

/*
	Is the save menu open
*/
extern int wcsavemenu_active();

/*
	Handle save menu
*/
extern void wcsavemenu_handle();

/*
	Draw save menu
*/
extern void wcsavemenu_draw();

#endif