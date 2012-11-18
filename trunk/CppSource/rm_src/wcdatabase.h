#ifndef WCDATABASE_H
#define WCDATABASE_H

/* Defines */
#define DATA_HEROES 0x0B
#define DATA_SKILLS 0x0C
#define DATA_ITEMS 0x0D
#define DATA_MONSTER 0x0E
#define DATA_GROUP 0x0F
#define DATA_TERRAIN 0x10
#define DATA_ATTRIBUTE 0x11
#define DATA_CONDITIONS 0x12
#define DATA_ANIMATION 0x13
#define DATA_TILESETS 0x14
#define DATA_VOCAB 0x15
#define DATA_SETTINGS 0x16
#define DATA_SWITCHES 0x17
#define DATA_VARIABLES 0x18
#define DATA_COMMON_EVENTS 0x19
#define DATA_BATTLESETS 0x20
#define DATA_CLASSES 0x1E
#define DATA_COMMANDS 0x1D

/* Includes */
#include "wctileset.h"

/*
	Loads database
*/
extern void wcdatabase_load();

/*
	Frees database
*/
extern void wcdatabase_free();

/*
	Requests a tileset
*/
extern tileset *wcdatabase_get_tileset(int id);

#endif