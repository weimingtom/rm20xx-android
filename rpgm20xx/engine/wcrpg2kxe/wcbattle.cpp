/*
	Battle Systems
*/

/* Includes */
#include <malloc.h>
#include "wcbattle.h"
#include "wcbattleatb.h"
#include "wcrpg.h"
#include "wctrans.h"
#include "wcparty.h"
#include "wcbattlecommand.h"
#include "wcvocab.h"
#include "wcmessage.h"
#include "wcnucleus.h"

/* Globals */
char *wcbattle_names[1] = {"ATB Remix"};
int wcbattle_mode = 0;
int wcbattle_terrain = 0;
int wcbattle_visible = 0;
void (*wcbattle_init)();
void (*wcbattle_exit)();
void (*wcbattle_draw)(int);
void (*wcbattle_main)();
void (*wcbattle_handle_hero)(heropos*);
monstergroup *wcbattle_current_group = 0;
arraylist wcbattle_pos = 0;
int wcbattle_result = WCBATTLE_ONGOING;

/* Handle standard animations */
void wcbattle_handle_standard_hero_pos_animations(heropos *hp)
{
	float ts;
	/* Get time step */
	ts = wcrpg_get_time_step();
	/* Animate reeling */
	if(hp->reel > 0.0f)
	{
		/* HUGH! */
		hp->pose = 5;
		hp->weapon = 0;
		hp->mirror = 0;
		hp->frame = 1;
		if(hp->reel <= 0.75f)
			hp->frame = 2;
		if(hp->reel <= 0.50f)
			hp->frame = 3;
		/* Reduce reel */
		hp->reel -= ts*0.15f;
		if(hp->reel < 0.0f)
		{
			/* Reset */
			hp->reel = 0.0f;
			/* If dead, fallen */
			if(hp->battler_link->get_health() <= 0)
				hp->fallen = 1.0f;
		}
	}
	/* Animate fallen */
	if(hp->fallen > 0.0f)
	{
		/* UAAAH-!! uuuah..! uuaa.! You Lose! */
		hp->pose = 4;
		hp->weapon = 0;
		hp->mirror = 0;
		hp->frame = 1;
		if(hp->fallen <= 0.75f)
			hp->frame = 2;
		if(hp->fallen <= 0.50f)
			hp->frame = 3;
		/* Reduce fallen */
		hp->fallen -= ts*0.1f;
		if(hp->fallen < 0.0f)
		{
			hp->downed = 1;
			hp->fallen = 0.0f;
		}
	}
	/* Downed hero */
	if(hp->downed)
	{
		hp->pose = 4;
		hp->mirror = 0;
		hp->frame = 3;
		hp->weapon = 0;
	}
}

/* Get hero position matching hero */
heropos *wcbattle_get_hero_pos_from_hero(hero *hr)
{
	int i;
	heropos *hp;
	/* Seek */
	for(i = 0;i < wcarray_size(wcbattle_pos);i++)
	{
		hp = (heropos*)wcarray_get(wcbattle_pos,i);
		if(hp->hero_link == hr)
			return hp; /* Match found */
	}
	/* No one was found */
	return 0;
}

/* Makes the hero reel back when hit */
void wcbattle_reel_hero(hero *hr)
{
	heropos *hp;
	hp = wcbattle_get_hero_pos_from_hero(hr);
	/* Can't reel back no one */
	if(!hp)
		return;
	/* Hit them! */
	hp->reel = 1.0f;
}

/* Sets battle result */
void wcbattle_set_result(int r)
{
	wcbattle_result = r;
}

/* Shows battle start message */
void wcbattle_start_message()
{
	wcmessage_show(wcvocab_get(38));
}

/* Show battle results */
void wcbattle_victory_message()
{
	wcmessage_show(wcvocab_get(5));
}

/* Show defeat message */
void wcbattle_defeat_message()
{
	wcmessage_show(wcvocab_get(6));
}

