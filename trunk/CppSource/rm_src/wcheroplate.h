#ifndef WCHEROPLATE_H
#define WCHEROPLATE_H

/*
	Draws the hero plate
*/
extern void wcheroplate_draw_party(int x,int y);

/*
	Draws a single entry on the plate
*/
extern void wcheroplate_draw_party_entry(int x,int y,int ix);

/*
	Initialize plates
*/
extern void wcheroplate_init();

/*
	Clean up plates
*/
extern void wcheroplate_exit();

/*
	Refresh plates
*/
extern void wcheroplate_refresh();

#endif