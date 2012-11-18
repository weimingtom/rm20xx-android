/*
	The current level is the map plus events on it
*/

/* Includes */
#include "wcmaptree.h"
#include "wcplayer.h"
#include "wcmap.h"
#include "wclevel.h"
#include "wcrpg.h"
#include "wcsound.h"
#include "wcrun.h"
#include "wcwindow.h"
#include "wcmove.h"
#include "wcanimation.h"
#include "wctileset.h"
#include "wcsaveio.h"

#include <android\log.h>

#include <string>

/* Globals */
float wclevel_camera_x = 0;
float wclevel_camera_y = 0;
float wclevel_camera_to_pan_x = 0;
float wclevel_camera_to_pan_y = 0;
float wclevel_camera_pan_x = 0;
float wclevel_camera_pan_y = 0;
float wclevel_camera_pan_speed = 0;
float wclevel_camera_force_x = 0;
float wclevel_camera_force_y = 0;
map *wclevel_map = 0;
int wclevel_id = 0;
int camera_lock = 0;

/* Locks up the camera */
void wclevel_camera_lock(int en)
{
	camera_lock = en;
}

/* Removes pan instantly */
void wclevel_camera_remove_pan()
{
	wclevel_camera_to_pan_x = 0;
	wclevel_camera_to_pan_y = 0;
	wclevel_camera_pan_x = 0;
	wclevel_camera_pan_y = 0;
	wclevel_camera_pan_speed = 0;
}

/* Adds to camera pan */
void wclevel_camera_pan(float mx,float my,float spd)
{
	/* Move the camera */
	wclevel_camera_to_pan_x += mx;
	wclevel_camera_to_pan_y += my;
	wclevel_camera_pan_speed = spd;
}

/* Reset camera pan */
void wclevel_camera_reset_pan(float spd)
{
	
	/* Move the camera */
	wclevel_camera_to_pan_x = 0.0f;
	wclevel_camera_to_pan_y = 0.0f;
	wclevel_camera_pan_speed = spd;
}

/* Finish panning */
void wclevel_camera_finish_pan()
{
	float dx,dy;
	dx = wclevel_camera_to_pan_x-wclevel_camera_pan_x;
	if(dx < 0)
		dx = -dx;
	dy = wclevel_camera_to_pan_y-wclevel_camera_pan_y;
	if(dy < 0)
		dy = -dy;
	if(dx <= wclevel_camera_pan_speed/2.0f && dy <= wclevel_camera_pan_speed/2.0f)
	{
		wclevel_camera_pan_x = wclevel_camera_to_pan_x;
		wclevel_camera_pan_y = wclevel_camera_to_pan_y;
	}
}

/* Is the camera panning? */
int wclevel_camera_panning()
{
	float dx,dy;
	dx = wclevel_camera_to_pan_x-wclevel_camera_pan_x;
	if(dx < 0)
		dx = -dx;
	dy = wclevel_camera_to_pan_y-wclevel_camera_pan_y;
	if(dy < 0)
		dy = -dy;
	if(dx <= wclevel_camera_pan_speed && dy <= wclevel_camera_pan_speed)
		return 0;
	return 1;
}

/* Gets an event */
void *wclevel_get_event(int id)
{
	int i;
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "wclevel_get_event");
	rpgevent *ev;
	if(!wclevel_map)
		return 0;
	for(i = 0;i < wclevel_map->get_event_count();i++)
	{
		ev = (rpgevent*)wclevel_map->get_event(i);
		if(ev->get_id() == id)
			return ev;
	}
	wcrpg_error("Could not find event ID %d",id);
	return 0;
}

/* Loads the starting level */
void wclevel_load_start()
{
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "wclevel_load_start");
	int id,x,y;
	id = wcmaptree_get_start_id(START_HERO);
	if(id == -1)
	{
		wcrpg_error("No hero start position defined");
		return;
	}
	x = wcmaptree_get_start_x(START_HERO);
	y = wcmaptree_get_start_y(START_HERO);
	wclevel_load(id,x,y);
}

/* Load a level */
void wclevel_load(int id,int x,int y)
{
	treenode *tn;
	/* Is map different from this one? */
	if(id != wclevel_id)
	{
		/* Announce */
		wclog_write(wcrpg_get_log(),"Entering new map ");
		wclog_write(wcrpg_get_log(),id);
		wclog_enter(wcrpg_get_log());
		/* Free old resources */
		wclevel_free();
		wcevent_clear_charset();
		wcanimation_reload_all_animset();
		wclevel_camera_remove_pan();
		/* Reset moves */
		wcmove_reset_active();
		/*wcwindow_clear_faceset();*/
		/* Load new resources */
		wclevel_id = id;
		wclevel_map = new map(id);
		/* Set cam */
		wclevel_camera_x = 0;
		wclevel_camera_y = 0;
		/* Reset player */
		wcplayer_start(x,y);
		/* Play music */
		tn = wcmaptree_get(id);
		wcsound_music_play(tn->get_music());
		/* Update terrains */
		wclevel_update_all_event_terrain();
		/* Reset tiles */
		wctileset_reset_sub();
		wctileset_reset_shader();
		/* Reset */
		wcrpg_time_start();
	}
	else
	{
		/* Announce */
		wclog_write(wcrpg_get_log(),"Teleporting on current map");
		wclog_enter(wcrpg_get_log());
		/* Just move them */
		wcplayer_get()->place(x,y);
	}
}

