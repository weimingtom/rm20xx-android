#ifndef WCNUCLEUS_H
#define WCNUCLEUS_H

/* Defines */
#define WCRPG_MODE_MAP_VIEW 0
#define WCRPG_TITLE_SCREEN 1
#define WCRPG_MAP 2
#define WCRPG_GAME_OVER 3

/*
	Handles the entire game for one frame
*/
extern void wcnucleus_handle();

/*
	Gets the current game mode
*/
extern int wcnucleus_get_mode();

/*
	Changes the game mode
*/
extern void wcnucleus_set_mode(int m);

/*
	Draws the entire game
*/
extern void wcnucleus_draw();

/*
	Sets the screen tint
*/
extern void wcnucleus_tint(float r,float g,float b,float s,float t);

/*
	Gets the tint colors
*/
extern float wcnucleus_get_tint_red();
extern float wcnucleus_get_tint_green();
extern float wcnucleus_get_tint_blue();
extern float wcnucleus_get_tint_sat();

/*
	Gets speed mode
*/
extern int wcnucleus_get_debug_speed_mode();

/*
	Gets info display mode (FPS display, etc.)
*/
extern int wcnucleus_get_debug_info_mode();

#endif