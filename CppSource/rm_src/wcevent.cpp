/*
	RPG Events
*/

/* Includes */
//#include <windows.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>
#include "wctype.h"
#include "wcevent.h"
#include "wcrpg.h"
#include "wcmaterial.h"
#include "wclevel.h"
#include "wcplayer.h"
#include "wcdialog.h"
#include "wcmisc.h"
#include "wcsaveio.h"

#include "android\log.h"

/* Globals */
arraylist charset_pool;
float wcevent_continue_anim[SPEED_STEPS];
int wcevent_anim_step[SPEED_STEPS];
int wcevent_anim_step_walk[SPEED_STEPS];

/* Handles continous animation */
void wcevent_animate()
{
	int i;
	float ts;
	ts = wcrpg_get_time_step();
	/* Advance clocks */
	wcevent_continue_anim[0] += ts*SPEED_EIGTH;
	wcevent_continue_anim[1] += ts*SPEED_QUARTER;
	wcevent_continue_anim[2] += ts*SPEED_HALF;
	wcevent_continue_anim[3] += ts*SPEED_NORMAL;
	wcevent_continue_anim[4] += ts*SPEED_2X;
	wcevent_continue_anim[5] += ts*SPEED_4X;
	/* Around */
	for(i = 0;i < SPEED_STEPS;i++)
	{
		if(wcevent_continue_anim[i] > 8.0f)
		{
			wcevent_continue_anim[i] = 0;
			/* Normal cycle */
			wcevent_anim_step[i]++;
			/* Walking cycle */
			if(wcevent_anim_step[i]%4 == 0)
				wcevent_anim_step_walk[i] = 1;
			if(wcevent_anim_step[i]%4 == 1)
				wcevent_anim_step_walk[i] = 2;
			if(wcevent_anim_step[i]%4 == 2)
				wcevent_anim_step_walk[i] = 1;
			if(wcevent_anim_step[i]%4 == 3)
				wcevent_anim_step_walk[i] = 0;
		}
	}
}

/* Releases a charset */
void wcevent_release_charset(void *v)
{
	charset *ch;
	ch = (charset*)v;
	free(ch->name);
	delete ch->tex;
	free(v);
}

/* Checks and adds the resource to the charset pool */
texture *wcevent_pool_charset(char *name)
{
	int i;
	charset *ch;
	/* Is it already here */
	for(i = 0;i < wcarray_size(charset_pool);i++)
	{
		ch = (charset*)wcarray_get(charset_pool,i);
		/* Already loaded for you */
		if(!strcmp(ch->name,name))
		{
			wclog_write(wcrpg_get_log(),"Obtained already allocated charset ");
			wclog_write(wcrpg_get_log(),name);
			wclog_enter(wcrpg_get_log());
			return ch->tex;
		}
	}
	/* Not loaded, so add */
	ch = (charset*)malloc(sizeof(charset));
	ch->name = wcrpg_string_clone(name);
	ch->tex = new texture(wcmaterial_request(MATERIAL_CHARSET,name));
	ch->tex->enable_flash();
	ch->tex->reduce();
	wclog_write(wcrpg_get_log(),"Loaded ");
	wclog_write(wcrpg_get_log(),name);
	wclog_write(wcrpg_get_log()," and obtained it (");
	wclog_write(wcrpg_get_log(),wcarray_size(charset_pool));
	wclog_write(wcrpg_get_log()," total)");
	wclog_enter(wcrpg_get_log());
	/* Add and return */
	wcarray_add(charset_pool,ch);
	return ch->tex;
}

/* Prepares event handling */
void wcevent_start()
{
	/* Allocate */
	charset_pool = wcarray_new();
	wcarray_set_clean(charset_pool,wcevent_release_charset);
	memset(wcevent_continue_anim,0,sizeof(wcevent_continue_anim));
	memset(wcevent_anim_step,0,sizeof(wcevent_anim_step));
	memset(wcevent_anim_step_walk,0,sizeof(wcevent_anim_step_walk));
}

/* Ends event handling */
void wcevent_end()
{
	/* Release */
	wcarray_delete(charset_pool);
}

/* Clears character cache */
void wcevent_clear_charset()
{
	wcevent_end();
	wcevent_start();
}

