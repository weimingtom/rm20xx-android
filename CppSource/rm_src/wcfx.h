#ifndef WCFX_H
#define WCFX_H

/*
	Initialize effects
*/
extern void wcfx_init();

/*
	Clean up effects
*/
extern void wcfx_exit();

/*
	Handle effects
*/
extern void wcfx_handle();

/*
	Draw effects
*/
extern void wcfx_draw();

/*
	Flash the screen
	(r,g,b) color
	p power
	t time (10ths seconds)
	rep repeat start/stop
*/
extern void wcfx_flash(int r,int g,int b,float p,int t,int rep);

#endif