/*
	Pop-Up Numbers
*/

/* Includes */
#include "wcpopnum.h"
#include "wcarray.h"
#include "wcwindow.h"
#include "wcrpg.h"

#include <string>

/* Globals */
popnum *wcpopnum_list[WCPOPNUM_MAX];
int wcpopnum_next = 0;

/* Create new popnum */
popnum :: popnum(int x,int y,int val,int t)
{
	/* Setup */
	pos_x = x;
	pos_y = y;
	str = 0;
	type = t;
	anim_dist = 0.0f;
	value = val;
	vel_y = -4.0f;
	add_y = 0.0f;
}

/* Create new popnum from string */
popnum :: popnum(int x,int y,char *c)
{
	/* Setup */
	pos_x = x;
	pos_y = y;
	str = new text(c);
	type = WCPOPNUM_STRING;
	anim_dist = 0.0f;
	value = 0;
	vel_y = -4.0f;
	add_y = 0.0f;
}

/* Delete popnum */
popnum :: ~popnum()
{
	if(str)
		delete str;
}

/* Handle popnum */
void popnum :: handle()
{
	float ts;
	ts = wcrpg_get_time_step();
	anim_dist += ts*0.1f;
	vel_y += ts;
	add_y += vel_y*ts;
	if(anim_dist > 1.0f)
		anim_dist = 1.0f;
}

/* Draw popnum */
void popnum :: draw()
{
	/* Draw damage number */
	if(type == WCPOPNUM_DAMAGE)
		wcwindow_draw_battle_number(pos_x,pos_y+(int)add_y,value);
	/* Draw message */
	if(type == WCPOPNUM_STRING)
	{
		str->move(pos_x,pos_y);
		str->draw();
	}
}

/* Is active */
int popnum :: active()
{
	if(anim_dist < 1.0f)
		return 1;
	return 0;
}

/* Spawn popnum */
void wcpopnum_show(int x,int y,int v,int t)
{
	popnum *pn;
	/* Make new */
	pn = new popnum(x,y,v,t);
	/* Add */
	if(wcpopnum_list[wcpopnum_next])
		delete wcpopnum_list[wcpopnum_next];
	wcpopnum_list[wcpopnum_next] = pn;
	wcpopnum_next++;
	/* Cycle */
	if(wcpopnum_next >= WCPOPNUM_MAX)
		wcpopnum_next = 0;
}

/* Spawn popnum string */
void wcpopnum_show(int x,int y,char *c)
{
	popnum *pn;
	/* Make new */
	pn = new popnum(x,y,c);
	/* Add */
	if(wcpopnum_list[wcpopnum_next])
		delete wcpopnum_list[wcpopnum_next];
	wcpopnum_list[wcpopnum_next] = pn;
	wcpopnum_next++;
	/* Cycle */
	if(wcpopnum_next >= WCPOPNUM_MAX)
		wcpopnum_next = 0;
}

/* Init popnums */
void wcpopnum_init()
{
	memset(wcpopnum_list,0,sizeof(wcpopnum_list));
}

/* Exit popnums */
void wcpopnum_exit()
{
	int i;
	for(i = 0;i < WCPOPNUM_MAX;i++)
	{
		if(wcpopnum_list[i])
			delete wcpopnum_list[i];
	}
	memset(wcpopnum_list,0,sizeof(wcpopnum_list));
}

/* Handle popnums */
void wcpopnum_handle()
{
	int i;
	for(i = 0;i < WCPOPNUM_MAX;i++)
	{
		if(wcpopnum_list[i])
		{
			if(wcpopnum_list[i]->active())
				wcpopnum_list[i]->handle();
		}
	}
}

/* Draw popnums */
void wcpopnum_draw()
{
	int i;
	for(i = 0;i < WCPOPNUM_MAX;i++)
	{
		if(wcpopnum_list[i])
		{
			if(wcpopnum_list[i]->active())
				wcpopnum_list[i]->draw();
		}
	}
}