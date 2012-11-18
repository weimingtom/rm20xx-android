/*
	Event Page
*/

/* Includes */
#include <malloc.h>
#include "wctype.h"
#include "wcpage.h"
#include "wcrpg.h"
#include "wcparty.h"
#include "wcmem.h"
#include "wcitem.h"
#include "wctimer.h"

#include "android\log.h"

/* Returns starting condition name */
char *wcpage_get_start_name(int code)
{
	if(code == START_KEY)
		return "Examine";
	if(code == START_IMPACT)
		return "Impact";
	if(code == START_TOUCH)
		return "Touch";
	if(code == START_AUTO)
		return "Auto";
	if(code == START_PARALLEL)
		return "Parallel";
	return "Unknown";
}

/* Returns event layer */
char *wcpage_get_layer_name(int code)
{
	if(code == EVENT_BELOW)
		return "Below";
	if(code == EVENT_SOLID)
		return "Solid";
	if(code == EVENT_ABOVE)
		return "Above";
	return "Unknown";
}

/* Returns comparator */
char *wcpage_get_variable_test(int code)
{
	if(code == COMP_EQUAL)
		return "equal to";
	if(code == COMP_GREATER_EQUAL)
		return "greater than or equal to";
	if(code == COMP_LESS_EQUAL)
		return "less than or equal to";
	if(code == COMP_GREATER)
		return "greater than";
	if(code == COMP_LESS)
		return "less than";
	if(code == COMP_NOT)
		return "not";
	return "somehow related to";
}

/* Process entry */
int page :: apply(int code,FILE *f)
{
	int len,rlen;
	/* Chracater set */
	if(code == 0x15)
	{
		charset = wcstr_read_file(f,&rlen);
		return 1;
	}
	/* Tile selection */
	if(code == 0x16)
	{
		t = 0;
		len = wcint_read_file(f,&rlen);
		fread(&t,1,len,f);
		return 1;
	}
	/* Transparent */
	if(code == 0x19)
	{
		len = wcint_read_file(f,&rlen);
		transparent = wcint_read_file(f,&rlen);
		return 1;
	}
	/* Start */
	if(code == 0x21)
	{
		len = wcint_read_file(f,&rlen);
		trigger = wcint_read_file(f,&rlen);
		return 1;
	}
	/* Layer */
	if(code == 0x22)
	{
		len = wcint_read_file(f,&rlen);
		layer = wcint_read_file(f,&rlen);
		return 1;
	}
	/* Start direction */
	if(code == 0x17)
	{
		len = wcint_read_file(f,&rlen);
		start_dir = wcint_read_file(f,&rlen);
		return 1;
	}
	/* Start foot */
	if(code == 0x18)
	{
		len = wcint_read_file(f,&rlen);
		start_foot = wcint_read_file(f,&rlen);
		return 1;
	}
	/* Move type */
	if(code == 0x1F)
	{
		len = wcint_read_file(f,&rlen);
		move_type = wcint_read_file(f,&rlen);
		return 1;
	}
	/* Anim type */
	if(code == 0x24)
	{
		len = wcint_read_file(f,&rlen);
		anim_type = wcint_read_file(f,&rlen);
		return 1;
	}
	/* Speed */
	if(code == 0x25)
	{
		len = wcint_read_file(f,&rlen);
		move_speed = wcint_read_file(f,&rlen);
		move_speed--;
		return 1;
	}
	/* Frequency */
	if(code == 0x20)
	{
		len = wcint_read_file(f,&rlen);
		move_freq = wcint_read_file(f,&rlen);
		return 1;
	}
	return 0;
}

