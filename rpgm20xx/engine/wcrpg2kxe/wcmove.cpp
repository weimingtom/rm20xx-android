/*
	Move Lists for Events
*/

/* Includes */
#include <malloc.h>
#include <memory.h>
#include "wcmove.h"
#include "wctype.h"
#include "wcrpg.h"
#include "wcevent.h"
#include "wcmem.h"
#include "wcpage.h"
#include "wcsaveio.h"

#include <string>
#include <stdlib.h>

/* Globals */
int wcmove_active_count = 0;
move *wcmove_default_face_random; /* These are all built-in move commands for the default routes */
move *wcmove_default_move_forward;
move *wcmove_default_move_up;
move *wcmove_default_move_down;
move *wcmove_default_move_right;
move *wcmove_default_move_left;
move *wcmove_default_move_to_hero;
move *wcmove_default_move_from_hero;

/* Loads moves */
void wcmove_init()
{
	int code;
	/* Load default moves */
	code = MOVE_FACE_RANDOM;
	wcmove_default_face_random = new move(&code);
	code = MOVE_FORWARD;
	wcmove_default_move_forward = new move(&code);
	code = MOVE_UP;
	wcmove_default_move_up = new move(&code);
	code = MOVE_DOWN;
	wcmove_default_move_down = new move(&code);
	code = MOVE_RIGHT;
	wcmove_default_move_right = new move(&code);
	code = MOVE_LEFT;
	wcmove_default_move_left = new move(&code);
	code = MOVE_AT_HERO;
	wcmove_default_move_to_hero = new move(&code);
	code = MOVE_AWAY_HERO;
	wcmove_default_move_from_hero = new move(&code);
}

/* Cleans up memory */
void wcmove_exit()
{
	/* Delete default moves */
	delete wcmove_default_face_random;
	delete wcmove_default_move_forward;
	delete wcmove_default_move_up;
	delete wcmove_default_move_down;
	delete wcmove_default_move_right;
	delete wcmove_default_move_left;
	delete wcmove_default_move_to_hero;
	delete wcmove_default_move_from_hero;
}

/* Adds to the active count */
void wcmove_add_active()
{
	wcmove_active_count++;
}

/* Reduces from active count */
void wcmove_remove_active()
{
	wcmove_active_count--;
	if(wcmove_active_count < 0)
		wcrpg_error("Active event count checksum failure (%d)",wcmove_active_count);
}

/* Returns if moves are finished */
int wcmove_finished()
{
	return (wcmove_active_count == 0);
}

/* Get number of events moving */
int wcmove_get_active()
{
	return wcmove_active_count;
}

/* Sets the active count */
void wcmove_set_active(int act)
{
	wcmove_active_count = act;
}

/* Resets the active count */
void wcmove_reset_active()
{
	wcmove_active_count = 0;
}

/* Create a new move */
move :: move(FILE *f)
{
	int rlen,slen,vol,tem,bal;
	/* Initialize */
	code = 0;
	snd = 0;
	str = 0;
	num = 0;
	/* Get code */
	code = wcint_read_file(f,&rlen);
	/* If parametric */
	if(code == MOVE_SWITCH_ON)
		num = wcint_read_file(f,&rlen);
	if(code == MOVE_SWITCH_OFF)
		num = wcint_read_file(f,&rlen);
	/* If change graphic */
	if(code == MOVE_CHANGE_GRAPHIC)
	{
		/* Read the string */
		slen = wcint_read_file(f,&rlen);
		str = (char*)malloc(slen+1);
		memset(str,0,slen+1);
		fread(str,1,slen,f);
		/* Read the subimage */
		num = wcint_read_file(f,&rlen);
	}
	/* If sound */
	if(code == MOVE_SOUND)
	{
		/* Read the string */
		slen = wcint_read_file(f,&rlen);
		str = (char*)malloc(slen+1);
		memset(str,0,slen+1);
		fread(str,1,slen,f);
		/* Read vol, tem, and balance */
		vol = wcint_read_file(f,&rlen);
		tem = wcint_read_file(f,&rlen);
		bal = wcint_read_file(f,&rlen);
		/* Make sound */
		snd = new sound(str,vol,tem,bal);
	}
}