/* Creates new event */
rpgevent :: rpgevent(FILE *f)
{
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "new event");
	page *pg;
	int code,len,rlen,i;
	/* Self vars */
	myvars = wcmem_new_self_vars();
	/* Make list */
	page_list = wcarray_new();
	/* Get id */
	id = wcint_read_file(f,&rlen);
	name = 0;
	phasing = 0;
	pos_x = 0;
	pos_y = 0;
	screen_x = 0;
	lock_facing = 0;
	speedadd = 0;
	terrain_standing = 0;
	nofoot = 0;
	initdir = 0;
	fade = 0;
	screen_y = 0;
	erased = 0;
	jumping = 0;
	foot = 1;
	last_page = -1;
	jump_faced = 0;
	transparent = 0;
	disembody = 0;
	flash_a = 0.0f;
	locking = 0;
	active_move_list = 0;
	footbase = 0;
	t = 0;
	tex = 0;
	direction = FACE_DOWN;
	mode = EVENT_IDLE;
	dist_x = 0.0f;
	dist_y = 0.0f;
	shader_id = 0;
	/* Get properties */
	for(;;)
	{
		/* Get code */
		code = 0;
		fread(&code,1,1,f);
		/* Now for the pages */
		if(code == 0x05)
			break;
		/* Name */
		if(code == 0x01)
			name = wcstr_read_file(f,&rlen);
		/* Position */
		if(code == 0x02)
		{
			len = wcint_read_file(f,&rlen);
			pos_x = wcint_read_file(f,&rlen);
		}
		if(code == 0x03)
		{
			len = wcint_read_file(f,&rlen);
			pos_y = wcint_read_file(f,&rlen);
		}
	}
	/* Get page size */
	len = wcint_read_file(f,&rlen);
	/* Get page count */
	page_count = wcint_read_file(f,&rlen);
	/* Read pages */
	for(i = 0;i < page_count;i++)
	{
		pg = new page(f);
		pg->report(wcrpg_get_log());
		pg->set_who(this);
		wcarray_add(page_list,pg);
	}
	/* Read null code */
	wcint_read_file(f,&rlen);
	/* Apply */
	update_graphics();
	draw_x = (float)(pos_x*16);
	draw_y = (float)(pos_y*16);
	/* Resolution modifier */
	rmag = 1;
	if(tex)
		rmag = tex->get_res_mag(288);
	/* Load program state */
	state = new rpgstate(this);
	/* Restore self vars */
	wcmem_restore_svar(this);
}

/* Update terrain standing */
void rpgevent :: update_terrain()
{
	/* Set terrains */
	terrain_standing = wclevel_get_terrain(pos_x,pos_y);
}

/* Update graphics */
void rpgevent :: update_graphics()
{
	char *ncharset;
	ncharset = 0;
	/* Is different? */
	if(get_page_id() == last_page)
		return;
	/* Update */
	if(get_page())
	{
		ncharset = ((page*)get_page())->get_charset();
		t = ((page*)get_page())->get_tile();
		set_transparent(((page*)get_page())->get_transparent());
	}
	if(ncharset)
		tex = wcevent_pool_charset(ncharset);
	else
		tex = 0;
	/* Face dir if either first time or fixed direction */
	if(!initdir || get_direction_fix())
	{
		if(get_page())
			face(((page*)get_page())->get_direction());
		initdir = 1;
	}
	last_page = get_page_id();
}

/* New blank event */
rpgevent :: rpgevent()
{
	name = 0;
	id = 0;
	pos_x = 0;
	pos_y = 0;
	transparent = 0;
	draw_x = 0;
	dist_x = 0.0f;
	dist_y = 0.0f;
	speedadd = 0;
	phasing = 0;
	jumping = 0;
	screen_x = 0;
	fade = 0;
	nofoot = 0;
	erased = 0;
	initdir = 0;
	jump_faced = 0;
	terrain_standing = 0;
	screen_y = 0;
	flash_a = 0.0f;
	state = 0;
	locking = 0;
	draw_y = 0;
	lock_facing = 0;
	active_move_list = 0;
	last_page = 0;
	direction = FACE_DOWN;
	mode = EVENT_IDLE;
	disembody = 0;
	foot = 1;
	footbase = 0;
	shader_id = 0;
	/* Run no program */
	page_list = 0;
	program_size = 0;
	page_count = 0;
	rmag = 1;
	t = 0;
	tex = 0;
	/* Self vars */
	myvars = wcmem_new_self_vars();
}

/* Delete event */
rpgevent :: ~rpgevent()
{
	int i;
	for(i = 0;i < page_count;i++)
		delete (page*)wcarray_get(page_list,i);
	if(page_list)
		wcarray_delete(page_list);
	/* Release */
	free(name);
	/* Release self vars */
	wcmem_delete_self_vars(myvars);
}

/* Report event */
void rpgevent :: report(elog l)
{
	/* Name */
	wclog_write(l,"Event ");
	if(name)
		wclog_write(l,name);
	wclog_write(l,": Pos=(");
	wclog_write(l,pos_x);
	wclog_write(l,",");
	wclog_write(l,pos_y);
	wclog_write(l,")");
	wclog_enter(l);
}

/* Update screen positions */
void rpgevent :: update_screen()
{
	int rdraw_x,rdraw_y;
	/* Store */
	rdraw_x = -wclevel_get_camera_x()+(int)draw_x;
	rdraw_y = -wclevel_get_camera_y()+(int)draw_y;
	wclevel_wrap_screen(&rdraw_x,&rdraw_y);
	screen_x = rdraw_x;
	screen_y = rdraw_y;
}