/* Apply condition */
int page :: condition(int code,FILE *f)
{
	int len,rlen;
	/* Flags */
	if(code == 0x01)
	{
		len = wcint_read_file(f,&rlen);
		cond_flags = wcint_read_file(f,&rlen);
		return 1;
	}
	/* Switches */
	if(code == 0x02)
	{
		len = wcint_read_file(f,&rlen);
		switch_a_id = wcint_read_file(f,&rlen);
		return 1;
	}
	if(code == 0x03)
	{
		len = wcint_read_file(f,&rlen);
		switch_b_id = wcint_read_file(f,&rlen);
		return 1;
	}
	/* Variables */
	if(code == 0x04)
	{
		len = wcint_read_file(f,&rlen);
		variable_id = wcint_read_file(f,&rlen);
		return 1;
	}
	if(code == 0x05)
	{
		len = wcint_read_file(f,&rlen);
		variable_comp = wcint_read_file(f,&rlen);
		return 1;
	}
	/* Item */
	if(code == 0x06)
	{
		len = wcint_read_file(f,&rlen);
		item_id = wcint_read_file(f,&rlen);
		return 1;
	}
	/* Hero */
	if(code == 0x07)
	{
		len = wcint_read_file(f,&rlen);
		hero_id = wcint_read_file(f,&rlen);
		return 1;
	}
	/* Timers */
	if(code == 0x08)
	{
		len = wcint_read_file(f,&rlen);
		timer_a_val = wcint_read_file(f,&rlen);
		return 1;
	}
	if(code == 0x09)
	{
		len = wcint_read_file(f,&rlen);
		timer_b_val = wcint_read_file(f,&rlen);
		return 1;
	}
	/* Comparator */
	if(code == 0x0A)
	{
		len = wcint_read_file(f,&rlen);
		variable_test = wcint_read_file(f,&rlen);
		return 1;
	}
	return 0;
}

/* Load page */
page :: page(FILE *f)
{
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "new page");
	unsigned char *d;
	int code,len,rlen;
	command *cmd;
	/* Default */
	charset = 0;
	t = 0;
	prog = 0;
	start_foot = 1;
	prog_len = 0;
	anim_type = 0;
	trigger = START_KEY;
	layer = EVENT_SOLID;
	move_type = 0;
	move_speed = 2;
	move_freq = 3;
	cond_flags = 0;
	mls = 0;
	who = 0;
	command_list = wcarray_new();
	/* Get Id */
	id = wcint_read_file(f,&rlen);
	code = 0;
	fread(&code,1,1,f);
	if(code != 0x02)
	{
		wcrpg_error("Page conditions were expected but not found");
		return;
	}
	len = wcint_read_file(f,&rlen);
	/* Reset conditions */
	switch_a_id = 1;
	switch_b_id = 1;
	variable_id = 1;
	variable_test = 1;
	variable_comp = 0;
	item_id = 1;
	hero_id = 1;
	timer_a_val = 0;
	timer_b_val = 0;
	/* Load conditions */
	for(;;)
	{
		/* Get next code */
		code = 0;
		fread(&code,1,1,f);
		/* End of codes */
		if(!code)
			break;
		/* Condition */
		if(!condition(code,f))
		{
			len = wcint_read_file(f,&rlen);
			d = (unsigned char*)malloc(len);
			fread(d,1,len,f);
			free(d);
		}
	}
	/* Load properties */
	for(;;)
	{
		/* Get code */
		code = 0;
		fread(&code,1,1,f);
		if(code == 0x29 || code == 0x33)
			break; /* Route or program */
		/* Apply */
		if(!apply(code,f))
		{
			len = wcint_read_file(f,&rlen);
			d = (unsigned char*)malloc(len);
			fread(d,1,len,f);
			free(d);
		}
	}
	/* Read moves */
	if(code == 0x29)
	{
		mls = new movelist(f);
		/*len = wcint_read_file(f,&rlen);
		d = (unsigned char*)malloc(len);
		fread(d,1,len,f);
		free(d);*/
		/* Get next code */
		code = 0;
		fread(&code,1,1,f);
	}
	/* Read program */
	if(code == 0x33)
	{
		len = wcint_read_file(f,&rlen);
		len = wcint_read_file(f,&rlen);
		code = 0;
		fread(&code,1,1,f);
		len = wcint_read_file(f,&rlen);
		for(;;)
		{
			cmd = new command(f);
			if(cmd->is_stop())
				break;
			wcarray_add(command_list,cmd);
		}
		/*
		prog_len = len;
		prog = (unsigned char*)malloc(len);
		fread(prog,1,len,f);
		*/
	}
	/* Set frequency */
	if(mls)
		mls->set_freq(move_freq);
	/* End */
	code = 0;
	fread(&code,1,1,f);
	/* Swap moves if not custom */
	if(move_type != MOVE_CUSTOM)
	{
		if(mls)
			delete mls;
		mls = new movelist(move_type);
		mls->set_freq(move_freq);
	}
}

