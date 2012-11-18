#ifndef WCTILESET_H
#define WCTILESET_H

/* Defines */
#define TILESET_NAME 0x01
#define TILESET_SOURCE 0x02
#define TILESET_TERRAIN 0x03
#define TILESET_PASS 0x04
#define TILESET_UPPER_PASS 0x05

#define TILESET_DOWN 1
#define TILESET_LEFT 2
#define TILESET_RIGHT 4
#define TILESET_UP 8
#define TILESET_STAR 16
#define TILESET_SQUARE 32
#define TILESET_COUNTER 64

#define TILESET_MAX 306
#define TILESET_HALF 144

/* Includes */
#include <stdio.h>
#include "wcdraw.h"
#include "wcarray.h"
#include "wclog.h"

/* Types */
typedef unsigned short tile;

/* Class */
class tileset
{
private:
	char *name; /* Name of the tileset */
	char *source; /* Source filename of tileset */
	unsigned short *terr; /* Terrain */
	unsigned char *pass; /* Passing */
	unsigned char *upass; /* Upper passing */
	int id; /* ID */
	int size;
public:
	/*
		Constructs a tileset from base location
	*/
	tileset(FILE *f);
	~tileset();
	/*
		Reports summary to log
	*/
	void report(elog l);
	/*
		Loads a texture and returns it
	*/
	texture *get_texture();
	/*
		Get tileset size
	*/
	int get_size();
	/*
		Get above/below codes
	*/
	int is_above(tile t);
	/*
		Get square code
	*/
	int is_square(int t);
	/*
		Get tile code
		i is NOT an RPG tile, it is the logical tile
	*/
	int get_code(int i,int low);
	/*
		Get collision code
		low is the lower tile and hi is the upper tile
	*/
	int get_collision_code(tile lo,tile hi);
	/*
		Get terrain code
	*/
	int get_terrain_code(tile lo);
};

/*
	Loads all tilesets and returns the list of them
*/
extern arraylist wctileset_load_all(FILE *f,int len);

/*
	Removes all tilesets
*/
extern void wctileset_free_all(arraylist ls);

/*
	Resets subbed tiles
*/
extern void wctileset_reset_sub();

/*
	Sub a tile
*/
extern void wctileset_set_sub(tile from,tile to);

/*
	Returns sub tiles
*/
extern tile wctileset_sub_tile(tile t);

/*
	Resets tile shaders
*/
extern void wctileset_reset_shader();

/*
	Sets a tile shader (on logical tile)
*/
extern void wctileset_set_shader(int lt,int sid);

/*
	Gets shader for tile
*/
extern int wctileset_get_shader(tile t);

#endif