#ifndef WCITEMMENU_H
#define WCITEMMENU_H

/* Defines */
#define WCITEMMENU_WIDTH 192
#define WCITEMMENU_MAX_VISIBLE 12

#define WCITEMMENU_ALL 0
#define WCITEMMENU_MEDICINE 1
#define WCITEMMENU_EQUIP 2
#define WCITEMMENU_SPECIAL 3
#define WCITEMMENU_ETC 4
#define WCITEMMENU_PERSONAL 5

/* Require */
#include "wcmenu.h"
#include "wcitem.h"

/*
	Open item menu
*/
extern void wcitemmenu_open(menu *m);

/*
	Close item menu
*/
extern void wcitemmenu_close();

/*
	Active?
*/
extern int wcitemmenu_active();

/*
	Handle
*/
extern void wcitemmenu_handle();

/*
	Draw
*/
extern void wcitemmenu_draw();

/*
	Refresh
*/
extern void wcitemmenu_refresh();

/*
	Set filter
*/
extern void wcitemmenu_set_personal_filter(int id,int t);

/*
	Return item
*/
extern item *wcitemmenu_get_selected();

#endif