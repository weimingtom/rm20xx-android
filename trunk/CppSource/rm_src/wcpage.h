#ifndef WCPAGE_H
#define WCPAGE_H

/* Defines */
#define START_KEY 0
#define START_IMPACT 1
#define START_TOUCH 2
#define START_AUTO 3
#define START_PARALLEL 4

#define EVENT_BELOW 0
#define EVENT_SOLID 1
#define EVENT_ABOVE 2

#define COND_SWITCH_A 1
#define COND_SWITCH_B 2
#define COND_VARIABLE 4
#define COND_ITEM 8
#define COND_HERO 16
#define COND_TIMER_A 32
#define COND_TIMER_B 64

#define ANIM_NORMAL 0
#define ANIM_ALWAYS 1
#define ANIM_FIX 2
#define ANIM_FIX_AND_ALWAYS 3
#define ANIM_NEVER 4
#define ANIM_SPIN 5

#define COMP_EQUAL 0
#define COMP_GREATER_EQUAL 1
#define COMP_LESS_EQUAL 2
#define COMP_GREATER 3
#define COMP_LESS 4
#define COMP_NOT 5

#define MOVE_STATIONARY 0
#define MOVE_RANDOMLY 1
#define MOVE_UP_DOWN 2
#define MOVE_LEFT_RIGHT 3
#define MOVE_TO_HERO 4
#define MOVE_FROM_HERO 5
#define MOVE_CUSTOM 6

/* Includes */
#include <stdio.h>
#include "wcmap.h"
#include "wclog.h"
#include "wcarray.h"
#include "wccommand.h"
#include "wcmove.h"

/* Classes */
class page
{
private:
	arraylist command_list;
	void *who;
	char *charset;
	int prog_len;
	unsigned char *prog;
	tile t;
	int id;
	int start_foot;
	int transparent;
	int move_type;
	int move_freq;
	int trigger;
	int layer;
	int anim_type;
	int move_speed;
	int cond_flags;
	int switch_a_id;
	int switch_b_id;
	int variable_id;
	int item_id;
	int hero_id;
	int timer_a_val;
	int timer_b_val;
	int variable_comp;
	int variable_test;
	int start_dir;
	movelist *mls;
	/*
		Apply entry
	*/
	int apply(int code,FILE *f);
	/*
		Apply condition
	*/
	int condition(int code,FILE *f);
public:
	/*
		Read a page from a file
	*/
	page(FILE *f);
	~page();
	/*
		Report page settings
	*/
	void report(elog l);
	/*
		Gets the charset for this page
	*/
	char *get_charset();
	/*
		Gets the tile for this page
	*/
	tile get_tile();
	/*
		Disassembles to file
	*/
	void disassemble(char *filename);
	/*
		Return layer
	*/
	int get_layer();
	/*
		Return starting direction
	*/
	int get_direction();
	/*
		Return animation type
	*/
	int get_animation();
	/*
		Gets a command at counter
	*/
	command *get_command(int counter);
	/*
		Get length
	*/
	int get_length();
	/*
		Get trigger
	*/
	int get_trigger();
	/*
		Get speed
	*/
	int get_speed();
	/*
		Get route
	*/
	movelist *get_route();
	/*
		Returns true if the page's conditions are met
	*/
	int is_true();
	/*
		Returns true if the page has a route at all of which the event will follow
	*/
	int has_route();
	/*
		Returns true if this page has transparent character
	*/
	int get_transparent();
	/*
		Handles the built-in routes on this event
	*/
	void handle_route(void *me);
	/*
		Sets the attached event
	*/
	void set_who(void *w);
	/*
		Gets the foot to start on
	*/
	int get_foot();
};

/*
	Returns the name of the comparator code for testing of variables
*/
extern char *wcpage_get_variable_test(int code);

#endif