/* Includes made just to allow the execution of related systems */
#include <stdlib.h>
#include "wcdialog.h"
#include "wcevent.h"
#include "wcrun.h"
#include "wcsound.h"
#include "wclevel.h"
#include "wcmanage.h"
#include "wcplayer.h"
#include "wclevel.h"
#include "wcparty.h"
#include "wcpicture.h"
#include "wchero.h"
#include "wcsave.h"
#include "wcsavemenu.h"
#include "wcitem.h"
#include "wcmisc.h"
#include "wcnucleus.h"
#include "wcinput.h"
#include "wcanimation.h"
#include "wcfx.h"
#include "wcbattle.h"
#include "wctrans.h"
#include "wcsystem.h"
#include "wctimer.h"

/* Decode and execute command */
int command :: run(void *me,void *st)
{
	hero *hr;
	item *it;
	sound *sn;
	rpgevent *ev;
	rpgevent *targ,*sorc;
	rpgstate *state;
	music *mus;
	state = (rpgstate*)st;
	ev = (rpgevent*)me;
	int i,j,k,m;
	float af,bf;
	/* Halt on transition */
	if(wctrans_active())
		return 0;
	/* Halt on battles */
	if(wcbattle_active())
		return 0;
	/* Halt on menu */
	if(wcmanage_active())
		return 0;
	/* Halt on pan */
	if(halt == COMMAND_HALT_ON_PAN)
	{
		if(wclevel_camera_panning())
		{
			return 0;
		}
		else
		{
			halt = 0;
			return 1;
		}
	}
	/* Message (str) */
	if(id == COMMAND_MESSAGE)
	{
		/* Request permission even if event is not present */
		if(ev)
		{
			/* Request permission */
			if(!wcdialog_request(ev->get_id()))
				return 0; /* Halt */
		}
		else
		{
			/* Request permission */
			if(!wcdialog_request(-1))
				return 0; /* Halt */
		}
		/* Reset line */
		wcdialog_reset_line();
		/* Look at the player */
		if(ev)
		{
			if(!ev->get_direction_fix())
				ev->face_player();
		}
		/* Add the message string */
		wcdialog_add_line(str);
		/* Open dialog box */
		wcdialog_open();
	}
	/* Show choice (str) */
	if(id == COMMAND_SHOW_CHOICE)
	{
		/* Request permission even if event is not present */
		if(ev)
		{
			/* Request permission */
			if(!wcdialog_request(ev->get_id()))
				return 0; /* Halt */
		}
		else
		{
			/* Request permission */
			if(!wcdialog_request(-1))
				return 0; /* Halt */
		}
		/* Open choice */
		wcdialog_open_choice(str);
		wcdialog_set_choice_default(arg[0]);
	}
	/* Add message line */
	if(id == COMMAND_MESSAGE_LINE)
	{
		/* Adds extra line to message string */
		wcdialog_add_line(str);
	}
	/* Non Message */
	if(id != COMMAND_MESSAGE && id != COMMAND_MESSAGE_LINE && id != COMMAND_SHOW_CHOICE)
	{
		wcdialog_reset_line();
		/* Held on dialog */
		if(ev)
		{
			if(wcdialog_halt(ev->get_id()))
				return 0; /* Halt */
		}
		else
		{
			if(wcdialog_halt(-1))
				return 0; /* Halt */
		}
	}
	/* Set message options */
	if(id == COMMAND_MESSAGE_OPTIONS)
	{
		/* Just a pass-through */
		wcdialog_options(arg[0],arg[1],arg[2],arg[3]);
	}
	/* Set face */
	if(id == COMMAND_SET_FACE)
	{
		/* Set the face */
		if(!str)
		{
			/* Clear face */
			wcdialog_erase_face();
		}
		else
		{
			/* Set the face, first argument is face index */
			wcdialog_set_face(str,arg[0],arg[1],arg[2]);
		}
	}
	/* Switch operations */
	if(id == COMMAND_SWITCH)
	{
		/* Single value / Many values */
		if(arg[0] == 0 || arg[0] == 1)
			wcmem_set_switch_ex(arg[1],arg[2],arg[3]);
		/* Variable reference */
		if(arg[0] == 2)
			wcmem_set_switch_ex(wcmem_get_variable(arg[1]),wcmem_get_variable(arg[1]),arg[3]);
	}
	/* Variable operations */
	if(id == COMMAND_VARIABLE)
	{
		/* First we need to select the variable(s) */
		/* Single variable */
		if(arg[0] == 0)
		{
			i = arg[1];
			j = arg[1];
		}
		/* Many variables */
		if(arg[0] == 1)
		{
			i = arg[1];
			j = arg[2];
		}
		/* Referenced variable */
		if(arg[0] == 2)
		{
			i = wcmem_get_variable(arg[1],me);
			j = i;
		}
		/* Then we need to select the operation */
		k = arg[3];
		/* Now find the target source */
		m = 0;
		if(arg[4] == 0)
		{
			/* A single number */
			m = arg[5];
		}
		if(arg[4] == 1)
		{
			/* A variable */
			m = wcmem_get_variable(arg[5],me);
		}
		if(arg[4] == 2)
		{
			/* A reference for variable */
			m = wcmem_get_variable(wcmem_get_variable(arg[5],me));
		}
		if(arg[4] == 3)
		{
			/* A random number */
			m = rand()%(arg[6]-arg[5]+1);
			m += arg[5];
		}
		if(arg[4] == 4)
		{
			/* Amount of item held/equipped */
			it = wcitem_get(arg[5]);
			/* Get amount held */
			if(arg[6] == 0)
				m = it->get_held();
			else
				m = it->get_equipped();
		}
		if(arg[4] == 5)
		{
			/* Hero stats */
			hr = wchero_get(arg[5]);
			/* Get level */
			if(arg[6] == 0)
				m = hr->get_level();
			if(arg[6] == 1)
				m = hr->get_exp();
			/* Get exhaust */
			if(arg[6] == 2)
				m = hr->get_health();
			if(arg[6] == 3)
				m = hr->get_mana();
			/* Get stat */
			if(arg[6] >= 4 && arg[6] < 10)
				m = hr->get_stat(arg[6]-4);
			/* Get equip */
			if(arg[6] >= 10)
				m = hr->get_equip(arg[6]-10);
		}
		if(arg[4] == 6)
		{
			/* Get event in question */
			if(arg[5] < 10000)
				targ = (rpgevent*)wclevel_get_event(arg[5]);
			if(arg[5] == EVENT_ME)
				targ = ev;
			if(arg[5] == EVENT_PLAYER)
				targ = wcplayer_get();
			/* Default case */
			m = 0;
			/* Get map ID */
			if(arg[6] == 0)
				m = targ->get_map_id();
			/* Get pos X */
			if(arg[6] == 1)
				m = targ->get_pos_x();
			/* Get pos Y */
			if(arg[6] == 2)
				m = targ->get_pos_y();
			/* Get facing?? */
			if(arg[6] == 3)
			{
				if(targ->get_direction() == FACE_UP)
					m = 8;
				if(targ->get_direction() == FACE_DOWN)
					m = 2;
				if(targ->get_direction() == FACE_RIGHT)
					m = 4;
				if(targ->get_direction() == FACE_LEFT)
					m = 6;
			}
			/* Get screen X */
			if(arg[6] == 4)
				m = targ->get_screen_x()+6+156+30-104-80;
			if(arg[6] == 5)
				m = targ->get_screen_y()+16;
		}
		if(arg[4] == 7)
		{
			/* Default */
			m = 0;
			/* Special stuff */
			if(arg[5] == 0)
				m = wcmisc_get_money(); /* Money */
			if(arg[5] == 1)
				m = wctimer_get(0); /* Timer 1 */
			if(arg[5] == 9)
				m = wctimer_get(1); /* Timer 2 */
			if(arg[5] == 2)
				m = wcparty_size(); /* Size of party */
			if(arg[5] == 3)
				m = wcmisc_get_save_count(); /* Times saved */
		}
		/* Execute */
		wcmem_set_variable_ex(i,j,k,m,me);
	}
	/* Teleport command */
	if(id == COMMAND_TELEPORT)
	{
		/* Remember face? */
		if(!arg[3])
			i = wcplayer_get()->get_direction();
		/* We need to trans out */
		if(!wctrans_get_teleport_mode())
		{
			if(wctrans_get_screen_state() == WCTRANS_SCREEN_SHOW)
			{
				wctrans_erase(wcsystem_get_map_trans(0));
				return 0;
			}
		}
		/* Do this only if changing maps */
		if(wclevel_get_id() != arg[0])
		{
			/* Save myself */
			wclevel_link_event(me);
			/* Load level */
			if(!ev->is_severed())
				wcrun_set_bridge_event(me);
		}
		/* Remember arg 3 */
		j = arg[3];
		wclevel_load(arg[0],arg[1],arg[2]);
		/* Do this only if changing maps */
		if(wclevel_get_id() != arg[0])
		{
			/* Pictures go away */
			wcpicture_wipe();
		}
		/* Get the hero and make them face a direction */
		if(j)
			wcplayer_get()->face(j-1);
		else
			wcplayer_get()->face(i);
		/* Trans back in */
		if(!wctrans_get_teleport_mode())
			wctrans_show(wcsystem_get_map_trans(1));
	}
	/* Wait command */
	if(id == COMMAND_WAIT)
	{
		/* Cause sleeping */
		if(arg[1] == 0)
		{
			state->sleep(0.1f*(float)(arg[0]));
			return 0;
		}
		/* Wait for key pressed */
		if(arg[1] == 1)
		{
			if(!state->check_ok_key())
				return 0;
		}
	}
	/* Sound command */
	if(id == COMMAND_SOUND)
	{
		/* Load if need to load */
		sn = new sound(str,arg[0],arg[1],arg[2]);
		/* Play */
		sn->play();
		/* Get rid of holder */
		delete sn;
	}
	/* Allow/disallow main menu */
	if(id == COMMAND_ENABLE_MAIN_MENU)
	{
		/* Set switch */
		wcmanage_enable(arg[0]);
	}
	/* Allow/disallow saving */
	if(id == COMMAND_ENABLE_SAVE)
	{
		/* Set switch */
		wcsave_allow(arg[0]);
	}
	/* Open save menu */
	if(id == COMMAND_OPEN_SAVE)
	{
		/* Open saves */
		wcsavemenu_open(0,0);
		wcmanage_stack();
	}
	/* Open main menu */
	if(id == COMMAND_OPEN_MAIN_MENU)
	{
		/* Open menus */
		wcmanage_open();
	}
	/* Erases an event */
	if(id == COMMAND_ERASE_EVENT)
	{
		/* Disable me */
		ev->erase();
		/* Can't run additional commands */
		return 0;
	}
	/* Loads move command */
	if(id == COMMAND_MOVE)
	{
		/* Set my itenerary */
		if(mls->get_target() == EVENT_ME)
			ev->set_move_list(mls);
		/* Set hero itenerary */
		if(mls->get_target() == EVENT_PLAYER)
			((rpgevent*)wcplayer_get())->set_move_list(mls);
		/* Set custom event */
		if(mls->get_target() < 10000)
			((rpgevent*)wclevel_get_event(mls->get_target()))->set_move_list(mls);
	}
	/* Halts until move commands are finshed */
	if(id == COMMAND_PROCEED_MOVE)
	{
		if(!wcmove_finished())
			return 0;
	}
	/* Show picture */
	if(id == COMMAND_SHOW_PICTURE)
	{
		/* Start off the picture */
		if(!arg[1])
			wcpicture_show(str,arg[0]-1,arg[2],arg[3],(!arg[7]));
		else
			wcpicture_show(str,arg[0]-1,wcmem_get_variable(arg[2]),wcmem_get_variable(arg[3]),(!arg[7]));
		/* Pinned? */
		if(arg[4])
			wcpicture_get(arg[0]-1)->pin();
		/* Set tones */
		wcpicture_get(arg[0]-1)->tone((float)(arg[8])/100.0f,(float)(arg[9])/100.0f,(float)(arg[10])/100.0f,(float)(arg[11])/100.0f);
		/* Set properties */
		if(arg[12] != 0)
		{
			if(arg[12] == 1)
				wcpicture_get(arg[0]-1)->set_rotation((float)(arg[13]));
			if(arg[12] == 2)
				wcpicture_get(arg[0]-1)->set_waver((float)(arg[13]));
		}
		else
		{
			wcpicture_get(arg[0]-1)->set_rotation(0.0f);
		}
		wcpicture_get(arg[0]-1)->set_transparency((float)(arg[6]/100.0f),(float)(arg[14]/100.0f));
		wcpicture_get(arg[0]-1)->set_scale((float)(arg[5]/100.0f));
	}
	/* Erase picture */
	if(id == COMMAND_ERASE_PICTURE)
	{
		wcpicture_erase(arg[0]-1);
	}
	if(id == COMMAND_MOVE_PICTURE)
	{
		/* Do nothing if it does not exist */
		if(!wcpicture_get(arg[0]-1))
			return 1;
		/* Set new rotation */
		af = 0.0f;
		bf = 0.0f;
		if(arg[12] != 0)
			af = (float)arg[13];
		/* Set wavering instead? */
		if(arg[12] == 2)
		{
			bf = af;
			af = 0.0f;
		}
		/* Finish */
		wcpicture_get(arg[0]-1)->finish();
		/* Set coordinates to move */
		if(!arg[1])
			wcpicture_get(arg[0]-1)->move(arg[2],arg[3],arg[14],(float)(arg[6]/100.0f),(float)(arg[16]/100.0f),(float)(arg[5]/100.0f),af,bf,(float)(arg[8])/100.0f,(float)(arg[9])/100.0f,(float)(arg[10])/100.0f,(float)(arg[11])/100.0f);
		else
			wcpicture_get(arg[0]-1)->move(wcmem_get_variable(arg[2]),wcmem_get_variable(arg[3]),arg[14],(float)(arg[6]/100.0f),(float)(arg[16]/100.0f),(float)(arg[5]/100.0f),af,bf,(float)(arg[8])/100.0f,(float)(arg[9])/100.0f,(float)(arg[10])/100.0f,(float)(arg[11])/100.0f);
		/* Wait */
		if(arg[15])
		{
			/* Cause sleeping */
			state->sleep(0.1f*(float)(arg[14]));
			/* Give up control */
			return 0;
		}
	}
	/* Tint screen */
	if(id == COMMAND_TINT_SCREEN)
	{
		/* Set tint */
		wcnucleus_tint((float)(arg[0]/100.0f),(float)(arg[1]/100.0f),(float)(arg[2]/100.0f),(float)(arg[3]/100.0f),(float)(arg[4]));
		/* Wait */
		if(arg[5])
		{
			/* Cause sleeping */
			state->sleep(0.1f*(float)(arg[4]));
			/* Give up control */
			return 0;
		}
	}
	/* Modify the party */
	if(id == COMMAND_CHANGE_PARTY)
	{
		/* Operate */
		if(arg[0] == 0)
		{
			/* Add member */
			if(arg[1] == 0)
			{
				/* Specific member */
				wcparty_add(arg[2]);
			}
			else
			{
				/* By variable */
				wcparty_add(wcmem_get_variable(arg[2]));
			}
		}
		else
		{
			/* Remove member */
			if(arg[1] == 0)
			{
				/* Specific member */
				wcparty_remove(arg[2]);
			}
			else
			{
				/* By variable */
				wcparty_remove(wcmem_get_variable(arg[2]));
			}
		}
		/* Update */
		wcplayer_refresh();
	}
	/* Change graphic */
	if(id == COMMAND_CHANGE_SPRITE)
	{
		/* Get the hero to modify */
		hr = wchero_get(arg[0]);
		/* Modify them */
		hr->set_charset(str,arg[1],arg[2]);
		/* Update */
		wcplayer_refresh();
	}
	/* Change face */
	if(id == COMMAND_CHANGE_FACE)
	{
		/* Get the hero to modify */
		hr = wchero_get(arg[0]);
		/* Modify them */
		hr->set_faceset(str,arg[1]);
		/* Update */
		wcparty_refresh();
	}
	/* Jump to label */
	if(id == COMMAND_GOTO)
	{
		/* Move program */
		state->jump_to_label(arg[0]);
	}
	/* Jump to option */
	if(id == COMMAND_CHOICE_OPTION)
	{
		/* Go there */
		state->jump_to_choice(wcdialog_get_choice());
	}
	/* End */
	if(id == COMMAND_BREAK)
	{
		/* Go to end */
		state->jump_to_end();
	}
	/* Branch */
	if(id == COMMAND_BRANCH)
	{
		/* Evaluation */
		i = 0;
		j = 0;
		/* Evaluation Switch */
		if(arg[0] == 0)
		{
			/* Switch is ON */
			if(arg[2] == 0 && wcmem_get_switch(arg[1]))
				i = 1;
			/* Switch is OFF */
			if(arg[2] == 1 && !wcmem_get_switch(arg[1]))
				i = 1;
		}
		/* Evaluation Variable */
		if(arg[0] == 1)
		{
			/* Sample the test subject */
			if(arg[2] == 0)
				j = arg[3];
			else
				j = wcmem_get_variable(arg[3],me);
			/* Equal to */
			if(arg[4] == 0 && wcmem_get_variable(arg[1],me) == j)
				i = 1;
			/* Greater equal */
			if(arg[4] == 1 && wcmem_get_variable(arg[1],me) >= j)
				i = 1;
			/* Lesser equal */
			if(arg[4] == 2 && wcmem_get_variable(arg[1],me) <= j)
				i = 1;
			/* Greater */
			if(arg[4] == 3 && wcmem_get_variable(arg[1],me) > j)
				i = 1;
			/* Lesser */
			if(arg[4] == 4 && wcmem_get_variable(arg[1],me) < j)
				i = 1;
			/* Not */
			if(arg[4] == 5 && wcmem_get_variable(arg[1],me) != j)
				i = 1;
		}
		/* Evaluation timer */
		if(arg[0] == 2)
		{
			if(arg[2] == 0 && wctimer_get(0) >= arg[1])
				i = 1;
			if(arg[2] == 1 && wctimer_get(0) <= arg[1])
				i = 1;
		}
		/* Evaluation timer 2 */
		if(arg[0] == 10)
		{
			if(arg[2] == 0 && wctimer_get(1) >= arg[1])
				i = 1;
			if(arg[2] == 1 && wctimer_get(1) <= arg[1])
				i = 1;
		}
		/* Evaluation money */
		if(arg[0] == 3)
		{
			/* Sample the test subject */
			j = arg[1];
			/* Need at least */
			if(arg[2] == 0 && wcmisc_get_money() >= j)
				i = 1;
			/* This or less */
			if(arg[2] == 1 && wcmisc_get_money() <= j)
				i = 1;
		}
		/* Evaluation items */
		if(arg[0] == 4)
		{
			/* Posses */
			if(!arg[2] && wcitem_get(arg[1])->get_held())
				i = 1;
			/* Does not have */
			if(arg[2] && !wcitem_get(arg[1])->get_held())
				i = 1;
		}
		/* Evaluation hero */
		if(arg[0] == 5)
		{
			/* Is in party */
			if(arg[2] == 0 && wcparty_has(arg[1]))
				i = 1;
			/* Name is str */
			if(arg[2] == 1 && !strcmp(wchero_get(arg[1])->get_name(),str))
				i = 1;
			/* Level is at least this */
			if(arg[2] == 2 && wchero_get(arg[1])->get_level() >= arg[3])
				i = 1;
			/* HP is at least this */
			if(arg[2] == 3 && wchero_get(arg[1])->get_health() >= arg[3])
				i = 1;
			/* Has this skill */
			if(arg[2] == 4 && wchero_get(arg[1])->has_skill(arg[3]))
				i = 1;
			/* Has this equip */
			if(arg[2] == 5 && wchero_get(arg[1])->has_equip(arg[3]))
				i = 1;
		}
		/* Jump based on eval */
		if(arg[5] && !i)
		{
			/* Else */
			state->jump_to_else();
		}
		if(!arg[5] && !i)
		{
			/* None */
			state->advance();
			state->jump_to_end();
		}
	}
	/* Return */
	if(id == COMMAND_END_WHILE)
	{
		/* Go backwards */
		state->jump_to_while();
	}
	/* Break out */
	if(id == COMMAND_BREAK_WHILE)
	{
		/* Go to end and then one more so you don't re-run the loop */
		state->jump_to_while_end();
		/*state->advance();*/
	}
	/* Call event */
	if(id == COMMAND_CALL_EVENT)
	{
		/* Call common event */
		if(arg[0] == 0)
			state->call_common_event(arg[1]-1);
		/* Call map event */
		if(arg[0] == 1)
			state->call_event(arg[1],arg[2]-1);
		/* Call map event by var */
		if(arg[0] == 2)
			state->call_event(wcmem_get_variable(arg[1]),wcmem_get_variable(arg[2])-1);
	}
	/* Add/Remove items */
	if(id == COMMAND_ITEM)
	{
		/* Get id and amount */
		i = 0;
		j = 0;
		if(arg[1])
			i = wcmem_get_variable(arg[2]);
		else
			i = arg[2];
		i--;
		if(arg[3])
			j = wcmem_get_variable(arg[4]);
		else
			j = arg[4];
		/* Do it! */
		if(!arg[0])
		{
			/* Add item */
			wcitem_add(i,j);
		}
		else
		{
			/* Add item */
			wcitem_remove(i,j);
		}
	}
	/* Modify equipments */
	if(id == COMMAND_CHANGE_EQUIPMENT)
	{
		hr = 0;
		/* Get hero directly */
		if(arg[0] == 1)
			hr = (hero*)wchero_get(arg[1]);
		/* Get hero indirectly */
		if(arg[0] == 2)
			hr = (hero*)wchero_get(wcmem_get_variable(arg[1],me));
		k = 1;
		/* All party */
		if(arg[0] == 0)
		{
			k = wcparty_size();
			hr = (hero*)wchero_get(wcparty_get(0));
		}
		/* Execute */
		for(j = 0;j < k;j++)
		{
			/* Remove equipment */
			if(arg[2] == 1)
			{
				/* Remove a part */
				if(arg[3] < 5)
					hr->unequip_item_slot(arg[3]);
				if(arg[3] == 5)
				{
					/* Remove all parts */
					for(i = 0;i < 5;i++)
						hr->unequip_item_slot(i);
				}
			}
			/* Add equipment */
			if(arg[2] == 0)
			{
				/* Add specific */
				if(arg[3] == 0)
					hr->equip_item(arg[4]);
				/* Add reference */
				if(arg[3] == 1)
					hr->equip_item(wcmem_get_variable(arg[4],me));
			}
			/* Next hero */
			if(j+1 < k)
				hr = (hero*)wchero_get(wcparty_get(j+1));
		}
	}
	/* Add/Remove cash */
	if(id == COMMAND_MONEY)
	{
		/* Get amount */
		i = arg[2];
		if(arg[1])
			i = wcmem_get_variable(arg[2]);
		/* Do it */
		if(!arg[0])
			wcmisc_add_money(i);
		else
			wcmisc_take_money(i);
	}
	/* Change levels */
	if(id == COMMAND_LEVEL_UP)
	{
		/* Get hero */
		i = arg[1];
		if(arg[0] == 2) /* ... by variable reference */
			i = wcmem_get_variable(arg[1]);
		/* Get amount */
		j = arg[4];
		if(arg[3] == 1)
			j = wcmem_get_variable(arg[4]);
		if(arg[2] == 1)
			j = -j; /* Reduction? */
		/* Apply */
		if(i != 0)
		{
			hr = wchero_get(i);
			hr->set_level(hr->get_level()+j);
		}
		else
		{
			for(k = 0;k < wcparty_size();k++)
			{
				hr = wchero_get(wcparty_get(k));
				hr->set_level(hr->get_level()+j);
			}
		}
	}
	/* Change EXP */
	if(id == COMMAND_CHANGE_EXP)
	{
		/* Get hero */
		i = arg[1];
		if(arg[0] == 2) /* ... by variable reference */
			i = wcmem_get_variable(arg[1]);
		/* Get amount */
		j = arg[4];
		if(arg[3] == 1)
			j = wcmem_get_variable(arg[4]);
		if(arg[2] == 1)
			j = -j; /* Reduction? */
		/* Apply */
		if(i != 0)
		{
			hr = wchero_get(i);
			hr->gain_exp(j);
		}
		else
		{
			for(k = 0;k < wcparty_size();k++)
			{
				hr = wchero_get(wcparty_get(k));
				hr->gain_exp(j);
			}
		}
	}
	/* Change Health */
	if(id == COMMAND_CHANGE_HEALTH)
	{
		/* Get hero */
		i = arg[1];
		if(arg[0] == 2) /* ... by variable reference */
			i = wcmem_get_variable(arg[1]);
		/* Get amount */
		j = arg[4];
		if(arg[3] == 1)
			j = wcmem_get_variable(arg[4]);
		if(arg[2] == 1)
			j = -j; /* Reduction? */
		/* Apply */
		if(i != 0)
		{
			hr = wchero_get(i);
			if(arg[5])
			{
				hr->set_health(hr->get_health()+j);
			}
			else
			{
				j = hr->get_health()+j;
				if(j < 0)
					j = 1;
				hr->set_health(j);
			}
		}
		else
		{
			for(k = 0;k < wcparty_size();k++)
			{
				hr = wchero_get(wcparty_get(k));
				if(arg[5])
				{
					hr->set_health(hr->get_health()+j);
				}
				else
				{
					j = hr->get_health()+j;
					if(j < 0)
						j = 1;
					hr->set_health(j);
				}
			}
		}
	}
	/* Change Mana */
	if(id == COMMAND_CHANGE_MANA)
	{
		/* Get hero */
		i = arg[1];
		if(arg[0] == 2) /* ... by variable reference */
			i = wcmem_get_variable(arg[1]);
		/* Get amount */
		j = arg[4];
		if(arg[3] == 1)
			j = wcmem_get_variable(arg[4]);
		if(arg[2] == 1)
			j = -j; /* Reduction? */
		/* Apply */
		if(i != 0)
		{
			hr = wchero_get(i);
			hr->set_mana(hr->get_mana()+j);
		}
		else
		{
			for(k = 0;k < wcparty_size();k++)
			{
				hr = wchero_get(wcparty_get(k));
				hr->set_mana(hr->get_mana()+j);
			}
		}
	}
	/* Change stats */
	if(id == COMMAND_CHANGE_STAT)
	{
		/* Get hero */
		i = arg[1];
		if(arg[0] == 2) /* ... by variable reference */
			i = wcmem_get_variable(arg[1]);
		/* Get amount */
		j = arg[5];
		if(arg[4] == 1)
			j = wcmem_get_variable(arg[5]);
		if(arg[2] == 1)
			j = -j; /* Reduction? */
		/* Apply */
		if(i != 0)
		{
			hr = wchero_get(i);
			hr->modify_stat(arg[3],j);
		}
		else
		{
			for(k = 0;k < wcparty_size();k++)
			{
				hr = wchero_get(wcparty_get(k));
				hr->modify_stat(arg[3],j);
			}
		}
	}
	/* Change skills */
	if(id == COMMAND_CHANGE_SKILLS)
	{
		/* Get hero */
		i = arg[1];
		if(arg[0] == 2) /* ... by variable reference */
			i = wcmem_get_variable(arg[1]);
		/* Get skill */
		j = arg[4];
		if(arg[3] == 1)
			j = wcmem_get_variable(arg[4]);
		/* Apply */
		if(i != 0)
		{
			hr = wchero_get(i);
			if(arg[2])
				hr->remove_skill(j);
			else
				hr->add_skill(j);
		}
		else
		{
			for(k = 0;k < wcparty_size();k++)
			{
				hr = wchero_get(wcparty_get(k));
				if(arg[2])
					hr->remove_skill(j);
				else
					hr->add_skill(j);
			}
		}
	}
	/* Battle animation */
	if(id == COMMAND_BATTLE_ANIMATION)
	{
		/* Get entity */
		if(arg[1] == EVENT_PLAYER)
			targ = wcplayer_get();
		if(arg[1] == EVENT_ME)
			targ = ev;
		if(arg[1] < 10000)
			targ = (rpgevent*)wclevel_get_event(arg[1]);
		/* Find position */
		if(!targ->is_tile())
		{
			i = targ->get_screen_x()+12;
			j = targ->get_screen_y()+16;
		}
		else
		{
			i = targ->get_screen_x()+8;
			j = targ->get_screen_y()+8;
		}
		/* Global */
		if(arg[3])
		{
			i = WCRPG_WIDTH/2;
			j = WCRPG_HEIGHT/2;
		}
		/* Play it */
		wcanimation_play(arg[0],i,j,32,targ);
		/* Wait */
		if(arg[2])
		{
			state->sleep(0.1f*(float)(wcanimation_length(arg[0]))/4.0f);
			return 0;
		}
	}
	/* Flashing */
	if(id == COMMAND_FLASH_EVENT)
	{
		/* Get entity */
		if(arg[0] == EVENT_PLAYER)
			targ = wcplayer_get();
		if(arg[0] == EVENT_ME)
			targ = ev;
		if(arg[0] < 10000)
			targ = (rpgevent*)wclevel_get_event(arg[0]);
		/* Flash it */
		targ->flash(arg[1]*8,arg[2]*8,arg[3]*8,(float)(arg[4])/31.0f,arg[5]);
		/* Wait */
		if(arg[6])
		{
			state->sleep(0.1f*(float)(arg[5]));
			return 0;
		}
	}
	/* Flashing screen */
	if(id == COMMAND_FLASH_SCREEN)
	{
		/* Stop flashing */
		if(arg[6] == 2)
		{
			/* Halt */
			wcfx_flash(0,0,0,0.0f,0,0);
		}
		else
		{
			/* Flash the screen */
			wcfx_flash(arg[0]*8,arg[1]*8,arg[2]*8,(float)(arg[3])/32.0f,arg[4],arg[6]);
			/* Wait */
			if(arg[5] && !arg[6])
			{
				state->sleep(0.1f*(float)(arg[4]));
				return 0;
			}
		}
	}
	/* Play music */
	if(id == COMMAND_PLAY_MUSIC)
	{
		/* Play a song */
		if(wccommand_play_music)
			delete wccommand_play_music;
		wccommand_play_music = new music(str,arg[1],arg[2],arg[3],arg[0]);
		wcsound_music_play(wccommand_play_music);
	}
	/* Fade music */
	if(id == COMMAND_FADE_MUSIC)
	{
		/* Fade out music */
		wcsound_set_volume_slide(1.0f,0.0f,arg[0]);
	}
	/* Remember song */
	if(id == COMMAND_MEMORIZE_MUSIC)
	{
		/* Memorize old memo */
		mus = wccommand_memo_music;
		/* Make memo */
		wccommand_memo_music = new music(wcsound_get_music_name(),wcsound_get_music_volume(),wcsound_get_music_tempo(),wcsound_get_music_balance(),wcsound_get_music_leadin());
		/* Then delete old */
		if(mus)
			delete mus;
	}
	/* Recall song */
	if(id == COMMAND_RECALL_MUSIC)
	{
		/* Make old song and play it */
		if(wccommand_memo_music)
			wcsound_music_play(wccommand_memo_music);
	}
	/* Input processing */
	if(id == COMMAND_KEY_INPUT)
	{
		/* Variable to store result into */
		i = arg[0];
		/* Timer */
		if(arg[8])
			state->start_time();
		/* Result */
		j = 0;
		if(!arg[1] || state->check_input_key())
		{
			/* Evaluate on arrow keys */
			if(arg[13] && wcinput_key(WCINPUT_UP))
				j = WCINPUT_UP;
			if(arg[12] && wcinput_key(WCINPUT_RIGHT))
				j = WCINPUT_RIGHT;
			if(arg[11] && wcinput_key(WCINPUT_LEFT))
				j = WCINPUT_LEFT;
			if(arg[10] && wcinput_key(WCINPUT_DOWN))
				j = WCINPUT_DOWN;
			/* Evaluate on main keys */
			if(arg[3] && wcinput_key(WCINPUT_OK))
				j = WCINPUT_OK;
			if(arg[4] && wcinput_key(WCINPUT_CANCEL))
				j = WCINPUT_CANCEL;
			if(arg[9] && wcinput_key(WCINPUT_SHIFT))
				j = WCINPUT_SHIFT;
			/* Evaluate numpad */
			if(arg[5])
			{
				/* Get number */
				for(k = WCINPUT_0;k <= WCINPUT_9;k++)
				{
					if(wcinput_key(k))
					{
						j = k;
						break;
					}
				}
			}
			/* Evaluate special codes */
			if(arg[6])
			{
				if(wcinput_key(WCINPUT_PLUS))
					j = WCINPUT_PLUS;
				if(wcinput_key(WCINPUT_MINUS))
					j = WCINPUT_MINUS;
				if(wcinput_key(WCINPUT_MULTIPLY))
					j = WCINPUT_MULTIPLY;
				if(wcinput_key(WCINPUT_DIVIDE))
					j = WCINPUT_DIVIDE;
				if(wcinput_key(WCINPUT_PERIOD))
					j = WCINPUT_PERIOD;
			}
		}
		/* Wait for key press? */
		if(arg[1] && j == 0)
		{
			return 0;
		}
		else
		{
			wcmem_set_variable(i,j,me);
			if(arg[8])
				wcmem_set_variable(arg[7],(var)state->end_time(),me);
		}
	}
	/* Pan the screen */
	if(id == COMMAND_PAN_SCREEN)
	{
		/* Lock screen */
		if(arg[0] == 0)
			wclevel_camera_lock(1);
		/* Unlock screen */
		if(arg[0] == 1)
			wclevel_camera_lock(0);
		/* Dir */
		af = 0;
		bf = 0;
		if(arg[1] == 0)
		{
			af = 0;
			bf = -1;
		}
		if(arg[1] == 1)
		{
			af = 1;
			bf = 0;
		}
		if(arg[1] == 2)
		{
			af = 0;
			bf = 1;
		}
		if(arg[1] == 3)
		{
			af = -1;
			bf = 0;
		}
		/* Multiply with tiles */
		af *= 16;
		bf *= 16;
		/* Pan length */
		af *= arg[2];
		bf *= arg[2];
		/* Set pan */
		if(arg[0] == 2)
		{
			if(arg[3] == 1)
				wclevel_camera_pan(af,bf,SPEED_EIGTH);
			if(arg[3] == 2)
				wclevel_camera_pan(af,bf,SPEED_QUARTER);
			if(arg[3] == 3)
				wclevel_camera_pan(af,bf,SPEED_HALF);
			if(arg[3] == 4)
				wclevel_camera_pan(af,bf,SPEED_NORMAL);
			if(arg[3] == 5)
				wclevel_camera_pan(af,bf,SPEED_2X);
			if(arg[3] == 6)
				wclevel_camera_pan(af,bf,SPEED_4X);
		}
		/* Reset pan */
		if(arg[0] == 3)
		{
			if(arg[3] == 1)
				wclevel_camera_reset_pan(SPEED_EIGTH);
			if(arg[3] == 2)
				wclevel_camera_reset_pan(SPEED_QUARTER);
			if(arg[3] == 3)
				wclevel_camera_reset_pan(SPEED_HALF);
			if(arg[3] == 4)
				wclevel_camera_reset_pan(SPEED_NORMAL);
			if(arg[3] == 5)
				wclevel_camera_reset_pan(SPEED_2X);
			if(arg[3] == 6)
				wclevel_camera_reset_pan(SPEED_4X);
		}
		/* Halt on pan */
		if(arg[4] && arg[0] >= 2)
		{
			halt = COMMAND_HALT_ON_PAN;
			return 0;
		}
	}
	/* Return! */
	if(id == COMMAND_RETURN)
	{
		state->return_end();
		return 0;
	}
	/* Change name */
	if(id == COMMAND_CHANGE_HERO_NAME)
	{
		hr = wchero_get(arg[0]);
		hr->set_name(str);
	}
	/* Change title */
	if(id == COMMAND_CHANGE_HERO_TITLE)
	{
		hr = wchero_get(arg[0]);
		hr->set_title(str);
	}
	/* Start combat */
	if(id == COMMAND_START_COMBAT)
	{
		/* Pick a fight with this enemy */
#ifdef NDEBUG
		wcrpg_error("Warning: The battle system is currently unfinished and will likely crash");
#endif
		wcbattle_start(arg[1],wcplayer_get()->get_terrain());
	}
	/* Erase screen */
	if(id == COMMAND_ERASE_SCREEN)
	{
		/* Erase the screen */
		if(arg[0] == -1)
			wctrans_erase(wcsystem_get_map_trans(0));
		else
			wctrans_erase(arg[0]);
		wctrans_set_teleport_mode(1);
	}
	/* Show screen */
	if(id == COMMAND_SHOW_SCREEN)
	{
		/* Show the screen */
		if(arg[0] == -1)
			wctrans_show(wcsystem_get_map_trans(1));
		else
			wctrans_show(arg[0]);
		wctrans_set_teleport_mode(0);
	}
	/* Get terrain */
	if(id == COMMAND_GET_TERRAIN)
	{
		/* Get the terrain ID position */
		if(arg[0])
		{
			i = wcmem_get_variable(arg[1],me);
			j = wcmem_get_variable(arg[2],me);
		}
		else
		{
			i = arg[1];
			j = arg[2];
		}
		/* Then get it */
		k = wclevel_get_terrain(i,j);
		/* Then set it IF IT IS VALID */
		if(k)
			wcmem_set_variable(arg[3],k,me);
	}
	/* Get event */
	if(id == COMMAND_GET_EVENT)
	{
		/* Get the terrain ID position */
		if(arg[0])
		{
			i = wcmem_get_variable(arg[1],me);
			j = wcmem_get_variable(arg[2],me);
		}
		else
		{
			i = arg[1];
			j = arg[2];
		}
		/* Then get it */
		k = wclevel_get_event_id(i,j);
		/* Then set it if k was found */
		wcmem_set_variable(arg[3],k,me);
	}
	/* Swap tile */
	if(id == COMMAND_SWAP_TILE)
	{
		/* Swap tiles */
		wctileset_set_sub(arg[1]+arg[0]*TILESET_HALF+18,arg[2]);
	}
	/* Place event */
	if(id == COMMAND_PLACE_EVENT)
	{
		/* Get entity */
		if(arg[0] == EVENT_PLAYER)
			targ = wcplayer_get();
		if(arg[0] == EVENT_ME)
			targ = ev;
		if(arg[0] < 10000)
			targ = (rpgevent*)wclevel_get_event(arg[0]);
		/* Get x and y */
		if(arg[1])
		{
			i = wcmem_get_variable(arg[2],me);
			j = wcmem_get_variable(arg[3],me);
		}
		else
		{
			i = arg[2];
			j = arg[3];
		}
		/* Remember face */
		k = targ->get_direction();
		/* Put them there */
		targ->place(i,j);
		/* Make them face */
		if(arg[4])
		{
			/* Face new */
			targ->face(arg[4]-1);
		}
		else
		{
			/* Face like they were */
			targ->face(k);
		}
	}
	/* Timer */
	if(id == COMMAND_TIMER)
	{
		/* Timer op */
		if(arg[0] == 0)
		{
			/* Set */
			if(arg[1] == 0)
				i = arg[2];
			else
				i = wcmem_get_variable(arg[2],me);
			wctimer_set(arg[5],i);
		}
		if(arg[0] == 1)
		{
			/* Start */
			wctimer_start(arg[5],arg[3],arg[4]);
		}
		if(arg[0] == 2)
		{
			/* Stop */
			wctimer_stop(arg[5]);
		}
	}
	/* Game over */
	if(id == COMMAND_GAME_OVER)
	{
		/* Game over.. >.< */
		if(!wctrans_active() && wctrans_get_screen_state() == WCTRANS_SCREEN_SHOW)
			wctrans_erase(WCTRANS_FADE);
		if(!wctrans_active() && wctrans_get_screen_state() == WCTRANS_SCREEN_HIDDEN)
		{
			/* Get rid of old pictures */
			wcpicture_wipe();
			/* Exit normal game for game over */
			wctrans_show(WCTRANS_FADE);
			wcsystem_play_gameover_music();
			wcnucleus_tint(1.0f,1.0f,1.0f,1.0f,0.0f);
			wcnucleus_set_mode(WCRPG_GAME_OVER);
		}
		return 0;
	}
	/* Change BG */
	if(id == COMMAND_CHANGE_BG)
	{
		/* Just change it */
		wclevel_change_parallax(str,arg[1],arg[4],arg[5],arg[0],arg[2],arg[3]);
	}
	/* Change system graphic */
	if(id == COMMAND_CHANGE_SYSTEM_GRAPHIC)
	{
		/* Swap out texture */
		wcsystem_set_graphic(str);
		/* Set settings */
		wcsystem_set_graphic_tile(arg[0]);
		wcsystem_set_font(arg[1]);
		/* Regenerate all fonts */
		wcwindow_generate_all_fonts();
	}
	/* Swap event locations */
	if(id == COMMAND_SWAP_EVENT_LOCATIONS)
	{
		/* From event */
		if(arg[0] == EVENT_ME)
			sorc = ev;
		else
			sorc = (rpgevent*)wclevel_get_event(arg[0]);
		/* To event */
		if(arg[1] == EVENT_ME)
			targ = ev;
		else
			targ = (rpgevent*)wclevel_get_event(arg[1]);
		/* Memorize sorc */
		i = sorc->get_pos_x();
		j = sorc->get_pos_y();
		/* Place sorc at targ */
		sorc->place(targ->get_pos_x(),targ->get_pos_y());
		targ->place(i,j);
	}
	/* Advance */
	return 1;
}