/* Rolls health and energy numbers */
void wcbattle_roll_numbers(heropos *hp)
{
	float ts;
	ts = wcrpg_get_time_step();
	hp->hp_roll += (((float)hp->battler_link->get_health())-hp->hp_roll)/5.0f*ts+1.0f;
	if(hp->hp_roll > (float)hp->battler_link->get_health())
		hp->hp_roll = (float)hp->battler_link->get_health();
	hp->mp_roll += (((float)hp->battler_link->get_mana())-hp->mp_roll)/5.0f*ts+1.0f;
	if(hp->mp_roll > (float)hp->battler_link->get_mana())
		hp->mp_roll = (float)hp->battler_link->get_mana();
}

/* Sorts the contents of the hero positions by y value */
void wcbattle_sort_hero_pos()
{
	int record,i,j,subrecord,holder,found;
	arraylist wcbattle_pos_new;
	heropos *hp,*np;
	/* Make a target array */
	wcbattle_pos_new = wcarray_new();
	wcarray_set_clean(wcbattle_pos_new,free);
	/* Record at top */
	record = 0;
	/* While left in old */
	while(wcarray_size(wcbattle_pos) > wcarray_size(wcbattle_pos_new))
	{
		/* The bottom */
		subrecord = WCRPG_HEIGHT;
		holder = 0;
		/* Find the lowest number above record */
		for(i = 0;i < wcarray_size(wcbattle_pos);i++)
		{
			hp = (heropos*)wcarray_get(wcbattle_pos,i);
			/* It's the new record but not as good as ones added */
			if(hp->sy < subrecord && hp->sy >= record)
			{
				/* Must not exist in new */
				found = 0;
				for(j = 0;j < wcarray_size(wcbattle_pos_new);j++)
				{
					np = (heropos*)wcarray_get(wcbattle_pos_new,j);
					if(np->hero_link == hp->hero_link)
						found = 1;
				}
				if(!found)
				{
					subrecord = hp->sy;
					holder = i;
				}
			}
		}
		/* Add */
		record = subrecord;
		wcarray_add(wcbattle_pos_new,wcarray_get(wcbattle_pos,holder));
	}
	/* Delete old */
	wcarray_set_clean(wcbattle_pos,0);
	wcarray_delete(wcbattle_pos);
	/* Swap */
	wcbattle_pos = wcbattle_pos_new;
}

/* Adds a hero pos */
void wcbattle_add_hero_pos(hero *hr,battler *bt)
{
	heropos *hp;
	/* Create and fill out */
	hp = (heropos*)malloc(sizeof(heropos));
	hp->sx = hr->get_battle_pos_x()-24;
	hp->sy = hr->get_battle_pos_y()-24;
	hp->x = (float)hp->sx;
	hp->y = (float)hp->sy;
	hp->state = 0;
	hp->pose = 0;
	hp->frame = WCBATTLESET_FRAME_AUTO;
	hp->hero_link = hr;
	hp->mirror = 0;
	hp->battler_link = bt;
	hp->hp_roll = 0.0f;
	hp->mp_roll = 0.0f;
	hp->anim_dist = 0.0f;
	hp->strike = 0;
	hp->weapon = 0;
	hp->command = 0;
	hp->use_skill = 0;
	hp->target = 0;
	hp->use_item = 0;
	hp->reel = 0.0f;
	hp->fallen = 0.0f;
	hp->downed = 0;
	/* Add */
	wcarray_add(wcbattle_pos,hp);
}

/* Gets a hero pos */
heropos *wcbattle_get_hero_pos(int ix)
{
	return (heropos*)wcarray_get(wcbattle_pos,ix);
}

/* Gets hero pos count */
int wcbattle_get_hero_pos_count()
{
	return wcarray_size(wcbattle_pos);
}

/* Set by code */
void wcbattle_set_system(int id)
{
	/* Set system */
	if(id == WCBATTLE_ATB_REMIX)
		wcbattle_set_system(wcbattleatb_init,wcbattleatb_exit,wcbattleatb_draw,wcbattleatb_main,wcbattleatb_handle_hero);
	/* Announce */
	wclog_write(wcrpg_get_log(),"Set battle system to ");
	wclog_write(wcrpg_get_log(),wcbattle_names[id]);
	wclog_enter(wcrpg_get_log());
}

