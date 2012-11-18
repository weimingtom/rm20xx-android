#ifndef WCSKILL_H
#define WCSKILL_H

/* Defines */
#define WCSKILL_NORMAL 0
#define WCSKILL_TELEPORT 1
#define WCSKILL_ESCAPE 2
#define WCSKILL_SWITCH 3
#define WCSKILL_EX_BASE 4

#define WCSKILL_TARGET_ENEMY 0
#define WCSKILL_TARGET_ALL_ENEMIES 1
#define WCSKILL_TARGET_SELF 2
#define WCSKILL_TARGET_ALLY 3
#define WCSKILL_TARGET_ALL_ALLIES 4

/* Includes */
#include <stdio.h>
#include "wctable.h"
#include "wcmem.h"

/* Classes */
class skill
{
private:
	char *name; /* Name of skill */
	char *desc; /* Name of description */
	int mp_need; /* Amount of MP needed to use this */
	int mp_percent; /* Amount of MP needed as percent */
	int mp_is_percent; /* Amount of MP needed will come from percent instead? */
	int type; /* Type of skill */
	int target; /* Target of skill */
	int effect_rating; /* Basic strength of skill */
	int success; /* Basic rate of skill working */
	int hp_effect,mp_effect,attack_effect,defense_effect,intellect_effect,speed_effect; /* Basic stat effects */
	int absorb,ignore_defense; /* Other effects */
	int physical_influence; /* 0~10 */
	int ether_influence; /* 0~10 */
	int variance; /* 0~10 */
	int reverse_treatment; /* Cures condition on enemy when set and causes condition on ally if set */
	int attribute_defense; /* Causes a change in attribute defense (up or down) */
	int animation_id; /* Animation used on target(s) */
public:
	/*
		Creates a skill from entry
	*/
	skill(entry *en);
	~skill();
	/*
		Get a name
	*/
	char *get_name();
	/*
		Get a description
	*/
	char *get_description();
	/*
		Returns power consumption
	*/
	int get_mp_need();
	/*
		Returns type
	*/
	int get_type();
	/*
		Accesses all top-level data at once
	*/
	void access_data(var *base);
};

/*
	Load all skills
*/
extern void wcskill_load_all(FILE *f,int len);

/*
	Free all skills
*/
extern void wcskill_free_all();

/*
	Get a skill
*/
extern skill *wcskill_get(int id);

#endif