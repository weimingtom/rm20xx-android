/*
	Animations
*/

/* Includes */
#include <string.h>
#include <malloc.h>
#include <memory.h>
#include "wcrpg.h"
#include "wcanimation.h"
#include "wcarray.h"
#include "wctype.h"
#include "wcmaterial.h"
#include "wcfx.h"

/* Globals */
table *animation_table;
arraylist animations;
animstate *active_anim[WCANIMATION_MAX];
arraylist animation_pool = 0;

/* Pool the animation set */
texture *wcanimation_pool_animset(char *name)
{
	int i;
	animset *as;
	/* Seek */
	for(i = 0;i < wcarray_size(animation_pool);i++)
	{
		as = (animset*)wcarray_get(animation_pool,i);
		/* Found? */
		if(!strcmp(name,as->name))
			return as->tex; /* Return resource */
	}
	/* Not found */
	as = (animset*)malloc(sizeof(animset));
	as->name = wcrpg_string_clone(name);
	as->tex = new texture(wcmaterial_request(MATERIAL_BATTLE,name));
	/* Add it and return it */
	wcarray_add(animation_pool,as);
	return as->tex;
}

/* Create new animation */
animation :: animation(entry *en)
{
	int i;
	table *frames_table;
	table *sfxs_table;
	/* Init */
	id = 0;
	source = 0;
	aim = WCANIMATION_MIDDLE;
	global = 0;
	frames = 0;
	sfxs = 0;
	/* Get ID */
	id = en->get_id();
	/* Get source */
	if(en->has_data(2))
		source = wcstr_read(en->get_data(2),en->get_length(2));
	/* Get scope */
	if(en->has_data(9))
		global = wcint_read(en->get_data(9));
	/* Get aim */
	if(en->has_data(10))
		aim = wcint_read(en->get_data(10));
	/* Get sfx */
	/*if(en->has_data(6))
		sfx = new table(en->get_data(6));*/
	/* Get frames */
	if(en->has_data(12))
	{
		/* Load table and prepare array list */
		frames_table = new table(en->get_data(12));
		frames = wcarray_new();
		/* Make frames */
		for(i = 0;i < frames_table->get_count();i++)
			wcarray_add(frames,new animframe(frames_table->get_entry(i)));
		/* Delete table */
		delete frames_table;
	}
	/* Get sounds and flashes */
	if(en->has_data(6))
	{
		/* Load table and prepare array list */
		sfxs_table = new table(en->get_data(6));
		sfxs = wcarray_new();
		/* Make frames */
		for(i = 0;i < sfxs_table->get_count();i++)
			wcarray_add(sfxs,new animsfx(sfxs_table->get_entry(i)));
		/* Delete table */
		delete sfxs_table;
	}
}

/* Delete animation */
animation :: ~animation()
{
	int i;
	free(source);
	/* Delete frames */
	if(frames)
	{
		/* Get rid of array elements */
		for(i = 0;i < wcarray_size(frames);i++)
			delete (animframe*)wcarray_get(frames,i);
		/* Get rid of list */
		wcarray_delete(frames);
	}
	/* Delete sound effects */
	if(sfxs)
	{
		/* Get rid of elements */
		for(i = 0;i < wcarray_size(sfxs);i++)
			delete (animsfx*)wcarray_get(sfxs,i);
		/* Get rid of list */
		wcarray_delete(sfxs);
	}
}

/* Get aim */
int animation :: get_aim()
{
	return aim;
}

/* Get frame */
animframe *animation :: get_frame(int i)
{
	return (animframe*)wcarray_get(frames,i);
}

/* Get global */
int animation :: is_global()
{
	return global;
}

/* Get sfx */
animsfx *animation :: get_sfx(int i)
{
	return (animsfx*)wcarray_get(sfxs,i);
}

/* Get sfx count */
int animation :: get_sfx_count()
{
	/* None */
	if(!sfxs)
		return 0;
	/* Return size */
	return wcarray_size(sfxs);
}

/* Get length */
int animation :: get_length()
{
	return wcarray_size(frames);
}

