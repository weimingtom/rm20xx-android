#ifndef WCEVENT_H
#define WCEVENT_H

/* Defines */
#define FACE_UP 0
#define FACE_RIGHT 1
#define FACE_DOWN 2
#define FACE_LEFT 3

#define EVENT_IDLE 0
#define EVENT_MOVING 1

#define JUMP_PREPARE 1
#define JUMP_EXECUTE 2

#define SPEED_STEPS 6
#define SPEED_NORMAL 8.0f
#define SPEED_EIGTH (SPEED_NORMAL/8.0f)
#define SPEED_QUARTER (SPEED_NORMAL/4.0f)
#define SPEED_HALF (SPEED_NORMAL/2.0f)
#define SPEED_2X (SPEED_NORMAL*2.0f)
#define SPEED_4X (SPEED_NORMAL*4.0f)

#define EVENT_ME 10005
#define EVENT_PLAYER 10001

/* Includes */
#include <stdio.h>
#include "wcpage.h"
#include "wcdraw.h"
#include "wclog.h"
#include "wcarray.h"
#include "wcmap.h"
#include "wccommand.h"
#include "wcrun.h"
#include "wcmove.h"
#include "wcmem.h"

/* Types */
typedef struct
{
	char *name;
	texture *tex;
}charset;

/* Classes */
class rpgevent
{
private:
	rpgstate *state;
	movelist *active_move_list;
	arraylist page_list;
	char *name;
	int id;
	int pos_x,pos_y;
	float draw_x,draw_y;
	int screen_x,screen_y;
	float dist_x,dist_y;
	int speedadd;
	int program_size;
	int page_count;
	int rmag;
	int direction;
	int foot;
	int footbase;
	int mode;
	int lock_facing;
	int locking;
	int last_page;
	int disembody;
	int erased;
	int phasing;
	int transparent;
	int nofoot;
	int fade;
	int initdir;
	int flash_red,flash_green,flash_blue;
	int terrain_standing;
	float flash_a,flash_speed,flash_strength;
	tile t;
	texture *tex;
	var *myvars;
	int shader_id;
	int jumping,jump_faced;
	int jump_from_x,jump_from_y,jump_to_x,jump_to_y;
	float jump_dist_x,jump_dist_y;
	float jump_draw_x,jump_draw_y;
	float jump_alpha;
	float jump_vel_y;
	/*
		Gets active page
	*/
	void *get_page();
	/*
		Correct the speed
	*/
	void correct_speed();
public:
	/*
		Creates a new event from file
	*/
	rpgevent(FILE *f);
	/*
		Creates a new blank event
	*/
	rpgevent();
	~rpgevent();
	/*
		Report this event to log
	*/
	void report(elog l);
	/*
		Draws this event at x and y,
		the event will still appear an offset draw_x and draw_y from this location.
	*/
	void draw(int x,int y);
	/*
		Draws this event using a tileset
	*/
	void draw(int x,int y,texture *tiletex);
	/*
		Places an event at x and y
	*/
	void place(int tx,int ty);
	/*
		Changes event graphics
	*/
	void change_graphics(texture *ntex,int n);
	/*
		Sets the camera to focus on this event
	*/
	void focus();
	/*
		Makes this event face a direction
	*/
	void face(int dir);
	/*
		Moves this event
		Returns true if could move
	*/
	int move(int xdir,int ydir);
	/*
		Handle
	*/
	void handle();
	/*
		Get event state
	*/
	int get_state();
	/*
		Disassemble the event's commands
	*/
	void disassemble(char *filename);
	/*
		Get name
	*/
	char *get_name();
	/*
		Get layer
	*/
	int get_layer();
	/*
		Returns true if this event is solid and resides at the tile location given
	*/
	int hit(int x,int y);
	/*
		Get animation type
	*/
	int get_animation();
	/*
		Returns true if auto walking animation
	*/
	int get_auto_walk();
	/*
		Returns true if auto spinning animation
	*/
	int get_auto_spin();
	/*
		Returns true if character has locked graphic
	*/
	int get_graphic_fix();
	/*
		Get tile pos
	*/
	int get_pos_x();
	int get_pos_y();
	/*
		Get screen pos
	*/
	int get_screen_x();
	int get_screen_y();
	/*
		Gets command at counter I on page ID
	*/
	command *get_command(int counter,int pagenum);
	/*
		Gets page length
	*/
	int get_page_length(int pagenum);
	/*
		Gets direction facing
	*/
	int get_direction();
	/*
		Handles an examination on this event
	*/
	int examine(int x,int y);
	/*
		Handles a step-on event
	*/
	int step_on(int x,int y);
	/*
		Get ID no
	*/
	int get_id();
	/*
		Get program state
	*/
	rpgstate *get_program_state();
	/*
		Get movement speed
	*/
	int get_speed();
	/*
		Get current route
	*/
	movelist *get_route();
	/*
		Turns this event to the right
	*/
	void turn_right();
	/*
		Turns this event to the left
	*/
	void turn_left();
	/*
		About face!
	*/
	void about_face();
	/*
		Makes this event face the player
	*/
	void face_player();
	/*
		Returns true if this event is fixed in direction
	*/
	int get_direction_fix();
	/*
		Gets current active page id
	*/
	int get_page_id();
	/*
		Updates look to match current page
	*/
	void update_graphics();
	/*
		Sepeates this event such that its commands cant effect the next maps
	*/
	void sever();
	/*
		Returns true if event is severed
	*/
	int is_severed();
	/*
		Disables this event
	*/
	void erase();
	/*
		Changes the move list which this event follows
	*/
	void set_move_list(movelist *mls);
	/*
		Enables/Disables phasing
	*/
	void set_phasing(int en);
	/*
		Enables/Disables transparent
	*/
	void set_transparent(int en);
	/*
		Gets tile
	*/
	tile get_tile();
	/*
		Is a tile?
	*/
	int is_tile();
	/*
		Gets the MAP ID of this event
	*/
	int get_map_id();
	/*
		Flashes this event some color
		Flash red, green, blue, power, and time
	*/
	void flash(int flr,int flg,int flb,float flp,int flt);
	/*
		Locks the facing direction of this event
	*/
	void set_lock_facing(int en);
	/*
		Sets a self var
	*/
	void set_var(int i,var v);
	/*
		Gets a self var
	*/
	var get_var(int i);
	/*
		Gets start foot
	*/
	int get_start_foot();
	/*
		Get terrain
	*/
	int get_terrain();
	/*
		Event is currently walking
	*/
	int walking();
	/*
		Update event terrain
	*/
	void update_terrain();
	/*
		Sets shader for THIS event
	*/
	void set_shader(int id);
	/*
		Sets fading transparency
	*/
	void set_fade(int fd);
	/*
		Gets fading transparency
	*/
	int get_fade();
	/*
		Locks animation
	*/
	void lock_feet(int en);
	/*
		Speed up!
	*/
	void speed_up();
	/*
		Speed down!
	*/
	void speed_down();
	/*
		Save my speed as player's speed
	*/
	void memorize_speed();
	/*
		Recall player's speed and set as my speed
	*/
	void recall_speed();
	/*
		Prepare to jump
	*/
	void start_jump();
	/*
		Execute jump, but only works and returns a non-zero number on success
	*/
	int end_jump();
	/*
		Saves event state
		Does not include self variables
	*/
	void save_state(FILE *f);
	/*
		Loads event state
	*/
	void load_state(FILE *f);
	/*
		Update screen relative positions
	*/
	void update_screen();
};

/*
	Prepares the system for event handling
*/
extern void wcevent_start();

/*
	Ends the event handler
*/
extern void wcevent_end();

/*
	Either loads or points to a loaded charset
	This will get you a loaded texture with the charset requested
*/
extern texture *wcevent_pool_charset(char *name);

/*
	Clears the charset cache
*/
extern void wcevent_clear_charset();

/*
	Handles animation of events for one frame
*/
extern void wcevent_animate();

#endif