/* Draw event */
void rpgevent :: draw(int x,int y)
{
	int sx,sy;
	int rdraw_x,rdraw_y;
	float fadef;
	/* Erased? */
	if(erased)
		return;
	/* Can't draw nothing */
	if(!tex)
		return;
	/* Can't draw if not paged */
	if(get_page_id() == -1)
		return;
	/* Set shader */
	wcdraw_set_shader(shader_id);
	/* Store */
	rdraw_x = x+(int)draw_x;
	rdraw_y = y+(int)draw_y;
	wclevel_wrap_screen(&rdraw_x,&rdraw_y);
	/* Set mag */
	wcdraw_set_local_mag(1.0f/(float)(rmag));
	/* Set flash */
	tex->set_flash(flash_a*flash_strength,flash_red,flash_green,flash_blue);
	/* Set trans */
	fadef = 0.0f;
	if(fade)
		fadef = ((float)fade)/8.0f;
	if(transparent)
		fadef = fadef*0.5f+0.5f;
	if(transparent || fade)
		tex->set_transparency(fadef,fadef);
	/* Calculate source */
	sx = t%4;
	sy = t/4;
	if(!get_graphic_fix())
	{
		if(!get_auto_spin())
		{
			if(!nofoot)
			{
				if(!get_auto_walk()){
					//__android_log_print(ANDROID_LOG_INFO, "adMOB", "get_auto_walk %d %d %d %d %d %d",rdraw_x-4,rdraw_y-16,24*rmag,32*rmag,sx*72*rmag+foot*24*rmag,sy*128*rmag+direction*32*rmag);
					tex->draw(rdraw_x-4,rdraw_y-16,24*rmag,32*rmag,sx*72*rmag+foot*24*rmag,sy*128*rmag+direction*32*rmag);
				}
				else{
					tex->draw(rdraw_x-4,rdraw_y-16,24*rmag,32*rmag,sx*72*rmag+wcevent_anim_step_walk[get_speed()]*24*rmag,sy*128*rmag+direction*32*rmag);
				}
			}
			else
			{
				tex->draw(rdraw_x-4,rdraw_y-16,24*rmag,32*rmag,sx*72*rmag+24*rmag,sy*128*rmag+direction*32*rmag);
			}
		}
		else
		{
			tex->draw(rdraw_x-4,rdraw_y-16,24*rmag,32*rmag,sx*72*rmag+(24*get_start_foot())*rmag,sy*128*rmag+wcevent_anim_step[get_speed()]%4*32*rmag);
		}
	}
	else
	{
		tex->draw(rdraw_x-4,rdraw_y-16,24*rmag,32*rmag,sx*72*rmag+(24*get_start_foot())*rmag,sy*128*rmag+direction*32*rmag);
	}
	/* Reset trans */
	tex->set_transparency(0.0f,0.0f);
	/* Reset mag */
	wcdraw_reset_local_mag();
	/* Update screen */
	update_screen();
	/* Reset flash */
	tex->set_flash(0.0f,0,0,0);
	/* Release shader */
	wcdraw_set_shader(0);
}

/* Draw event as tile */
void rpgevent :: draw(int x,int y,texture *tiletex)
{
	int st,sx,sy;
	int rdraw_x,rdraw_y;
	float fadef;
	/* Erased? */
	if(erased)
		return;
	/* Can't have a charset */
	if(tex)
		return;
	/* Can't draw if not paged */
	if(get_page_id() == -1)
		return;
	/* Set shader */
	wcdraw_set_shader(shader_id);
	/* Set flash */
	tiletex->set_flash(flash_a*flash_strength,flash_red,flash_green,flash_blue);
	/* Wrap */
	rdraw_x = x+(int)draw_x;
	rdraw_y = y+(int)draw_y;
	wclevel_wrap_screen(&rdraw_x,&rdraw_y);
	/* Set mag */
	wcdraw_set_local_mag(1.0f/(float)(rmag));
	/* Set trans */
	fadef = 0.0f;
	if(fade)
		fadef = ((float)fade)/8.0f;
	if(transparent)
		fadef = fadef*0.5f+0.5f;
	if(transparent || fade)
		tiletex->set_transparency(fadef,fadef);
	/* Get tile */
	st = t;
	/* Fuckhead correctness, seriously what the hell Enterbrain? */
	if(st == 129)
		st = 128;
	if(st == 385)
		st = 129;
	if(st == 641)
		st = 130;
	if(st == 897)
		st = 131;
	if(st == 1153)
		st = 132;
	if(st == 1409)
		st = 133;
	if(st == 1665)
		st = 134;
	if(st == 1921)
		st = 135;
	if(st == 2177)
		st = 136;
	if(st == 2433)
		st = 137;
	if(st == 2689)
		st = 138;
	if(st == 2945)
		st = 139;
	if(st == 3201)
		st = 140;
	if(st == 3457)
		st = 141;
	if(st == 3713)
		st = 142;
	if(st == 3969)
		st = 143;
	/* Calculate source */
	st += 48;
	sx = st%6;
	if(st >= 96)
		sx += 6;
	sy = (st/6)%16;
	/* Draw it */
	if(transparent)
		tiletex->set_transparency(0.5f,0.5f);
	tiletex->draw(rdraw_x,rdraw_y,16*rmag,16*rmag,288*rmag+sx*16*rmag,sy*16*rmag);
	if(transparent)
		tiletex->set_transparency(0.0f,0.0f);
	/* Reset trans */
	tiletex->set_transparency(0.0f,0.0f);
	/* Reset mag */
	wcdraw_reset_local_mag();
	/* Update screen */
	update_screen();
	/* Reset flash */
	tiletex->set_flash(0.0f,0,0,0);
	/* Release shader */
	wcdraw_set_shader(0);
}