/* Get texture */
texture *animation :: get_texture()
{
	if(!source)
		return 0;
	return wcanimation_pool_animset(source);
}

/* Release animation set */
void wcanimation_release_animset(void *v)
{
	animset *as;
	as = (animset*)v;
	free(as->name);
	delete as->tex;
	free(v);
}

/* Reload animation sets */
void wcanimation_reload_all_animset()
{
	/* Delete old */
	if(animation_pool)
		wcarray_delete(animation_pool);
	/* Make new */
	animation_pool = wcarray_new();
	wcarray_set_clean(animation_pool,wcanimation_release_animset);
}

/* Load all */
void wcanimation_load_all(FILE *f,int len)
{
	int i;
	animation *an;
	/* Prepare */
	memset(active_anim,0,sizeof(active_anim));
	/* Prepare things */
	animation_table = new table(f);
	animations = wcarray_new();
	/* Create animations */
	for(i = 0;i < animation_table->get_count();i++)
	{
		an = new animation(animation_table->get_entry(i));
		wcarray_add(animations,an);
	}
	/* Create pool */
	wcanimation_reload_all_animset();
}

/* Get rid of all */
void wcanimation_free_all()
{
	int i;
	/* Delete pool */
	wcarray_delete(animation_pool);
	/* Free table */
	for(i = 0;i < wcarray_size(animations);i++)
		delete (animation*)wcarray_get(animations,i);
	for(i = 0;i < WCANIMATION_MAX;i++)
	{
		if(active_anim[i])
		{
			delete active_anim[i];
			active_anim[i] = 0;
		}
	}
	wcarray_delete(animations);
	delete animation_table;
}

/* Get animation */
animation *wcanimation_get(int id)
{
	if(id < 1 || id > wcarray_size(animations))
	{
		wcrpg_error("Invalid animation id (%d)",id);
		return 0;
	}
	id--;
	return (animation*)wcarray_get(animations,id);
}

/* Play animation */
int wcanimation_play(int id,int x,int y,int ht,rpgevent *ev)
{
	int i,htoff;
	animation *an;
	an = wcanimation_get(id);
	htoff = 0;
	/* Find blank */
	for(i = 0;i < WCANIMATION_MAX;i++)
	{
		/* Empty? */
		if(!active_anim[i])
		{
			/* Get aim */
			if(an->get_aim() == WCANIMATION_HEAD)
				htoff = -ht/2;
			if(an->get_aim() == WCANIMATION_FEET)
				htoff = ht/2;
			/* Add to active list */
			active_anim[i] = new animstate(an);
			active_anim[i]->move(x,y+htoff);
			active_anim[i]->set_height_offset(htoff);
			/* Targeting */
			if(ev)
				active_anim[i]->set_target(ev);
			return i;
		}
	}
	return -1;
}

/* Play animation upon monster */
void wcanimation_play(int id,int x,int y,int ht,monstergroup *gp,int ix)
{
	int handle;
	handle = wcanimation_play(id,x,y,ht,0);
	active_anim[handle]->set_target(0);
	active_anim[handle]->set_target(gp,ix);
}

/* Get length */
int wcanimation_length(int id)
{
	return wcanimation_get(id)->get_length();
}

/* Handle all animations */
void wcanimation_handle()
{
	int i;
	for(i = 0;i < WCANIMATION_MAX;i++)
	{
		/* Simply handle all animations, and terminate on end */
		if(active_anim[i])
		{
			/* Handle */
			if(!active_anim[i]->handle())
			{
				/* Delete and free */
				delete active_anim[i];
				active_anim[i] = 0;
			}
		}
	}
}

/* Draw all aniamtions */
void wcanimation_draw()
{
	int i;
	for(i = 0;i < WCANIMATION_MAX;i++)
	{
		/* Simply draw all active animations */
		if(active_anim[i])
			active_anim[i]->draw();
	}
}

