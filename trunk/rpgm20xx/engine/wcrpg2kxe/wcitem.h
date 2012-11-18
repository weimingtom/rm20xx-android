#ifndef WCITEM_H
#define WCITEM_H

/* Includes */
#include <stdio.h>
#include "wctable.h"
#include "wcarray.h"
#include "wcmenu.h"
#include "wchero.h"

/* Defines */
#define WCITEM_COMMON 0
#define WCITEM_WEAPON 1
#define WCITEM_SHIELD 2
#define WCITEM_ARMOR 3
#define WCITEM_HELMET 4
#define WCITEM_ACC 5
#define WCITEM_MEDICINE 6
#define WCITEM_SKILL 7
#define WCITEM_SEED 8
#define WCITEM_SCROLL 9
#define WCITEM_SWITCH 10

/* Classes */
class item
{
private:
	int id;
	/* Common item */
	char *name; /* Name of item */
	int type; /* Type of item */
	int price; /* Price of item */
	char *desc; /* Description for item */
	int held; /* Amount held */
	int times; /* Times can be used */
	/* Switch item */
	int switch_in_field; /* Can be switched in field */
	int switch_id; /* ID of switch to start */
	/* Four stats */
	int attack_power;
	int defense_power;
	int special_power;
	int speed_power;
	/* Usable */
	int hero_use_num;
	unsigned char *hero_use;
	int class_use_num;
	unsigned char *class_use;
	/* Effects and attributes */
	int att_use_num;
	unsigned char *att_use;
	int eff_use_num;
	unsigned char *eff_use;
	int eff_power;
	int eff_dir;
	/* Weapon attributes */
	int animation_id;
	int accuracy;
public:
	/*
		Create new item from entry
	*/
	item(entry *en);
	~item();
	/*
		Get name
	*/
	char *get_name();
	/*
		Get type
	*/
	int get_type();
	/*
		Get price
	*/
	int get_price();
	/*
		Get desc
	*/
	char *get_description();
	/*
		Get amount held by player
	*/
	int get_held();
	/*
		Get amount equipped by player
	*/
	int get_equipped();
	/*
		Set amount held by player
	*/
	void set_held(int h);
	/*
		Can this item be used in the "field"
	*/
	int usable_in_field();
	/*
		Use this item on the given hero.
		If the item effects the party, this parameter doesn't matter
		Does not effect anyone if 0 is given
	*/
	void use(hero *hr);
	/*
		Gets the number of times a single item can be used
	*/
	int get_times();
	/*
		Returns true if the hero ID can use this item
	*/
	int can_use(int id);
	/*
		Get attack power
	*/
	int get_attack_power();
	/*
		Get defense power
	*/
	int get_defense_power();
	/*
		Get special power
	*/
	int get_special_power();
	/*
		Get speed power
	*/
	int get_speed_power();
	/*
		Get the attribute by id,
		id is the id of attribute to check, and true is returned if this item expressed the attribute
	*/
	int get_attribute(int id);
	/*
		Returns the number of defined attributes
	*/
	int get_attribute_count();
	/*
		Get item animation
	*/
	int get_animation();
	/*
		Get basic hit rate
	*/
	int get_accuracy();
};

/*
	Load all items
*/
extern void wcitem_load_all(FILE *f,int len);

/*
	Free all items
*/
extern void wcitem_free_all();

/*
	Get particular item
*/
extern item *wcitem_get(int id);

/*
	Save items
*/
extern void wcitem_save(FILE *f);

/*
	Load items
*/
extern void wcitem_load(FILE *f);

/*
	Add items
*/
extern void wcitem_add(int i,int n);

/*
	Remove items
*/
extern void wcitem_remove(int i,int n);

/*
	Returns a filtered view of items
*/
extern arraylist wcitem_filter(int (*fil)(void*));

/*
	Enables/disables items that can be used in field
*/
extern void wcitem_enable_field_usable_items(arraylist lso,menu *m);

/*
	Enables all items
*/
extern void wcitem_enable_all(arraylist lso,menu *m);

/*
	Returns true if the item id is valid
*/
extern int wcitem_valid_id(int id);

#endif