/* Creates a new move from a list of numbers */
move :: move(int *arg)
{
	int slen,vol,tem,bal,i;
	/* Initialize */
	code = 0;
	snd = 0;
	str = 0;
	num = 0;
	/* Get code */
	code = arg[0];
	/* If parametric */
	if(code == MOVE_SWITCH_ON)
		num = arg[1];
	if(code == MOVE_SWITCH_OFF)
		num = arg[1];
	/* If change graphic */
	if(code == MOVE_CHANGE_GRAPHIC)
	{
		/* Read the string */
		slen = arg[1];
		str = (char*)malloc(slen+1);
		memset(str,0,slen+1);
		for(i = 0;i < slen;i++)
			str[i] = arg[2+i];
		/* Read the subimage */
		num = arg[2+slen];
	}
	/* If sound */
	if(code == MOVE_SOUND)
	{
		/* Read the string */
		slen = arg[1];
		str = (char*)malloc(slen+1);
		memset(str,0,slen+1);
		for(i = 0;i < slen;i++)
			str[i] = arg[2+i];
		/* Read vol, tem, and balance */
		vol = arg[2+slen];
		tem = arg[2+slen+1];
		bal = arg[2+slen+2];
		/* Make sound */
		snd = new sound(str,vol,tem,bal);
	}
}

/* Delete this move */
move :: ~move()
{
	/* Release string */
	free(str);
	if(snd)
		delete snd;
}

/* Length of paramter */
int move :: parametric_length()
{
	if(code == MOVE_CHANGE_GRAPHIC)
		return (int)strlen(str)+2;
	if(code == MOVE_SOUND)
		return (int)strlen(str)+4;
	if(code == MOVE_SWITCH_ON)
		return 1;
	if(code == MOVE_SWITCH_OFF)
		return 1;
	return 0;
}

/* Get name */
char *move :: get_name()
{
	/* Return code */
	switch(code)
	{
	case MOVE_UP:
		return "Up";
	case MOVE_RIGHT:
		return "Right";
	case MOVE_DOWN:
		return "Down";
	case MOVE_LEFT:
		return "Left";
	case MOVE_FACE_UP:
		return "Face Up";
	case MOVE_FACE_RIGHT:
		return "Face Right";
	case MOVE_FACE_DOWN:
		return "Face Down";
	case MOVE_FACE_LEFT:
		return "Face Left";
	case MOVE_TURN_RIGHT:
		return "Turn Right";
	case MOVE_FACE_HERO:
		return "Face Hero";
	case MOVE_PHASING_ON:
		return "Phasing ON";
	case MOVE_PHASING_OFF:
		return "Phasing OFF";
	case MOVE_FACE_RANDOM:
		return "Face Random";
	case MOVE_FORWARD:
		return "Move Forward";
	case MOVE_AT_HERO:
		return "Move to Hero";
	case MOVE_AWAY_HERO:
		return "Move Away From Hero";
	case MOVE_LOCK_FACING:
		return "Lock Facing";
	case MOVE_UNLOCK_FACING:
		return "Unlock Facing";
	}
	/* Return parametric code */
	switch(code)
	{
	case MOVE_SWITCH_ON:
		return "Switch ON";
	case MOVE_SWITCH_OFF:
		return "Switch OFF";
	case MOVE_CHANGE_GRAPHIC:
		return "Change Graphic";
	case MOVE_SOUND:
		return "Play Sound";
	}
	/* Unknown move code */
	return "????";
}

/* Get code */
int move :: get_code()
{
	return code;
}

/* Blank move */
move :: move()
{
	/* Blank out */
	code = 0;
	str = 0;
	snd = 0;
	num = 0;
}

