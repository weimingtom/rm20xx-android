#ifndef WCPLAYER_H
#define WCPLAYER_H

/* Includes */
#include <stdio.h>
#include "wcevent.h"

/*
	(Re)Creates the player at the tile position given
*/
extern void wcplayer_start(int tx,int ty);

/*
	Draws the player an offset x and y
*/
extern void wcplayer_draw(int x,int y);

/*
	Gets the player
*/
extern rpgevent *wcplayer_get();

/*
	Handles the player for one frame
*/
extern void wcplayer_handle();

/*
	Trips auto fire for examining
	Useful for when closing a dialog box and preventing the same event from being examined immediatly afterwards
*/
extern void wcplayer_trip_examine_auto_fire();

/*
	Locks the player's controls
	Used when events trigger on examine, impact, or auto
*/
extern void wcplayer_lock();

/*
	Unlocks the player's controls
*/
extern void wcplayer_unlock();

/*
	Refresh player
*/
extern void wcplayer_refresh();

/*
	Returns true if the player is allowed to move
*/
extern int wcplayer_is_allowed_to_move();

/*
	Saves player state
*/
extern void wcplayer_save(FILE *f);

/*
	Loads player state
*/
extern void wcplayer_load(FILE *f);

#endif