/* Make new animstate */
animstate :: animstate(animation *an)
{
	/* Source anim */
	src = an;
	pos = 0.0f;
	pos_x = 0;
	pos_y = 0;
	pos_frame = 0;
	has_target = 0;
	event_target = 0;
	htoff = 0;
	/* Load texture */
	tex = an->get_texture();
}

/* Delete animstate */
animstate :: ~animstate()
{
}

/* Set height offset */
void animstate :: set_height_offset(int h)
{
	htoff = h;
}

/* Set target */
void animstate :: set_target(rpgevent *et)
{
	/* Target set */
	has_target = 1;
	event_target = et;
}

/* Set target */
void animstate :: set_target(monstergroup *gp,int ix)
{
	group_target = gp;
	index_target = ix;
}

/* Move */
void animstate :: move(int x,int y)
{
	/* Enforce global focus */
	if(src->is_global())
	{
		pos_x = WCRPG_WIDTH/2;
		pos_y = WCRPG_HEIGHT/2;
		return;
	}
	/* Let move */
	pos_x = x;
	pos_y = y;
}

/* Handle */
int animstate :: handle()
{
	animsfx *sfx;
	float ts;
	int i;
	ts = wcrpg_get_time_step();
	pos += ts*2.0f;
	/* Magnetize to targets */
	if(event_target)
	{
		if(!event_target->is_tile())
		{
			pos_x = event_target->get_screen_x()+12-4;
			pos_y = event_target->get_screen_y()+16-8;
		}
		else
		{
			pos_x = event_target->get_screen_x()+8;
			pos_y = event_target->get_screen_y()+8;
		}
		pos_y += htoff;
	}
	/* Advance */
	if(pos_frame < (int)pos)
	{
		/* Play all effects relevant to current frame */
		for(i = 0;i < src->get_sfx_count();i++)
		{
			/* Get */
			sfx = src->get_sfx(i);
			/* Is current frame? */
			if(sfx->get_frame() == pos_frame)
			{
				if(!has_target)
				{
					sfx->play(); /* Play it */
				}
				else
				{
					/* Play on target */
					if(event_target)
						sfx->play(event_target);
					if(group_target)
						sfx->play(group_target,index_target);
				}
			}
		}
		/* Advance the frame */
		pos_frame++;
	}
	/* Terminate */
	if(pos_frame >= src->get_length())
		return 0;
	/* Keep going*/
	return 1;
}

/* Draw a frame */
void animstate :: draw_frame(int f,int x,int y)
{
	animframe *af;
	af = src->get_frame(f);
	af->draw(tex,x,y);
}

/* Draw */
void animstate :: draw()
{
	draw_frame(pos_frame,pos_x,pos_y);
}

/* Make new frame */
animframe :: animframe(entry *en)
{
	int i;
	table *cell_table;
	/* Load cells from table */
	cell_table = new table(en->get_data(1));
	cells = wcarray_new();
	for(i = 0;i < cell_table->get_count();i++)
		wcarray_add(cells,new animcell(cell_table->get_entry(i)));
	delete cell_table;
}

/* Delete frame */
animframe :: ~animframe()
{
	int i;
	for(i = 0;i < wcarray_size(cells);i++)
		delete (animcell*)wcarray_get(cells,i);
	wcarray_delete(cells);
}

/* Draw frame */
void animframe :: draw(texture *tex,int x,int y)
{
	int i;
	for(i = 0;i < wcarray_size(cells);i++)
		((animcell*)wcarray_get(cells,i))->draw(tex,x,y);
}

