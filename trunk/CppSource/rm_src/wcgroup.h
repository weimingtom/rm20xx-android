#ifndef WCGROUP_H
#define WCGROUP_H

/* Includes */
#include <stdio.h>
#include "wcarray.h"
#include "wctable.h"

/* Types */
typedef struct
{
	int id,x,y,hidden;
	float flash_dist,flash_a;
	int red,green,blue;
	int dead;
	float fade;
}monsterentry;

/* Class */
class monstergroup
{
private:
	char *name; /* Name of enemy */
	arraylist monster_list; /* Positions of all enemies */
public:
	/*
		Creates a new group from entry
	*/
	monstergroup(entry *en);
	~monstergroup();
	/*
		Get monster entry
	*/
	monsterentry *get_entry(int ix);
	/*
		Adds all the monsters inside this group to the field
	*/
	void add_to_field();
	/*
		Counts the number of enemies on group
	*/
	int get_count();
	/*
		Returns name of group
	*/
	char *get_name();
	/*
		Draws all the monsters in the group on screen
	*/
	void draw();
	/*
		Flash a monster
	*/
	void flash(int ix,float a,int r,int g,int b);
	/*
		Defeat a monster (makes the monster appear as defeated regardless of actual status)
	*/
	void defeat(int ix);
	/*
		Handles monster groups
	*/
	void handle();
};

/*
	Load all groups
*/
extern void wcgroup_load_all(FILE *f,int len);

/*
	Free all groups
*/
extern void wcgroup_free_all();

/*
	Return
*/
extern monstergroup *wcgroup_get(int id);

#endif