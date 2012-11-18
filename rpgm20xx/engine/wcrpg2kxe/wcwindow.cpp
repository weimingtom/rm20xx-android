/*
	System Graphic Drawing Tool
*/

/* Includes */
#include <malloc.h>
#include <memory.h>
#include <stdlib.h>
//#include "resource.h"
#include "wchero.h"
#include "wcwindow.h"
#include "wcsystem.h"
#include "wcrpg.h"
#include "wctext.h"
#include "wcmem.h"
#include "wcparty.h"
#include "wcmaterial.h"

#include <android\log.h>

/* Globals */
float wcwindow_anim_dist = 0;
int wcwindow_anim_frame = 0;
texture *wcwindow_ex_font = 0;
font *wcwindow_shadow_font = 0;
float text_speed = 1.0f; /* Text speed factor */
arraylist faceset_pool = 0;
font *wcwindow_color_font[FONT_COLOR_COUNT] = {0,0,0,0,0, 0,0,0,0,0,
                                               0,0,0,0,0, 0,0,0,0,0};

/* Text system */
#include "wcwindowtext.h"

/* Free faceset */
void wcwindow_release_faceset(void *v)
{
	faceset *fs;
	fs = (faceset*)v;
	free(fs->name);
	delete fs->tex;
	free(v);
}

/* Pools a faceset */
texture *wcwindow_pool_faceset(char *str)
{
	int i;
	faceset *fs;
	/* Reboot faces if empty */
	if(!faceset_pool)
		wcwindow_clear_faceset();
	/* Use blanks if blank */
	if(!str)
		return 0;
	/* Is it already here? */
	for(i = 0;i < wcarray_size(faceset_pool);i++)
	{
		fs = (faceset*)wcarray_get(faceset_pool,i);
		if(!strcmp(fs->name,str))
		{
			wclog_write(wcrpg_get_log(),"Obtained already allocated faceset ");
			wclog_write(wcrpg_get_log(),str);
			wclog_enter(wcrpg_get_log());
			return fs->tex;
		}
	}
	/* Not loaded, so add */
	fs = (faceset*)malloc(sizeof(faceset));
	fs->name = wcrpg_string_clone(str);
	fs->tex = new texture(wcmaterial_request(MATERIAL_FACESET,str));
	fs->tex->reduce();
	wclog_write(wcrpg_get_log(),"Loaded ");
	wclog_write(wcrpg_get_log(),str);
	wclog_write(wcrpg_get_log()," and obtained it (");
	wclog_write(wcrpg_get_log(),wcarray_size(faceset_pool));
	wclog_write(wcrpg_get_log()," total)");
	wclog_enter(wcrpg_get_log());
	/* Add and return */
	wcarray_add(faceset_pool,fs);
	return fs->tex;
}

/* Initialize */
void wcwindow_clear_faceset()
{
	/* Get rid of old */
	if(faceset_pool)
		wcarray_delete(faceset_pool);
	/* Recreate */
	faceset_pool = wcarray_new();
	wcarray_set_clean(faceset_pool,wcwindow_release_faceset);
}

/* Generates a font based on a part of the system graphic */
font *wcwindow_generate_font(int sx,int sy)
{
	//__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcwindow_generate_font [1]");	
	texture *tex;
	image *newfon,*srcfon,*srcsys;
	/* Get materials */
	//__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcwindow_generate_font [2]");	
	srcfon = wcrpg_get_font()->get_image();
	//__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcwindow_generate_font [3]");	
	newfon = new image(srcfon->get_width(),srcfon->get_height());
	//__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcwindow_generate_font [4]");	
	srcsys = wcsystem_get_graphic()->get_image();
	//__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcwindow_generate_font [5]");	
	/* Pattern image */
	//__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcwindow_generate_font [6]");	
	//newfon->blit_tile(srcsys,sx,sy,16,16);
	//__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcwindow_generate_font [7]");	
	/* Multiply image */
	//newfon->multiply(srcfon);
	//__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcwindow_generate_font [8]");	
	/* Create texture */
	//__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcwindow_generate_font [9]");	
	//newfon->extend(newfon->get_width()*2,newfon->get_height());
	//__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcwindow_generate_font [10]");	
	tex = new texture(newfon);
	//__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcwindow_generate_font [11]");	
	//tex->reduce();
	//__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcwindow_generate_font [12]");	
	return new font(tex);
}

