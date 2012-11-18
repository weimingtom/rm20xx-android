#ifndef WCTRANS_H
#define WCTRANS_H

/* Defines */
#define WCTRANS_SCREEN_SHOW 0
#define WCTRANS_SCREEN_HIDDEN 1

#define WCTRANS_FADE 0
#define WCTRANS_BLOCKS 1
#define WCTRANS_WIPE_DOWN 2
#define WCTRANS_WIPE_UP 3
#define WCTRANS_BLINDS 4
#define WCTRANS_VERTICAL_BLINDS 5
#define WCTRANS_HORIZONTAL_BLINDS 6
#define WCTRANS_SHRINK_SQUARE 7
#define WCTRANS_EXPAND_SQUARE 8
#define WCTRANS_SCREEN_UP 9
#define WCTRANS_SCREEN_DOWN 10
#define WCTRANS_SCREEN_LEFT 11
#define WCTRANS_SCREEN_RIGHT 12
#define WCTRANS_VERTICAL_DIVIDE 13
#define WCTRANS_HORIZONTAL_DIVIDE 14
#define WCTRANS_QUAD 15
#define WCTRANS_ZOOM 16
#define WCTRANS_MOSAIC 17
#define WCTRANS_WAVER 18
#define WCTRANS_INSTANT 19
#define WCTRANS_NONE 20

/*
	Handles transitions
*/
extern void wctrans_handle();

/*
	Returns true if transitions are going on
*/
extern int wctrans_active();

/*
	Erase screen
*/
extern void wctrans_erase(int eff);

/*
	Show screen
*/
extern void wctrans_show(int eff);

/*
	Init trans
*/
extern void wctrans_init();

/*
	Exit trans
*/
extern void wctrans_exit();

/*
	Draw trans
*/
extern void wctrans_draw();

/*
	Get the screen state
*/
extern int wctrans_get_screen_state();

/*
	Set anti teleport mode
*/
extern void wctrans_set_teleport_mode(int en);

/*
	Get anti teleport mode
*/
extern int wctrans_get_teleport_mode();

#endif