/*
	Game Map
*/

/* Includes */
//#include <windows.h>
#include <stdio.h>
#include <malloc.h>
#include "wcmap.h"
#include "wclog.h"
#include "wcrpg.h"
#include "wctype.h"
#include "wcplayer.h"
#include "wctext.h"
#include "wcmaterial.h"
#include "wctileset.h"

#include "../jniUtil.h"

/* Globals */
float wcmap_ocean_step = 0;
float wcmap_anim_step = 0;
int wcmap_ocean_off = 0;
int wcmap_anim_off = 0;

/* Animate maps */
void wcmap_animate()
{
	/* Increase */
	wcmap_ocean_step += wcrpg_get_time_step()*0.75f;
	wcmap_anim_step += wcrpg_get_time_step()*2.0f;
	/* Oh that classic pattern */
	wcmap_ocean_off = 1;
	wcmap_anim_off = 0;
	/* Step 1 */
	if(wcmap_ocean_step >= 4.0f)
		wcmap_ocean_off = 2;
	/* Step 2 */
	if(wcmap_ocean_step >= 8.0f)
		wcmap_ocean_off = 1;
	/* Step 3 */
	if(wcmap_ocean_step >= 12.0f)
		wcmap_ocean_off = 0;
	/* Wrap */
	if(wcmap_ocean_step >= 16.0f)
		wcmap_ocean_step = 0;
	/* Step 1 */
	if(wcmap_anim_step >= 4.0f)
		wcmap_anim_off = 1;
	/* Step 2 */
	if(wcmap_anim_step >= 8.0f)
		wcmap_anim_off = 2;
	/* Step 3 */
	if(wcmap_anim_step >= 12.0f)
		wcmap_anim_off = 3;
	/* Wrap */
	if(wcmap_anim_step >= 16.0f)
		wcmap_anim_step = 0;
}

/* Load events */
void map :: load_events(FILE *f)
{
	char buffer[260];
	rpgevent *ev;
	int code,rlen,len,i;
	/* Load code */
	code = 0;
	fread(&code,1,1,f);
	if(!code)
		return;
	if(code != 0x51)
	{
		wcrpg_error("Event section is not found where expected");
		return;
	}
	/* Get count */
	len = wcint_read_file(f,&rlen);
	/* Read event count */
	event_count = wcint_read_file(f,&rlen);
	len -= rlen;
	wclog_write(wcrpg_get_log(),event_count);
	wclog_write(wcrpg_get_log()," events");
	wclog_enter(wcrpg_get_log());
	/* Read events */
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "event_count [%d] ",event_count);
	for(i = 0;i < event_count;i++)
	{
		ev = new rpgevent(f);
		sprintf(buffer,"Logs/%s",ev->get_name());
		ev->disassemble(buffer);
		ev->report(wcrpg_get_log());
		wcarray_add(event_list,ev);
	}
}

