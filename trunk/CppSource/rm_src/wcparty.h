#ifndef WCPARTY_H
#define WCPARTY_H

/* Defines */
#define MAX_PARTY_SIZE 128
#define WCPARTY_NOBODY 0

/* Includes */
#include <stdio.h>
#include "wchero.h"
#include "wcdraw.h"

/*
	Adds a party member given an id
*/
extern void wcparty_add(int id);

/*
	Removes a party member given an id
*/
extern void wcparty_remove(int id);

/*
	Initialize party
*/
extern void wcparty_init();

/*
	Clean party
*/
extern void wcparty_exit();

/*
	Gets the party leader
*/
extern hero *wcparty_leader();

/*
	Gets the party leader's charset
*/
extern char *wcparty_leader_charset();

/*
	Gets the party size
*/
extern int wcparty_size();

/*
	Gets a party face
*/
extern texture *wcparty_get_face(int i);

/*
	Gets a party member
*/
extern int wcparty_get(int i);

/*
	Returns true if the hero is in this party already
*/
extern int wcparty_has(int id);

/*
	Refreshes changes in graphics to the party
*/
extern void wcparty_refresh();

/*
	Saves party to file
*/
extern void wcparty_save(FILE *f);

/*
	Loads party from file
*/
extern void wcparty_load(FILE *f);

#endif