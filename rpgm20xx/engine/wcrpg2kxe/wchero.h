#ifndef WCHERO_H
#define WCHERO_H

/* Defines */
#define MAX_LEVEL 99
#define MAX_LEVEL_OLD 50
#define MAX_STATS 6
#define MAX_COMMANDS 6
#define MAX_EQUIPS 5

#define STAT_HEALTH 0
#define STAT_MANA 1
#define STAT_ATTACK 2
#define STAT_DEFENSE 3
#define STAT_POWER 4
#define STAT_SPEED 5

/* Includes */
#include "wctable.h"
#include "wcwindow.h"

/* Classes */
class skillentry
{
private:
	int level_learned; /* Level to learn */
	int skill_learned; /* Skill to learn */
public:
	/*
		Create new skill entry
	*/
	skillentry(entry *en);
	~skillentry();
	/*
		Get level to learn at
	*/
	int get_level();
	/*
		Get skill to learn
	*/
	int get_skill();
};

class hero
{
private:
	char *name;
	text *name_text;
	char *charset;
	char *faceset;
	char *title;
	text *title_text;
	int battle_x,battle_y;
	int battleset_id;
	int hero_id;
	int level;
	int max_level;
	int faceframe;
	int baseframe;
	int transparent;
	int health,mana;
	int expa,expb,expc;
	int exp;
	unsigned short stats[MAX_STATS][MAX_LEVEL];
	int stat_mod[MAX_STATS];
	int equips[MAX_EQUIPS];
	int start_equips[MAX_EQUIPS];
	int current_class;
	arraylist skill_list;
	arraylist current_skills;
	int battle_commands[MAX_COMMANDS];
	int animation_id;
	unsigned char *elms;
	int elms_size;
public:
	/*
		Create new hero from entry
	*/
	hero(entry *en);
	~hero();
	/*
		Gets name
	*/
	char *get_name();
	/*
		Gets name text
	*/
	text *get_name_text();
	/*
		Sets name
	*/
	void set_name(char *str);
	/*
		Gets charset
	*/
	char *get_charset();
	/*
		Sets charset
	*/
	void set_charset(char *str,int fr,int tr);
	/*
		Gets faceset
	*/
	char *get_faceset();
	/*
		Sets faceset
	*/
	void set_faceset(char *str,int fr);
	/*
		Gets face number
		0-15
	*/
	int get_faceset_number();
	/*
		Gets charset number
		0-7
	*/
	int get_charset_number();
	/*
		Returns true if hero is transparent
	*/
	int is_transparent();
	/*
		Return hero title
	*/
	char *get_title();
	/*
		Return hero title text
	*/
	text *get_title_text();
	/*
		Set hero title
	*/
	void set_title(char *str);
	/*
		Gets a hero's stat
	*/
	int get_stat(int st);
	/*
		Gets a hero's level
	*/
	int get_level();
	/*
		Sets a hero's level
	*/
	void set_level(int lv);
	/*
		Get hero health
	*/
	int get_health();
	/*
		Get hero mana
	*/
	int get_mana();
	/*
		Sets health
	*/
	void set_health(int amt);
	/*
		Sets mana
	*/
	void set_mana(int amt);
	/*
		Refresh text
	*/
	void refresh();
	/*
		Get EXP
	*/
	int get_exp();
	/*
		Set EXP
	*/
	void set_exp(int ex);
	/*
		Gets EXP for next level
	*/
	int get_exp_for_next();
	/*
		Gets the stat offset specifically
	*/
	int get_stat_offset(int st);
	/*
		Mods a stat
	*/
	void modify_stat(int st,int amt);
	/*
		Gains/Loses EXP
		Returns 1 if character levels up
	*/
	int gain_exp(int amt);
	/*
		All effects are removed, HP and MP recovered
	*/
	void full_restore();
	/*
		Gets an equipment in slot
	*/
	int get_equip(int slot);
	/*
		Equips item in slot (does not take it from inventory)
	*/
	void set_equip(int slot,int id);
	/*
		Gets current class
	*/
	int get_class();
	/*
		Validates equipment
	*/
	void validate_equipment(int id);
	/*
		Equips item given by id
	*/
	void equip_item(int id);
	/*
		Unequips item given by id
	*/
	void unequip_item(int id);
	/*
		Unequips item given by slot
	*/
	void unequip_item_slot(int id);
	/*
		Returns true if the hero is wearing the item id given
	*/
	int has_equip(int id);
	/*
		Sets a stat bonus to a precise value
	*/
	void set_stat(int st,int amt);
	/*
		Returns the number of skills currently held by player
	*/
	int get_skill_count();
	/*
		Gets the ID of a skill currently held by this player
	*/
	int get_skill(int ix);
	/*
		Returns true if the hero knows this skill
	*/
	int has_skill(int id);
	/*
		Adds a skill (if not already has) to the player
	*/
	void add_skill(int id);
	/*
		Removes a skill if the player has it
	*/
	void remove_skill(int id);
	/*
		Adds all skills that are at or below player's level in native list
	*/
	void learn_current_skills();
	/*
		Get battle position
	*/
	int get_battle_pos_x();
	int get_battle_pos_y();
	/*
		Get battler
	*/
	int get_battleset();
	/*
		Get command by index
	*/
	int get_command(int ix);
	/*
		Gets current battle animation
	*/
	int get_battle_animation();
	/*
		Returns element effect of element id given
	*/
	int get_element_effect(int ix);
};

/*
	Load all heroes
*/
extern void wchero_load_all(FILE *f,int len);

/*
	Free all heroes
*/
extern void wchero_free_all();

/*
	Get hero
*/
extern hero *wchero_get(int id);

/*
	Save heroes
*/
extern void wchero_save(FILE *f);

/*
	Load heroes
*/
extern void wchero_load(FILE *f);

/*
	Refresh all heroes
*/
extern void wchero_refresh();

/*
	Validate all hero equipment
*/
extern void wchero_validate_all_equipment();

#endif