/* Load map */
map :: map(int id)
{
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "load map %d", id);
	FILE *f;
	char buffer[260];
	char *filetype;
	int code,len,rlen;
	/* Allocate */
	event_list = wcarray_new();
	/* Defaults */
	link_event = 0;
	ptex = 0;
	map_id = id;
	h_pan = 0;
	h_speed = 0;
	h_auto = 0;
	v_pan = 0;
	v_speed = 0;
	v_auto = 0;
	parallax_anim_x = 0;
	parallax_anim_y = 0;
	parallax = 0;
	current_parallax = 0;
	tset_id = 1;
	width = 20;
	height = 15;
	/* Get file name */
	sprintf(buffer,"%sMap%04d.lmu","/sdcard/wcr2kxx/assets/",id);
	/* Announce operation */
	wclog_write(wcrpg_get_log(),"Loading map ");
	wclog_write(wcrpg_get_log(),buffer);
	wclog_write(wcrpg_get_log(),"...");
	wclog_enter(wcrpg_get_log());
	/* Open a channel */
	f=fopen(buffer,"rb");
	/* Error */
	if(!f)
	{
		wcrpg_error("Could not open %s",buffer);
		return;
	}
	/* Read the type */
	filetype = wcstr_read_file(f,&rlen);
	wclog_write(wcrpg_get_log(),"Type: ");
	wclog_write(wcrpg_get_log(),filetype);
	wclog_enter(wcrpg_get_log());
	/* Read properties */
	parallax = 0;
	for(;;)
	{
		code = wcint_read_file(f,&rlen);
		if(code == WCMAP_LOWER)
			break;
		gather_data(code,f);
	}
	/* Read lower map */
	len = wcint_read_file(f,&rlen);
	lower = (tile*)malloc(len);
	wclog_write(wcrpg_get_log(),"Lower Map: ");
	wclog_write(wcrpg_get_log(),len);
	wclog_write(wcrpg_get_log()," bytes");
	wclog_enter(wcrpg_get_log());
	fread(lower,1,len,f);
	/* Read upper map */
	code = wcint_read_file(f,&rlen);
	len = wcint_read_file(f,&rlen);
	upper = (tile*)malloc(len);
	wclog_write(wcrpg_get_log(),"Upper Map: ");
	wclog_write(wcrpg_get_log(),len);
	wclog_write(wcrpg_get_log()," bytes");
	wclog_enter(wcrpg_get_log());
	fread(upper,1,len,f);
	/* Read events */
	load_events(f);
	/* Release */
	free(filetype);
	/* Close the channel */
	fclose(f);
	/* Derive values */
	tset = wcdatabase_get_tileset(tset_id);
	wclog_write(wcrpg_get_log(),"Map is using ");
	tset->report(wcrpg_get_log());
	tex = tset->get_texture();
	rmag = tex->get_res_mag(480);
	wclog_write(wcrpg_get_log(),"Chipset is in x");
	wclog_write(wcrpg_get_log(),rmag);
	wclog_write(wcrpg_get_log()," resolution");
	wclog_enter(wcrpg_get_log());
}

/* Delete map */
map :: ~map()
{
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "delete map");
	int i;
	free(parallax);
	free(current_parallax);
	delete tex;
	for(i = 0;i < wcarray_size(event_list);i++)
	{
		/* Must not be the link event */
		if(wcarray_get(event_list,i) != link_event)
			delete (rpgevent*)wcarray_get(event_list,i);
	}
	wcarray_delete(event_list);
	if(ptex)
		delete ptex;
}

/* Set link event */
void map :: set_link_event(void *ev)
{
	link_event = ev;
}

/* Read field */
void map :: read_field(char *name,int *val,int len,FILE *f,int numerical)
{
	int rlen;
	/* RPG-Int data? */
	if(numerical)
	{
		*val = wcint_read_file(f,&rlen);
	}
	else
	{
		/* Read */
		*val = 0;
		fread(val,1,len,f);
	}
	/* Report */
	wclog_write(wcrpg_get_log(),name);
	wclog_write(wcrpg_get_log(),*val);
	wclog_enter(wcrpg_get_log());
}

/* Set property */
void map :: gather_data(int code,FILE *f)
{
	/* Get length of code */
	int unknown;
	int len = 0;
	int rlen;
	if(code != WCMAP_BACKGROUND)
		len = wcint_read_file(f,&rlen);
	if(len > 4)
		len = 4;
	/* Read codes */
	switch(code)
	{
	case WCMAP_TILESET:
		read_field("Chipset: ",&tset_id,len,f,1);
		return;
	case WCMAP_WIDTH:
		read_field("Width: ",&width,len,f,1);
		return;
	case WCMAP_HEIGHT:
		read_field("Height: ",&height,len,f,1);
		return;
	case WCMAP_SCROLL:
		read_field("Scroll Mode: ",&scroll,len,f,1);
		return;
	case WCMAP_HAS_BACKGROUND:
		read_field("Has Background?: ",&has_parallax,len,f,1);
		return;
	case WCMAP_BACKGROUND:
		parallax = wcstr_read_file(f,&rlen);
		current_parallax = wcrpg_string_clone(parallax);
		wclog_write(wcrpg_get_log(),"Background: ");
		wclog_write(wcrpg_get_log(),parallax);
		wclog_enter(wcrpg_get_log());
		ptex = new texture(wcmaterial_request(MATERIAL_PANORAMA,parallax),1);
		ptex->reduce();
		return;
	case WCMAP_HORZ:
		read_field("H-Pan?: ",&h_pan,len,f,1);
		return;
	case WCMAP_VERT:
		read_field("V-Pan?: ",&v_pan,len,f,1);
		return;
	case WCMAP_HORZ_AUTO:
		read_field("Auto H-Pan?: ",&h_auto,len,f,1);
		return;
	case WCMAP_VERT_AUTO:
		read_field("Auto V-Pan?: ",&v_auto,len,f,1);
		return;
	case WCMAP_HORZ_SPEED:
		read_field("H-Speed: ",&h_speed,len,f,1);
		return;
	case WCMAP_VERT_SPEED:
		read_field("V-Speed: ",&v_speed,len,f,1);
		return;
	}
	/* Unknown codes */
	fread(&unknown,1,len,f);
}

