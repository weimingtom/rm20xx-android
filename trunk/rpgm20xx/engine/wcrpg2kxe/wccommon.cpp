/*
	Common Events
*/

/* Includes */
#include <malloc.h>
#include "wccommon.h"
#include "wctype.h"
#include "wcrpg.h"
#include "wcmem.h"
#include "wcsaveio.h"

/* Globals */
arraylist wccommon_events;

/* Load event */
comevent :: comevent(FILE *f)
{
	int rlen,count;
	unsigned char code;
	command *cmd;
	/* My ID */
	id = wcint_read_file(f,&rlen);
	name = 0;
	trigger = 0;
	trigger_switch = 0;
	has_trigger = 0;
	count = 0;
	state = new rpgstate(0);
	command_list = wcarray_new();
	/* And the rest of my datas */
	for(;;)
	{
		/* Get code */
		fread(&code,1,1,f);
		/* Nullify */
		if(!code)
			break;
		/* Header */
		if(code == 0x01)
			name = wcstr_read_file(f,&rlen);
		if(code == 0x0B)
		{
			wcint_read_file(f,&rlen);
			trigger = wcint_read_file(f,&rlen);
		}
		if(code == 0x0C)
		{
			wcint_read_file(f,&rlen);
			has_trigger = wcint_read_file(f,&rlen);
		}
		if(code == 0x0D)
		{
			wcint_read_file(f,&rlen);
			trigger_switch = wcint_read_file(f,&rlen);
		}
		if(code == 0x15)
		{
			wcint_read_file(f,&rlen);
			count = wcint_read_file(f,&rlen);
		}
		/* Body */
		if(code == 0x16)
		{
			/* Length... */
			wcint_read_file(f,&rlen);
			/* Load program */
			for(;;)
			{
				/* Get codes */
				cmd = new command(f);
				if(cmd->is_stop())
					break;
				wcarray_add(command_list,cmd);
			}
		}
	}
}

/* Collapse event */
comevent :: ~comevent()
{
	int i;
	/* Delete program */
	for(i = 0;i < wcarray_size(command_list);i++)
		delete (command*)wcarray_get(command_list,i);
	wcarray_delete(command_list);
	/* Delete name */
	free(name);
}

/* Get command */
command *comevent :: get_command(int i)
{
	return (command*)wcarray_get(command_list,i);
}

/* Run */
void comevent :: run()
{
	/* Always common if started like this */
	state->set_common_state(id);
	/* Go! */
	state->run();
}

/* Get length */
int comevent :: get_length()
{
	return wcarray_size(command_list);
}

/* Handle */
void comevent :: handle()
{
	/* Run program, but do not reboot */
	if(state->is_running())
		state->handle();
	/* Stop */
	if(trigger != WCCOMMON_PARALLEL && trigger != WCCOMMON_AUTO)
		return;
	/* Am I even supposed to be running? */
	if(has_trigger && !wcmem_get_switch(trigger_switch))
		return;
	/* Always common */
	state->set_common_state(id);
	/* Reboot always */
	if(!state->is_running())
		state->run();
}

/* Is active auto */
int comevent :: is_active_auto()
{
	/* Auto eveeents */
	if(trigger == WCCOMMON_AUTO && state->is_running())
		return 1;
	if(trigger == WCCOMMON_AUTO && !has_trigger)
		return 1;
	if(trigger == WCCOMMON_AUTO && has_trigger && wcmem_get_switch(trigger_switch))
		return 1;
	return 0;
}

/* Save state */
void comevent :: save_state(FILE *f)
{
	wcsaveio_start(f);
	state->save_state(f);
	wcsaveio_end();
}

/* Load state */
void comevent :: load_state(FILE *f)
{
	wcsaveio_read(f);
	state->load_state(f);
	wcsaveio_next();
}

/* Returns true if there are auto common events active */
int wccommon_auto_active()
{
	int i;
	for(i = 0;i < wcarray_size(wccommon_events);i++)
	{
		if(((comevent*)wcarray_get(wccommon_events,i))->is_active_auto())
			return 1;
	}
	return 0;
}

/* Handles all */
void wccommon_handle()
{
	int i;
	/* Handle all */
	for(i = 0;i < wcarray_size(wccommon_events);i++)
		((comevent*)wcarray_get(wccommon_events,i))->handle();
}

/* Initialize and build */
void wccommon_load_all(FILE *f,int len)
{
	int count,i,rlen;
	comevent *com;
	/* Allocate list */
	wccommon_events = wcarray_new();
	/* Find out how many */
	count = wcint_read_file(f,&rlen);
	/* Load them */
	for(i = 0;i < count;i++)
	{
		/* Read + add */
		com = new comevent(f);
		wcarray_add(wccommon_events,com);
	}
}

/* Clean */
void wccommon_free_all()
{
	int i;
	/* Release list */
	for(i = 0;i < wcarray_size(wccommon_events);i++)
		delete (comevent*)wcarray_get(wccommon_events,i);
	wcarray_delete(wccommon_events);
}

/* Return a command */
command *wccommon_get(int id,int ix)
{
	comevent *com;
	com = (comevent*)wcarray_get(wccommon_events,id-1);
	return com->get_command(ix);
}

/* Return size */
int wccommon_length(int id)
{
	comevent *com;
	com = (comevent*)wcarray_get(wccommon_events,id-1);
	return com->get_length();
}

/* Runs common event */
void wccommon_run(int id)
{
	comevent *com;
	com = (comevent*)wcarray_get(wccommon_events,id-1);
	com->run();
}

/* Save all common events */
void wccommon_save(FILE *f)
{
	int i;
	/* Write count */
	wcsaveio_start(f);
	wcsaveio_write_num(wcarray_size(wccommon_events));
	wcsaveio_end();
	/* Write all */
	for(i = 0;i < wcarray_size(wccommon_events);i++)
		((comevent*)wcarray_get(wccommon_events,i))->save_state(f);
}

/* Load all common events */
void wccommon_load(FILE *f)
{
	int i,len;
	/* Get count */
	wcsaveio_read(f);
	len = wcsaveio_read_num();
	wcsaveio_next();
	/* Read all */
	for(i = 0;i < len;i++)
	{
		if(i >= wcarray_size(wccommon_events))
		{
			/* Skip over */
			wcsaveio_read(f);
			wcsaveio_next();
		}
		else
		{
			/* Read in */
			((comevent*)wcarray_get(wccommon_events,i))->load_state(f);
		}
	}
}