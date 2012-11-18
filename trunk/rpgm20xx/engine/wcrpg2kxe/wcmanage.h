#ifndef WCMANAGE_H
#define WCMANAGE_H

/* Require */
#include <stdio.h>

/*
	Initialize manager
*/
extern void wcmanage_init();

/*
	Handle manager
*/
extern void wcmanage_handle();

/*
	Draw manager
*/
extern void wcmanage_draw();

/*
	Close manager
*/
extern void wcmanage_close();

/*
	Clean up manager
*/
extern void wcmanage_exit();

/*
	Return active
*/
extern int wcmanage_active();

/*
	Enable/Disable manager
*/
extern void wcmanage_enable(int en);

/*
	Sub menu
*/
extern void wcmanage_return();

/*
	Stacks a sub menu
*/
extern void wcmanage_stack();

/*
	Opens main menu
*/
extern void wcmanage_open();

/*
	Scrolls the hero summary plates around
*/
extern void wcmanage_scroll_hero_plate(int pos);

/*
	Returns the pixel offset of the hero plate
*/
extern int wcmanage_get_hero_scroll_pos();

/*
	Remaps menu commands
*/
extern void wcmanage_remap(int cmd,int com,int en);

/*
	Saves options
*/
extern void wcmanage_save(FILE *f);

/*
	Loads options
*/
extern void wcmanage_load(FILE *f);

/*
	Sub menu active?
*/
extern int wcmanage_submenu_active();

#endif