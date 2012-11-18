/*
	Special Effects (Weather, Flashing Screen, etc.)
*/

/* Includes */
#include "wcimage.h"
#include "wcdraw.h"
#include "wcfx.h"
#include "wcrpg.h"

/* Globals */
texture *wcfx_screen_flasher = 0;
float screen_fa,screen_fp,screen_fs;
int screen_fr,screen_fg,screen_fb;
int screen_frep;

/* Flash screen */
void wcfx_flash(int r,int g,int b,float p,int t,int rep)
{
	/* No time or no power is no action */
	if(p == 0.0f || t == 0)
	{
		screen_fp = 0.0f;
		screen_fa = 0.0f;
		screen_frep = 0;
		return;
	}
	/* Set */
	screen_fa = 1.0f;
	screen_fp = p;
	screen_fs = (1.0f/(0.1f*(float)t))/14.0f;
	screen_fr = r;
	screen_fg = g;
	screen_fb = b;
	screen_frep = rep;
}

/* Init */
void wcfx_init()
{
	color whitecolor;
	image *img;
	/* Set white */
	whitecolor.r = 255;
	whitecolor.g = 255;
	whitecolor.b = 255;
	whitecolor.a = 255;
	/* Make plane */
	img = new image(32,32,whitecolor);
	wcfx_screen_flasher = new texture(img);
	/* Configure */
	wcfx_screen_flasher->enable_flash();
	wcfx_screen_flasher->set_transparency(1.0f,1.0f);
	/* Set */
	screen_fa = 0.0f;
}

/* Handle */
void wcfx_handle()
{
	float ts;
	/* Get time */
	ts = wcrpg_get_time_step();
	/* Handle flashing */
	if(screen_fa > 0.0f)
	{
		/* Reduce */
		screen_fa -= screen_fs*ts;
		/* Gone */
		if(screen_fa <= 0.0f)
		{
			if(screen_frep)
				screen_fa = 1.0f; /* Repeat */
			else
				screen_fa = 0.0f; /* Stop */
		}
	}
}

/* Draw */
void wcfx_draw()
{
	float sx,sy;
	/* Handle screen flashes */
	if(screen_fa > 0.0f)
	{
		/* Figure scales */
		sx = (float)WCRPG_WIDTH;
		sy = (float)WCRPG_HEIGHT;
		sx /= 32.0f;
		sy /= 32.0f;
		/* Draw */
		wcfx_screen_flasher->set_flash(screen_fa*screen_fp,screen_fr,screen_fg,screen_fb);
		wcfx_screen_flasher->draw(0,0,32,32,0,0,(int)(32*sx),(int)(32*sy));
	}
}

/* Exit */
void wcfx_exit()
{
	delete wcfx_screen_flasher;
}