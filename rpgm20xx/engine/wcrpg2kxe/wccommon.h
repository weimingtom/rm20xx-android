#ifndef WCCOMMON_H
#define WCCOMMON_H

/* Defines */
#define WCCOMMON_CALL 5
#define WCCOMMON_PARALLEL 4
#define WCCOMMON_AUTO 3

/* Requires */
#include <stdio.h>
#include "wcarray.h"
#include "wccommand.h"
#include "wcrun.h"

/* Classes */
class comevent
{
private:
	int id;
	char *name;
	int trigger;
	int has_trigger;
	int trigger_switch;
	arraylist command_list;
	rpgstate *state;
public:
	/*
		Creates a new common event from file
	*/
	comevent(FILE *f);
	~comevent();
	/*
		Get command
	*/
	command *get_command(int i);
	/*
		Get page length
	*/
	int get_length();
	/*
		Handle process
	*/
	void handle();
	/*
		Returns true if this common event is active AND an auto event
	*/
	int is_active_auto();
	/*
		Forces this common event to run by itself
	*/
	void run();
	/*
		Saves the state of the event to file
	*/
	void save_state(FILE *f);
	/*
		Loads the state of the event from file
	*/
	void load_state(FILE *f);
};

/*
	Load all common events
*/
extern void wccommon_load_all(FILE *f,int len);

/*
	Free all common events
*/
extern void wccommon_free_all();

/*
	Gets a command
*/
extern command *wccommon_get(int id,int ix);

/*
	Gets length of a common event
*/
extern int wccommon_length(int id);

/*
	Handle all common events
*/
extern void wccommon_handle();

/*
	Any auto events running?
*/
extern int wccommon_auto_active();

/*
	Forces a common event to run
*/
extern void wccommon_run(int id);

/*
	Saves all common events
*/
extern void wccommon_save(FILE *f);

/*
	Loads all common events
*/
extern void wccommon_load(FILE *f);

#endif