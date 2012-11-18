#ifndef WCLEVEL_H
#define WCLEVEL_H

/* Includes */
#include "wcevent.h"

/*
	Loads a level and places the user at x and y in tile coordinates
*/
extern void wclevel_load(int id,int x,int y);

/*
	Draws the current level
*/
extern void wclevel_draw();

/*
	Releases a level
*/
extern void wclevel_free();

/*
	Moves the camera by some amount in the x and y direction
*/
extern void wclevel_move_camera(float mx,float my);

/*
	Returns the current map number
*/
extern int wclevel_get_id();

/*
	Loads the starting map with the player at starting position
*/
extern void wclevel_load_start();

/*
	Sets the camera to a position
*/
extern void wclevel_set_camera(float x,float y);

/*
	Handles the entire level for one frame
*/
extern void wclevel_handle();

/*
	Checks for possible moves in entire level, taking into account boundaries,
	events and the map. If pl is true, then also checks for player as solid event
*/
extern int wclevel_can_move(int x,int y,int dir,void *me);

/*
	Examines the location in the level given
*/
extern int wclevel_examine(int x,int y);

/*
	Steps on the event at location given
*/
extern int wclevel_step(int x,int y);

/*
	Get an event from this map
*/
extern void *wclevel_get_event(int id);

/*
	Get camera pos
*/
extern int wclevel_get_camera_x();
extern int wclevel_get_camera_y();

/*
	Get tile is above
*/
extern int wclevel_tile_is_above(tile t);

/*
	Wraps an absolute location according to the current wrapping rules
*/
extern void wclevel_wrap_screen(int *x,int *y);

/*
	Sets a link event so that the event can survive the map being deleted
*/
extern void wclevel_link_event(void *ev);

/*
	Pans the camera
*/
extern void wclevel_camera_pan(float mx,float my,float spd);

/*
	Returns true if the camera has not yet finished panning
*/
extern int wclevel_camera_panning();

/*
	Locks the camera
*/
extern void wclevel_camera_lock(int en);

/*
	Resets camera pan, panning it back to center
*/
extern void wclevel_camera_reset_pan(float spd);

/*
	Gets rid of the panning effect instantly
*/
extern void wclevel_camera_remove_pan();

/*
	Gets terrain code
*/
extern int wclevel_get_terrain(int x,int y);

/*
	FORCES the camera to be offset by this amount
	Used for transitions and shaking, etc.
*/
extern void wclevel_force_camera(float fx,float fy);

/*
	Returns the event ID at the location given, or 0 for none
*/
extern int wclevel_get_event_id(int x,int y);

/*
	Updates the terrain codes for all events
*/
extern void wclevel_update_all_event_terrain();

/*
	Returns true if can land here
*/
extern int wclevel_can_land(int x,int y,void *me);

/*
	Saves level state
*/
extern void wclevel_save_state(FILE *f);

/*
	Loads level state
*/
extern void wclevel_load_state(FILE *f);

/*
	Changes map parallax
*/
extern void wclevel_change_parallax(char *fname,int vs,int vsa,int vsp,int hs,int hsa,int hsp);

/*
	Gets map parallax bg name
*/
extern char *wclevel_get_parallax_name();

/*
	Gets map parallax scrolling settings vector
*/
extern void wclevel_get_parallax_scroll(int *scrollv);

#endif