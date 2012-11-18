/*
	RPG Program Executor
*/

/* Include */
#include <stdio.h>
#include <memory.h>
#include "wcevent.h"
#include "wcrun.h"
#include "wctext.h"
#include "wcrpg.h"
#include "wcplayer.h"
#include "wccommon.h"
#include "wclevel.h"
#include "wcmanage.h"
#include "wcsaveio.h"

#include <string>


/* Globals */
rpgevent *wcrun_bridge_event = 0;

/* Sets a bridge event that will be handled */
void wcrun_set_bridge_event(void *ev)
{
	/* Get rid of old */
	if(wcrun_bridge_event)
		delete wcrun_bridge_event;
	wcrun_bridge_event = (rpgevent*)ev;
	wcrun_bridge_event->sever();
}

/* Handles bridge event */
void wcrun_handle()
{
	rpgevent *ev;
	if(!wcrun_bridge_event)
		return;
	ev = (rpgevent*)wcrun_bridge_event;
	ev->handle();
}

/* Cleans up running engine */
void wcrun_end()
{
	/* Get rid of old */
	if(wcrun_bridge_event)
		delete wcrun_bridge_event;
}

/* Make new state */
rpgstate :: rpgstate(void *who)
{
	int i;
	/* Set attached */
	me = who;
	/* Reset */
	memset(current_page,0,sizeof(current_page));
	memset(counter,0,sizeof(counter));
	memset(common,0,sizeof(common));
	memset(them,0,sizeof(them));
	memset(input_autofire,0,sizeof(input_autofire));
	for(i = 0;i < STACK_SIZE;i++)
		common[i] = -1;
	stack = 0;
	them[0] = me;
	waitclk = 0.0f;
	active = 0;
	nofocus = 0;
	playerdelay = 0;
	wait_autofire = 0;
	consecutive = 0;
	distance = 0;
	time_sample = 0.0f;
	time_active = 0;
}

/* Calls common event */
void rpgstate :: call_common_event(int id)
{
	/* Advance */
	counter[stack]++;
	/* Stack up */
	stack++;
	if(stack >= STACK_SIZE)
		wcrpg_error("Stack overflow on calling common event (%d)",id);
	/* Register */
	current_page[stack] = 0;
	counter[stack] = 0;
	common[stack] = id;
	them[stack] = me;
}

/* Calls map event */
void rpgstate :: call_event(int id,int pg)
{
	rpgevent *oev;
	/* Seek */
	oev = (rpgevent*)wclevel_get_event(id);
	/* Advance */
	counter[stack]++;
	/* Stack up */
	stack++;
	if(stack >= STACK_SIZE)
		wcrpg_error("Stack overflow on calling map event (%s)",oev->get_name());
	/* Register */
	current_page[stack] = pg;
	counter[stack] = -1;
	common[stack] = -1;
	them[stack] = oev;
}

/* Sets as common state */
void rpgstate :: set_common_state(int id)
{
	/* Must be initial stack */
	if(stack != 0)
		return;
	/* Set them and common */
	them[stack] = 0;
	common[stack] = id-1;
}

/* Current length */
int rpgstate :: current_length()
{
	/* Event */
	if(common[stack] < 0)
		return ((rpgevent*)them[stack])->get_page_length(current_page[stack]);
	/* Common */
	return wccommon_length(common[stack]+1);
}

/* Current page command */
command *rpgstate :: current_command(int i)
{
	/* Event */
	if(common[stack] < 0)
		return ((rpgevent*)them[stack])->get_command(i,current_page[stack]);
	/* Common */
	return wccommon_get(common[stack]+1,i);
}

/* Fetch */
command *rpgstate :: fetch()
{
	/* Event */
	if(common[stack] < 0)
		return ((rpgevent*)them[stack])->get_command(counter[stack],current_page[stack]);
	/* Common */
	return wccommon_get(common[stack]+1,counter[stack]);
}

/* End of the page */
int rpgstate :: at_end()
{
	/* Event */
	if(common[stack] < 0)
		return (counter[stack] >= ((rpgevent*)them[stack])->get_page_length(current_page[stack]));
	/* Common */
	return (counter[stack] >= wccommon_length(common[stack]+1));
}