/* Delete page */
page :: ~page()
{
	int i;
	for(i = 0;i < wcarray_size(command_list);i++)
		delete (command*)wcarray_get(command_list,i);
	wcarray_delete(command_list);
	if(mls)
		delete mls;
	free(charset);
	free(prog);
}

/* Report page */
void page :: report(elog l)
{
	wclog_write(l,"Page ");
	wclog_write(l,id);
	wclog_write(l,": ");
	if(charset)
	{
		wclog_write(l,charset);
		wclog_write(l," (");
		wclog_write(l,t);
		wclog_write(l,")");
	}
	else
	{
		wclog_write(l,t);
	}
	wclog_enter(l);
}

/* Get charset */
char *page :: get_charset()
{
	return charset;
}

/* Get tile */
tile page :: get_tile()
{
	return t;
}

/* Disassembles page */
void page :: disassemble(char *filename)
{
	FILE *f;
	int i;
	command *cmd;
	/* Dump it */
	//fopen(filename,"wb");
	if(f)
	{
		/* Movement settings */
		fprintf(f,"Dir: %d Anim: %d Spd: %d Freq: %d",start_dir,anim_type,move_speed,move_freq);
		fprintf(f,"%c",(char)0x0D);
		fprintf(f,"%c",(char)0x0A);
		fprintf(f,"(Frame = %d)",t);
		fprintf(f,"%c",(char)0x0D);
		fprintf(f,"%c",(char)0x0A);
		/* Print settings */
		fprintf(f,"Trigger: %s",wcpage_get_start_name(trigger));
		fprintf(f,"%c",(char)0x0D);
		fprintf(f,"%c",(char)0x0A);
		fprintf(f,"Layer: %s",wcpage_get_layer_name(layer));
		fprintf(f,"%c",(char)0x0D);
		fprintf(f,"%c",(char)0x0A);
		/* Print conditions */
		if(cond_flags&COND_SWITCH_A)
		{
			fprintf(f,"if switch %d is on ",switch_a_id);
			fprintf(f,"%c",(char)0x0D);
			fprintf(f,"%c",(char)0x0A);
		}
		if(cond_flags&COND_SWITCH_B)
		{
			fprintf(f,"if switch %d is on ",switch_b_id);
			fprintf(f,"%c",(char)0x0D);
			fprintf(f,"%c",(char)0x0A);
		}
		if(cond_flags&COND_VARIABLE)
		{
			fprintf(f,"if var %d is %s %d",variable_id,wcpage_get_variable_test(variable_test),variable_comp);
			fprintf(f,"%c",(char)0x0D);
			fprintf(f,"%c",(char)0x0A);
		}
		if(cond_flags&COND_ITEM)
		{
			fprintf(f,"if item %d is held",item_id);
			fprintf(f,"%c",(char)0x0D);
			fprintf(f,"%c",(char)0x0A);
		}
		if(cond_flags&COND_HERO)
		{
			fprintf(f,"if hero %d is in party",hero_id);
			fprintf(f,"%c",(char)0x0D);
			fprintf(f,"%c",(char)0x0A);
		}
		if(cond_flags&COND_TIMER_A)
		{
			fprintf(f,"if timer 1 has reached %ds",timer_a_val);
			fprintf(f,"%c",(char)0x0D);
			fprintf(f,"%c",(char)0x0A);
		}
		if(cond_flags&COND_TIMER_B)
		{
			fprintf(f,"if timer 2 has reached %ds",timer_b_val);
			fprintf(f,"%c",(char)0x0D);
			fprintf(f,"%c",(char)0x0A);
		}
		fprintf(f,"%c",(char)0x0D);
		fprintf(f,"%c",(char)0x0A);
		/* Move list */
		mls->dissasemble(f);
		fprintf(f,"%c",(char)0x0D);
		fprintf(f,"%c",(char)0x0A);
		fprintf(f,"%c",(char)0x0D);
		fprintf(f,"%c",(char)0x0A);
		/* Show commands */
		for(i = 0;i < wcarray_size(command_list);i++)
		{
			cmd = (command*)wcarray_get(command_list,i);
			fprintf(f,"%d\t",i);
			cmd->print(f);
			fprintf(f,"%c",(char)0x0D);
			fprintf(f,"%c",(char)0x0A);
		}
		fclose(f);
	}
}