/* Place event */
void rpgevent :: place(int tx,int ty)
{
	/* Update */
	pos_x = tx;
	pos_y = ty;
	/* Set terrains */
	update_terrain();
	draw_x = (float)(pos_x*16);
	draw_y = (float)(pos_y*16);
}

/* Change graphics */
void rpgevent :: change_graphics(texture *ntex,int n)
{
	tex = ntex;
	t = n;
	rmag = ntex->get_res_mag(288);
}

/* Centers the camera about this event */
void rpgevent :: focus()
{
	wclevel_set_camera(draw_x-WCRPG_WIDTH/2+16,draw_y-WCRPG_HEIGHT/2+8);
}

/* Set direction */
void rpgevent :: face(int dir)
{
	if(jumping && jump_faced)
		return;
	if(jumping && !jump_faced)
		jump_faced = 1;
	direction = dir;
}

/* Move the event */
int rpgevent :: move(int xdir,int ydir)
{
	int targ_x,targ_y;
	/* Fail if already moving */
	if(mode == EVENT_MOVING)
		return 0;
	/* Fail if jumping */
	if(jumping == JUMP_EXECUTE)
		return 0;
	/* Adjust jump target if jumping */
	if(jumping == JUMP_PREPARE)
	{
		jump_to_x += xdir;
		jump_to_y += ydir;
		return 1;
	}
	/* Hit player */
	targ_x = pos_x;
	targ_y = pos_y;
	targ_x += xdir;
	targ_y += ydir;
	/* If I am the player, step on things */
	if(this == wcplayer_get())
		wclevel_step(targ_x,targ_y);
	/* Only hit anything if not phasing */
	if(!phasing)
	{
		if(get_page())
		{
			if(((page*)get_page())->get_trigger() == START_TOUCH)
			{
				if(wcplayer_get()->hit(targ_x,targ_y))
				{
					/* BAM! */
					state->run();
				}
			}
		}
		/* Hit player but no process */
		if(wcplayer_get()->hit(targ_x,targ_y))
			return 0;
	}
	/* Can move? */
	if(!phasing)
	{
		if(xdir > 0)
		{
			if(!wclevel_can_move(pos_x,pos_y,FACE_RIGHT,this))
				return 0;
		}
		if(xdir < 0)
		{
			if(!wclevel_can_move(pos_x,pos_y,FACE_LEFT,this))
				return 0;
		}
		if(ydir > 0)
		{
			if(!wclevel_can_move(pos_x,pos_y,FACE_DOWN,this))
				return 0;
		}
		if(ydir < 0)
		{
			if(!wclevel_can_move(pos_x,pos_y,FACE_UP,this))
				return 0;
		}
	}
	/* Reset distance */
	dist_x = 0.0f;
	dist_y = 0.0f;
	/* Create distance */
	if(xdir > 0)
		dist_x = 16.0f;
	if(xdir < 0)
		dist_x = -16.0f;
	if(ydir > 0)
		dist_y = 16.0f;
	if(ydir < 0)
		dist_y = -16.0f;
	/* Move physical tile location */
	pos_x += xdir;
	pos_y += ydir;
	/* Set my standing */
	terrain_standing = wclevel_get_terrain(pos_x,pos_y);
	/* I am now moving */
	mode = EVENT_MOVING;
	return 1;
}

/* Get mode */
int rpgevent :: get_state()
{
	return mode;
}

/* Get route */
movelist *rpgevent :: get_route()
{
	/* Active */
	if(active_move_list)
		return active_move_list;
	/* None */
	if(!get_page())
		return 0;
	/* Current */
	return ((page*)get_page())->get_route();
}

/* Get foot */
int rpgevent :: get_start_foot()
{
	/* None */
	if(!get_page())
		return 0;
	/* Current */
	return ((page*)get_page())->get_foot();
}

