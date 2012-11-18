#ifndef WCRUN_H
#define WCRUN_H

/* Defines */
#define STACK_SIZE 512
#define MAX_LOOP_LENGTH 1024

/* Require */
#include <stdio.h>
#include "wcinput.h"

/* Program state */
class rpgstate
{
private:
	arraylist command_list; /* Disembodied list */
	void *me; /* The event running this state */
	void *them[STACK_SIZE]; /* Pages from other events possibly being executed */
	int current_page[STACK_SIZE]; /* Current page being executed */
	int counter[STACK_SIZE]; /* Current line number being executed */
	int common[STACK_SIZE]; /* Common event being run if running common event */
	int stack; /* Stack pointer for current stack */
	int active; /* Executing? */
	int distance; /* Number of lines executed */
	int playerdelay; /* Wait for hero to stop moving? */
	int nofocus; /* Unfocused event like a parallel event */
	int consecutive; /* Consecutive commands run */
	float waitclk; /* Time to wait */
	int wait_autofire; /* Anti autofire for wait commands */
	float time_sample; /* Time sampler for the clock */
	int time_active; /* Activly sampling the time */
	int input_autofire[WCINPUT_NUM_CODES]; /* Anti autofire for key input commands */
	/*
		Current length
	*/
	int current_length();
	/*
		Get command at current page
	*/
	command *current_command(int i);
public:
	/*
		Creates a new program state with attached event
	*/
	rpgstate(void *who);
	/*
		Fetch current instruction
	*/
	command *fetch();
	/*
		Returns true if this state reached the end of the page
	*/
	int at_end();
	/*
		Advance instruction, returning 0 if end of frame
	*/
	int advance();
	/*
		Returns true if the program is active
	*/
	int is_running();
	/*
		Executes program for one frame
	*/
	void handle();
	/*
		Starts the program
	*/
	void run();
	/*
		Has the next run actually occur when the player finished moving
	*/
	void delay_until_hero_is_finished_moving();
	/*
		Prevents this program state from getting the focus,
		this is used for parallel events
	*/
	void prevent_focus();
	/*
		Sleep for this many seconds
	*/
	void sleep(float t);
	/*
		Returns the location of this label
	*/
	int find_label(int lb);
	/*
		Changes program counter to the label with this value on the current page
	*/
	void jump_to_label(int lb);
	/*
		Finds a choice selection case,
		cur - current program counter
		lev - level to find it on
		op - option selection
	*/
	int find_choice(int cur,int lev,int op);
	/*
		Jumps to a choice selection.
	*/
	void jump_to_choice(int op);
	/*
		Finds the ending from this break command
	*/
	int find_end(int cur,int lev);
	/*
		Jumps to the end of this block
	*/
	void jump_to_end();
	/*
		Finds the else from this position
	*/
	int find_else(int cur,int lev);
	/*
		Jumps to the next else clause
	*/
	void jump_to_else();
	/*
		Finds a while command on this same level
	*/
	int find_while(int cur,int lev);
	/*
		Resets this control back to the matching while
	*/
	void jump_to_while();
	/*
		Finds the end of a while command loop
	*/
	int find_while_end(int cur,int lev);
	/*
		Resets this control back to the matching end while
	*/
	void jump_to_while_end();
	/*
		Creates a stack entry and calls a common event
	*/
	void call_common_event(int id);
	/*
		Creates a stack entry into a map event somewhere else
	*/
	void call_event(int id,int pg);
	/*
		Sets this program state as a common state
	*/
	void set_common_state(int id);
	/*
		Returns 1 if the OK key is being pressed once
	*/
	int check_ok_key();
	/*
		Returns 1 if input keys are being pressed once
	*/
	int check_input_key();
	/*
		Starts measuring the time
	*/
	void start_time();
	/*
		Stops measuring the time
	*/
	float end_time();
	/*
		'return' is a keyword
	*/
	int return_end();
	/*
		Save program state
	*/
	void save_state(FILE *f);
	/*
		Load program state
	*/
	void load_state(FILE *f);
};

/*
	Sets the bridge event
*/
extern void wcrun_set_bridge_event(void *ev);

/*
	Handles the bridge event
*/
extern void wcrun_handle();

/*
	Cleans up the engine
*/
extern void wcrun_end();

#endif