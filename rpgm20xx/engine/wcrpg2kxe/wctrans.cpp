/*
	Transitions
*/

/* Includes */
#include <stdlib.h>
//#include "resource.h"
#include "wctrans.h"
#include "wcdraw.h"
#include "wcrpg.h"
#include "wclevel.h"

/* Globals */
float wctrans_blocks[WCRPG_WIDTH/8*WCRPG_HEIGHT/8];
int wctrans_state = WCTRANS_SCREEN_SHOW;
int wctrans_action = 0;
int wctrans_eff = WCTRANS_NONE;
float wctrans_dist = 0.0f;
texture *wctrans_dark_panel = 0;
int wctrans_teleport = 0;

/* Set teleport */
void wctrans_set_teleport_mode(int en)
{
	wctrans_teleport = en;
}

/* Get teleport */
int wctrans_get_teleport_mode()
{
	return wctrans_teleport;
}

/* Generate block distances */
void wctrans_shuffle_blocks()
{
	int x,y;
	for(y = 0;y < WCRPG_HEIGHT/8;y++)
		for(x = 0;x < WCRPG_WIDTH/8;x++)
			wctrans_blocks[x+y*(WCRPG_WIDTH/8)] = (float)(rand()%100)/99.0f;
}

/* Generate wipe down shuffle blocks */
void wctrans_shuffle_blocks_down()
{
	int x,y;
	for(y = 0;y < WCRPG_HEIGHT/8;y++)
		for(x = 0;x < WCRPG_WIDTH/8;x++)
			wctrans_blocks[x+y*(WCRPG_WIDTH/8)] = ((float)(rand()%100)/99.0f)*((float)(y)/(float)(WCRPG_HEIGHT/8));
}

/* Generate wipe up shuffle blocks */
void wctrans_shuffle_blocks_up()
{
	int x,y;
	for(y = 0;y < WCRPG_HEIGHT/8;y++)
		for(x = 0;x < WCRPG_WIDTH/8;x++)
			wctrans_blocks[x+y*(WCRPG_WIDTH/8)] = ((float)(rand()%100)/99.0f)*(1.0f-((float)(y)/(float)(WCRPG_HEIGHT/8)));
}

/* Draw ven blinds */
void wctrans_draw_ven_blinds(float d)
{
	int y;
	wctrans_dark_panel->set_transparency(0.0f,0.0f);
	for(y = 0;y < WCRPG_HEIGHT/8;y++)
		wctrans_dark_panel->draw(0,y*8,8,8,0,0,WCRPG_WIDTH,(int)(8.0f*d));
}

/* Draw vert blinds */
void wctrans_draw_vertical_blinds(float d)
{
	int yt,y;
	wctrans_dark_panel->set_transparency(0.0f,0.0f);
	yt = (int)(WCRPG_HEIGHT/8.0f*d);
	for(y = 0;y < yt;y += 2)
		wctrans_dark_panel->draw(0,y*8,8,8,0,0,WCRPG_WIDTH,8);
	for(y = WCRPG_HEIGHT/8-1;y >= WCRPG_HEIGHT/8-1-yt;y -= 2)
		wctrans_dark_panel->draw(0,y*8,8,8,0,0,WCRPG_WIDTH,8);
}

/* Draw horz blinds */
void wctrans_draw_horizontal_blinds(float d)
{
	int xt,x;
	wctrans_dark_panel->set_transparency(0.0f,0.0f);
	xt = (int)(WCRPG_WIDTH/8.0f*d);
	for(x = 0;x < xt;x += 2)
		wctrans_dark_panel->draw(x*8,0,8,8,0,0,8,WCRPG_HEIGHT);
	for(x = WCRPG_WIDTH/8-1;x >= WCRPG_WIDTH/8-1-xt;x -= 2)
		wctrans_dark_panel->draw(x*8,0,8,8,0,0,8,WCRPG_HEIGHT);
}

/* Draw outer square */
void wctrans_draw_out_square(float d)
{
	wctrans_dark_panel->set_transparency(0.0f,0.0f);
	wctrans_dark_panel->draw(0,0,8,8,0,0,WCRPG_WIDTH,(int)(WCRPG_HEIGHT/2*d));
	wctrans_dark_panel->draw(0,0,8,8,0,0,(int)(WCRPG_WIDTH/2*d),WCRPG_HEIGHT);
	wctrans_dark_panel->draw(0,WCRPG_HEIGHT-(int)(WCRPG_HEIGHT/2*d),8,8,0,0,WCRPG_WIDTH,WCRPG_HEIGHT/2);
	wctrans_dark_panel->draw(WCRPG_WIDTH-(int)(WCRPG_WIDTH/2*d),0,8,8,0,0,WCRPG_WIDTH/2,WCRPG_HEIGHT);
}

/* Draw inner square */
void wctrans_draw_in_square(float d)
{
	wctrans_dark_panel->set_transparency(0.0f,0.0f);
	wctrans_dark_panel->draw((int)(WCRPG_WIDTH/2*d),(int)(WCRPG_HEIGHT/2*d),8,8,0,0,(int)(WCRPG_WIDTH*(1.0f-d)),(int)(WCRPG_HEIGHT*(1.0f-d)));
}

