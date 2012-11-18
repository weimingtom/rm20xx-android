#ifndef WCMESSAGE_H
#define WCMESSAGE_H

/*
	Initialize messages
*/
extern void wcmessage_init();

/*
	Exit messages
*/
extern void wcmessage_exit();

/*
	Handle messages
*/
extern void wcmessage_handle();

/*
	Draw messages
*/
extern void wcmessage_draw();

/*
	Show messages
*/
extern void wcmessage_show(char *msg);

#endif