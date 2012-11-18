#ifndef WCPOPNUM_H
#define WCPOPNUM_H

/* Defines */
#define WCPOPNUM_MAX 64
#define WCPOPNUM_DAMAGE 0
#define WCPOPNUM_STRING 1

/* Includes */
#include "wcwindow.h"

/* Clases */
class popnum
{
private:
	text *str; /* String instead */
	int pos_x,pos_y; /* Location */
	float anim_dist; /* Distance and time left in pop num animation */
	float vel_y; /* Velocity y for animation */
	float add_y; /* Addition in y on position */
	int type; /* Kind of display */
	int value; /* Number to show */
public:
	/*
		Creates a new pop number with value and type
	*/
	popnum(int x,int y,int val,int t);
	/*
		Creates a new pop "number" with the given string
	*/
	popnum(int x,int y,char *c);
	~popnum();
	/*
		Handles animation
	*/
	void handle();
	/*
		Draws
	*/
	void draw();
	/*
		Returns true if this pop number is active
	*/
	int active();
};

/*
	Shows a pop up number
*/
extern void wcpopnum_show(int x,int y,int v,int t);

/*
	Shows a pop up string
*/
extern void wcpopnum_show(int x,int y,char *c);

/*
	Initialize popnums
*/
extern void wcpopnum_init();

/*
	Exit popnums
*/
extern void wcpopnum_exit();

/*
	Handle all popnums
*/
extern void wcpopnum_handle();

/*
	Draw all popnums
*/
extern void wcpopnum_draw();

#endif