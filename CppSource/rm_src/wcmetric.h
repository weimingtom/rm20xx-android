#ifndef WCMETRIC_H
#define WCMETRIC_H

/* Defines */
#define WCMETRIC_DEFAULT 0

/* Includes */
#include <stdio.h>
#include "wchero.h"
#include "wcmonster.h"

/* Classes */
class battler
{
private:
	hero *hero_link; /* Reference links for hero */
	monster *monster_link; /* Reference links for monster */
	int hp,mp; /* Health and energy */
	float tb; /* Time bar */
	int x,y; /* Location */
	int index; /* Index */
public:
	/*
		Creates a new battler out of a hero
	*/
	battler(hero *hr,int ix);
	/*
		Creates a new battler out of a monster
	*/
	battler(monster *mn,int px,int py,int ix);
	~battler();
	/*
		Get name
	*/
	char *get_name();
	/*
		Returns true if the battler is a playable character
	*/
	int is_hero();
	/*
		Returns the hero
	*/
	hero *get_hero();
	/*
		Returns the monster
	*/
	monster *get_monster();
	/*
		Sets time bar
	*/
	void set_time_bar(float n);
	/*
		Gets time bar
	*/
	float get_time_bar();
	/*
		Gets the character's speed
	*/
	int get_speed();
	/*
		Gets the character's remaining health
	*/
	int get_health();
	/*
		Gets the character's remaining mana
	*/
	int get_mana();
	/*
		Gets battler initial position
	*/
	int get_pos_x();
	int get_pos_y();
	/*
		Gets battler index in field
	*/
	int get_index();
	/*
		Gets battler attack and defense
	*/
	int get_attack();
	int get_defense();
	/*
		Hurts this battler
	*/
	void damage(int amt);
	/*
		Returns true when the battler is dead
	*/
	int is_dead();
	/*
		Returns the effect of the element given on this battler
	*/
	int get_element_effect(int ix);
};

/* External globals */
extern FILE *stdbattle;

/*
	Initialize metrics
*/
extern void wcmetric_init();

/*
	End metrics
*/
extern void wcmetric_exit();

/*
	Add hero to fight
*/
extern void wcmetric_add_hero(hero *hr);

/*
	Add monster to fight
*/
extern void wcmetric_add_monster(monster *mn,int x,int y);

/*
	Clear field
*/
extern void wcmetric_clear_field();

/*
	Returns the number of participants in the field
*/
extern int wcmetric_field_size();

/*
	Returns a battler
*/
extern battler *wcmetric_get_battler(int ix);

/*
	Returns a battler using a team filter and index
*/
extern battler *wcmetric_get_battler(int hr,int ix);

/*
	Draws party
*/
extern void wcbattle_draw_party();

/*
	Returns the lowest speed of all the battlers on the field
*/
extern int wcmetric_get_lowest_speed();

/*
	Count the enemies remaining
*/
extern int wcmetric_enemy_count();

/*
	Return the next monster id
*/
extern int wcmetric_next_enemy(int ix);

/*
	Return the next alive monster
*/
extern battler *wcmetric_next_enemy(battler *cur);

/*
	Returns true if all playable characters are dead
*/
extern int wcmetric_game_over();

/*
	Returns an attack
*/
extern int wcmetric_attack(battler *a,battler *b);

/*
	Returns a roll to hit with an attack
*/
extern int wcmetric_hit_rate(battler *a,battler *b);

/*
	Returns whether or not a roll on this integral percentage is successful
*/
extern int wcmetric_roll(int p);

#endif