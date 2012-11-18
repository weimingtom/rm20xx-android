/*
	ATB Remix
*/

/* Includes */
#include <stdlib.h>
#include "wcbattle.h"
#include "wcbattleatb.h"
#include "wcterrain.h"
#include "wcwindow.h"
#include "wcmenu.h"
#include "wchero.h"
#include "wcvocab.h"

/* Globals */
menu *wcbattleatb_command_menu = 0;
menu *wcbattleatb_target_menu = 0;
heropos *wcbattleatb_focus = 0;
float wcbattleatb_slide = 0.0f;
int wcbattleatb_go = 0;
int wcbattleatb_message_shown = 0;
int wcbattleatb_command_types[MAX_COMMANDS];
int wcbattleatb_menu_mode = WCBATTLEATB_MENU_WAIT;
heropos *wcbattleatb_decide_player = 0;
int wcbattleatb_decide_command = 0;
int wcbattleatb_autofire = 0;
int wcbattleatb_select_heroes = 0;
int wcbattleatb_victory_shown = 0;
int wcbattleatb_defeat_shown = 0;

/* Reload and open the command menu for current user */
void wcbattleatb_open_command_menu(hero *hr)
{
	/* Release old */
	if(wcbattleatb_command_menu)
		delete wcbattleatb_command_menu;
	/* Make new */
	memset(wcbattleatb_command_types,0,sizeof(wcbattleatb_command_types));
	wcbattleatb_command_menu = wcbattle_create_command_menu(hr,wcbattleatb_command_types);
	/* Open */
	wcbattleatb_command_menu->activate();
}

/* Close command menu */
void wcbattleatb_close_command_menu()
{
	wcbattleatb_command_menu->close();
}

/* Focus back to menu */
void wcbattleatb_focus_command_menu()
{
	wcbattleatb_command_menu->activate();
}

/* Reload and open the target menu */
void wcbattleatb_open_target_menu()
{
	/* Release old */
	if(wcbattleatb_target_menu)
		delete wcbattleatb_target_menu;
	/* Make new */
	wcbattleatb_target_menu = wcbattle_create_target_menu(wcbattleatb_select_heroes);
	/* Open */
	wcbattleatb_target_menu->activate();
}

/* Close target menu */
void wcbattleatb_close_target_menu()
{
	wcbattleatb_target_menu->close();
}

/* Get current target */
int wcbattleatb_decide_target()
{
	return wcbattleatb_target_menu->peek_selection();
}

/* ATB window */
void wcbattleatb_draw_window(int x,int y)
{
	int i;
	heropos *hp;
	wcwindow_draw(x,y,WCBATTLEATB_ATB_WIDTH,16+16*wcbattle_get_hero_pos_count());
	/* Draw the info */
	for(i = 0;i < wcbattle_get_hero_pos_count();i++)
	{
		hp = wcbattle_get_hero_pos(i);
		/* Draw name */
		hp->hero_link->refresh();
		hp->hero_link->get_name_text()->move(x+8,y+8+i*16);
		hp->hero_link->get_name_text()->draw();
		/* Draw current HP */
		wcwindow_draw_number(x+8+96,y+8+i*16,(int)hp->hp_roll);
		/* Draw time */
		wcwindow_draw_bar(x+80+64,y+8+i*16,(int)(hp->battler_link->get_time_bar()*100.0f),BAR_T);
	}
}