/* Get map no. */
int wclevel_get_id()
{
	return wclevel_id;
}

/* Draws the current level */
void wclevel_draw()
{
	/* Draw the level */
	if(wclevel_map)
		wclevel_map->draw(-wclevel_get_camera_x(),-wclevel_get_camera_y());
}

/* Frees levels */
void wclevel_free()
{
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "wclevel_free");
	wclevel_id=0;
	/* Release map */
	if(wclevel_map)
		delete wclevel_map;
}

/* Moves the camera by some amount */
void wclevel_move_camera(float mx,float my)
{
	if(camera_lock)
		return;
	wclevel_camera_x += mx;
	wclevel_camera_y += my;
}

/* Sets the camera */
void wclevel_set_camera(float x,float y)
{
	if(camera_lock)
		return;
	wclevel_camera_x = x;
	wclevel_camera_y = y;
}

/* Handles the level */
void wclevel_handle()
{
	int i;
	float ts;
	/* Get time step */
	ts = wcrpg_get_time_step();
	/* Slide camera pan */
	if(wclevel_camera_pan_x < wclevel_camera_to_pan_x)
		wclevel_camera_pan_x += ts*wclevel_camera_pan_speed;
	if(wclevel_camera_pan_x > wclevel_camera_to_pan_x)
		wclevel_camera_pan_x -= ts*wclevel_camera_pan_speed;
	if(wclevel_camera_pan_y < wclevel_camera_to_pan_y)
		wclevel_camera_pan_y += ts*wclevel_camera_pan_speed;
	if(wclevel_camera_pan_y > wclevel_camera_to_pan_y)
		wclevel_camera_pan_y -= ts*wclevel_camera_pan_speed;
	wclevel_camera_finish_pan();
	/* Handle map */
	if(wclevel_map)
		wclevel_map->handle();
	/* Handle events */
	//__android_log_print(ANDROID_LOG_INFO, "adMOB", "wclevel_handle");
	for(i = 0;i < wclevel_map->get_event_count();i++)
		((rpgevent*)wclevel_map->get_event(i))->handle();
	/* Handle bridge */
	wcrun_handle();
	/* Handle the player */
	wcplayer_handle();
}

/* Gets possible move */
int wclevel_can_move(int x,int y,int dir,void *me)
{
	/* Map bounds */
	if(wclevel_map)
	{
		/* Can't move off the edge of actual map */
		if(x == 0 && dir == FACE_LEFT && !wclevel_map->wrap_x())
			return 0;
		if(y == 0 && dir == FACE_UP && !wclevel_map->wrap_y())
			return 0;
		if(x == wclevel_map->get_tile_width()-1 && dir == FACE_RIGHT && !wclevel_map->wrap_x())
			return 0;
		if(y == wclevel_map->get_tile_height()-1 && dir == FACE_DOWN && !wclevel_map->wrap_y())
			return 0;
		/* Map check */
		if(!wclevel_map->can_move(x,y,dir,me))
			return 0;
	}
	/* Can move */
	return 1;
}

/* Examines the location given */
int wclevel_examine(int x,int y)
{
	int i,ret;
	rpgevent *et;
	/* Set result */
	ret = 0;
	/* Run */
	if(wclevel_map)
	{
		/* Wrap */
		wclevel_map->wrap(&x,&y);
		/* From lowest ID to highest ID, this is important */
		//__android_log_print(ANDROID_LOG_INFO, "adMOB", "wclevel_examine");
		for(i = 0;i < wclevel_map->get_event_count();i++)
		{
			/* Get event */
			et = (rpgevent*)wclevel_map->get_event(i);
			/* Hit it */
			if(et->examine(x,y))
				ret = 1;
		}
	}
	/* Miss all */
	return ret;
}

/* Steps on top of the location given */
int wclevel_step(int x,int y)
{
	int i;
	rpgevent *et;
	if(wclevel_map)
	{
		/* Wrap */
		wclevel_map->wrap(&x,&y);
		/* From lowest ID to highest ID, this is important */
		//__android_log_print(ANDROID_LOG_INFO, "adMOB", "wclevel_step");
		for(i = 0;i < wclevel_map->get_event_count();i++)
		{
			/* Get event */
			et = (rpgevent*)wclevel_map->get_event(i);
			/* Step on it */
			if(et->step_on(x,y))
				return 1;
		}
	}
	/* Miss all */
	return 0;
}