/* Save state */
void move :: save_state()
{
	/* Save code */
	wcsaveio_write_num(code);
	/* Save string */
	wcsaveio_write_str(str);
	/* Save sound */
	if(!snd)
	{
		wcsaveio_write_num(0);
	}
	else
	{
		wcsaveio_write_num(1);
		wcsaveio_write_str(snd->get_file());
		wcsaveio_write_num(snd->get_volume());
		wcsaveio_write_num(snd->get_tempo());
		wcsaveio_write_num(snd->get_balance());
	}
	/* Save param */
	wcsaveio_write_num(num);
}

/* Load state */
void move :: load_state()
{
	int hassnd;
	char *sndf;
	int sndv,sndt,sndb;
	/* Read code */
	code = wcsaveio_read_num();
	/* Read string */
	str = wcsaveio_read_str();
	/* Read sound */
	hassnd = wcsaveio_read_num();
	if(hassnd)
	{
		/* Read params */
		sndf = wcsaveio_read_str();
		sndv = wcsaveio_read_num();
		sndt = wcsaveio_read_num();
		sndb = wcsaveio_read_num();
		/* Create */
		snd = new sound(sndf,sndb,sndt,sndb);
		/* Release name */
		free(sndf);
	}
	/* Read param */
	num = wcsaveio_read_num();
}

/* Create new movelist */
movelist :: movelist(int arrcnt,int *arr)
{
	move *mv;
	int i;
	/* Make new list */
	ls = wcarray_new();
	step = 0;
	wclk = 0;
	deflist = 0;
	defstate = 0;
	clk = 0;
	/* Redirection pointer */
	pointer = arr[0];
	/* Read freq, repeat, and ignore */
	freq = arr[1];
	freqadd = 0;
	repeat = arr[2];
	ignore = arr[3];
	/* Add moves */
	for(i = 4;i < arrcnt;i++)
	{
		mv = new move(&arr[i]);
		wcarray_add(ls,mv);
		i += mv->parametric_length();
	}
}

/* Create new movelist */
movelist :: movelist(FILE *f)
{
	move *mv;
	int code,len,rlen,count,i;
	/* Make new list */
	ls = wcarray_new();
	repeat = 1;
	ignore = 0;
	defstate = 0;
	step = 0;
	count = 0;
	deflist = 0;
	pointer = 10005;
	clk = 0;
	wclk = 0;
	freq = 8;
	freqadd = 0;
	/* Read byte size */
	len = wcint_read_file(f,&rlen);
	/* Read count */
	code = wcint_read_file(f,&rlen);
	if(code != 0x0B)
	{
		count = wcint_read_file(f,&rlen);
	}
	else
	{
		len = wcint_read_file(f,&rlen);
		count = wcint_read_file(f,&rlen);
		code = wcint_read_file(f,&rlen);
		count = wcint_read_file(f,&rlen);
	}
	/* Read moves */
	for(i = 0;i < count;i++)
	{
		mv = new move(f);
		wcarray_add(ls,mv);
		count -= mv->parametric_length();
	}
	/* Read options */
	for(;;)
	{
		code = wcint_read_file(f,&rlen);
		if(code == 0x15)
		{
			len = wcint_read_file(f,&rlen);
			repeat = wcint_read_file(f,&rlen);
		}
		if(code == 0x16)
		{
			len = wcint_read_file(f,&rlen);
			ignore = wcint_read_file(f,&rlen);
		}
		if(!code)
			break;
	}
}

/* Releases movelist */
movelist :: ~movelist()
{
	int i;
	if(ls)
	{
		for(i = 0;i < wcarray_size(ls);i++)
			delete ((move*)wcarray_get(ls,i));
		wcarray_delete(ls);
	}
}

/* Length of list */
int movelist :: length()
{
	return wcarray_size(ls);
}

