#ifndef WCANIMATION_H
#define WCANIMATION_H

/* Defines */
#define WCANIMATION_MAX 256

#define WCANIMATION_HEAD 0
#define WCANIMATION_MIDDLE 1
#define WCANIMATION_FEET 2

/* Includes */
#include <stdio.h>
#include "wctable.h"
#include "wcdraw.h"
#include "wcsound.h"
#include "wcevent.h"
#include "wcgroup.h"

/* Types */
typedef struct
{
	char *name;
	texture *tex;
}animset;

/* Classes */
class animframe
{
private:
	arraylist cells;
public:
	/*
		Creates a new animframe from an entry
	*/
	animframe(entry *en);
	~animframe();
	/*
		Draws this frame at x and y
	*/
	void draw(texture *tex,int x,int y);
};
class animsfx
{
private:
	int frame;
	sound *snd;
	int flash_scope;
	int flash_red,flash_green,flash_blue,flash_power;
public:
	/*
		Creates a new anim sound effect
	*/
	animsfx(entry *en);
	~animsfx();
	/*
		Flashes and/or plays the associated sound
	*/
	void play();
	/*
		Flashes and/or plays the associated sound on a target
	*/
	void play(rpgevent *ev);
	/*
		Flashes and/or plays the associated sound on a monster in a group
	*/
	void play(monstergroup *gp,int ix);
	/*
		Get frame this sfx is set to
	*/
	int get_frame();
};
class animation
{
private:
	int id;
	char *source; /* Source of animation */
	int global; /* Cover the screen? */
	int aim; /* Top / Middle / Bottom */
	arraylist frames; /* Frames */
	arraylist sfxs; /* Effects */
public:
	/*
		Creates new animation from table
	*/
	animation(entry *en);
	~animation();
	/*
		Pools and returns texture
	*/
	texture *get_texture();
	/*
		Gets a frame
	*/
	animframe *get_frame(int i);
	/*
		Gets an effect
	*/
	animsfx *get_sfx(int i);
	/*
		Gets number of effects
	*/
	int get_sfx_count();
	/*
		Gets length in frames
	*/
	int get_length();
	/*
		Returns true if global
	*/
	int is_global();
	/*
		Returns aim
	*/
	int get_aim();
};
class animstate
{
private:
	animation *src; /* Source */
	float pos; /* Position in animation */
	int pos_frame; /* Integral position in animation */
	int pos_x,pos_y; /* Position on screen */
	texture *tex; /* Source texture */
	int has_target; /* Is this state on a target? */
	rpgevent *event_target; /* The target that is the event */
	monstergroup *group_target; /* The target that is the group */
	int index_target; /* Index in target */
	int htoff; /* Height offset */
public:
	/*
		Creates new animation instance
	*/
	animstate(animation *an);
	~animstate();
	/*
		Moves this animation
	*/
	void move(int x,int y);
	/*
		Handles this state and returns 0 if end of animation
	*/
	int handle();
	/*
		Draws a specific frame at x and y
	*/
	void draw_frame(int f,int x,int y);
	/*
		Draws this animation state
	*/
	void draw();
	/*
		Sets the target of this battle animation to be that of an event
	*/
	void set_target(rpgevent *et);
	/*
		Sets the target of this battle animation to be that of a monster group and index
	*/
	void set_target(monstergroup *gp,int ix);
	/*
		Sets the height offset
	*/
	void set_height_offset(int h);
};
class animcell
{
private:
	int visible; /* Visible? */
	int frame; /* Cell frame source */
	int pos_x,pos_y; /* Position */
	float scale; /* Size */
	float alpha; /* Fade */
	float r,g,b,s; /* Tone */
public:
	animcell(entry *en);
	~animcell();
	/*
		Draws this cell at x and y
	*/
	void draw(texture *tex,int x,int y);
};

/*
	Loads all animations
*/
extern void wcanimation_load_all(FILE *f,int len);

/*
	Releases all animations
*/
extern void wcanimation_free_all();

/*
	Get animation
*/
extern animation *wcanimation_get(int id);

/*
	Handle animations
*/
extern void wcanimation_handle();

/*
	Draw animations
*/
extern void wcanimation_draw();

/*
	Play an animation
*/
extern int wcanimation_play(int id,int x,int y,int ht,rpgevent *ev);

/*
	Play an animation upon monster
*/
extern void wcanimation_play(int id,int x,int y,int ht,monstergroup *gp,int ix);

/*
	Get length
*/
extern int wcanimation_length(int id);

/*
	Reloads all animation sets
*/
extern void wcanimation_reload_all_animset();

#endif