/* Get camera position */
int wclevel_get_camera_x()
{
	int rx;
	rx = (int)wclevel_camera_x;
	/* Constrain camera once */
	if(wclevel_map)
	{
		if(rx < 0 && !wclevel_map->wrap_x())
			rx = 0;
		if(rx > wclevel_map->get_width()-WCRPG_WIDTH && !wclevel_map->wrap_x())
			rx = wclevel_map->get_width()-WCRPG_WIDTH;
	}
	rx += (int)wclevel_camera_pan_x;
	/* Constrain camera twice */
	if(wclevel_map)
	{
		if(rx < 0 && !wclevel_map->wrap_x())
			rx = 0;
		if(rx > wclevel_map->get_width()-WCRPG_WIDTH && !wclevel_map->wrap_x())
			rx = wclevel_map->get_width()-WCRPG_WIDTH;
	}
	rx += (int)wclevel_camera_force_x;
	return rx;
}
int wclevel_get_camera_y()
{
	int ry;
	ry = (int)wclevel_camera_y;
	/* Constrain camera once */
	if(wclevel_map)
	{
		if(ry < 0 && !wclevel_map->wrap_y())
			ry = 0;
		if(ry > wclevel_map->get_height()-WCRPG_HEIGHT && !wclevel_map->wrap_y())
			ry = wclevel_map->get_height()-WCRPG_HEIGHT;
	}
	ry += (int)wclevel_camera_pan_y;
	/* Constrain camera twice */
	if(wclevel_map)
	{
		if(ry < 0 && !wclevel_map->wrap_y())
			ry = 0;
		if(ry > wclevel_map->get_height()-WCRPG_HEIGHT && !wclevel_map->wrap_y())
			ry = wclevel_map->get_height()-WCRPG_HEIGHT;
	}
	ry += (int)wclevel_camera_force_y;
	return ry;
}

/* Set camera force */
void wclevel_force_camera(float fx,float fy)
{
	wclevel_camera_force_x = fx;
	wclevel_camera_force_y = fy;
}

/* Checks a tile for being in the above layer */
int wclevel_tile_is_above(tile t)
{
	if(!wclevel_map)
		return 0;
	return wclevel_map->tile_is_above(t);
}

/* Wraps an absolute position */
void wclevel_wrap_screen(int *x,int *y)
{
	if(wclevel_map)
		wclevel_map->wrap_screen(x,y);
}

/* Links an event on the map to destinations unknown */
void wclevel_link_event(void *ev)
{
	if(wclevel_map)
		wclevel_map->set_link_event(ev);
}

/* Gets terrain */
int wclevel_get_terrain(int x,int y)
{
	if(wclevel_map)
		return wclevel_map->get_terrain_code(x,y);
	return 0;
}

/* Get event */
int wclevel_get_event_id(int x,int y)
{
	if(wclevel_map)
		return wclevel_map->get_event_id(x,y);
	return 0;
}

/* Redo terrains */
void wclevel_update_all_event_terrain()
{
	int i;
	rpgevent *ev;
	if(wclevel_map)
	{
		__android_log_print(ANDROID_LOG_INFO, "adMOB", "wclevel_update_all_event_terrain");
		for(i = 0;i < wclevel_map->get_event_count();i++)
		{
			ev = (rpgevent*)wclevel_map->get_event(i);
			ev->update_terrain();
		}
	}
}

/* Can land */
int wclevel_can_land(int x,int y,void *me)
{
	/* Return map */
	if(wclevel_map)
		return wclevel_map->can_land(x,y,me);
	/* No map, land everywhere */
	return 1;
}

/* Save level state */
void wclevel_save_state(FILE *f)
{
	int i;
	/* Save count */
	wcsaveio_start(f);
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "wclevel_save_state");
	wcsaveio_write_num(wclevel_map->get_event_count());
	wcsaveio_end();
	/* Save all states */
	for(i = 0;i < wclevel_map->get_event_count();i++)
		((rpgevent*)wclevel_map->get_event(i))->save_state(f);
}

/* Load level state */
void wclevel_load_state(FILE *f)
{
	int i,len;
	/* Load count */
	wcsaveio_read(f);
	len = wcsaveio_read_num();
	wcsaveio_next();
	/* Read all state */
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "wclevel_load_state");
	for(i = 0;i < len;i++)
	{
		if(i >= wclevel_map->get_event_count())
		{
			/* Skip over */
			wcsaveio_read(f);
			wcsaveio_next();
		}
		else
		{
			/* Read in */
			((rpgevent*)wclevel_map->get_event(i))->load_state(f);
		}
	}
}

/* Change level BG */
void wclevel_change_parallax(char *fname,int vs,int vsa,int vsp,int hs,int hsa,int hsp)
{
	/* Does nothing if no map */
	if(!wclevel_map)
		return;
	wclevel_map->change_parallax(fname,vs,vsa,vsp,hs,hsa,hsp);
}

/* Get level BG */
char *wclevel_get_parallax_name()
{
	if(!wclevel_map)
		return 0;
	return wclevel_map->get_parallax_name();
}

/* Get level BG scroll */
void wclevel_get_parallax_scroll(int *scrollv)
{
	if(!wclevel_map)
		memset(scrollv,0,sizeof(int)*6);
	wclevel_map->get_parallax_scroll(scrollv);
}