/* Find label */
int rpgstate :: find_label(int lb)
{
	int i;
	command *lbcmd;
	/* Go from beginning to end of page */
	for(i = 0;i < current_length();i++)
	{
		/* Fetch */
		lbcmd = current_command(i);
		/* Is label */
		if(lbcmd->get_id() == COMMAND_LABEL)
		{
			if(lbcmd->get_argument(0) == lb)
				return i; /* Label is here */
		}
	}
	/* Label not found */
	return -1;
}

/* Find choice */
int rpgstate :: find_choice(int cur,int lev,int op)
{
	int i;
	command *ccmd;
	/* Go from this position to end of page */
	for(i = cur;i < current_length();i++)
	{
		/* Fetch */
		ccmd = current_command(i);
		/* Is option */
		if(ccmd->get_id() == COMMAND_CHOICE_OPTION)
		{
			if(ccmd->get_level() == lev)
			{
				if(ccmd->get_argument(0) == op)
					return i; /* Option is here */
			}
		}
	}
	/* Not found */
	return -1;
}

/* Find ending */
int rpgstate :: find_end(int cur,int lev)
{
	int i;
	command *ecmd;
	/* Go from this position to end of page */
	for(i = cur;i < current_length();i++)
	{
		/* Fetch */
		ecmd = current_command(i);
		/* Is ending */
		if(ecmd->is_end() && ecmd->get_level() == lev-1)
			return i;
	}
	/* Not found */
	return -1;
}

/* Find else */
int rpgstate :: find_else(int cur,int lev)
{
	int i;
	command *ecmd;
	/* Go from this position to end of page */
	for(i = cur;i < current_length();i++)
	{
		/* Fetch */
		ecmd = current_command(i);
		/* Is ending */
		if(ecmd->get_id() == COMMAND_ELSE && ecmd->get_level() == lev)
			return i;
	}
	/* Not found */
	return -1;
}

/* Find while end */
int rpgstate :: find_while_end(int cur,int lev)
{
	int i;
	command *ecmd;
	/* Go from this position to end of page */
	for(i = cur;i < current_length();i++)
	{
		/* Fetch */
		ecmd = current_command(i);
		/* Is ending */
		if(ecmd->get_id() == COMMAND_END_WHILE && ecmd->get_level() <= lev)
			return i;
	}
	/* Not found */
	return -1;
}

/* Find while backwards */
int rpgstate :: find_while(int cur,int lev)
{
	int i;
	command *ecmd;
	/* Go from this position to end of page */
	for(i = cur;i >= 0;i--)
	{
		/* Fetch */
		ecmd = current_command(i);
		/* Is ending */
		if(ecmd->get_id() == COMMAND_WHILE && ecmd->get_level() == lev)
			return i;
	}
	/* Not found */
	return -1;
}

/* Jump to ending */
void rpgstate :: jump_to_end()
{
	int cpos;
	command *ecmd;
	ecmd = current_command(counter[stack]);
	cpos = find_end(counter[stack],ecmd->get_level());
	if(cpos == -1)
		return;
	counter[stack] = cpos;
}

/* Jump to else */
void rpgstate :: jump_to_else()
{
	int cpos;
	command *ecmd;
	ecmd = current_command(counter[stack]);
	cpos = find_else(counter[stack],ecmd->get_level());
	if(cpos == -1)
		return;
	counter[stack] = cpos;
}

/* Jump to while */
void rpgstate :: jump_to_while()
{
	int cpos;
	command *ecmd;
	ecmd = current_command(counter[stack]);
	cpos = find_while(counter[stack],ecmd->get_level());
	if(cpos == -1)
		return;
	counter[stack] = cpos;
}

/* Jump to end of while */
void rpgstate :: jump_to_while_end()
{
	int cpos;
	command *ecmd;
	ecmd = current_command(counter[stack]);
	cpos = find_while_end(counter[stack],ecmd->get_level());
	if(cpos == -1)
		return;
	counter[stack] = cpos;
}