/* Get layer */
int page :: get_layer()
{
	return layer;
}

/* Get direction */
int page :: get_direction()
{
	return start_dir;
}

/* Get animation */
int page :: get_animation()
{
	return anim_type;
}

/* Get command */
command *page :: get_command(int counter)
{
	/* Wrong move */
	if(counter < 0 || counter >= wcarray_size(command_list))
	{
		wcrpg_error("Invalid command counter (%d)",counter);
		return 0;
	}
	return (command*)wcarray_get(command_list,counter);
}

/* Get length */
int page :: get_length()
{
	return wcarray_size(command_list);
}

/* Get trigger */
int page :: get_trigger()
{
	return trigger;
}

/* Get speed */
int page :: get_speed()
{
	return move_speed;
}

/* Get route */
movelist *page :: get_route()
{
	return mls;
}

/* Evaluate page conditions */
int page :: is_true()
{
	/* Switch A */
	if(cond_flags&COND_SWITCH_A)
	{
		if(!wcmem_get_switch(switch_a_id))
			return 0;
	}
	/* Switch B */
	if(cond_flags&COND_SWITCH_B)
	{
		if(!wcmem_get_switch(switch_b_id))
			return 0;
	}
	/* Variable */
	if(cond_flags&COND_VARIABLE)
	{
		if(variable_test == COMP_EQUAL)
		{
			if(wcmem_get_variable(variable_id,who) != variable_comp)
				return 0;
		}
		if(variable_test == COMP_GREATER_EQUAL)
		{
			if(wcmem_get_variable(variable_id,who) < variable_comp)
				return 0;
		}
		if(variable_test == COMP_LESS_EQUAL)
		{
			if(wcmem_get_variable(variable_id,who) > variable_comp)
				return 0;
		}
		if(variable_test == COMP_GREATER)
		{
			if(wcmem_get_variable(variable_id,who) <= variable_comp)
				return 0;
		}
		if(variable_test == COMP_LESS)
		{
			if(wcmem_get_variable(variable_id,who) >= variable_comp)
				return 0;
		}
		if(variable_test == COMP_NOT)
		{
			if(wcmem_get_variable(variable_id,who) == variable_comp)
				return 0;
		}
	}
	/* Item */
	if(cond_flags&COND_ITEM)
	{
		/* Is true */
		if(wcitem_get(item_id)->get_held() > 0)
			return 1;
		else
			return 0;
	}
	/* Hero */
	if(cond_flags&COND_HERO)
	{
		/* Is true */
		if(wcparty_has(hero_id))
			return 1;
		else
			return 0; /* Not */
	}
	/* Timer A */
	if(cond_flags&COND_TIMER_A)
	{
		/* Is true */
		if(wctimer_get(0) <= timer_a_val)
			return 1;
		else
			return 0; /* Not */
	}
	/* Timer B */
	if(cond_flags&COND_TIMER_B)
	{
		/* Is true */
		if(wctimer_get(1) <= timer_b_val)
			return 1;
		else
			return 0; /* Not */
	}
	/* Is true */
	return 1;
}

/* Has route? */
int page :: has_route()
{
	return (move_type == MOVE_CUSTOM);
}

/* Has trans */
int page :: get_transparent()
{
	return transparent;
}

/* Handle built in route */
void page :: handle_route(void *me)
{
	/* Must be built-in route */
	if(move_type == MOVE_STATIONARY || move_type == MOVE_CUSTOM)
		return;
	/* Handle it */
	mls->run(me);
}

/* Set attached event */
void page :: set_who(void *w)
{
	who = w;
}

/* Get foot to start on */
int page :: get_foot()
{
	return start_foot;
}