/* Set handlers */
void wcbattle_set_system(void (*initf)(),void (*exitf)(),void (*drawf)(int),void (*mainf)(),void (*hherof)(heropos*))
{
	wcbattle_init = initf;
	wcbattle_exit = exitf;
	wcbattle_draw = drawf;
	wcbattle_main = mainf;
	wcbattle_handle_hero = hherof;
}

/* Handle nucleus */
void wcbattle_system_init()
{
	/* Reset hero positions */
	if(wcbattle_pos)
		wcarray_delete(wcbattle_pos);
	wcbattle_pos = wcarray_new();
	wcarray_set_clean(wcbattle_pos,free);
	/* Begin */
	if(wcbattle_init)
		wcbattle_init();
}
void wcbattle_system_exit()
{
	/* Call exit */
	if(wcbattle_exit)
		wcbattle_exit();
	/* End battle */
	wcbattle_current_group = 0;
	wcmetric_clear_field();
}
void wcbattle_system_main()
{
	int i;
	/* Don't handle if not in battle */
	if(!wcbattle_active())
		return;
	/* Don't handle if exiting */
	if(wcbattle_mode == 2)
	{
		if(wctrans_get_screen_state() == WCTRANS_SCREEN_SHOW)
			return;
		wcbattle_system_exit();
		wcbattle_mode = 0;
		wctrans_show(wcsystem_get_victory_trans(1));
		if(wcbattle_result != WCBATTLE_DEFEAT)
			wcsound_recall_song();
		else
			wcnucleus_set_mode(WCRPG_GAME_OVER);
		return;
	}
	/* Trans in */
	if(wctrans_get_screen_state() == WCTRANS_SCREEN_HIDDEN)
	{
		wcbattle_visible = 1;
		wctrans_show(wcsystem_get_battle_trans(1));
	}
	/* Handle heroes */
	if(wcbattle_handle_hero)
	{
		for(i = 0;i < wcbattle_get_hero_pos_count();i++)
			wcbattle_handle_hero(wcbattle_get_hero_pos(i));
	}
	/* Handle groups */
	wcbattle_current_group->handle();
	/* Handle it */
	if(wcbattle_main)
		wcbattle_main();
}
void wcbattle_system_draw(int menulayer)
{
	if(!wcbattle_active())
		return;
	if(!wcbattle_visible)
		return;
	if(wcbattle_draw)
		wcbattle_draw(menulayer);
}

/* Start combat with monster group id given */
void wcbattle_start(int mid,int tid)
{
	/* Enter enemies! */
	wcbattle_current_group = wcgroup_get(mid);
#ifndef NDEBUG
	fprintf(stdbattle,"*** Battle has begun against #%d: %s\n",mid,wcbattle_current_group->get_name());
#endif
	wcbattle_current_group->add_to_field();
	/* Prepare */
	wcbattle_system_init();
	/* Battle start! */
	wcsystem_play_sound(WCSYSTEM_BATTLE);
	wcsound_memorize_song();
	wcsystem_play_battle_music();
	wcbattle_mode = 1;
	wcbattle_terrain = tid;
	/* Screen out */
	wcbattle_visible = 0;
	wctrans_erase(wcsystem_get_battle_trans(0));
	/* Begin */
	wcbattle_result = WCBATTLE_ONGOING;
}

/* Ends the battle and returns to whatever */
void wcbattle_end()
{
	/* Erase screen */
	wctrans_erase(wcsystem_get_victory_trans(0));
	/* Report */
#ifndef NDEBUG
	fprintf(stdbattle,"*** Battle has finished.\n");
#endif
	/* Battle out */
	wcbattle_mode = 2;
}

/* Returns true if battle is happening */
int wcbattle_active()
{
	return wcbattle_mode;
}