/* Changes map bg */
void map :: change_parallax(char *fname,int vs,int vsa,int vsp,int hs,int hsa,int hsp)
{
	/* Swap names */
	free(current_parallax);
	current_parallax = wcrpg_string_clone(fname);
	/* Swap textures */
	delete ptex;
	ptex = new texture(wcmaterial_request(MATERIAL_PANORAMA,fname),1);
	ptex->reduce();
	/* Set settings */
	h_pan = hs;
	h_auto = hsa;
	h_speed = hsp;
	v_pan = vs;
	v_auto = vsa;
	v_speed = vsp;
	parallax_anim_x = 0;
	parallax_anim_y = 0;
}

/* Drawing code for autotiles and such */
#include "wcmapdraw.h"

/* Draws the collision map */
void map :: draw_collision(int x,int y)
{
	int tx,ty;
	int dx,dy;
	int btx,bty;
	font *sfont;
	/* Get font */
	sfont = wcrpg_get_sysfont();
	/* Find out the base position to draw from */
	btx = (-x/16)-1;
	bty = (-y/16)-1;
	/* Draw the map */
	for(ty = bty;ty < bty+15+2;ty++)
	{
		for(tx = btx;tx < btx+20+2;tx++)
		{
			/* Copy */
			dx = tx;
			dy = ty;
			/* Wrap */
			wrap(&dx,&dy);
			/* Draw */
			if(dx >= 0 && dx < width && dy >= 0 && dy < height)
				sfont->draw_text(x+tx*16,y+ty*16,"%d",tset->get_collision_code(lower[dx+dy*width],upper[dx+dy*width]));
		}
	}
}

/* Draws the terrain map */
void map :: draw_terrain(int x,int y)
{
	int tx,ty;
	int dx,dy;
	int btx,bty;
	font *sfont;
	/* Get font */
	sfont = wcrpg_get_sysfont();
	/* Find out the base position to draw from */
	btx = (-x/16)-1;
	bty = (-y/16)-1;
	/* Draw the map */
	for(ty = bty;ty < bty+15+2;ty++)
	{
		for(tx = btx;tx < btx+20+2;tx++)
		{
			/* Copy */
			dx = tx;
			dy = ty;
			/* Wrap */
			wrap(&dx,&dy);
			/* Draw */
			if(dx >= 0 && dx < width && dy >= 0 && dy < height)
				sfont->draw_text(x+tx*16,y+ty*16,"%d",tset->get_terrain_code(lower[dx+dy*width]));
		}
	}
}

/* Will wrap X */
int map :: wrap_x()
{
	if(scroll == WCMAP_WRAP_X || scroll == WCMAP_WRAP_BOTH)
		return 1;
	return 0;
}

/* Will wrap Y */
int map :: wrap_y()
{
	if(scroll == WCMAP_WRAP_Y || scroll == WCMAP_WRAP_BOTH)
		return 1;
	return 0;
}