/* Generates all fonts */
void wcwindow_generate_all_fonts()
{
	int i;
	/* Erase old */
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcwindow_generate_all_fonts [1]");	
	if(wcwindow_shadow_font)
		delete wcwindow_shadow_font;
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcwindow_generate_all_fonts [2]");	
	for(i = 0;i < FONT_COLOR_COUNT;i++)
	{
		if(wcwindow_color_font[i])
			delete wcwindow_color_font[i];
	}
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcwindow_generate_all_fonts [3]");	
	if(wcwindow_ex_font)
		delete wcwindow_ex_font;
	/* Make new */
	wcwindow_shadow_font = wcwindow_generate_font(16,32);
	wcwindow_shadow_font->set_compression(2);
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcwindow_generate_all_fonts [4]");	
	for(i = 0;i < FONT_COLOR_COUNT;i++)
	{
		__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcwindow_generate_all_fonts [5]");	
		wcwindow_color_font[i] = wcwindow_generate_font(16*(i%10),48+(i/10)*16);
		__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcwindow_generate_all_fonts [6]");	
		wcwindow_color_font[i]->set_compression(2);
	}
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcwindow_generate_all_fonts [7]");	
	/*wcwindow_ex_font = new texture("exfont.bmp");*/
	//wcwindow_ex_font = new texture(bmp_exfont,256,64);
}

/* Advances the animations */
void wcwindow_animate()
{
	float ts;
	ts = wcrpg_get_time_step();
	/* Advance */
	wcwindow_anim_dist += ts;
	/* Next frame and return */
	if(wcwindow_anim_dist > 1.5f)
	{
		wcwindow_anim_dist = 0;
		wcwindow_anim_frame++;
	}
}

/* Draws a window */
void wcwindow_draw(int x,int y,int w,int h)
{
	int xx,yy;
	int limx,limy;
	texture *tex;
	/* Get system graphic */
	tex = wcsystem_get_graphic();
	/* Nothing? */
	if(!tex)
		return;
	/* Back the window */
	if(!wcsystem_get_graphic_tile())
	{
		/* Simple stretch */
		tex->draw(x,y,32,32,0,0,w,h);
	}
	else
	{
		/* Complicated tile */
		for(yy = 0;yy < h;yy += 32)
		{
			for(xx = 0;xx < w;xx += 32)
			{
				limx = 32;
				limy = 32;
				if(xx+32 > w)
					limx = w-xx;
				if(yy+32 > h)
					limy = h-yy;
				tex->draw(x+xx,y+yy,limx,limy,0,0);
			}
		}
	}
	/* Corner the window */
	tex->draw(x,y,8,8,32,0);
	tex->draw(x+w-8,y,8,8,32+24,0);
	tex->draw(x+w-8,y+h-8,8,8,32+24,24);
	tex->draw(x,y+h-8,8,8,32,24);
	/* Top/Bottom the window */
	for(xx = 8;xx < w-8;xx += 16)
	{
		if((w-8)-xx < 16)
		{
			tex->draw(x+xx,y,(w-8)-xx,8,32+8,0);
			tex->draw(x+xx,y+h-8,(w-8)-xx,8,32+8,24);
		}
		else
		{
			tex->draw(x+xx,y,16,8,32+8,0);
			tex->draw(x+xx,y+h-8,16,8,32+8,24);
		}
	}
	/* Left/Right the window */
	for(yy = 8;yy < h-8;yy += 16)
	{
		if((h-8)-yy < 16)
		{
			tex->draw(x,y+yy,8,(h-8)-yy,32,8);
			tex->draw(x+w-8,y+yy,8,(h-8)-yy,32+24,8);
		}
		else
		{
			tex->draw(x,y+yy,8,16,32,8);
			tex->draw(x+w-8,y+yy,8,16,32+24,8);
		}
	}
}