/* Print list */
void movelist :: dissasemble(FILE *f)
{
	int i;
	/* If default */
	if(deflist != MOVE_CUSTOM)
	{
		fprintf(f,"Default List: %d",deflist);
		fprintf(f,"%c",(char)0x0D);
		fprintf(f,"%c",(char)0x0A);
		return;
	}
	/* Number of moves */
	fprintf(f,"Moves: %d",length());
	fprintf(f,"%c",(char)0x0D);
	fprintf(f,"%c",(char)0x0A);
	/* The moves */
	for(i = 0;i < length();i++)
	{
		fprintf(f,"(%d) %s",((move*)wcarray_get(ls,i))->get_code(),((move*)wcarray_get(ls,i))->get_name());
		fprintf(f,"%c",(char)0x0D);
		fprintf(f,"%c",(char)0x0A);
	}
}

/* Face hero */
int move :: move_forward(void *who)
{
	rpgevent *ev;
	ev = (rpgevent*)who;
	if(ev->get_direction() == FACE_UP)
		return ev->move(0,-1);
	if(ev->get_direction() == FACE_DOWN)
		return ev->move(0,1);
	if(ev->get_direction() == FACE_LEFT)
		return ev->move(-1,0);
	if(ev->get_direction() == FACE_RIGHT)
		return ev->move(1,0);
	return 0;
}

/* Move random */
int move :: move_random(void *who)
{
	rpgevent *ev;
	ev = (rpgevent*)who;
	ev->face(rand()%4);
	return move_forward(who);
}