/* Make new cell */
animcell :: animcell(entry *en)
{
	/* Defaults */
	visible = 1;
	frame = 0;
	pos_x = 0;
	pos_y = 0;
	scale = 1.0f;
	alpha = 0.0f;
	r = 1.0f;
	g = 1.0f;
	b = 1.0f;
	s = 1.0f;
	/* Load visible */
	if(en->has_data(1))
		visible = wcint_read(en->get_data(1));
	/* Get cell */
	if(en->has_data(2))
		frame = wcint_read(en->get_data(2));
	/* Get position */
	if(en->has_data(3))
		pos_x = wcint_read(en->get_data(3));
	if(en->has_data(4))
		pos_y = wcint_read(en->get_data(4));
	/* Get scale */
	if(en->has_data(5))
	{
		scale = (float)wcint_read(en->get_data(5));
		scale /= 100.0f;
	}
	/* Get tone */
	if(en->has_data(6))
		r = (float)(wcint_read(en->get_data(6)))/100.0f;
	if(en->has_data(7))
		g = (float)(wcint_read(en->get_data(7)))/100.0f;
	if(en->has_data(8))
		b = (float)(wcint_read(en->get_data(8)))/100.0f;
	if(en->has_data(9))
		s = (float)(wcint_read(en->get_data(9)))/100.0f;
	/* Get trans */
	if(en->has_data(10))
	{
		alpha = (float)wcint_read(en->get_data(10));
		alpha /= 100.0f;
	}
}

/* Delete cell */
animcell :: ~animcell()
{
}

/* Draw cell */
void animcell :: draw(texture *tex,int x,int y)
{
	int sx,sy;
	/* Not used */
	if(!visible)
		return;
	/* Find cell source */
	sx = frame%5;
	sy = frame/5;
	sx *= 96;
	sy *= 96;
	/* Draw it */
	tex->tone(r,g,b,s);
	tex->set_transparency(alpha,alpha);
	tex->draw(x-(int)(48*scale)+pos_x,y-(int)(48*scale)+pos_y,96,96,sx,sy,(int)(96*scale),(int)(96*scale));
}

/* Create new sfx */
animsfx :: animsfx(entry *en)
{
	/* Init */
	frame = 0;
	snd = 0;
	flash_scope = 0;
	flash_red = 31;
	flash_green = 31;
	flash_blue = 31;
	flash_power = 31;
	/* Get sound */
	if(en->has_data(2))
		snd = new sound(en->get_data(2));
	/* Load settings */
	if(en->has_data(1))
		frame = wcint_read(en->get_data(1));
	if(en->has_data(3))
		flash_scope = wcint_read(en->get_data(3));
	if(en->has_data(4))
		flash_red = wcint_read(en->get_data(4));
	if(en->has_data(5))
		flash_green = wcint_read(en->get_data(5));
	if(en->has_data(6))
		flash_blue = wcint_read(en->get_data(6));
	if(en->has_data(7))
		flash_power = wcint_read(en->get_data(7));
}

/* Delete sfx */
animsfx :: ~animsfx()
{
	/* Delete possible sound */
	if(snd)
		delete snd;
}

/* Play */
void animsfx :: play()
{
	/* Play sound if have sound */
	if(snd)
		snd->play();
	/* Flash */
	if(flash_scope)
	{
		/* Screen */
		if(flash_scope == 2)
			wcfx_flash(flash_red*8,flash_green*8,flash_blue*8,(float)(flash_power)/31.0f,5,0);
	}
}

/* Play on event */
void animsfx :: play(rpgevent *ev)
{
	/* Play sound if have sound */
	if(snd)
		snd->play();
	/* Flash */
	if(flash_scope)
	{
		/* Screen */
		if(flash_scope == 2)
			wcfx_flash(flash_red*8,flash_green*8,flash_blue*8,(float)(flash_power)/31.0f,5,0);
		/* Event */
		if(flash_scope == 1)
			ev->flash(flash_red*8,flash_green*8,flash_blue*8,(float)(flash_power)/31.0f,5);
	}
}

/* Play on monster */
void animsfx :: play(monstergroup *gp,int ix)
{
	/* Play sound if have sound */
	if(snd)
		snd->play();
	/* Flash */
	if(flash_scope)
	{
		/* Screen */
		if(flash_scope == 2)
			wcfx_flash(flash_red*8,flash_green*8,flash_blue*8,(float)(flash_power)/31.0f,5,0);
		/* Monster */
		if(flash_scope == 1)
			gp->flash(ix,(float)(flash_power)/31.0f,flash_red*8,flash_green*8,flash_blue*8);
	}
}

/* Get frame */
int animsfx :: get_frame()
{
	return frame;
}