/* Jump to option */
void rpgstate :: jump_to_choice(int op)
{
	int cpos;
	command *ccmd;
	ccmd = current_command(counter[stack]);
	cpos = find_choice(counter[stack],ccmd->get_level(),op);
	if(cpos == -1)
		return;
	counter[stack] = cpos;
}

/* Jump to label */
void rpgstate :: jump_to_label(int lb)
{
	int lbpos;
	lbpos = find_label(lb);
	/* Do nothing if label not found */
	if(lbpos == -1)
		return;
	/* Jump */
	counter[stack] = lbpos;
}

/* Return */
int rpgstate :: return_end()
{
	/* Reduce stack */
	them[stack] = 0;
	counter[stack] = 0;
	current_page[stack] = 0;
	common[stack] = -1;
	stack--;
	/* End of stack? */
	if(stack < 0)
	{
		/* Return to initial state */
		them[0] = me;
		stack = 0;
		active = 0;
		return 0;
	}
	return 1;
}

/* Advance */
int rpgstate :: advance()
{
	/* Must be active */
	if(!active)
		return 0;
	/* Move one */
	counter[stack]++;
	distance++;
	consecutive++;
	/* End of current page? */
	if(at_end())
	{
		if(!return_end())
			return 0;
	}
	/* Loop break! */
	if(consecutive >= MAX_LOOP_LENGTH)
	{
		consecutive = 0;
		return 0;
	}
	/* Continue */
	return 1;
}

/* Execute */
void rpgstate :: handle()
{
	command *cmd;
	float ts;
	/* If to be activated? */
	if(active)
	{
		/* Get ts */
		ts = wcrpg_get_time_step();
		/* Clock runs */
		if(time_active)
			time_sample += ts/16.0f*10.0f;
		/* Wait for player to stop? */
		if(playerdelay && wcplayer_get()->get_state() == EVENT_MOVING)
			return;
		/* No-delay any longer */
		playerdelay = 0;
		/* Wait clock time? */
		if(waitclk > 0)
		{
			waitclk -= ts;
			/* Resume */
			if(waitclk < 0)
			{
				waitclk = 0;
				advance();
			}
		}
		else
		{
			/* Execute */
			for(;;)
			{
				/* At end? */
				if(at_end())
				{
					advance();
					break;
				}
				/* Fetch */
				cmd = fetch();
				/* Decode and Execute */
				if(cmd->run(me,this))
				{
					/* Increment program counter */
					if(!advance())
						break;
				}
				else
				{
					/* Give up program control to the next event since a halt was reached */
					break;
				}
			}
			/* Reset consecutive */
			consecutive = 0;
		}
	}
}

/* Delay until hero is done */
void rpgstate :: delay_until_hero_is_finished_moving()
{
	consecutive = 0;
	playerdelay = 1;
}

/* Sets no focus */
void rpgstate :: prevent_focus()
{
	nofocus = 1;
}

/* Run */
void rpgstate :: run()
{
	rpgevent *ev;
	ev = (rpgevent*)me;
	/* Trip the switch */
	active = 1;
	/* Set autofires */
	wait_autofire = 1;
	/* Set page */
	if(me)
		current_page[0] = ev->get_page_id();
}

/* Is running? */
int rpgstate :: is_running()
{
	return active;
}

/* Sleep */
void rpgstate :: sleep(float t)
{
	consecutive = 0;
	if(t == 0)
		advance();
	else
		waitclk = t*16.0f;
}

/* Autofire */
int rpgstate :: check_ok_key()
{
	/* Reset if key let go */
	if(!wcinput_ok())
		wait_autofire = 0;
	/* Halt always once fired until key release */
	if(wait_autofire)
		return 0;
	/* Key press */
	if(wcinput_ok() && !wait_autofire)
	{
		wait_autofire = 1;
		wcplayer_trip_examine_auto_fire();
		return 1;
	}
	/* No key press */
	return 0;
}

