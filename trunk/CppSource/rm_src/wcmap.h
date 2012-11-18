#ifndef WCMAP_H
#define WCMAP_H

/* Defines */
#define WCMAP_TILESET 0x01
#define WCMAP_WIDTH 0x02
#define WCMAP_HEIGHT 0x03
#define WCMAP_SCROLL 0x0B
#define WCMAP_HAS_BACKGROUND 0x1F
#define WCMAP_BACKGROUND 0x20
#define WCMAP_VERT 0x22
#define WCMAP_HORZ 0x21
#define WCMAP_VERT_AUTO 0x25
#define WCMAP_HORZ_AUTO 0x23
#define WCMAP_VERT_SPEED 0x26
#define WCMAP_HORZ_SPEED 0x24
#define WCMAP_LOWER 0x47
#define WCMAP_UPPER 0x48
#define WCMAP_EVENT 0x51

#define WCMAP_WRAP_NONE 0
#define WCMAP_WRAP_X 1
#define WCMAP_WRAP_Y 2
#define WCMAP_WRAP_BOTH 3

#define WCMAP_BELOW 0
#define WCMAP_ABOVE 1

/* Includes */
#include "wcarray.h"
#include "wcdraw.h"
#include "wcdatabase.h"
#include "wcevent.h"

/* Classes */
class map
{
private:
	int map_id; /* The maps ID */
	arraylist event_list; /* List of events */
	char *parallax; /* Parallax background */
	char *current_parallax; /* Current background */
	tile *lower; /* Tile lower map */
	tile *upper; /* Tile upper map */
	tileset *tset; /* Tileset */
	texture *tex; /* Tileset texture */
	int width,height; /* Width and height of map in tiles */
	int scroll; /* Scroll type */
	int has_parallax; /* Has a parallax? */
	int h_pan,v_pan; /* Background can pan in h and v dirs? */
	int h_auto,v_auto; /* Automatic scroll? */
	int h_speed,v_speed; /* Background auto-scrolls in h and v dirs? */
	int tset_id; /* Tileset to use */
	int rmag; /* Resolution mag */
	int event_count; /* Number of events */
	texture *ptex; /* Parallax texture */
	float parallax_anim_x,parallax_anim_y; /* Parallax animation offsets */
	void *link_event; /* Event to continue running even when the map explodes */
	/*
		Loads a specific part of map data
	*/
	void gather_data(int code,FILE *f);
	/*
		Read and report entry field for map settings
	*/
	void read_field(char *name,int *val,int len,FILE *f,int numerical);
	/*
		Draws a single tile
	*/
	void draw_tile(tile t,int x,int y,int l);
	/*
		Draws a tile fragment
	*/
	void draw_tile_fragment(int x,int y,int sx,int sy,int a,int b,int c,int d);
	/*
		Draws an auto tile
	*/
	void draw_tile_auto(int x,int y,int sx,int sy,int code);
	/*
		Draws a beach tile
	*/
	void draw_tile_beach(int x,int y,int sx,int sy,int code);
	/*
		Draws an ocean tile
	*/
	void draw_tile_ocean(int x,int y,int sx,int sy,int code,int deep);
	/*
		Loads events
	*/
	void load_events(FILE *f);
public:
	/*
		Loads the map with id given
	*/
	map(int id);
	~map();
	/*
		Draws the map
	*/
	void draw(int x,int y);
	/*
		Draws collision mappings
	*/
	void draw_collision(int x,int y);
	/*
		Draws terrain mappings
	*/
	void draw_terrain(int x,int y);
	/*
		Gets dimension
	*/
	int get_width();
	int get_height();
	/*
		Gets tile dimension
	*/
	int get_tile_width();
	int get_tile_height();
	/*
		Gets possible moving
	*/
	int can_move(int x,int y,int dir,void *me);
	/*
		Gets possible landing
	*/
	int can_land(int x,int y,void *me);
	/*
		Gets event by id
	*/
	void *get_event(int id);
	/*
		Returns number of events
	*/
	int get_event_count();
	/*
		Gets the collision code of some tile
	*/
	int get_collision_code(tile lo,tile hi);
	/*
		Gets the terrain code of the position
	*/
	int get_terrain_code(int x,int y);
	/*
		Gets the lower tile here
	*/
	tile get_lower_tile(int x,int y);
	/*
		Gets the upper tile here
	*/
	tile get_upper_tile(int x,int y);
	/*
		Gets the upper tile here ignoring the entity given for collision
	*/
	tile get_upper_tile(int x,int y,void *me);
	/*
		Returns true if this tile is above
	*/
	int tile_is_above(tile t);
	/*
		Will wrap X?
	*/
	int wrap_x();
	/*
		Will wrap Y?
	*/
	int wrap_y();
	/*
		Wraps the position according to this map
	*/
	void wrap(int *x,int *y);
	/*
		Wraps the screen/map/absolute positon according to this map
	*/
	void wrap_screen(int *x,int *y);
	/*
		Gets this map's ID
	*/
	int get_id();
	/*
		Draws the map's parallax
	*/
	void draw_parallax(int x,int y);
	/*
		Handle auto stuff on map
	*/
	void handle();
	/*
		Sets a link event that can't be destroyed
	*/
	void set_link_event(void *ev);
	/*
		Returns an event that resides at this location
	*/
	int get_event_id(int x,int y);
	/*
		Changes map background
		fname - filename
		vs - vertical scroll
		vsa - auto scroll
		vsp - vertical scroll speed
		hs - horizontal scroll
		hsa - auto scroll
		hsp - horizontal scroll speed
	*/
	void change_parallax(char *fname,int vs,int vsa,int vsp,int hs,int hsa,int hsp);
	/*
		Gets information about the parallax
	*/
	char *get_parallax_name();
	/*
		Copies all six settings (vs,vsa,vsp,hs,hsa,hsp) into a pointer to an integer holding these six integers.
		This strange vector method is for the ease of saving and recalling to game
	*/
	void get_parallax_scroll(int *scrollv);
};

/*
	Handles animation frames for drawn maps
*/
extern void wcmap_animate();

#endif