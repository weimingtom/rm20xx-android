#ifndef WCBATTLECOMMAND_H
#define WCBATTLECOMMAND_H

/* Includes */
#include "wctable.h"

/* Classes */
class battlecommand
{
private:
	char *name; /* Name of command */
	int type; /* Command archetype */
public:
	/*
		Creates a new battle command from entry
	*/
	battlecommand(entry *en);
	~battlecommand();
	/*
		Gets name of battle command
	*/
	char *get_name();
	/*
		Gets type of battle command
	*/
	int get_type();
};

/*
	Load all battle commands
*/
extern void wcbattlecommand_load_all(FILE *f,int len);

/*
	Free all battle commands
*/
extern void wcbattlecommand_free_all();

/*
	Get battle command
*/
extern battlecommand *wcbattlecommand_get(int id);

#endif