/* Draws the map */
void map :: draw(int x,int y)
{
	int tx,ty;
	int wx,wy;
	int dx,dy;
	int btx,bty;
	int i,rank;
	rpgevent *ev,*sev;
	arraylist screen_list;
	/* Make list */
	screen_list = wcarray_new();
	/* Draw parallax */
	draw_parallax(x,y);
	/* Find out the base position to draw from */
	btx = (-x/16)-1;
	bty = (-y/16)-1;
	/* Filter visible */
	for(i = 0;i < wcarray_size(event_list);i++)
	{
		sev = (rpgevent*)wcarray_get(event_list,i);
		if(!wrap_x() && !wrap_y())
		{
			if(sev->get_pos_x() >= btx && sev->get_pos_x() <= btx+(WCRPG_WIDTH/16)+1)
				if(sev->get_pos_y() >= bty && sev->get_pos_y() <= bty+(WCRPG_HEIGHT/16)+2)
					wcarray_add(screen_list,sev);
		}
		else
		{
			wcarray_add(screen_list,sev);
		}
	}
	/* Set mag */
	wcdraw_set_local_mag(1.0f/(float)(rmag));
	/* Draw the map */
	for(ty = bty;ty < bty+(WCRPG_HEIGHT/16)+2;ty++)
	{
		for(tx = btx;tx < btx+(WCRPG_WIDTH/16)+2;tx++)
		{
			/* Copy */
			dx = tx;
			dy = ty;
			/* Wrap */
			wrap(&dx,&dy);
			/* Draw */
			if(dx >= 0 && dx < width && dy >= 0 && dy < height)
			{
				draw_tile(lower[dx+dy*width],x+tx*16,y+ty*16,WCMAP_BELOW);
				draw_tile(upper[dx+dy*width],x+tx*16,y+ty*16,WCMAP_BELOW);
			}
		}
	}
	/* Reset local mag */
	wcdraw_reset_local_mag();
	/* Draw character lower */
	for(rank = bty;rank < bty+(WCRPG_HEIGHT/2)+3;rank++)
	{
		for(i = 0;i < wcarray_size(screen_list);i++)
		{
			ev = (rpgevent*)wcarray_get(screen_list,i);
			wx = ev->get_pos_x();
			wy = ev->get_pos_y();
			wrap(&wx,&wy);
			if(ev->get_layer() == EVENT_BELOW && rank == wy)
			{
				ev->draw(x,y);
				ev->draw(x,y,tex);
				wcarray_remove(screen_list,i);
				i--;
			}
		}
	}
	/* Draw character middle */
	for(rank = bty;rank < bty+(WCRPG_HEIGHT/2)+3;rank++)
	{
		/* Draw events */
		for(i = 0;i < wcarray_size(screen_list);i++)
		{
			ev = (rpgevent*)wcarray_get(screen_list,i);
			wx = ev->get_pos_x();
			wy = ev->get_pos_y();
			wrap(&wx,&wy);
			if(ev->get_layer() == EVENT_SOLID && rank == wy)
			{
				ev->draw(x,y);
				ev->draw(x,y,tex);
				wcarray_remove(screen_list,i);
				i--;
			}
		}
		/* Draw player */
		wx = wcplayer_get()->get_pos_x();
		wy = wcplayer_get()->get_pos_y();
		wrap(&wx,&wy);
		if(rank == wy)
			wcplayer_draw(x,y);
	}
	/* Find out the base position to draw from */
	btx = (-x/16)-1;
	bty = (-y/16)-1;
	/* Set mag */
	wcdraw_set_local_mag(1.0f/(float)(rmag));
	/* Draw the map */
	for(ty = bty;ty < bty+(WCRPG_HEIGHT/16)+2;ty++)
	{
		for(tx = btx;tx < btx+(WCRPG_WIDTH/16)+2;tx++)
		{
			/* Copy */
			dx = tx;
			dy = ty;
			/* Wrap */
			wrap(&dx,&dy);
			/* Draw */
			if(dx >= 0 && dx < width && dy >= 0 && dy < height)
			{
				draw_tile(lower[dx+dy*width],x+tx*16,y+ty*16,WCMAP_ABOVE);
				draw_tile(upper[dx+dy*width],x+tx*16,y+ty*16,WCMAP_ABOVE);
			}
		}
	}
	/* Reset local mag */
	wcdraw_reset_local_mag();
	/* Draw character upper */
	for(rank = bty;rank < bty+(WCRPG_HEIGHT/2)+3;rank++)
	{
		for(i = 0;i < wcarray_size(screen_list);i++)
		{
			ev = (rpgevent*)wcarray_get(screen_list,i);
			wx = ev->get_pos_x();
			wy = ev->get_pos_y();
			wrap(&wx,&wy);
			if(ev->get_layer() == EVENT_ABOVE && rank == wy)
			{
				ev->draw(x,y);
				ev->draw(x,y,tex);
				wcarray_remove(screen_list,i);
				i--;
			}
		}
	}
	/* Collapse list */
	wcarray_delete(screen_list);
}