/* Applies this move command to the event given */
int move :: run(void *v)
{
	rpgevent *ev;
	ev = (rpgevent*)v;
	/* Move normal */
	if(code == MOVE_UP)
	{
		if(!ev->get_direction_fix())
			ev->face(FACE_UP);
		return ev->move(0,-1);
	}
	if(code == MOVE_RIGHT)
	{
		if(!ev->get_direction_fix())
			ev->face(FACE_RIGHT);
		return ev->move(1,0);
	}
	if(code == MOVE_DOWN)
	{
		if(!ev->get_direction_fix())
			ev->face(FACE_DOWN);
		return ev->move(0,1);
	}
	if(code == MOVE_LEFT)
	{
		if(!ev->get_direction_fix())
			ev->face(FACE_LEFT);
		return ev->move(-1,0);
	}
	/* Move diagonal */
	if(code == MOVE_UP_RIGHT)
	{
		if(!ev->get_direction_fix())
		{
			if(ev->get_direction() == FACE_UP || ev->get_direction() == FACE_DOWN)
				ev->face(FACE_UP);
			if(ev->get_direction() == FACE_RIGHT || ev->get_direction() == FACE_LEFT)
				ev->face(FACE_RIGHT);
		}
		return ev->move(1,-1);
	}
	if(code == MOVE_DOWN_RIGHT)
	{
		if(!ev->get_direction_fix())
		{
			if(ev->get_direction() == FACE_UP || ev->get_direction() == FACE_DOWN)
				ev->face(FACE_DOWN);
			if(ev->get_direction() == FACE_RIGHT || ev->get_direction() == FACE_LEFT)
				ev->face(FACE_RIGHT);
		}
		return ev->move(1,1);
	}
	if(code == MOVE_DOWN_LEFT)
	{
		if(!ev->get_direction_fix())
		{
			if(ev->get_direction() == FACE_UP || ev->get_direction() == FACE_DOWN)
				ev->face(FACE_DOWN);
			if(ev->get_direction() == FACE_RIGHT || ev->get_direction() == FACE_LEFT)
				ev->face(FACE_LEFT);
		}
		return ev->move(-1,1);
	}
	if(code == MOVE_UP_LEFT)
	{
		if(!ev->get_direction_fix())
		{
			if(ev->get_direction() == FACE_UP || ev->get_direction() == FACE_DOWN)
				ev->face(FACE_UP);
			if(ev->get_direction() == FACE_RIGHT || ev->get_direction() == FACE_LEFT)
				ev->face(FACE_LEFT);
		}
		return ev->move(-1,-1);
	}
	/* Move special */
	if(code == MOVE_RANDOM)
	{
		return move_random(ev);
	}
	if(code == MOVE_FORWARD)
	{
		return move_forward(ev);
	}
	if(code == MOVE_AT_HERO)
	{
		/* Face hero */
		ev->face_player();
		/* Move forward */
		return move_forward(ev);
	}
	if(code == MOVE_AWAY_HERO)
	{
		/* Face hero and turn around */
		ev->face_player();
		ev->turn_right();
		ev->turn_right();
		return move_forward(ev);
	}
	/* Facing */
	if(code == MOVE_FACE_UP)
	{
		ev->face(FACE_UP);
		return 1;
	}
	if(code == MOVE_FACE_RIGHT)
	{
		ev->face(FACE_RIGHT);
		return 1;
	}
	if(code == MOVE_FACE_DOWN)
	{
		ev->face(FACE_DOWN);
		return 1;
	}
	if(code == MOVE_FACE_LEFT)
	{
		ev->face(FACE_LEFT);
		return 1;
	}
	if(code == MOVE_TURN_RIGHT)
	{
		ev->turn_right();
		return 1;
	}
	if(code == MOVE_TURN_LEFT)
	{
		ev->turn_left();
		return 1;
	}
	if(code == MOVE_ABOUT_FACE)
	{
		ev->about_face();
		return 1;
	}
	if(code == MOVE_TURN_RANDOM)
	{
		if(rand()%2)
			ev->turn_right();
		else
			ev->turn_left();
		return 1;
	}
	if(code == MOVE_FACE_HERO)
	{
		ev->face_player();
		return 1;
	}
	if(code == MOVE_FACE_AWAY_HERO)
	{
		ev->face_player();
		ev->about_face();
		return 1;
	}
	if(code == MOVE_FACE_RANDOM)
	{
		ev->face(rand()%4);
		return 1;
	}
	if(code == MOVE_LOCK_FACING)
	{
		ev->set_lock_facing(1);
		ev->get_route()->advance_clock();
		return 1;
	}
	if(code == MOVE_UNLOCK_FACING)
	{
		ev->set_lock_facing(0);
		ev->get_route()->advance_clock();
		return 1;
	}
	/* Waiting */
	if(code == MOVE_WAIT)
	{
		ev->get_route()->wait_clock();
		return 1;
	}
	/* Frequency change */
	if(code == MOVE_FREQ_UP)
	{
		ev->get_route()->increase_freq();
		ev->get_route()->advance_clock();
		return 1;
	}
	if(code == MOVE_FREQ_DOWN)
	{
		ev->get_route()->decrease_freq();
		ev->get_route()->advance_clock();
		return 1;
	}
	/* Phasing */
	if(code == MOVE_PHASING_ON)
	{
		ev->set_phasing(1);
		ev->get_route()->advance_clock();
		return 1;
	}
	if(code == MOVE_PHASING_OFF)
	{
		ev->set_phasing(0);
		ev->get_route()->advance_clock();
		return 1;
	}
	/* Trans */
	if(code == MOVE_TRANS_UP)
	{
		ev->set_fade(ev->get_fade()+1);
		ev->get_route()->advance_clock();
		return 1;
	}
	if(code == MOVE_TRANS_DOWN)
	{
		ev->set_fade(ev->get_fade()-1);
		ev->get_route()->advance_clock();
		return 1;
	}
	/* Lock Animation */
	if(code == MOVE_STOP_ANIM)
	{
		ev->lock_feet(1);
		ev->get_route()->advance_clock();
		return 1;
	}
	if(code == MOVE_START_ANIM)
	{
		ev->lock_feet(0);
		ev->get_route()->advance_clock();
		return 1;
	}
	/* Speed */
	if(code == MOVE_SPEED_UP)
	{
		ev->speed_up();
		ev->get_route()->advance_clock();
		return 1;
	}
	if(code == MOVE_SPEED_DOWN)
	{
		ev->speed_down();
		ev->get_route()->advance_clock();
		return 1;
	}
	/* Switch */
	if(code == MOVE_SWITCH_ON)
	{
		wcmem_set_switch(num,1);
		ev->get_route()->advance_clock();
		return 1;
	}
	if(code == MOVE_SWITCH_OFF)
	{
		wcmem_set_switch(num,0);
		ev->get_route()->advance_clock();
		return 1;
	}
	/* Graphic */
	if(code == MOVE_CHANGE_GRAPHIC)
	{
		ev->change_graphics(wcevent_pool_charset(str),num);
		ev->get_route()->advance_clock();
		return 1;
	}
	/* Sound */
	if(code == MOVE_SOUND)
	{
		snd->play();
		ev->get_route()->advance_clock();
		return 1;
	}
	/* Jumping */
	if(code == MOVE_START_JUMP)
	{
		ev->start_jump();
		ev->get_route()->advance_clock();
		return 1;
	}
	if(code == MOVE_END_JUMP)
	{
		ev->get_route()->advance_clock();
		return ev->end_jump();
	}
	return 1;
}