/* Autofire input */
int rpgstate :: check_input_key()
{
	int inputs[WCINPUT_NUM_CODES];
	int i,ret;
	/* Get input */
	wcinput_vector(inputs);
	/* Release autofire */
	for(i = 0;i < WCINPUT_NUM_CODES;i++)
	{
		if(!inputs[i])
			input_autofire[i] = 0;
	}
	/* Any active past the autofire? */
	ret = 0;
	for(i = 0;i < WCINPUT_NUM_CODES;i++)
	{
		if(inputs[i] && !input_autofire[i])
		{
			ret = 1;
			input_autofire[i] = 1;
		}
	}
	/* Return state */
	return ret;
}

/* Start measuring time */
void rpgstate :: start_time()
{
	/* Clock already running */
	if(time_active)
		return;
	/* Begin */
	time_active = 1;
	time_sample = 0.0f;
}

/* End measuring time */
float rpgstate :: end_time()
{
	/* End */
	time_active = 0;
	return time_sample;
}

/* Write event ID */
void wcrun_save_event_reference(void *v)
{
	rpgevent *ev;
	ev = (rpgevent*)v;
	/* Either ID or zero */
	if(!v)
		wcsaveio_write_num(0);
	else
		wcsaveio_write_num(ev->get_id());
}

/* Read event ID and return event pointer */
rpgevent *wcrun_load_event_reference()
{
	int id;
	/* Read ID */
	id = wcsaveio_read_num();
	/* Return */
	if(!id)
		return 0;
	else
		return (rpgevent*)wclevel_get_event(id);
	/* Error? */
	return 0;
}

/* Save program */
void rpgstate :: save_state(FILE *f)
{
	int i;
	/* Write me */
	wcrun_save_event_reference(me);
	/* Write stack size */
	wcsaveio_write_num(STACK_SIZE);
	/* Write stack */
	for(i = 0;i < STACK_SIZE;i++)
	{
		wcrun_save_event_reference(them[i]);
		wcsaveio_write_num(current_page[i]);
		wcsaveio_write_num(counter[i]);
		wcsaveio_write_num(common[i]);
	}
	/* Write status */
	wcsaveio_write_num(stack);
	wcsaveio_write_num(active);
	wcsaveio_write_num(distance);
	wcsaveio_write_num(playerdelay);
	wcsaveio_write_num(nofocus);
	/* Write clock */
	wcsaveio_write_float(waitclk);
	wcsaveio_write_num(wait_autofire);
	wcsaveio_write_float(time_sample);
	wcsaveio_write_num(time_active);
	/* Write input size */
	wcsaveio_write_num(WCINPUT_NUM_CODES);
	/* Write input */
	for(i = 0;i < WCINPUT_NUM_CODES;i++)
		wcsaveio_write_num(input_autofire[i]);
}

/* Load program */
void rpgstate :: load_state(FILE *f)
{
	int i,len;
	/* Read me */
	me = wcrun_load_event_reference();
	/* Read stack size */
	len = wcsaveio_read_num();
	/* Read stack */
	for(i = 0;i < len;i++)
	{
		if(i >= STACK_SIZE)
		{
			/* Read extra data */
			wcrun_load_event_reference();
			wcsaveio_read_num();
			wcsaveio_read_num();
			wcsaveio_read_num();
		}
		else
		{
			/* Read data */
			them[i] = wcrun_load_event_reference();
			current_page[i] = wcsaveio_read_num();
			counter[i] = wcsaveio_read_num();
			common[i] = wcsaveio_read_num();
		}
	}
	/* Read status */
	stack = wcsaveio_read_num();
	active = wcsaveio_read_num();
	distance = wcsaveio_read_num();
	playerdelay = wcsaveio_read_num();
	nofocus = wcsaveio_read_num();
	/* Read clock */
	waitclk = wcsaveio_read_float();
	wait_autofire = wcsaveio_read_num();
	time_sample = wcsaveio_read_float();
	time_active = wcsaveio_read_num();
	/* Read input size */
	len = wcsaveio_read_num();
	/* Read input */
	for(i = 0;i < len;i++)
	{
		if(i >= WCINPUT_NUM_CODES)
			wcsaveio_read_num();
		else
			input_autofire[i] = wcsaveio_read_num();
	}
}