/*
	The two timers
*/

/* Includes */
#include <memory.h>
#include "wctimer.h"
#include "wcrpg.h"
#include "wcwindow.h"
#include "wcbattle.h"

#include <string.h>

/* Globals */
float wctimer_clk[WCTIMER_MAX];
int wctimer_s[WCTIMER_MAX];
int wctimer_mode[WCTIMER_MAX];
int wctimer_vis[WCTIMER_MAX];
int wctimer_battle[WCTIMER_MAX];

/* Inits timer */
void wctimer_init()
{
	memset(wctimer_s,0,sizeof(wctimer_s));
	memset(wctimer_mode,0,sizeof(wctimer_mode));
	memset(wctimer_vis,0,sizeof(wctimer_vis));
	memset(wctimer_battle,0,sizeof(wctimer_battle));
	memset(wctimer_clk,0,sizeof(wctimer_clk));
}

/* Sets a timer */
void wctimer_set(int tid,int s)
{
	wctimer_s[tid] = s;
	wctimer_clk[tid] = 0.0f;
}

/* Starts a timer */
void wctimer_start(int tid,int vis,int btl)
{
	wctimer_mode[tid] = 1;
	wctimer_vis[tid] = vis;
	wctimer_battle[tid] = btl;
}

/* Stops a timer */
void wctimer_stop(int tid)
{
	wctimer_mode[tid] = 0;
	wctimer_vis[tid] = 0;
}

/* Handles timers */
void wctimer_handle()
{
	int i;
	float ts;
	/* Get time */
	ts = wcrpg_get_time_step();
	/* Handle all timers */
	for(i = 0;i < WCTIMER_MAX;i++)
	{
		/* If the timer is active */
		if(wctimer_mode[i])
		{
			/* Must either run during battle or there being no battles */
			if(wctimer_battle[i] || !wcbattle_active())
			{
				/* Increase clock */
				wctimer_clk[i] += ts;
				/* Step */
				if(wctimer_clk[i] >= WCTIMER_STEP)
				{
					/* Reset step and decrease */
					wctimer_clk[i] = 0.0f;
					wctimer_s[i]--;
					/* Can't go below zero */
					if(wctimer_s[i] < 0)
					{
						wctimer_stop(i);
						wctimer_s[i] = 0;
					}
				}
			}
		}
	}
}

/* Draws timers */
void wctimer_draw()
{
	int xrank,i,sec,min;
	/* Init */
	xrank = 0;
	sec = 0;
	min = 0;
	/* Handle all timers */
	for(i = 0;i < WCTIMER_MAX;i++)
	{
		/* If the timer may be drawn */
		if(wctimer_vis[i])
		{
			/* Chop times */
			min = wctimer_s[i]/60;
			sec = wctimer_s[i]%60;
			/* Draw min */
			if(min > 9)
			{
				wcwindow_draw_number(8+xrank,8,min);
			}
			else
			{
				wcwindow_draw_number(8+xrank,8,0);
				wcwindow_draw_number(16+xrank,8,min);
			}
			/* Draw sec */
			if(sec > 9)
			{
				wcwindow_draw_number(8+24+xrank,8,sec);
			}
			else
			{
				wcwindow_draw_number(8+24+xrank,8,0);
				wcwindow_draw_number(16+24+xrank,8,sec);
			}
			/* Draw colon */
			if(wctimer_clk[i] < WCTIMER_STEP/2.0f)
				wcwindow_draw_colon(24+xrank,8);
		}
		/* Increase rank */
		xrank += WCRPG_WIDTH-32-24;
	}
}

/* Gets seconds */
int wctimer_get(int tid)
{
	return wctimer_s[tid];
}