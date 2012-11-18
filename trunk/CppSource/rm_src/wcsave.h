#ifndef WCSAVE_H
#define WCSAVE_H

/* Defines */
#define WCSAVE_SLOTS 16

/* Require */
#include "wcparty.h"

/* Header type */
typedef struct
{
	int used; /* Is this slot being used? */
	int party[MAX_PARTY_SIZE]; /* Party */
	char *face[MAX_PARTY_SIZE]; /* Faces */
	int face_index[MAX_PARTY_SIZE]; /* Face index */
	char *leadername; /* Leader's name */
}savetag;

/*
	Initialize save system
*/
extern void wcsave_init();

/*
	Release save system
*/
extern void wcsave_exit();

/*
	Return base system
*/
extern char *wcsave_get_base();

/*
	Return a suitable save file name for current game
*/
extern char *wcsave_get_file(int slot);

/*
	Return a suitable save file name for the save catalog
*/
extern char *wcsave_get_tag_file();

/*
	Loads all game save tags from catalog
*/
extern void wcsave_load_tags();

/*
	Saves current game into slot given
*/
extern void wcsave_save(int slot);

/*
	Loads game from slot given
*/
extern void wcsave_load(int slot);

/*
	Gets a save tag
*/
extern savetag *wcsave_get_tag(int slot);

/*
	Returns 1 if there are game saves we can load
*/
extern int wcsave_has_game_saves();

/*
	Returns 1 if a game has just been loaded into memory
*/
extern int wcsave_game_loaded();

/*
	Resets the game loaded flag
*/
extern void wcsave_game_reset_loaded();

/*
	Get last file
*/
extern int wcsave_get_last();

/*
	Allow/Disallow save
*/
extern void wcsave_allow(int en);

/*
	Is saving allowed from the menu?
*/
extern int wcsave_is_allowed();

/*
	Get game save base dir
*/
extern char *wcsave_get_game_base();

#endif