/* Handle the event */
void rpgevent :: handle()
{
	float ts;
	/* Erased? */
	if(erased)
		return;
	/* Correct speed */
	correct_speed();
	/* Update graphics */
	update_graphics();
	/* Update screen */
	update_screen();
	/* Flashing */
	if(flash_a > 0.0f)
	{
		/* Reduce */
		ts = wcrpg_get_time_step();
		flash_a -= flash_speed*ts;
		/* Out of flash */
		if(flash_a < 0.0f)
			flash_a = 0.0f;
	}
	/* Execute program */
	if(state)
	{
		state->handle();
		/* Just now erased */
		if(erased)
			return;
		/* Unlocks controls when examine/impact/auto event is finished */
		if(locking && !state->is_running() && !wcdialog_active())
		{
			/* Faces are always cleared when these events exit */
			wcdialog_erase_face();
			wcdialog_reset_line();
			locking = 0;
			wcplayer_unlock();
		}
	}
	/* Parallel? */
	if(get_page() && !is_severed())
	{
		if(((page*)get_page())->get_trigger() == START_PARALLEL)
		{
			/* Always keep running program */
			if(state)
			{
				state->prevent_focus();
				state->run();
			}
		}
		/* Auto? */
		if(((page*)get_page())->get_trigger() == START_AUTO)
		{
			/* Always keep running program */
			if(state)
			{
				locking = 1;
				wcplayer_lock();
				state->run();
			}
		}
	}
	/* Execute route */
	if(get_route())
	{
		if(!active_move_list)
		{
			if(!wcdialog_active())
			{
				if(((page*)get_page())->has_route())
					get_route()->run(this);
				else
					((page*)get_page())->handle_route(this);
			}
		}
		else
		{
			active_move_list->run(this);
			if(active_move_list->is_finished())
			{
				if(!walking())
				{
					wcmove_remove_active();
					active_move_list->reset();
					active_move_list = 0;
				}
			}
		}
	}
	/* Find out speed to move */
	if(get_speed() == 0)
		ts = wcrpg_get_time_step()*SPEED_EIGTH;
	if(get_speed() == 1)
		ts = wcrpg_get_time_step()*SPEED_QUARTER;
	if(get_speed() == 2)
		ts = wcrpg_get_time_step()*SPEED_HALF;
	if(get_speed() == 3)
		ts = wcrpg_get_time_step()*SPEED_NORMAL;
	if(get_speed() == 4)
		ts = wcrpg_get_time_step()*SPEED_2X;
	if(get_speed() == 5)
		ts = wcrpg_get_time_step()*SPEED_4X;
	/* Set foot */
	foot = 1;
	/* Make me jump */
	if(jumping == JUMP_EXECUTE)
	{
		/* Move alpha */
		jump_alpha += ts/16.0f;
		/* Calculate distances */
		draw_x = jump_draw_x+jump_dist_x*jump_alpha;
		draw_y = jump_draw_y+jump_dist_y*jump_alpha;
		jump_draw_y += jump_vel_y;
		jump_vel_y += ts/3.5f;
		/* End alpha? */
		if(jump_alpha >= 1.0f)
		{
			/* Put it there */
			jumping = 0;
			place(jump_to_x,jump_to_y);
			draw_x = (float)(pos_x*16);
			draw_y = (float)(pos_y*16);
		}
	}
	/* Make me move */
	if(mode == EVENT_MOVING && !jumping)
	{
		/* Positive moving */
		if(dist_x > 0)
		{
			if(dist_x < ts)
			{
				draw_x += dist_x;
				dist_x = 0;
			}
			else
			{
				draw_x += ts;
				dist_x -= ts;
			}
		}
		if(dist_y > 0)
		{
			if(dist_y < ts)
			{
				draw_y += dist_y;
				dist_y = 0;
			}
			else
			{
				draw_y += ts;
				dist_y -= ts;
			}
		}
		/* Negative moving */
		if(dist_x < 0)
		{
			if(dist_x > -ts)
			{
				draw_x += dist_x;
				dist_x = 0;
			}
			else
			{
				draw_x -= ts;
				dist_x += ts;
			}
		}
		if(dist_y < 0)
		{
			if(dist_y > -ts)
			{
				draw_y += dist_y;
				dist_y = 0;
			}
			else
			{
				draw_y -= ts;
				dist_y += ts;
			}
		}
		/* Halfway there? */
		if((dist_x > 0 && dist_x <= 8) || (dist_y > 0 && dist_y <= 8) || (dist_x < 0 && dist_x >= -8) || (dist_y < 0 && dist_y >= -8))
		{
			if(!footbase)
				foot = 2;
			if(footbase)
				foot = 0;
		}
		/* Done moving */
		if(dist_x == 0 && dist_y == 0)
		{
			footbase = (footbase+1)%2;
			draw_x = (float)(pos_x*16);
			draw_y = (float)(pos_y*16);
			mode = EVENT_IDLE;
		}
	}
}

#ifndef NDEBUG

/* Dump */
void rpgevent :: disassemble(char *filename)
{
	char buffer[260];
	int i;
	page *pg;
	/* In end-user performance evaluation mode, they do not care about event dissaasembly */
#ifdef WCPERFORM
	return;
#endif
	/* Announce */
	wclog_write(wcrpg_get_log(),"Disassembling ");
	wclog_write(wcrpg_get_log(),page_count);
	wclog_write(wcrpg_get_log()," pages to ");
	wclog_write(wcrpg_get_log(),filename);
	wclog_enter(wcrpg_get_log());
	/* Through all pages */
	for(i = 0;i < page_count;i++)
	{
		/* Make filename */
		sprintf(buffer,"%s.%d.txt",filename,i);
		/* Get page */
		pg = (page*)wcarray_get(page_list,i);
		/* Take it apart */
		pg->disassemble(buffer);
	}
}

#endif
#ifdef NDEBUG

/* Dump */
void rpgevent :: disassemble(char *filename)
{
}

#endif

/* Get name */
char *rpgevent :: get_name()
{
	return name;
}