/* Get sizes */
int map :: get_width()
{
	return width*16;
}
int map :: get_height()
{
	return height*16; 
}

/* Get sizes in tiles */
int map :: get_tile_width()
{
	return width;
}
int map :: get_tile_height()
{
	return height;
}

/* Get collision code */
int map :: get_collision_code(tile lo,tile hi)
{
	return tset->get_collision_code(lo,hi);
}

/* Gets the upper tile here */
tile map :: get_upper_tile(int x,int y,void *me)
{
	int i;
	rpgevent *et;
	/* Wrap */
	wrap(&x,&y);
	/* Draw */
	for(i = 0;i < wcarray_size(event_list);i++)
	{
		/* Get event */
		et = (rpgevent*)wcarray_get(event_list,i);
		if(et != me)
		{
			/* Return their code if any */
			if(et->is_tile() && et->get_page_id() != -1)
			{
				if(et->get_pos_x() == x && et->get_pos_y() == y)
				{
					if(et->get_tile() != 10000)
						return et->get_tile();
				}
			}
		}
	}
	/* Else, map tile */
	return upper[x+y*width];
}

/* Gets the upper tile here */
tile map :: get_upper_tile(int x,int y)
{
	/* Normal */
	return get_upper_tile(x,y,0);
}

/* Gets the lower tile here */
tile map :: get_lower_tile(int x,int y)
{
	/* Wrap */
	wrap(&x,&y);
	return lower[x+y*width];
}

/* Gets the terrain code here */
int map :: get_terrain_code(int x,int y)
{
	/* Handle errors silently */
	if(x < 0 || x >= width || y < 0 || y >= height)
		return 0;
	return tset->get_terrain_code(get_lower_tile(x,y));
}

/* Gets whether or not this tile is above */
int map :: tile_is_above(tile t)
{
	return tset->is_above(t);
}

/* Can land on */
int map :: can_land(int x,int y,void *me)
{
	int c,i;
	/* Check destination for pass */
	c = get_collision_code(get_lower_tile(x,y),get_upper_tile(x,y,me));
	/* All blocked off */
	if(!(c&TILESET_RIGHT) && !(c&TILESET_LEFT) && !(c&TILESET_UP) && !(c&TILESET_DOWN))
		return 0;
	/* Check destination for event hits */
	for(i = 0;i < wcarray_size(event_list);i++)
	{
		/* If not ME! */
		if(wcarray_get(event_list,i) != me)
		{
			if(((rpgevent*)wcarray_get(event_list,i))->hit(x,y))
				return 0; /* Hit! */
			if(wcplayer_get()->hit(x,y))
				return 0; /* HIT PLAYER! */
		}
	}
	/* Passed */
	return 1;
}

/* Can move */
int map :: can_move(int x,int y,int dir,void *me)
{
	int c,i;
	/* Check source for pass */
	c = get_collision_code(get_lower_tile(x,y),get_upper_tile(x,y,me));
	/* Blockages */
	if(dir == FACE_RIGHT && !(c&TILESET_RIGHT))
		return 0;
	if(dir == FACE_LEFT && !(c&TILESET_LEFT))
		return 0;
	if(dir == FACE_UP && !(c&TILESET_UP))
		return 0;
	if(dir == FACE_DOWN && !(c&TILESET_DOWN))
		return 0;
	/* Check destination for pass */
	if(dir == FACE_RIGHT)
		x++;
	if(dir == FACE_LEFT)
		x--;
	if(dir == FACE_UP)
		y--;
	if(dir == FACE_DOWN)
		y++;
	c = get_collision_code(get_lower_tile(x,y),get_upper_tile(x,y,me));
	/* Blockages */
	if(dir == FACE_RIGHT && !(c&TILESET_LEFT))
		return 0;
	if(dir == FACE_LEFT && !(c&TILESET_RIGHT))
		return 0;
	if(dir == FACE_UP && !(c&TILESET_DOWN))
		return 0;
	if(dir == FACE_DOWN && !(c&TILESET_UP))
		return 0;
	/* Check destination for event hits */
	for(i = 0;i < wcarray_size(event_list);i++)
	{
		if(((rpgevent*)wcarray_get(event_list,i))->hit(x,y))
			return 0; /* Hit! */
		if(wcplayer_get()->hit(x,y))
			return 0; /* HIT PLAYER! */
	}
	/* Cases passed */
	return 1;
}

