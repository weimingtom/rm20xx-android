#ifndef WCMONSTER_H
#define WCMONSTER_H

/* Defines */
#define WCMONSTER_DEFAULT_BATTLESET -1
#define WCMONSTER_NO_BATTLESET 0

/* Includes */
#include <stdio.h>
#include "wcmem.h"
#include "wctable.h"
#include "wcdraw.h"

/* Types */
typedef struct
{
	char *name;
	texture *tex;
}monsterpic;

/* Classes */
class monster
{
private:
	char *name; /* Name of monster */
	char *pic; /* Picture file */
	int battleset_id; /* ID of battleset (cache only) */
	int hp,mp; /* Health and energy */
	int attack,defense,power,speed; /* Other stats */
	int elms_size; /* Elemental reactions */
	int exp_win,money_win; /* EXP and money gotten from defeating */
	int item_id,item_chance; /* Item ID and chance of dropping */
	int crit_chance,crit_has; /* Has crit and crit chance */
	int often_miss,flying,trans; /* Flags */
	unsigned char *elms;
public:
	/*
		Creates monsters from entries
	*/
	monster(entry *en);
	~monster();
	/*
		Draws this monster at x and y
	*/
	void draw(int x,int y);
	/*
		Draws this monster at x and y with the given flashes
	*/
	void draw(int x,int y,float a,int r,int g,int b,float alpha);
	/*
		Gets battleset id
	*/
	int get_battleset_id();
	/*
		Sets battleset id
	*/
	void set_battleset_id(int id);
	/*
		Get name
	*/
	char *get_name();
	/*
		Get max health
	*/
	int get_max_hp();
	/*
		Get max mana
	*/
	int get_max_mp();
	/*
		Get monster's width
	*/
	int get_width();
	/*
		Get monster's height
	*/
	int get_height();
	/*
		Get other stats
	*/
	int get_attack();
	int get_defense();
	int get_power();
	int get_speed();
	/*
		Gets the effect of element referenced by ix on this monster
	*/
	int get_element_effect(int ix);
	/*
		Copies data to variables
	*/
	void access_data(var *base);
};

/*
	Loads all monster data
*/
extern void wcmonster_load_all(FILE *f,int len);

/*
	Frees all monster data
*/
extern void wcmonster_free_all();

/*
	Returns a monster by ID
*/
extern monster *wcmonster_get(int id);

/*
	Releases memory used by unused monster pictures
*/
extern void wcmonster_reset_picture_pool();

/*
	Returns picture belonging to a monster, loading it if not loaded already
*/
extern texture *wcmonster_pool_picture(char *name);

/*
	Draws monster given by id at x and y, may draw battleset instead
*/
extern void wcmonster_draw(int id,int x,int y,int p);

/*
	Draws a monster given by id at x and y with flash and might draw battlese instead
*/
extern void wcmonster_draw(int id,int x,int y,int p,float a,int r,int g,int b,float alpha);

/*
	Enables/Disables auto assign battle charset for monster if both have same editor name
*/
extern void wcmonster_enable_auto_battle_charset(int en);

/*
	Returns true if battle charsets will be automatically assigned to monsters with same editor names
*/
extern int wcmonster_get_auto_battle_charset();

#endif