/* Get page */
void *rpgevent :: get_page()
{
	page *pg;
	int i;
	if(!page_list)
		return 0;
	/* Find the most accurate page from the right down */
	for(i = wcarray_size(page_list)-1;i >= 0;i--)
	{
		if(((page*)wcarray_get(page_list,i))->is_true())
			break;
	}
	/* No pages? */
	if(i < 0)
		return 0;
	/* Return that page */
	pg = (page*)wcarray_get(page_list,i);
	return pg;
}

/* Get page id */
int rpgevent :: get_page_id()
{
	int i;
	if(!page_list)
		return 0;
	/* Find the most accurate page from the right down */
	for(i = wcarray_size(page_list)-1;i >= 0;i--)
	{
		if(((page*)wcarray_get(page_list,i))->is_true())
			break;
	}
	/* No pages? */
	if(i < 0)
		return -1;
	/* Return that page */
	return i;
}

/* Get layer */
int rpgevent :: get_layer()
{
	/* Solid event */
	if(!get_page())
		return EVENT_BELOW;
	return ((page*)get_page())->get_layer();
}

/* Get hit */
int rpgevent :: hit(int x,int y)
{
	if(phasing)
		return 0;
	if(erased)
		return 0;
	/* Player is always solid */
	if(this == wcplayer_get())
	{
		if(x == pos_x && y == pos_y)
			return 1;
	}
	/* Can't be solid if not paged */
	if(get_page_id() == -1)
		return 0;
	if(x == pos_x && y == pos_y && get_layer() == EVENT_SOLID)
		return 1;
	return 0;
}

/* Get animation type */
int rpgevent :: get_animation()
{
	if(!get_page())
		return ANIM_NORMAL;
	return ((page*)get_page())->get_animation();
}

/* Returns true if event is auto walking animation */
int rpgevent :: get_auto_walk()
{
	int an;
	/* Get anim */
	an = get_animation();
	/* Check */
	if(an == ANIM_ALWAYS || an == ANIM_FIX_AND_ALWAYS)
		return 1;
	return 0;
}

/* Returns true if event is auto spinning */
int rpgevent :: get_auto_spin()
{
	int an;
	/* Get anim */
	an = get_animation();
	/* Check */
	if(an == ANIM_SPIN)
		return 1;
	return 0;
}

/* Returns fixed graphic */
int rpgevent :: get_graphic_fix()
{
	int an;
	/* Get anim */
	an = get_animation();
	if(an == ANIM_NEVER)
		return 1;
	return 0;
}

/* Returns true if event cant turn normally by walking */
int rpgevent :: get_direction_fix()
{
	int an;
	an = get_animation();
	if(lock_facing)
		return 1;
	if(an == ANIM_NEVER || an == ANIM_FIX || an == ANIM_FIX_AND_ALWAYS || an == ANIM_SPIN)
		return 1;
	return 0;
}

/* Returns position */
int rpgevent :: get_pos_x()
{
	return pos_x;
}
int rpgevent :: get_pos_y()
{
	return pos_y;
}

/* Get terrain */
int rpgevent :: get_terrain()
{
	return terrain_standing;
}

/* Return command */
command *rpgevent :: get_command(int counter,int pagenum)
{
	/* Error number */
	if(pagenum < 0 || pagenum >= wcarray_size(page_list))
	{
		wcrpg_error("Invalid page number (%d)",pagenum);
		return 0;
	}
	return (command*)((page*)wcarray_get(page_list,pagenum))->get_command(counter);
}

/* Gets the page length */
int rpgevent :: get_page_length(int pagenum)
{
	/* Error number */
	if(pagenum < 0 || pagenum >= wcarray_size(page_list))
	{
		wcrpg_error("Invalid page number (%d)",pagenum);
		return 0;
	}
	return ((page*)wcarray_get(page_list,pagenum))->get_length();
}

/* Gets direction facing */
int rpgevent :: get_direction()
{
	return direction;
}

/* Examine? */
int rpgevent :: examine(int x,int y)
{
	if(erased)
		return 0;
	if(x == pos_x && y == pos_y)
	{
		if(get_page())
		{
			/* Execute only if i can be examined */
			if(((page*)get_page())->get_trigger() == START_KEY)
			{
				/* Can only really lock if theres anything on this page to run */
				if(get_page_length(get_page_id()) > 0)
				{
					locking = 1;
					wcplayer_lock();
				}
				state->run();
				return 1;
			}
		}
	}
	return 0;
}

/* Step on? */
int rpgevent :: step_on(int x,int y)
{
	if(erased)
		return 0;
	if(x == pos_x && y == pos_y)
	{
		if(get_page())
		{
			/* Execute only if i can be impacted */
			if(((page*)get_page())->get_trigger() == START_IMPACT || ((page*)get_page())->get_trigger() == START_TOUCH)
			{
				locking = 1;
				wcplayer_lock();
				state->delay_until_hero_is_finished_moving();
				state->run();
				return 1;
			}
		}
	}
	return 0;
}

/* Get ID no */
int rpgevent :: get_id()
{
	return id;
}

/* Get state */
rpgstate *rpgevent :: get_program_state()
{
	return state;
}

/* Get speed */
int rpgevent :: get_speed()
{
	/* Hero speed */
	if(!get_page())
		return 3+speedadd;
	/* My speed */
	return ((page*)get_page())->get_speed()+speedadd;
}