/* Get event by id */
void *map :: get_event(int id)
{
	if(id < 0 || id >= wcarray_size(event_list))
	{
		wcrpg_error("Invalid event (%d)",id);
		return 0;
	}
	return wcarray_get(event_list,id);
}

/* Get event count */
int map :: get_event_count()
{
	//__android_log_print(ANDROID_LOG_INFO, "adMOB", "get_event_count");
	return wcarray_size(event_list);
}

/* Wraps the x and y coord according to this map */
void map :: wrap(int *x,int *y)
{
	if(wrap_x())
	{
		while(*x < 0)
			*x += width;
		while(*x >= width)
			*x -= width;
	}
	if(wrap_y())
	{
		while(*y < 0)
			*y += height;
		while(*y >= height)
			*y -= height;
	}
}

/* Wraps screen positions according to the map */
void map :: wrap_screen(int *x,int *y)
{
	int pwidth,pheight;
	/* Get pixel size */
	pwidth = width*16;
	pheight = height*16;
	/* Wrap */
	if(wrap_x())
	{
		while(*x < -16)
			*x += pwidth;
		while(*x >= pwidth)
			*x -= pwidth;
	}
	if(wrap_y())
	{
		while(*y < -32)
			*y += pheight;
		while(*y >= pheight)
			*y -= pheight;
	}
}

/* Gets this map ID */
int map :: get_id()
{
	return map_id;
}

/* Handle the map */
void map :: handle()
{
	float ts;
	/* Get speed */
	ts = wcrpg_get_time_step();
	/* Scroll parallaxes */
	if(h_auto && h_pan)
		parallax_anim_x += ts*(float)(1+h_speed*12)*0.1f;
	if(v_auto && v_pan)
		parallax_anim_y += ts*(float)(1+v_speed*12)*0.1f;
}

/* Draw the parallax */
void map :: draw_parallax(int x,int y)
{
	unsigned int pw,ph;
	unsigned int xx,yy;
	/* Cannot draw nothing */
	if(!ptex)
		return;
	/* Find size */
	pw = ptex->get_visible_width();
	ph = ptex->get_visible_height();
	/* Camera advance on wrapping.. don't know why this occurs */
	if(wrap_x())
		x -= get_width();
	/* Scrolling */
	if(v_pan)
		y /= 2;
	if(h_pan)
		x /= 2;
	/* Calculate animation offset */
	x += (int)parallax_anim_x;
	y += (int)parallax_anim_y;
	/* Positive */
	while(x < 0)x += pw;
	while(y < 0)y += ph;
	/* Convert */
	xx = x;
	xx = xx%pw;
	yy = y;
	yy = yy%ph;
	/* Draw it */
	ptex->draw(xx,yy);
	ptex->draw(xx-pw,yy);
	ptex->draw(xx,yy-ph);
	ptex->draw(xx-pw,yy-ph);
}

/* Get event id */
int map :: get_event_id(int x,int y)
{
	int i,j,k;
	rpgevent *ev;
	/* Wrap */
	wrap(&x,&y);
	/* Seek */
	for(i = 0;i < wcarray_size(event_list);i++)
	{
		ev = (rpgevent*)wcarray_get(event_list,i);
		j = ev->get_pos_x();
		k = ev->get_pos_y();
		wrap(&j,&k);
		if(x == j && y == k)
			return ev->get_id();
	}
	/* Unfound */
	return 0;
}

/* Get parallax */
char *map :: get_parallax_name()
{
	return current_parallax;
}

/* Get parallax settings */
void map :: get_parallax_scroll(int *scrollv)
{
	/* Load vectors */
	scrollv[0] = v_pan;
	scrollv[1] = v_auto;
	scrollv[2] = v_speed;
	scrollv[3] = h_pan;
	scrollv[4] = h_auto;
	scrollv[5] = h_speed;
}