/* Nucleus */
void wcbattleatb_handle_hero(heropos *hp)
{
	float ts,atb,atbf;
	/* Get time step */
	ts = wcrpg_get_time_step();
	/* Roll me */
	if(wcbattleatb_go)
		wcbattle_roll_numbers(hp);
	/* Kill me if im no hp left */
	if(hp->battler_link->get_health() <= 0)
		hp->state = WCBATTLEATB_DOWNED;
	/* Walk on in */
	if(hp->state == WCBATTLEATB_ENTERING)
	{
		/* I'm walking left */
		hp->pose = 8;
		hp->mirror = 0;
		hp->frame = WCBATTLESET_FRAME_AUTO;
		hp->weapon = 0;
		/* Move me to the left */
		hp->x -= 6.0f*ts;
		/* If I made it */
		if(hp->x < (float)hp->sx)
		{
			/* Stand at the ready */
			hp->x = (float)hp->sx;
			hp->state = WCBATTLEATB_READY;
		}
	}
	/* At the ready */
	if(hp->state == WCBATTLEATB_READY)
	{
		/* The battle officially started */
		wcbattleatb_go = 1;
		if(!wcbattleatb_message_shown)
		{
			wcbattle_start_message();
			wcbattleatb_message_shown = 1;
		}
		/* I'm standing and facing my enemy */
		hp->pose = 0;
		hp->mirror = 0;
		hp->frame = WCBATTLESET_FRAME_AUTO;
		hp->weapon = 0;
		/* Find ATB factor */
		atbf = (float)hp->battler_link->get_speed();
		atbf /= (float)wcmetric_get_lowest_speed();
		/* Build ATB if battle not paused */
		if(wcbattleatb_menu_mode <= WCBATTLEATB_MENU_COMMAND)
		{
			atb = hp->battler_link->get_time_bar();
			atb += ts*0.015f*atbf;
			if(atb > 1.0f)
			{
				hp->state = WCBATTLEATB_COMMAND;
				atb = 1.0f;
			}
			hp->battler_link->set_time_bar(atb);
		}
	}
	/* Command! */
	if(hp->state == WCBATTLEATB_COMMAND)
	{
		/* Get user attention */
		if(!wcbattleatb_focus)
		{
			/* Open window */
			wcbattleatb_open_command_menu(hp->hero_link);
			wcbattleatb_decide_player = hp;
			wcbattleatb_focus = hp;
			wcbattleatb_menu_mode = WCBATTLEATB_MENU_COMMAND;
		}
		/* I'm standing and facing my enemy */
		hp->pose = 0;
		hp->mirror = 0;
		hp->frame = WCBATTLESET_FRAME_AUTO;
		hp->weapon = 0;
	}
	/* Attttaaaackkk! */
	if(hp->state == WCBATTLEATB_EXECUTE)
	{
		/* I'm swiping at my enemy */
		hp->pose = 1;
		hp->mirror = 0;
		if(hp->anim_dist < 1.0f)
			hp->frame = 1+(int)(2*hp->anim_dist);
		else
			hp->frame = 3;
		hp->weapon = 1;
		/* Anim cycle */
		hp->anim_dist += ts*0.4f;
		if(hp->anim_dist > 1.0f && !hp->strike)
		{
			if(hp->target->is_dead())
				hp->target = wcmetric_next_enemy(hp->target);
			hp->strike = 1;
			wcanimation_play(hp->hero_link->get_battle_animation(),hp->target->get_pos_x(),hp->target->get_pos_y(),0,wcbattle_get_group(),hp->target->get_index());
		}
		/* Prepare to strike */
		if(hp->anim_dist > 3.0f)
		{
			if(wcmetric_roll(wcmetric_hit_rate(hp->battler_link,hp->target)))
			{
				/* STRIKE! */
				if(hp->target->is_dead())
					hp->target = wcmetric_next_enemy(hp->target);
				wcpopnum_show(hp->target->get_pos_x(),hp->target->get_pos_y(),wcmetric_attack(hp->battler_link,hp->target),WCPOPNUM_DAMAGE);
				if(hp->target->is_dead())
				{
					if(!hp->target->is_hero())
						wcsystem_play_sound(WCSYSTEM_DEFEATED);
					wcbattle_get_group()->defeat(hp->target->get_index());
				}
			}
			else
			{
				/* MISS! */
				wcpopnum_show(hp->target->get_pos_x(),hp->target->get_pos_y(),wcvocab_get(39));
			}
			/* Prepare for next action */
			hp->anim_dist = 3.0f;
			hp->state = WCBATTLEATB_READY;
			hp->battler_link->set_time_bar(0.0f);
		}
	}
	/* Victory */
	if(wcbattleatb_menu_mode == WCBATTLEATB_MENU_VICTORY && hp->state != WCBATTLEATB_WIN)
	{
		/* Yay */
		hp->pose = 10;
		hp->weapon = 0;
		hp->mirror = 0;
		hp->frame = 1;
		hp->anim_dist = 0.0f;
		hp->state = WCBATTLEATB_WIN;
	}
	/* Animate victory */
	if(hp->state == WCBATTLEATB_WIN)
	{
		/* Move */
		hp->anim_dist += ts*0.1f;
		if(hp->anim_dist > 1.0f)
			hp->anim_dist = 1.0f;
		if(hp->anim_dist < 1.0f)
			hp->frame = 1+(int)(2*hp->anim_dist);
		else
			hp->frame = 3;
	}
	/* Handle standard animations */
	wcbattle_handle_standard_hero_pos_animations(hp);
}
void wcbattleatb_init()
{
	int i;
	/* Add the players who will participate */
	wcbattle_enter_party(WCBATTLEATB_PLAYERS);
	/* Move them all to the right edge of the screen so they can walk in */
	for(i = 0;i < wcbattle_get_hero_pos_count();i++)
	{
		wcbattle_get_hero_pos(i)->x = (float)WCRPG_WIDTH;
		wcbattle_get_hero_pos(i)->x += (float)(rand()%32);
	}
	/* Reset slide */
	wcbattleatb_slide = 0.0f;
	wcbattleatb_go = 0;
	wcbattleatb_message_shown = 0;
	wcbattleatb_victory_shown = 0;
	wcbattleatb_defeat_shown = 0;
	wcbattleatb_menu_mode = WCBATTLEATB_MENU_WAIT;
}
void wcbattleatb_exit()
{
	wcbattleatb_focus = 0;
	wcbattleatb_decide_player = 0;
}
void wcbattleatb_main()
{
	float ts;
	ts = wcrpg_get_time_step();
	/* Handle auto fire */
	if(!wcinput_cancel() && !wcinput_ok() && !wcinput_x())
		wcbattleatb_autofire = 0;
	/* Handle the focus suddenly dying */
	if(wcbattleatb_decide_player && wcbattleatb_menu_mode != WCBATTLEATB_MENU_DEFEAT)
	{
		/* If the focus is dead.. */
		if(wcbattleatb_decide_player->battler_link->get_health() <= 0)
		{
			/* Go back to waiting and cancel windows */
			wcbattleatb_decide_player->state = WCBATTLEATB_DOWNED;
			wcbattleatb_decide_player->battler_link->set_time_bar(0.0f);
			wcbattleatb_close_command_menu();
			wcbattleatb_close_target_menu();
			wcbattleatb_autofire = 1;
			wcbattleatb_focus = 0;
			wcbattleatb_menu_mode = WCBATTLEATB_MENU_WAIT;
		}
	}
	/* Handle victory */
	if(wcmetric_enemy_count() <= 0)
	{
		wcbattleatb_menu_mode = WCBATTLEATB_MENU_VICTORY;
		if(wcbattleatb_command_menu)
			wcbattleatb_command_menu->close();
		if(wcbattleatb_target_menu)
			wcbattleatb_target_menu->close();
	}
	/* Handle defeat */
	if(wcmetric_game_over())
	{
		wcbattleatb_menu_mode = WCBATTLEATB_MENU_DEFEAT;
		if(wcbattleatb_command_menu)
			wcbattleatb_command_menu->close();
		if(wcbattleatb_target_menu)
			wcbattleatb_target_menu->close();
	}
	/* Handle menus */
	if(wcbattleatb_command_menu)
		wcbattleatb_command_menu->handle();
	if(wcbattleatb_target_menu)
		wcbattleatb_target_menu->handle();
	/* Select command */
	if(wcbattleatb_menu_mode == WCBATTLEATB_MENU_COMMAND)
	{
		/* Fight! */
		if(wcbattleatb_command_menu->get_selection() == WCBATTLE_ATTACK_ARCHTYPE)
		{
			wcbattleatb_menu_mode = WCBATTLEATB_MENU_TARGET;
			wcbattleatb_decide_command = WCBATTLE_ATTACK_ARCHTYPE;
			wcbattleatb_select_heroes = 0; /* Default to enemies */
			wcbattleatb_open_target_menu();
			wcbattleatb_autofire = 1;
		}
	}
	/* Enter target */
	if(wcbattleatb_menu_mode == WCBATTLEATB_MENU_TARGET)
	{
		/* Go back */
		if(wcinput_cancel() && !wcbattleatb_autofire)
		{
			wcbattleatb_menu_mode = WCBATTLEATB_MENU_COMMAND;
			wcbattleatb_close_target_menu();
			wcbattleatb_focus_command_menu();
			wcbattleatb_autofire = 1;
		}
		/* Swap targets */
		if(wcinput_x() != 0 && !wcbattleatb_autofire)
		{
			if(wcbattleatb_select_heroes == 0)
				wcbattleatb_select_heroes = 1;
			else
				wcbattleatb_select_heroes = 0;
			wcbattleatb_open_target_menu();
			wcbattleatb_autofire = 1;
		}
		/* Select target and fire! */
		if(wcinput_ok() && !wcbattleatb_autofire)
		{
			/* Input command */
			wcbattleatb_decide_player->command = wcbattleatb_decide_command;
			wcbattleatb_decide_player->target = wcmetric_get_battler(wcbattleatb_select_heroes,wcbattleatb_decide_target());
			wcbattleatb_decide_player->use_item = 0;
			wcbattleatb_decide_player->use_skill = 0;
			wcbattleatb_decide_player->anim_dist = 0.0f;
			wcbattleatb_decide_player->strike = 0;
			/* Reset my ATB */
			wcbattleatb_decide_player->state = WCBATTLEATB_EXECUTE;
			wcbattleatb_close_command_menu();
			wcbattleatb_close_target_menu();
			wcbattleatb_autofire = 1;
			wcbattleatb_focus = 0;
			wcbattleatb_menu_mode = WCBATTLEATB_MENU_WAIT;
		}
	}
	/* Show victory */
	if(wcbattleatb_menu_mode == WCBATTLEATB_MENU_VICTORY)
	{
		/* Victory! */
		if(!wcbattleatb_victory_shown)
		{
			wcbattle_victory_message();
			wcsystem_play_victory_music();
			wcbattleatb_victory_shown = 1;
			wcbattleatb_autofire = 1;
		}
		/* Exit the battling */
		if(wcinput_ok() && !wcbattleatb_autofire)
		{
			wcbattle_set_result(WCBATTLE_VICTORY);
			wcbattle_end();
		}
	}
	/* Show defeat */
	if(wcbattleatb_menu_mode == WCBATTLEATB_MENU_DEFEAT)
	{
		/* Defeat.. */
		if(!wcbattleatb_defeat_shown)
		{
			wcbattle_defeat_message();
			wcsystem_play_gameover_music();
			wcbattleatb_defeat_shown = 1;
			wcbattleatb_autofire = 1;
		}
		/* Exit the battling */
		if(wcinput_ok() && !wcbattleatb_autofire)
		{
			wcbattle_set_result(WCBATTLE_DEFEAT);
			wcbattle_end();
		}
	}
	/* Slide */
	if(wcbattleatb_go)
		wcbattleatb_slide += (1.0f-wcbattleatb_slide)/2.0f*ts;
}
void wcbattleatb_draw(int menulayer)
{
	terrain *tr;
	battler *bt;
	/* Draw the basics */
	if(!menulayer)
	{
		/* Draw terrain */
		tr = (terrain*)wcterrain_get(wcbattle_get_terrain());
		tr->draw();
		/* Draw monsters */
		wcbattle_get_group()->draw();
		/* Draw heroes */
		wcbattle_draw_party();
	}
	else
	{
		/* Draw atbs and stuff */
		wcbattleatb_draw_window(WCRPG_WIDTH-(int)(WCBATTLEATB_ATB_WIDTH*wcbattleatb_slide),WCRPG_HEIGHT-16-16*wcbattle_get_hero_pos_count());
		if(wcbattleatb_command_menu)
			wcbattleatb_command_menu->draw(0,WCRPG_HEIGHT-16*WCBATTLE_COMMAND_VISIBLE-16);
		if(wcbattleatb_target_menu)
			wcbattleatb_target_menu->draw(0,WCRPG_HEIGHT-16*WCBATTLE_COMMAND_VISIBLE-16);
		/* Point an arrow at the enemy */
		if(wcbattleatb_menu_mode == WCBATTLEATB_MENU_TARGET)
		{
			/* Get the target */
			bt = wcmetric_get_battler(wcbattleatb_select_heroes,wcbattleatb_decide_target());
			wcwindow_draw_hand(bt->get_pos_x()+32,bt->get_pos_y(),HAND_LEFT);
		}
	}
}