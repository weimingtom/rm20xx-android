#ifndef WCGUIBUILD_H
#define WCGUIBUILD_H

/* Defines */
#define GUI_BUILDER_NUMBER 0
#define GUI_BUILDER_WINDOW 1
#define GUI_BUILDER_HEALTH_BAR 2
#define GUI_BUILDER_MANA_BAR 3
#define GUI_BUILDER_TIME_BAR 4
#define GUI_BUILDER_BATTLE_CHARACTER 5
#define GUI_BUILDER_BATTLE_ATTACK_RIGHT 6
#define GUI_BUILDER_BATTLE_ATTACK_LEFT 7
#define GUI_BUILDER_MONSTER 8
#define GUI_BUILDER_IMAGE 9
#define GUI_BUILDER_NAME 10

/* Includes */
#include "wcdraw.h"

/* Typedefs */
typedef struct
{
	int resid;
	char *name;
	texture *tex;
}imageentry;

/* Classes */
class guielement
{
private:
	int type; /* Kind */
	int pos_x,pos_y; /* Position */
	int width,height; /* Size */
	int num; /* Number */
	int option; /* Option for the element */
	int frame,mirror; /* Frame and mirror options */
public:
	/*
		Creates a new element with blank parameters
	*/
	guielement(int t,int x,int y);
	/*
		Sets the number
	*/
	void set_number(int n);
	/*
		Sets the option
	*/
	void set_option(int o);
	/*
		Sets the size
	*/
	void set_size(int w,int h);
	/*
		Draws this element
	*/
	void draw();
};

/*
	Initialize builder
*/
extern void wcguibuild_init();

/*
	Exit builder
*/
extern void wcguibuild_exit();

/*
	Add to builder
*/
extern void wcguibuild_add(guielement *ge);

/*
	Draw and clear builder list
*/
extern void wcguibuild_draw();

/*
	Resets image pooling
*/
extern void wcguibuild_reset_image_pool();

/*
	Pools an image
*/
extern texture *wcguibuild_pool_image(int resid);

#endif