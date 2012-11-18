#ifndef WCRPG_H
#define WCRPG_H

/* Includes */
////#include <windows.h>
#include "wclog.h"
#include "wctext.h"

/* Defines */
#ifndef WCRPG_WIDESCREEN
#define WCRPG_WIDTH 320
#define WCRPG_HEIGHT 240
#endif
#ifdef WCRPG_WIDESCREEN
#define WCRPG_WIDTH 400
#define WCRPG_HEIGHT 240
#endif
#define WCRPG_DEFAULT_MAG 1
#define WCRPG_FPS_LENGTH 32

/*
	Reports the error message to the user.
*/
extern void wcrpg_error(char *msg);

/*
	Reports the error message to the user,
	inserting the given string where %s is found in the message string.
*/
extern void wcrpg_error(char *msg,char *str);

/*
	Reports the error message to the user,
	inserting the two given strings where %s is found twice in the message string in order.
*/
extern void wcrpg_error(char *msg,char *str1,char *str2);

/*
	Reports the error message to the user,
	inserting the given number where %d is found in the message string.
*/
extern void wcrpg_error(char *msg,int n);

/*
	Returns the handle to instance for this program.
*/
//extern HINSTANCE wcrpg_get_instance();
extern int wcrpg_get_instance();

/*
	Returns the system font
*/
extern font *wcrpg_get_sysfont();

/*
	Returns the working folder for the target game to run.
*/
extern char *wcrpg_get_root();

/*
	Returns the title of the game.
*/
extern char *wcrpg_get_title();

/*
	Returns the main log.
*/
extern elog wcrpg_get_log();

/*
	Clones a string.
*/
extern char *wcrpg_string_clone(char *c);

/*
	Returns the current screen magnification level
*/
extern int wcrpg_get_mag();

/*
	Returns the time step of the system
*/
extern float wcrpg_get_time_step();

/*
	Draws debug info
*/
extern void wcrpg_draw_debug();

/*
	Gets the currently selected in-game font
*/
extern font *wcrpg_get_font();

/*
	Signals the engine to stop and quit without any confirmation of saving data first
*/
extern void wcrpg_exit();

/*
	Returns true if the engine is in 20XX mode
*/
extern int wcrpg_neo_mode();

/*
	Returns true if the engine is in 2000 mode
*/
extern int wcrpg_old_mode();

/*
	Returns true if the engine is test play
*/
extern int wcrpg_test_play();

/*
	Sets the engine into a mode supporting 2000 games
*/
extern void wcrpg_set_old_mode();

/*
	Resets time counter (so you can skip over disk loading and such)
*/
extern void wcrpg_time_start();

/*
	Sets game speed
*/
extern void wcrpg_set_game_speed(float spd);

extern int init_();

extern void render_();

extern void destroy_();

#endif