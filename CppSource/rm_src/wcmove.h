#ifndef WCMOVE_H
#define WCMOVE_H

/* Defines */
#define MOVE_UP 0
#define MOVE_RIGHT 1
#define MOVE_DOWN 2
#define MOVE_LEFT 3

#define MOVE_UP_RIGHT 4
#define MOVE_DOWN_RIGHT 5
#define MOVE_DOWN_LEFT 6
#define MOVE_UP_LEFT 7

#define MOVE_RANDOM 8
#define MOVE_AT_HERO 9
#define MOVE_AWAY_HERO 10
#define MOVE_FORWARD 11

#define MOVE_FACE_UP 12
#define MOVE_FACE_RIGHT 13
#define MOVE_FACE_DOWN 14
#define MOVE_FACE_LEFT 15
#define MOVE_TURN_RIGHT 16
#define MOVE_TURN_LEFT 17
#define MOVE_ABOUT_FACE 18
#define MOVE_TURN_RANDOM 19
#define MOVE_FACE_RANDOM 20
#define MOVE_FACE_HERO 21
#define MOVE_FACE_AWAY_HERO 22

#define MOVE_WAIT 23

#define MOVE_START_JUMP 24
#define MOVE_END_JUMP 25

#define MOVE_LOCK_FACING 26
#define MOVE_UNLOCK_FACING 27

#define MOVE_SPEED_UP 28
#define MOVE_SPEED_DOWN 29

#define MOVE_FREQ_UP 30
#define MOVE_FREQ_DOWN 31

#define MOVE_SWITCH_ON 32
#define MOVE_SWITCH_OFF 33
#define MOVE_CHANGE_GRAPHIC 34
#define MOVE_SOUND 35

#define MOVE_PHASING_ON 36
#define MOVE_PHASING_OFF 37

#define MOVE_STOP_ANIM 38
#define MOVE_START_ANIM 39

#define MOVE_TRANS_UP 40
#define MOVE_TRANS_DOWN 41

/* Includes */
#include <stdio.h>
#include "wcarray.h"
#include "wcsound.h"

/* Classes */
class move
{
private:
	int code; /* Move command to execute */
	char *str; /* String parameter, if any */
	sound *snd; /* Sound parameter, if any */
	int num; /* Extra parameter */
	/*
		Move forward
	*/
	int move_forward(void *who);
	/*
		Move random
	*/
	int move_random(void *who);
public:
	/*
		Reads a move from input file
	*/
	move(FILE *f);
	/*
		Reads a move from a string of numbers
	*/
	move(int *arg);
	/*
		Creates a blank move
	*/
	move();
	~move();
	/*
		Gets the name of this movement command
	*/
	char *get_name();
	/*
		Runs this move command on the event given
	*/
	int run(void *v);
	/*
		Get code
	*/
	int get_code();
	/*
		Is a parameter code
	*/
	int parametric_length();
	/*
		Saves the move to game save
	*/
	void save_state();
	/*
		Loads the move from game save
	*/
	void load_state();
};

class movelist
{
private:
	arraylist ls; /* The list of moves */
	int repeat; /* Cycle this over and over */
	int ignore; /* Ignoe illegal moves */
	int step; /* Current move to execute */
	int freq; /* Set frequency */
	int freqadd; /* Addition frequency */
	float clk; /* Movement clock */
	float wclk; /* Wait clock */
	int pointer; /* Pointer to event to effect with this list */
	int deflist; /* Default listing */
	int defstate; /* Default listing state (for left/right and up/down patterns) */
public:
	/*
		Reads a move list from input file
	*/
	movelist(FILE *f);
	/*
		Reads a move from a string of command arguments,
		this is typically out of a move event command
	*/
	movelist(int arrcnt,int *arr);
	/*
		Creates a move list based on a built-in moving route
	*/
	movelist(int defroute);
	/*
		Creates a blank list
	*/
	movelist();
	~movelist();
	/*
		Length
	*/
	int length();
	/*
		Dissasembles this list to a file
	*/
	void dissasemble(FILE *f);
	/*
		Runs THIS move list onto THAT event given,
		the state of the list is stored in THIS list
	*/
	void run(void *v);
	/*
		Sets the moving frequency of this list
	*/
	void set_freq(int fq);
	/*
		Move list is at its end
	*/
	int is_finished();
	/*
		Resets state of move list
	*/
	void reset();
	/*
		Gets the target of this list
	*/
	int get_target();
	/*
		Temporarily increases move frequency
	*/
	void increase_freq();
	/*
		Temporarily decrease move frequency
	*/
	void decrease_freq();
	/*
		Makes the next move run instantly
	*/
	void advance_clock();
	/*
		Triggers the waiting clock
	*/
	void wait_clock();
	/*
		Saves state
	*/
	void save_state();
	/*
		Loads state
	*/
	void load_state();
};

/*
	Adds to the number of events moving
*/
extern void wcmove_add_active();

/*
	Reduces from the number of events moving
*/
extern void wcmove_remove_active();

/*
	Returns true if there are no events running commanded moves
*/
extern int wcmove_finished();

/*
	Load moves
*/
extern void wcmove_init();

/*
	Clean up moves
*/
extern void wcmove_exit();

/*
	Reset the active move count
*/
extern void wcmove_reset_active();

/*
	Get active moves
*/
extern int wcmove_get_active();

/*
	Sets active moves
*/
extern void wcmove_set_active(int act);

#endif