/* Draws a cursor */
void wcwindow_draw_cursor(int x,int y,int w,int h,int f)
{
	int xx,yy;
	texture *tex;
	/* Animation gears */
	if(f)
	{
		/* Let the animation gears touch the window */
		f = (wcwindow_anim_frame/2)%2;
	}
	/* Get system graphic */
	tex = wcsystem_get_graphic();
	/* Nothing */
	if(!tex)
		return;
	/* Back the cursor */
	if(!wcsystem_get_graphic_tile())
	{
		tex->draw(x+8,y+8,16,16,64+32*f+8,8,w-16,h-16);
	}
	/* Corner the cursor */
	tex->draw(x,y,8,8,64+32*f,0);
	tex->draw(x+w-8,y,8,8,64+24+32*f,0);
	tex->draw(x+w-8,y+h-8,8,8,64+24+32*f,24);
	tex->draw(x,y+h-8,8,8,64+32*f,24);
	/* Top/Bottom the window */
	for(xx = 8;xx < w-8;xx += 16)
	{
		if((w-8)-xx < 16)
		{
			tex->draw(x+xx,y,(w-8)-xx,8,64+8+32*f,0);
			tex->draw(x+xx,y+h-8,(w-8)-xx,8,64+8+32*f,24);
		}
		else
		{
			tex->draw(x+xx,y,16,8,64+8+32*f,0);
			tex->draw(x+xx,y+h-8,16,8,64+8+32*f,24);
		}
	}
	/* Left/Right the window */
	for(yy = 8;yy < h-8;yy += 16)
	{
		if((h-8)-yy < 16)
		{
			tex->draw(x,y+yy,8,(h-8)-yy,64+32*f,8);
			tex->draw(x+w-8,y+yy,8,(h-8)-yy,64+24+32*f,8);
		}
		else
		{
			tex->draw(x,y+yy,8,16,64+32*f,8);
			tex->draw(x+w-8,y+yy,8,16,64+24+32*f,8);
		}
	}
}

/* Draws one of the icons */
void wcwindow_draw_compare(int x,int y,int c)
{
	texture *tex;
	/* Get graphic */
	tex = wcsystem_get_graphic();
	if(!tex)
		return;
	/* Draw it */
	tex->draw(x,y,8,8,128+8*(wcwindow_anim_frame%4),c*8);
}

/* Draws a number */
void wcwindow_draw_number(int x,int y,int n)
{
	int i;
	char buffer[32];
	int c;
	texture *tex;
	/* Get graphic */
	tex = wcsystem_get_graphic();
	if(!tex)
		return;
	/* Format text */
	sprintf(buffer,"%d",n);
	/* Draw text */
	for(i = 0;i < sizeof(buffer);i++)
	{
		/* Stop? */
		if(!buffer[i])
			return;
		/* Get absolute code */
		c = buffer[i]-'0';
		/* Draw it */
		tex->draw(x+i*8,y,8,16,32+c*8,32);
	}
}

/* Draws a colon */
void wcwindow_draw_colon(int x,int y)
{
	texture *tex;
	/* Get graphic */
	tex = wcsystem_get_graphic();
	if(!tex)
		return;
	/* Draw it */
	tex->draw(x,y,8,16,32+80,32);
}

/* Draws an airship shadow */
void wcwindow_draw_shadow(int x,int y,int s)
{
	texture *tex;
	/* Get graphic */
	tex = wcsystem_get_graphic();
	if(!tex)
		return;
	/* Draw it */
	tex->draw(x,y,16,16,128+s*16,32);
}

/* Here's the kicker, draw the text */
void wcwindow_draw_text(int x,int y,int c,char *str)
{
	font *fon;
	/* Illegal color */
	if(c < 0 || c >= FONT_COLOR_COUNT)
	{
		wcrpg_error("Invalid text color (%d)",c);
		return;
	}
	/* Get shadow font */
	fon = wcwindow_shadow_font;
	/* Draw it */
	fon->draw_text(x+1,y+1,str);
	/* Get font */
	fon = wcwindow_color_font[c];
	/* Draw it */
	fon->draw_text(x,y,str);
}

