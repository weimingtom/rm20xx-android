#ifndef WCTIMER_H
#define WCTIMER_H

/* Defines */
#define WCTIMER_MAX 2
#define WCTIMER_STEP 16.0f

/*
	Initializes timer system
*/
extern void wctimer_init();

/*
	Sets a timer
*/
extern void wctimer_set(int tid,int s);

/*
	Starts a timer
*/
extern void wctimer_start(int tid,int vis,int btl);

/*
	Stops a timer
*/
extern void wctimer_stop(int tid);

/*
	Handles timers
*/
extern void wctimer_handle();

/*
	Draws timers
*/
extern void wctimer_draw();

/*
	Get time
*/
extern int wctimer_get(int tid);

#endif