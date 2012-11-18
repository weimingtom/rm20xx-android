#ifndef WCBATTLE_H
#define WCBATTLE_H

/* Includes */
#include "wcgroup.h"
#include "wcmetric.h"
#include "wchero.h"
#include "wcrpg.h"
#include "wcmenu.h"
#include "wcinput.h"
#include "wcskill.h"
#include "wcitem.h"
#include "wcbattleset.h"
#include "wcanimation.h"
#include "wcpopnum.h"
#include "wcsystem.h"

/* Defines */
#define WCBATTLE_ATB_REMIX 0
#define WCBATTLE_COMMAND_WIDTH 80
#define WCBATTLE_TARGET_WIDTH 128
#define WCBATTLE_COMMAND_VISIBLE 4

#define WCBATTLE_ATTACK_ARCHTYPE 0
#define WCBATTLE_SKILL_ARCHTYPE 1
#define WCBATTLE_SKILL_SUBSET_ARCHTYPE 2
#define WCBATTLE_DEFENSE_ARCHTYPE 3
#define WCBATTLE_ITEM_ARCHTYPE 4
#define WCBATTLE_RUN_ARCHTYPE 5
#define WCBATTLE_SPECIAL_ARCHTYPE 6

#define WCBATTLE_ONGOING 0
#define WCBATTLE_VICTORY 1
#define WCBATTLE_ESCAPE 2
#define WCBATTLE_DEFEAT 3

/* Types */
typedef struct
{
	hero *hero_link;
	battler *battler_link;
	float hp_roll,mp_roll; /* These are 'rolling' display numbers that can spin around */
	int sx,sy;
	float x,y;
	int pose;
	int frame;
	int mirror;
	int weapon;
	int state;
	int strike;
	float anim_dist;
	int command; /* These three are needed to complete actions */
	battler *target;
	skill *use_skill;
	item *use_item;
	float flash_dist,flash_a; /* These are the flashing distance, alpha, and color */
	int red,green,blue;
	float reel; /* Reeling back distance. If 1.0f, character starts reeling */
	float fallen; /* Fallen distance, this should be set when the character has died from a blow */
	int downed; /* This is set to 1 if the hero is displayed as fully down on the ground */
}heropos;

/*
	Starts the battle system upon the monster group given
*/
extern void wcbattle_start(int mid,int tid);

/*
	Returns true if the battle system is active
*/
extern int wcbattle_active();

/*
	Set battle system
*/
extern void wcbattle_set_system(void (*initf)(),void (*exitf)(),void (*drawf)(int),void (*mainf)(),void (*hherof)(heropos*));

/*
	Set battle system by ID code
*/
extern void wcbattle_set_system(int id);

/*
	Get battle terrain
*/
extern int wcbattle_get_terrain();

/*
	Sends in the first N party members, or the entire party if the party is small enough
*/
extern void wcbattle_enter_party(int n);

/*
	Gets the current monster group
*/
extern monstergroup *wcbattle_get_group();

/*
	Draw party
*/
extern void wcbattle_draw_party();

/*
	Get a hero position
*/
extern heropos *wcbattle_get_hero_pos(int ix);

/*
	Get number of hero positions
*/
extern int wcbattle_get_hero_pos_count();

/*
	Make a menu based on hero commands
	The second parameter is a pointer to integer array acceping command types
*/
extern menu *wcbattle_create_command_menu(hero *hr,int *tps);

/*
	Rolls the health and mana numbers for effect
*/
extern void wcbattle_roll_numbers(heropos *hp);

/*
	Shows battle messages
*/
extern void wcbattle_start_message();

/*
	Shows victory messages
*/
extern void wcbattle_victory_message();

/*
	Shows defeat message
*/
extern void wcbattle_defeat_message();

/*
	Create target menu,
	if hr is nonzero, the menu targets heroes
*/
extern menu *wcbattle_create_target_menu(int hr);

/*
	End battle
*/
extern void wcbattle_end();

/*
	Sets the results of battle
*/
extern void wcbattle_set_result(int r);

/*
	Gets a hero position from hero pointer
*/
extern heropos *wcbattle_get_hero_pos_from_hero(hero *hr);

/*
	Makes this hero reel back as though hit in battle (without actually hurting them)
*/
extern void wcbattle_reel_hero(hero *hr);

/*
	Handles all the standard battle animation overrides
*/
extern void wcbattle_handle_standard_hero_pos_animations(heropos *hp);

/*
	Nucleus
*/
extern void wcbattle_system_init();
extern void wcbattle_system_exit();
extern void wcbattle_system_main();
extern void wcbattle_system_draw(int menulayer);

#endif