/* Draw hands */
void wcwindow_draw_hand(int x,int y,int d)
{
	texture *tex;
	/* Get graphic */
	tex = wcsystem_get_gauge();
	if(!tex)
		return;
	/* Draw hands */
	tex->draw(x,y,16,16,((wcwindow_anim_frame/2)%3)*16,d*16);
}

/* Draws extended bars */
void wcwindow_draw_bar(int x,int y,int p,int t,int w)
{
	texture *tex;
	/* Get graphic */
	tex = wcsystem_get_gauge();
	if(!tex)
		return;
	/* Draw caps */
	tex->draw(x,y,16,16,0,32+t*16);
	tex->draw(x+16+w,y,16,16,32,32+t*16);
	/* Draw middle */
	tex->draw(x+16,y,16,16,16,32+t*16,w,16);
	/* No bar? */
	if(p <= 0)
		return;
	/* Draw pixels */
	if(p < w)
		tex->draw(x+16,y,16,16,48,32+t*16,p,16);
	else
		tex->draw(x+16,y,16,16,48+16,32+t*16,p,16);
}

/* Draws bars */
void wcwindow_draw_bar(int x,int y,int p,int t)
{
	/* Value as % */
	if(p >= 100)
		p = 16;
	else
		p = (int)(16*((float)(p)/100.0f));
	/* Draw it */
	wcwindow_draw_bar(x,y,p,t,16);
}

/* Draws battle numbers */
void wcwindow_draw_battle_number(int x,int y,int n)
{
	char buffer[32];
	texture *tex;
	int i;
	int c;
	/* Get graphic */
	tex = wcsystem_get_gauge();
	if(!tex)
		return;
	/* Format */
	sprintf(buffer,"%d",n);
	/* Draw */
	for(i = 0;i < 32;i++)
	{
		/* End of the line */
		if(!buffer[i])
			return;
		c = buffer[i]-'0';
		tex->draw(x+i*8,y,8,16,c*8,32+48);
	}
}

/* Draws up down arrows */
void wcwindow_draw_arrow(int x,int y,int d)
{
	texture *tex;
	/* Get graphic */
	tex = wcsystem_get_graphic();
	if(!tex)
		return;
	/* Draw */
	tex->draw(x,y,16,8,32+8,8+8*d);
}

/* Draws exfont icon */
void wcwindow_draw_icon(int x,int y,int i)
{
	int sx,sy;
	/* Find source */
	sx = i%13;
	sy = i/13;
	sx *= 16;
	sy *= 16;
	/* OK */
	wcwindow_ex_font->draw(x,y,16,16,sx,sy);
}

/* Returns the width of a character of the current system font */
int wcwindow_font_character_width()
{
	return wcwindow_color_font[0]->get_character_width();
}

/* Returns the height of a character of the current system font */
int wcwindow_font_character_height()
{
	if(!wcwindow_color_font[0])
		return 16;
	return wcwindow_color_font[0]->get_character_height();
}

/* Sets text speed */
void wcwindow_set_text_speed(int spd)
{
	float spdf;
	/* Figure out the speed */
	spdf = (float)spd;
	spdf = (21.0f-spd)/10.0f;
	/* Set it */
	text_speed = spdf;
}

/* Calculates text speed */
float wcwindow_calculate_text_speed(int spd)
{
	float spdf;
	int i;
	/* Figure out the speed */
	spdf = 2.0f;
	for(i = 0;i < spd;i++)
		spdf /= 1.5f;
	/* Return it */
	return spdf;
}

/* Gets text speed */
float wcwindow_get_text_speed()
{
	return text_speed;
}

/* Draws a face */
void wcwindow_draw_face(texture *tex,int ix,int x,int y)
{
	int sx,sy;
	/* No face */
	if(!tex)
		return;
	/* Find source */
	sx = ix%4;
	sy = ix/4;
	sx *= 48;
	sy *= 48;
	/* Draw */
	tex->draw(x,y,48,48,sx,sy);
}