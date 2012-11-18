/*
	Screenshot Maker
*/

/* Includes */
//#include <windows.h>
#include <stdio.h>
#include "wcscreenshot.h"
#include "wcsave.h"
#include "wcsaveio.h"
#include "wcdraw.h"
#include "wcrpg.h"
#include "wcinput.h"
#include "wctext.h"

/* Globals */
int wcscreenshot_current = 0;
int wcscreenshot_autofire = 0;
float wcscreenshot_fade = 0.0f;

/* Draw screenshot indicator */
void wcscreenshot_draw()
{
	font *fon;
	char buffer[64];
	/* Fade out */
	if(wcscreenshot_fade > 0.0f)
	{
		fon = wcrpg_get_sysfont();
		memset(buffer,0,sizeof(buffer));
		sprintf_s(buffer,sizeof(buffer),"Screenshot %d saved",wcscreenshot_current-1);
		fon->draw_text(8,WCRPG_HEIGHT-16,buffer);
	}
}

/* Load screen catalog */
void wcscreenshot_load_catalog()
{
	char buffer[260];
	FILE *shcat;
	/* Attempt to load screen catalog */
	memset(buffer,0,sizeof(buffer));
	sprintf_s(buffer,sizeof(buffer),"%sscreeninfo.sav",wcsave_get_game_base());
	fopen_s(&shcat,buffer,"rb");
	/* Load it */
	if(shcat)
	{
		/* Load data */
		wcsaveio_read(shcat);
		wcscreenshot_current = wcsaveio_read_num();
		wcsaveio_next();
		/* End */
		fclose(shcat);
	}
}

/* Save screen catalog */
void wcscreenshot_save_catalog()
{
	char buffer[260];
	FILE *shcat;
	/* Attempt to save screen catalog */
	memset(buffer,0,sizeof(buffer));
	sprintf_s(buffer,sizeof(buffer),"%sscreeninfo.sav",wcsave_get_game_base());
	fopen_s(&shcat,buffer,"wb");
	/* Save it */
	wcsaveio_start(shcat);
	wcsaveio_write_num(wcscreenshot_current);
	wcsaveio_end();
	/* End */
	fclose(shcat);
}

/* Initialize screenshot data */
void wcscreenshot_init()
{
	/* Must have save base */
	if(!*wcsave_get_game_base())
	{
		wcrpg_error("Save folder was not located first before initializing screenshots");
		return;
	}
	/* Load catalog */
	wcscreenshot_load_catalog();
}

/* Save a screenshot */
void wcscreenshot_snap()
{
	char buffer[260];
	image *img;
	/* Snap */
	img = wcdraw_image_screen();
	/* Save */
	memset(buffer,0,sizeof(buffer));
	sprintf_s(buffer,sizeof(buffer),"%sscreen%d.bmp",wcsave_get_game_base(),wcscreenshot_current);
	img->save(buffer);
	wclog_write(wcrpg_get_log(),"Screenshot taken: ");
	wclog_write(wcrpg_get_log(),buffer);
	wclog_enter(wcrpg_get_log());
	wcscreenshot_current++;
	/* Save catalog */
	wcscreenshot_save_catalog();
}

/* Handle screenshot power */
void wcscreenshot_handle()
{
	float ts;
	ts = wcrpg_get_time_step();
	/* Take shot */
	if(!wcscreenshot_autofire && wcinput_key(WCINPUT_DEBUG_SCREENSHOT))
	{
		wcscreenshot_snap();
		wcscreenshot_autofire = 1;
		wcscreenshot_fade = 1.0f;
	}
	wcscreenshot_fade -= ts*0.05f;
	if(wcscreenshot_fade < 0.0f)
		wcscreenshot_fade = 0.0f;
	/* Undo autofire lock */
	if(!wcinput_key(WCINPUT_DEBUG_SCREENSHOT))
		wcscreenshot_autofire = 0;
}