#ifndef WCSTATUSMENU_H
#define WCSTATUSMENU_H

/* Require */
#include "wcmenu.h"

/*
	Open menu
*/
extern void wcstatusmenu_open(menu *m);

/*
	Close menu
*/
extern void wcstatusmenu_close();

/*
	Menu is active?
*/
extern int wcstatusmenu_active();

/*
	Handle menu
*/
extern void wcstatusmenu_handle();

/*
	Draw menu
*/
extern void wcstatusmenu_draw();

/*
	Initialize
*/
extern void wcstatusmenu_init();

/*
	Exit
*/
extern void wcstatusmenu_exit();

/*
	Open status menu, but for the purpose of changing equipment
*/
extern void wcstatusmenu_open_equip(menu *m);

/*
	Open status menu, but for the purpose of viewing and possibly using skills
*/
extern void wcstatusmenu_open_skill(menu *m);

#endif