/* Turn right */
void rpgevent :: turn_right()
{
	if(direction == FACE_UP)
	{
		face(FACE_RIGHT);
		return;
	}
	if(direction == FACE_RIGHT)
	{
		face(FACE_DOWN);
		return;
	}
	if(direction == FACE_DOWN)
	{
		face(FACE_LEFT);
		return;
	}
	if(direction == FACE_LEFT)
	{
		face(FACE_UP);
		return;
	}
}

/* Turn left */
void rpgevent :: turn_left()
{
	if(direction == FACE_UP)
	{
		face(FACE_LEFT);
		return;
	}
	if(direction == FACE_RIGHT)
	{
		face(FACE_UP);
		return;
	}
	if(direction == FACE_DOWN)
	{
		face(FACE_RIGHT);
		return;
	}
	if(direction == FACE_LEFT)
	{
		face(FACE_DOWN);
		return;
	}
}

/* About face! */
void rpgevent :: about_face()
{
	if(direction == FACE_UP)
	{
		face(FACE_DOWN);
		return;
	}
	if(direction == FACE_RIGHT)
	{
		face(FACE_LEFT);
		return;
	}
	if(direction == FACE_DOWN)
	{
		face(FACE_UP);
		return;
	}
	if(direction == FACE_LEFT)
	{
		face(FACE_RIGHT);
		return;
	}
}

/* Face player */
void rpgevent :: face_player()
{
	rpgevent *ev;
	int px,py,dx,dy;
	/* Locate player */
	ev = wcplayer_get();
	if(!ev)
		return; /* Can't face player when there is no player */
	px = ev->get_pos_x();
	py = ev->get_pos_y();
	dx = px-pos_x;
	dy = py-pos_y;
	/* Face */
	if(dy*dy >= dx*dx)
	{
		if(py < pos_y)
			face(FACE_UP);
		else
			face(FACE_DOWN);
	}
	else
	{
		if(px < pos_x)
			face(FACE_LEFT);
		else
			face(FACE_RIGHT);
	}
}

/* Disembody this event */
void rpgevent :: sever()
{
	locking = 1;
	wcplayer_lock();
	disembody = 1;
}

/* Is sever? */
int rpgevent :: is_severed()
{
	return disembody;
}

/* Get screen x */
int rpgevent :: get_screen_x()
{
	return screen_x;
}

/* Get screen y */
int rpgevent :: get_screen_y()
{
	return screen_y;
}

/* Erase */
void rpgevent :: erase()
{
	/* Unlock */
	if(locking)
	{
		locking = 0;
		wcplayer_unlock();
	}
	/* Erase */
	erased = 1;
}

/* Sets a move list */
void rpgevent :: set_move_list(movelist *mls)
{
	/* Add? */
	if(!active_move_list)
		wcmove_add_active();
	/* Run this to its end */
	active_move_list = mls;
	if(active_move_list)
		active_move_list->reset();
}

/* Sets phasing */
void rpgevent :: set_phasing(int en)
{
	phasing = en;
}

/* Sets transparent */
void rpgevent :: set_transparent(int en)
{
	transparent = en;
}

/* Gets tile */
tile rpgevent :: get_tile()
{
	return t+10000;
}

/* Is tile */
int rpgevent :: is_tile()
{
	return (!tex);
}

/* Get the ID of the map this event is on */
int rpgevent :: get_map_id()
{
	/* Usually just the current map ID */
	return wclevel_get_id();
}

/* Flash */
void rpgevent :: flash(int flr,int flg,int flb,float flp,int flt)
{
	/* Set flashing */
	flash_a = 1.0f;
	flash_strength = flp;
	flash_speed = (1.0f/(0.1f*(float)flt))/14.0f;
	flash_red = flr;
	flash_green = flg;
	flash_blue = flb;
}

/* Lock facing */
void rpgevent :: set_lock_facing(int en)
{
	lock_facing = en;
}

/* Set var */
void rpgevent :: set_var(int i,var v)
{
	/* Do nothing if no vars */
	if(!myvars)
		return;
	/* Set var */
	myvars[i] = v;
}

/* Get var */
var rpgevent :: get_var(int i)
{
	/* Zero if no vars */
	if(!myvars)
		return 0;
	/* Get var */
	return myvars[i];
}

/* Walking */
int rpgevent :: walking()
{
	return (dist_x != 0.0f || dist_y != 0.0f || jumping);
}

/* Set shader */
void rpgevent :: set_shader(int id)
{
	shader_id = id;
}

/* Get fade */
int rpgevent :: get_fade()
{
	return fade;
}

/* Set fade */
void rpgevent :: set_fade(int fd)
{
	if(fd < 0)
		fd = 0;
	if(fd > 7)
		fd = 7;
	fade = fd;
}

/* Lock feet */
void rpgevent :: lock_feet(int en)
{
	nofoot = en;
}

/* Correct speed */
void rpgevent :: correct_speed()
{
	if(get_speed() >= SPEED_STEPS)
		speedadd -= get_speed()-SPEED_STEPS+1;
	if(get_speed() < 0)
		speedadd += -get_speed();
}