/* Applies this move list to the event given */
void movelist :: run(void *v)
{
	int rdir,i,ffreq;
	float ts;
	move *mv;
	rpgevent *ev;
	ev = (rpgevent*)v;
	/* Get ffreq */
	ffreq = freq+freqadd;
	if(ffreq < 0)
		ffreq = 0;
	if(ffreq > 8)
		ffreq = 8;
	/* Get ts */
	ts = wcrpg_get_time_step();
	ts *= 1.0f;
	/* Fill the clock */
	if(clk < 48.0f && ffreq < 8 && !ev->walking())
	{
		clk += ts;
		if(ffreq-1 >= 1)
			clk += ts; /* Totals 2 */
		if(ffreq-1 >= 2)
			clk += ts*2.0f; /* Totals 4 */
		if(ffreq-1 >= 3)
			clk += ts*4.0f; /* Totals 8 */
		if(ffreq-1 >= 4)
			clk += ts*8.0f; /* Totals 16 */
		if(ffreq-1 >= 5)
			clk += ts*16.0f; /* Totals 32 */
		if(ffreq-1 >= 6)
			clk += ts*32.0f; /* Totals 64 */
		if(ffreq >= 7)
			clk += ts*64.0f; /* Totals 128 */
		if(ffreq >= 8)
			clk += ts*128.0f; /* Totals 256 */
		return;
	}
	/* Fill the clock */
	if(wclk < 0.0f)
	{
		wclk += ts*8.0f;
		return;
	}
	/* Reset */
	clk = 0.0f;
	/* Must be ready to accept new command */
	if(ev->get_state() != EVENT_IDLE)
		return;
	/* Must not be at the end of run-once and no moves */
	if(ls)
	{
		if(step >= wcarray_size(ls) && !repeat && !deflist)
			return;
		if(wcarray_size(ls) <= 0 && !deflist)
			return;
	}
	/* Standard moving */
	if(!deflist)
	{
		mv = (move*)wcarray_get(ls,step);
		/* Remember the old direction */
		rdir = ev->get_direction();
		/* Act upon it */
		if(mv->run(v))
		{
			step++;
		}
		else
		{
			/* Interrupt clock */
			clk = 48.0f;
			/* Events don't get to face where they WANT to move like the player does */
			ev->face(rdir);
			if(ignore)
				step++;
		}
		if(step >= wcarray_size(ls) && repeat)
			step = 0;
	}
	else
	{
		/* Auto moving */
		rdir = ev->get_direction();
		i = 1;
		/* Act on auto */
		if(deflist == MOVE_RANDOMLY)
		{
			/* Roam */
			wcmove_default_face_random->run(v);
			if(!wcmove_default_move_forward->run(v))
				i = 0;
		}
		if(deflist == MOVE_UP_DOWN)
		{
			/* Up Down */
			if(defstate)
			{
				if(!wcmove_default_move_up->run(v))
				{
					i = 0;
					defstate = 0;
				}
			}
			else
			{
				if(!wcmove_default_move_down->run(v))
				{
					i = 0;
					defstate = 1;
				}
			}
		}
		if(deflist == MOVE_LEFT_RIGHT)
		{
			/* Up Down */
			if(defstate)
			{
				if(!wcmove_default_move_left->run(v))
				{
					i = 0;
					defstate = 0;
				}
			}
			else
			{
				if(!wcmove_default_move_right->run(v))
				{
					i = 0;
					defstate = 1;
				}
			}
		}
		if(deflist == MOVE_TO_HERO)
		{
			if(!wcmove_default_move_to_hero->run(v))
				i = 0;
		}
		if(deflist == MOVE_FROM_HERO)
		{
			if(!wcmove_default_move_from_hero->run(v))
				i = 0;
		}
		/* Reset? */
		if(!i)
		{
			/* Interrupt clock */
			clk = 48.0f;
			/* Events don't get to face where they WANT to move like the player does */
			ev->face(rdir);
		}
	}
}

