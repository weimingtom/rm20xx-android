#ifndef WCDIALOG_H
#define WCDIALOG_H

/* Defines */
#define WCDIALOG_LINES 4
#define WCDIALOG_POS_TOP 0
#define WCDIALOG_POS_MIDDLE 1
#define WCDIALOG_POS_BOTTOM 2

/* Includes */
#include "wcarray.h"
#include "wcmenu.h"

/*
	Opens a dialog box with all the previous settings applied
*/
extern void wcdialog_open();

/*
	Draws the current state of the dialog box
*/
extern void wcdialog_draw();

/*
	Adds a line of dialog
*/
extern void wcdialog_add_line(char *ln);

/*
	Reset dialog stack, does not clear dialog text
*/
extern void wcdialog_reset_line();

/*
	Returns true if dialog is currently active
*/
extern int wcdialog_active();

/*
	Request that the dialog box permission be given
*/
extern int wcdialog_request(int id);

/*
	Returns true if this event id should halt for dialog to finish
*/
extern int wcdialog_halt(int id);

/*
	Handles dialog
*/
extern void wcdialog_handle();

/*
	Resets dialog face to blank
*/
extern void wcdialog_erase_face();

/*
	Sets the dialog face
*/
extern void wcdialog_set_face(char *face,int index,int rt,int fp);

/*
	Draws the current dialog face, if any
*/
extern void wcdialog_draw_face(int x,int y);

/*
	Sets dialog options
*/
extern void wcdialog_options(int trans,int pos,int autopos,int nowait);

/*
	Creates a menu from a show choice string
*/
extern menu *wcdialog_make_choice_menu(char *str);

/*
	Shows choice
*/
extern void wcdialog_open_choice(char *str);

/*
	Get menu desicion
*/
extern int wcdialog_get_choice();

/*
	Set default desicion
*/
extern void wcdialog_set_choice_default(int d);

#endif