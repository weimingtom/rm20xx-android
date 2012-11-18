#ifndef WCSKILLMENU_H
#define WCSKILLMENU_H

/* Defines */
#define WCSKILLMENU_WIDTH 192
#define WCSKILLMENU_MAX_VISIBLE 12

/* Require */
#include "wcmenu.h"
#include "wcskill.h"
#include "wchero.h"

/*
	Opens the skill menu focused on the given hero
*/
extern void wcskillmenu_open(hero *hr);

/*
	Closes skill menu
*/
extern void wcskillmenu_close();

/*
	Handles skill menu
*/
extern void wcskillmenu_handle();

/*
	Draw skill menu
*/
extern void wcskillmenu_draw();

#endif