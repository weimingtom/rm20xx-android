#ifndef WCMAPTREE_H
#define WCMAPTREE_H

/* Defines */
#define NODE_ROOT 0
#define NODE_MAP 1
#define NODE_AREA 2
#define NODE_INHERIT 0
#define NODE_UNCHANGED 1
#define NODE_SPECIFY 2

#define START_HERO 0
#define START_SKIFF 1
#define START_SHIP 2
#define START_AIRSHIP 3

/* Includes */
#include <stdio.h>
#include "wcsound.h"

/* Classes */
class treenode
{
private:
	music *mus;
	char *name;
	char *battle_background_file;
	int id;
	int parent;
	int depth;
	int type;
	int has_children;	
	int music_mode;
	int battle_background;	
	int teleport_option;
	int escape_option;
	int save_option;
	int encounter_steps;
	/*
		Apply a field in the tree entry
	*/
	int apply(FILE *f,int code,int len);
public:
	/*
		Creates a new treenode from file stream.
	*/
	treenode(FILE *f);
	~treenode();
	/*
		Am I a map?
	*/
	int is_map();
	/*
		Get BGM
	*/
	music *get_music();
	/*
		Get ID
	*/
	int get_id();
};

/*
	Loads the map tree
*/
extern void wcmaptree_load();

/*
	Frees the map tree
*/
extern void wcmaptree_free();

/*
	Gets a node
*/
extern treenode *wcmaptree_get(int id);

/*
	Gets the size of map tree
*/
extern int wcmaptree_size();

/*
	Gets starting info
*/
extern int wcmaptree_get_start_id(int kind);
extern int wcmaptree_get_start_x(int kind);
extern int wcmaptree_get_start_y(int kind);

#endif