/* Draw full screen block up */
void wctrans_draw_up_block(float d)
{
	wctrans_dark_panel->set_transparency(0.0f,0.0f);
	wctrans_dark_panel->draw(0,(int)(WCRPG_HEIGHT*(1.0f-d)),8,8,0,0,WCRPG_WIDTH,WCRPG_HEIGHT);
}

/* Draw full screen block down */
void wctrans_draw_down_block(float d)
{
	wctrans_dark_panel->set_transparency(0.0f,0.0f);
	wctrans_dark_panel->draw(0,(int)(WCRPG_HEIGHT*d)-WCRPG_HEIGHT,8,8,0,0,WCRPG_WIDTH,WCRPG_HEIGHT);
}

/* Draw full screen left block */
void wctrans_draw_left_block(float d)
{
	wctrans_dark_panel->set_transparency(0.0f,0.0f);
	wctrans_dark_panel->draw(WCRPG_WIDTH-(int)(WCRPG_WIDTH*d),0,8,8,0,0,WCRPG_WIDTH,WCRPG_HEIGHT);
}

/* Draw full screen right block */
void wctrans_draw_right_block(float d)
{
	wctrans_dark_panel->set_transparency(0.0f,0.0f);
	wctrans_dark_panel->draw((int)(WCRPG_WIDTH*d)-WCRPG_WIDTH,0,8,8,0,0,WCRPG_WIDTH,WCRPG_HEIGHT);
}

/* Draw block distances */
void wctrans_draw_blocks(int shw)
{
	int x,y,ix;
	wctrans_dark_panel->set_transparency(0.0f,0.0f);
	for(y = 0;y < WCRPG_HEIGHT/8;y++)
	{
		for(x = 0;x < WCRPG_WIDTH/8;x++)
		{
			ix = x+y*(WCRPG_WIDTH/8);
			/* Screen eraser/shower */
			if(!shw)
			{
				if(wctrans_dist >= wctrans_blocks[ix])
					wctrans_dark_panel->draw(x*8,y*8);
			}
			else
			{
				if(wctrans_dist <= wctrans_blocks[ix])
					wctrans_dark_panel->draw(x*8,y*8);
			}
		}
	}
}

/* Init */
void wctrans_init()
{
	//wctrans_dark_panel = new texture(bmp_darkpan,8,8);
}

/* Exit */
void wctrans_exit()
{
	delete wctrans_dark_panel;
}

/* Erase screen */
void wctrans_erase(int eff)
{
	/* Already erased */
	if(wctrans_state == WCTRANS_SCREEN_HIDDEN || wctrans_active())
		return;
	/* Reset dist and flip */
	wctrans_dist = 0.0f;
	wctrans_action = 1;
	wctrans_eff = eff;
	/* Prepare */
	if(wctrans_eff == WCTRANS_BLOCKS)
		wctrans_shuffle_blocks();
	if(wctrans_eff == WCTRANS_WIPE_DOWN)
		wctrans_shuffle_blocks_down();
	if(wctrans_eff == WCTRANS_WIPE_UP)
		wctrans_shuffle_blocks_up();
}

/* Show screen */
void wctrans_show(int eff)
{
	/* Already shown */
	if(wctrans_state == WCTRANS_SCREEN_SHOW || wctrans_active())
		return;
	/* Reset dist and flip */
	wctrans_dist = 0.0f;
	wctrans_action = 1;
	wctrans_eff = eff;
	/* Prepare */
	if(wctrans_eff == WCTRANS_BLOCKS)
		wctrans_shuffle_blocks();
	if(wctrans_eff == WCTRANS_WIPE_DOWN)
		wctrans_shuffle_blocks_down();
	if(wctrans_eff == WCTRANS_WIPE_UP)
		wctrans_shuffle_blocks_up();
}

/* Are transitions going on? */
int wctrans_active()
{
	return wctrans_action;
}

/* Handle transition */
void wctrans_handle()
{
	float ts;
	/* Inactive */
	if(!wctrans_active())
		return;
	/* Get time step */
	ts = wcrpg_get_time_step();
	/* Advance */
	wctrans_dist += ts/10.0f;
	if(wctrans_dist >= 1.0f)
	{
		/* Settle */
		wctrans_dist = 1.0f;
		/* Finish */
		if(wctrans_state == WCTRANS_SCREEN_SHOW)
			wctrans_state = WCTRANS_SCREEN_HIDDEN;
		else
			wctrans_state = WCTRANS_SCREEN_SHOW;
		wctrans_action = 0;
	}
}

