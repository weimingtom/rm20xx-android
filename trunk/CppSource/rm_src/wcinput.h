#ifndef WCINPUT_H
#define WCINPUT_H

/* Defines */
#define WCINPUT_NUM_CODES 27

#define WCINPUT_NONE 0

#define WCINPUT_DOWN 1
#define WCINPUT_LEFT 2
#define WCINPUT_RIGHT 3
#define WCINPUT_UP 4

#define WCINPUT_OK 5
#define WCINPUT_CANCEL 6
#define WCINPUT_SHIFT 7

#define WCINPUT_0 10
#define WCINPUT_1 11
#define WCINPUT_2 12
#define WCINPUT_3 13
#define WCINPUT_4 14
#define WCINPUT_5 15
#define WCINPUT_6 16
#define WCINPUT_7 17
#define WCINPUT_8 18
#define WCINPUT_9 19

#define WCINPUT_PLUS 20
#define WCINPUT_MINUS 21
#define WCINPUT_MULTIPLY 22
#define WCINPUT_DIVIDE 23
#define WCINPUT_PERIOD 24

#define WCINPUT_PAGE_DOWN 25
#define WCINPUT_PAGE_UP 26

#define WCINPUT_DEBUG_SAVE 27
#define WCINPUT_DEBUG_SCREENSHOT 28
#define WCINPUT_DEBUG_FOLDER 29
#define WCINPUT_DEBUG_PLAY 30
#define WCINPUT_DEBUG_SLOW 31
#define WCINPUT_DEBUG_FAST 32
#define WCINPUT_DEBUG_INFO 33

/*
	Collects input information for one frame.
*/
extern void wcinput_handle();

/*
	Returns the direction of input movement in the X direction.
*/
extern int wcinput_x();

/*
	Returns the direction of input movement in the Y direction.
*/
extern int wcinput_y();

/*
	Returns OK button
*/
extern int wcinput_ok();

/*
	Returns CANCEL button
*/
extern int wcinput_cancel();

/*
	Returns menu button
*/
extern int wcinput_menu();

/*
	Test keys
*/
extern int wcinput_map_test();
extern int wcinput_sound_test();

/*
	Unifying key mapping test function
*/
extern int wcinput_key(int code);

/*
	Return fast scroll keys
*/
extern int wcinput_scroll_y();

/*
	Get input vector
*/
extern void wcinput_vector(int *iv);

#endif