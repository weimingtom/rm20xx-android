#ifndef WCBATTLESET_H
#define WCBATTLESET_H

/* Defines */
#define WCBATTLESET_SLOW 0
#define WCBATTLESET_MEDIUM 14
#define WCBATTLESET_QUICK 8
#define WCBATTLESET_RIGHT_HAND 0
#define WCBATTLESET_LEFT_HAND 1
#define WCBATTLESET_FRAME_AUTO -1

/* Require */
#include <stdio.h>
#include "wcarray.h"
#include "wctable.h"
#include "wcdraw.h"

/* Types */
typedef struct /* Pool type for characters */
{
	char *name;
	int mir; /* Mirror */
	image *im;
	texture *tex;
}battlechar;
typedef struct /* Pool type for weapons */
{
	char *name;
	int mir; /* Mirror */
	image *im;
	texture *tex;
}battleweapon;

/* Classes */
class battleframe
{
private:
	char *source; /* Source of the file */
	texture *tex;
	int charset; /* Character set number 0-7 */
	int use_animation; /* Use battle animation instead? */
	int animation_id; /* Animation ID to play */
	int is_weapon; /* Is this a weapon? */
public:
	/*
		Creates a new battle frame from entry
	*/
	battleframe(entry *en,int wp);
	/*
		Draws this frame at x and y with speed s
	*/
	void draw(int x,int y,int s);
	/*
		Delete a battle frame
	*/
	~battleframe();
};
class battleset
{
private:
	char *name; /* The name (this is actually used in the enemies as battle char sets 20XX feature */
	int speed; /* 0 = slow / 14 = medium / 8 = quick */
	arraylist bodies; /* Physical bodies of the character */
	arraylist weapons; /* Weapons to be drawn on character */
public:
	/*
		Creates a new battle set from table entry
	*/
	battleset(entry *en);
	~battleset();
	/*
		Draws this character at x and y using pose p
	*/
	void draw(int x,int y,int p);
	/*
		Draws an armed character at x and y using weapon w and hand h
	*/
	void draw(int x,int y,int w,int h);
	/*
		Gets name
	*/
	char *get_name();
};

/*
	Load all battlesets
*/
extern void wcbattleset_load_all(FILE *f,int len);

/*
	Purge and reset picture resource pool
*/
extern void wcbattleset_reload_all_set();

/*
	Free all battlesets
*/
extern void wcbattleset_free_all();

/*
	Pools a charset texture and returns it
*/
extern texture *wcbattleset_pool_battlechar(char *name,int mir);

/*
	Pools a weapon texture and returns it
*/
extern texture *wcbattleset_pool_battleweapon(char *name,int mir);

/*
	Draws a battle character
*/
extern void wcbattleset_draw(int x,int y,int c,int p);

/*
	Handles the battleset auto animations
*/
extern void wcbattleset_handle();

/*
	Draws an attack
*/
extern void wcbattleset_draw_attack(int x,int y,int c,int w,int h);

/*
	Changes the way battle char sets are displayed
*/
extern void wcbattleset_set_option(int f,int m);

/*
	Gets the enforced display frame, or WCBATTLESET_FRAME_AUTO for auto animate
*/
extern int wcbattleset_get_frame();

/*
	Gets the enforced mirror mode
*/
extern int wcbattleset_get_reverse();

/*
	Gets a battleset id by name
*/
extern int wcbattleset_seek(char *name);

#endif