/* Draw transition */
void wctrans_draw()
{
	/* Inactive */
	if(!(wctrans_active() || wctrans_state == WCTRANS_SCREEN_HIDDEN))
	{
		wclevel_force_camera(0.0f,0.0f);
		return;
	}
	/* Full */
	if(wctrans_dist >= 1.0f)
	{
		wctrans_dark_panel->set_transparency(0.0f,0.0f);
		wctrans_dark_panel->draw(0,0,8,8,0,0,WCRPG_WIDTH,WCRPG_HEIGHT);
		return;
	}
	/* Fading */
	if(wctrans_eff == WCTRANS_FADE)
	{
		if(wctrans_state == WCTRANS_SCREEN_SHOW)
			wctrans_dark_panel->set_transparency(1.0f-wctrans_dist,1.0f-wctrans_dist);
		else
			wctrans_dark_panel->set_transparency(wctrans_dist,wctrans_dist);
		wctrans_dark_panel->draw(0,0,8,8,0,0,WCRPG_WIDTH,WCRPG_HEIGHT);
	}
	/* Blocks */
	if(wctrans_eff == WCTRANS_BLOCKS || wctrans_eff == WCTRANS_WIPE_DOWN || wctrans_eff == WCTRANS_WIPE_UP)
	{
		if(wctrans_state == WCTRANS_SCREEN_SHOW)
			wctrans_draw_blocks(0);
		else
			wctrans_draw_blocks(1);
	}
	/* Ven Blinds */
	if(wctrans_eff == WCTRANS_BLINDS)
	{
		if(wctrans_state == WCTRANS_SCREEN_SHOW)
			wctrans_draw_ven_blinds(wctrans_dist);
		else
			wctrans_draw_ven_blinds(1.0f-wctrans_dist);
	}
	/* Vert blinds */
	if(wctrans_eff == WCTRANS_VERTICAL_BLINDS)
	{
		if(wctrans_state == WCTRANS_SCREEN_SHOW)
			wctrans_draw_vertical_blinds(wctrans_dist);
		else
			wctrans_draw_vertical_blinds(1.0f-wctrans_dist);
	}
	/* Horz blinds */
	if(wctrans_eff == WCTRANS_HORIZONTAL_BLINDS)
	{
		if(wctrans_state == WCTRANS_SCREEN_SHOW)
			wctrans_draw_horizontal_blinds(wctrans_dist);
		else
			wctrans_draw_horizontal_blinds(1.0f-wctrans_dist);
	}
	/* Recede */
	if(wctrans_eff == WCTRANS_SHRINK_SQUARE)
	{
		if(wctrans_state == WCTRANS_SCREEN_SHOW)
			wctrans_draw_out_square(wctrans_dist);
		else
			wctrans_draw_in_square(wctrans_dist);
	}
	/* Expand */
	if(wctrans_eff == WCTRANS_EXPAND_SQUARE)
	{
		if(wctrans_state == WCTRANS_SCREEN_SHOW)
			wctrans_draw_in_square(1.0f-wctrans_dist);
		else
			wctrans_draw_out_square(1.0f-wctrans_dist);
	}
	/* Up/Down */
	if(wctrans_eff == WCTRANS_SCREEN_UP)
	{
		if(wctrans_state == WCTRANS_SCREEN_SHOW)
		{
			wctrans_draw_up_block(wctrans_dist);
			wclevel_force_camera(0.0f,WCRPG_HEIGHT*wctrans_dist);
		}
		else
		{
			wctrans_draw_up_block(1.0f-wctrans_dist);
			wclevel_force_camera(0.0f,WCRPG_HEIGHT*(1.0f-wctrans_dist));
		}
	}
	/* Down/Up */
	if(wctrans_eff == WCTRANS_SCREEN_DOWN)
	{
		if(wctrans_state == WCTRANS_SCREEN_SHOW)
		{
			wctrans_draw_down_block(wctrans_dist);
			wclevel_force_camera(0.0f,0.0f-WCRPG_HEIGHT*wctrans_dist);
		}
		else
		{
			wctrans_draw_down_block(1.0f-wctrans_dist);
			wclevel_force_camera(0.0f,0.0f-WCRPG_HEIGHT*(1.0f-wctrans_dist));
		}
	}
	/* Left/Left */
	if(wctrans_eff == WCTRANS_SCREEN_LEFT)
	{
		if(wctrans_state == WCTRANS_SCREEN_SHOW)
		{
			wctrans_draw_left_block(wctrans_dist);
			wclevel_force_camera(WCRPG_WIDTH*wctrans_dist,0.0f);
		}
		else
		{
			wctrans_draw_right_block(1.0f-wctrans_dist);
			wclevel_force_camera(0.0f-WCRPG_WIDTH*(1.0f-wctrans_dist),0.0f);
		}
	}
	/* Right/Right */
	if(wctrans_eff == WCTRANS_SCREEN_RIGHT)
	{
		if(wctrans_state == WCTRANS_SCREEN_SHOW)
		{
			wctrans_draw_right_block(wctrans_dist);
			wclevel_force_camera(0.0f-WCRPG_WIDTH*wctrans_dist,0.0f);
		}
		else
		{
			wctrans_draw_left_block(1.0f-wctrans_dist);
			wclevel_force_camera(WCRPG_WIDTH*(1.0f-wctrans_dist),0.0f);
		}
	}
}

/* Returns the screen state */
int wctrans_get_screen_state()
{
	return wctrans_state;
}