/* Speed up! */
void rpgevent :: speed_up()
{
	speedadd++;
	correct_speed();
}

/* Speed down! */
void rpgevent :: speed_down()
{
	speedadd--;
	correct_speed();
}

/* Memorize speed */
void rpgevent :: memorize_speed()
{
	wcmisc_set_player_speed(speedadd);
}

/* Recall speed */
void rpgevent :: recall_speed()
{
	speedadd = wcmisc_get_player_speed();
}

/* Start jump */
void rpgevent :: start_jump()
{
	/* Event is now about to jump */
	jumping = JUMP_PREPARE;
	jump_faced = 0;
	/* Set to and from */
	jump_from_x = get_pos_x();
	jump_from_y = get_pos_y();
	jump_to_x = jump_from_x;
	jump_to_y = jump_from_y;
	/* Reset dist */
	jump_alpha = 0.0f;
}

/* Execute jump */
int rpgevent :: end_jump()
{
	/* Must be jumping */
	if(!jumping)
		return 1;
	/* Must not be executing jump */
	if(jumping == JUMP_EXECUTE)
		return 0;
	/* Must be able to land */
	if(!wclevel_can_land(jump_to_x,jump_to_y,this) && !phasing)
		return 0;
	/* Execute */
	jumping = JUMP_EXECUTE;
	/* Find out distances */
	jump_dist_x = (float)(jump_to_x-jump_from_x);
	jump_dist_y = (float)(jump_to_y-jump_from_y);
	jump_dist_x *= 16.0f;
	jump_dist_y *= 16.0f;
	jump_draw_x = draw_x;
	jump_draw_y = draw_y;
	/* Jump! */
	jump_vel_y = -2.0f;
	return 0;
}

/* Saves state */
void rpgevent :: save_state(FILE *f)
{
	wcsaveio_start(f);
	/* Save my position and facing */
	wcsaveio_write_num(pos_x);
	wcsaveio_write_num(pos_y);
	wcsaveio_write_num(direction);
	/* Save my program */
	if(this != wcplayer_get())
		state->save_state(f);
	/* Save my moves */
	if(active_move_list)
	{
		wcsaveio_write_num(1);
		active_move_list->save_state();
	}
	else
	{
		wcsaveio_write_num(0);
	}
	/* Save other state values */
	wcsaveio_write_num(speedadd);
	wcsaveio_write_float(draw_x);
	wcsaveio_write_float(draw_y);
	wcsaveio_write_float(dist_x);
	wcsaveio_write_float(dist_y);
	wcsaveio_write_num(nofoot);
	wcsaveio_write_num(fade);
	wcsaveio_write_num(shader_id);
	wcsaveio_write_num(erased);
	wcsaveio_write_num(phasing);
	/* Save jumps */
	wcsaveio_write_num(jumping);
	wcsaveio_write_num(jump_faced);
	wcsaveio_write_num(jump_from_x);
	wcsaveio_write_num(jump_from_y);
	wcsaveio_write_num(jump_to_x);
	wcsaveio_write_num(jump_to_y);
	wcsaveio_write_float(jump_dist_x);
	wcsaveio_write_float(jump_dist_y);
	wcsaveio_write_float(jump_draw_x);
	wcsaveio_write_float(jump_draw_y);
	wcsaveio_write_float(jump_alpha);
	wcsaveio_write_float(jump_vel_y);
	wcsaveio_end();
}

/* Loads state */
void rpgevent :: load_state(FILE *f)
{
	int hasmoves;
	wcsaveio_read(f);
	/* Read my position and facing */
	pos_x = wcsaveio_read_num();
	pos_y = wcsaveio_read_num();
	direction = wcsaveio_read_num();
	/* Load my program */
	if(this != wcplayer_get())
		state->load_state(f);
	/* Load my moves */
	hasmoves = wcsaveio_read_num();
	if(hasmoves)
	{
		if(!active_move_list)
			active_move_list = new movelist();
		active_move_list->load_state();
	}
	/* Load other state values */
	speedadd = wcsaveio_read_num();
	draw_x = wcsaveio_read_float();
	draw_y = wcsaveio_read_float();
	dist_x = wcsaveio_read_float();
	dist_y = wcsaveio_read_float();
	nofoot = wcsaveio_read_num();
	fade = wcsaveio_read_num();
	shader_id = wcsaveio_read_num();
	erased = wcsaveio_read_num();
	phasing = wcsaveio_read_num();
	/* Read jumps */
	jumping = wcsaveio_read_num();
	jump_faced = wcsaveio_read_num();
	jump_from_x = wcsaveio_read_num();
	jump_from_y = wcsaveio_read_num();
	jump_to_x = wcsaveio_read_num();
	jump_to_y = wcsaveio_read_num();
	jump_dist_x = wcsaveio_read_float();
	jump_dist_y = wcsaveio_read_float();
	jump_draw_x = wcsaveio_read_float();
	jump_draw_y = wcsaveio_read_float();
	jump_alpha = wcsaveio_read_float();
	jump_vel_y = wcsaveio_read_float();
	wcsaveio_next();
}