/* Sets the frequency */
void movelist :: set_freq(int fq)
{
	freq = fq;
	clk = 0.0f;
}

/* Returns if this list has finished */
int movelist :: is_finished()
{
	if(step >= wcarray_size(ls))
		return 1;
	return 0;
}

/* Resets movelist */
void movelist :: reset()
{
	freqadd = 0;
	step = 0;
}

/* Gets pointer */
int movelist :: get_target()
{
	return pointer;
}

/* Creates new default route movelist */
movelist :: movelist(int defroute)
{
	/* Make blank list */
	ls = 0;
	repeat = 1;
	ignore = 0;
	wclk = 0;
	defstate = 0;
	step = 0;
	pointer = 10005;
	clk = 0;
	freq = 8;
	freqadd = 0;
	deflist = defroute;
}

/* Increase freq */
void movelist :: increase_freq()
{
	freqadd++;
}

/* Decrease freq */
void movelist :: decrease_freq()
{
	freqadd--;
}

/* Advance clock */
void movelist :: advance_clock()
{
	clk = 48.0f;
}

/* Wait clock */
void movelist :: wait_clock()
{
	wclk = -48.0f;
}

/* Save state */
void movelist :: save_state()
{
	int i;
	/* Save headings */
	wcsaveio_write_num(repeat);
	wcsaveio_write_num(ignore);
	wcsaveio_write_num(step);
	wcsaveio_write_num(freq);
	wcsaveio_write_num(freqadd);
	wcsaveio_write_float(clk);
	wcsaveio_write_float(wclk);
	wcsaveio_write_num(pointer);
	wcsaveio_write_num(deflist);
	wcsaveio_write_num(defstate);
	/* Save list */
	wcsaveio_write_num(wcarray_size(ls));
	/* Save elements */
	for(i = 0;i < wcarray_size(ls);i++)
		((move*)wcarray_get(ls,i))->save_state();
}

/* Blank list */
movelist :: movelist()
{
	/* Blank out state */
	ls = 0;
	repeat = 0;
	ignore = 0;
	step = 0;
	freq = 0;
	freqadd = 0;
	clk = 0.0f;
	wclk = 0.0f;
	pointer = 0;
	deflist = 0;
	defstate = 0;
}

/* Load state */
void movelist :: load_state()
{
	int i,len;
	move *mv;
	/* Load headings */
	repeat = wcsaveio_read_num();
	ignore = wcsaveio_read_num();
	step = wcsaveio_read_num();
	freq = wcsaveio_read_num();
	freqadd = wcsaveio_read_num();
	clk = wcsaveio_read_float();
	wclk = wcsaveio_read_float();
	pointer = wcsaveio_read_num();
	deflist = wcsaveio_read_num();
	defstate = wcsaveio_read_num();
	/* Read list size */
	len = wcsaveio_read_num();
	/* Make my list */
	if(ls)
	{
		for(i = 0;i < wcarray_size(ls);i++)
			delete ((move*)wcarray_get(ls,i));
		wcarray_delete(ls);
	}
	ls = wcarray_new();
	/* Read all */
	for(i = 0;i < len;i++)
	{
		mv = new move();
		mv->load_state();
		wcarray_add(ls,mv);
	}
}