/* Returns the currently set battle terrain */
int wcbattle_get_terrain()
{
	return wcbattle_terrain;
}

/* Adds up to N first party members */
void wcbattle_enter_party(int n)
{
	int i;
	/* Limit number if party size is less than the requested */
	if(wcparty_size() < n)
		n = wcparty_size();
	/* Add */
	for(i = 0;i < n;i++)
	{
		wcmetric_add_hero(wchero_get(wcparty_get(i)));
		wcbattle_add_hero_pos(wchero_get(wcparty_get(i)),wcmetric_get_battler(wcmetric_field_size()-1));
	}
	/* Sort */
	wcbattle_sort_hero_pos();
}

/* Gets current monster group */
monstergroup *wcbattle_get_group()
{
	return wcbattle_current_group;
}

/* Draws all heroes */
void wcbattle_draw_party()
{
	int i;
	heropos *hp;
	/* Find all heroes */
	for(i = 0;i < wcbattle_get_hero_pos_count();i++)
	{
		/* Get hero and draw it */
		hp = wcbattle_get_hero_pos(i);
		wcbattleset_set_option(hp->frame,hp->mirror);
		if(hp->weapon)
			wcbattleset_draw_attack((int)hp->x,(int)hp->y,hp->hero_link->get_battleset()-1,hp->weapon-1,WCBATTLESET_RIGHT_HAND);
		else
			wcbattleset_draw((int)hp->x,(int)hp->y,hp->hero_link->get_battleset()-1,hp->pose);
	}
}

/* Generate a menu holding the commands from a hero */
menu *wcbattle_create_command_menu(hero *hr,int *tps)
{
	int i,ct;
	menu *mn;
	battlecommand *bcmd;
	/* Count them */
	ct = 0;
	for(i = 0;i < MAX_COMMANDS;i++)
	{
		if(hr->get_command(i) != -1 && hr->get_command(i) != 0)
			ct++;
	}
	/* Load them */
	if(ct < WCBATTLE_COMMAND_VISIBLE)
		mn = new menu(WCBATTLE_COMMAND_WIDTH,ct);
	else
		mn = new menu(WCBATTLE_COMMAND_WIDTH,WCBATTLE_COMMAND_VISIBLE);
	for(i = 0;i < ct;i++)
	{
		bcmd = wcbattlecommand_get(hr->get_command(i));
		mn->add(wcrpg_string_clone(bcmd->get_name()));
		tps[i] = bcmd->get_type();
	}
	/* Add */
	return mn;
}

/* Generate a menu holding the targets */
menu *wcbattle_create_target_menu(int hr)
{
	int i,ct;
	menu *mn;
	battler *bt;
	/* Blank out enemy targets if all dead */
	if(wcmetric_enemy_count() <= 0)
		return new menu(WCBATTLE_TARGET_WIDTH,1);
	/* Count them */
	ct = 0;
	for(i = 0;i < wcmetric_field_size();i++)
	{
		if(!wcmetric_get_battler(i)->is_hero() && !hr && !wcmetric_get_battler(i)->is_dead())
			ct++;
		if(wcmetric_get_battler(i)->is_hero() && hr)
			ct++;
	}
	/* Load them */
	if(ct < WCBATTLE_COMMAND_VISIBLE)
		mn = new menu(WCBATTLE_TARGET_WIDTH,ct);
	else
		mn = new menu(WCBATTLE_TARGET_WIDTH,WCBATTLE_COMMAND_VISIBLE);
	/* Load target names */
	for(i = 0;i < wcmetric_field_size();i++)
	{
		if(!wcmetric_get_battler(i)->is_hero() && !hr && !wcmetric_get_battler(i)->is_dead())
		{
			bt = wcmetric_get_battler(i);
			mn->add(wcrpg_string_clone(bt->get_name()));
		}
		if(wcmetric_get_battler(i)->is_hero() && hr)
		{
			bt = wcmetric_get_battler(i);
			mn->add(wcrpg_string_clone(bt->get_name()));
		}
	}
	/* Return */
	return mn;
}