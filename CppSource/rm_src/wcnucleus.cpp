/*
	This is the core of the RPG engine
*/

/* Includes */
#include "wcrpg.h"
#include "wclevel.h"
#include "wcinput.h"
#include "wcnucleus.h"
#include "wcmap.h"
#include "wcsystem.h"
#include "wcdraw.h"
#include "wcmaterial.h"
#include "wcmaptree.h"
#include "wcwindow.h"
#include "wcdialog.h"
#include "wcmenu.h"
#include "wcmanage.h"
#include "wcguibuild.h"
#include "wcpicture.h"
#include "wccommon.h"
#include "wcvocab.h"
#include "wcsavemenu.h"
#include "wcitemmenu.h"
#include "wcquitmenu.h"
#include "wcstatusmenu.h"
#include "wcskillmenu.h"
#include "wcsave.h"
#include "wcanimation.h"
#include "wcfx.h"
#include "wcbattleset.h"
#include "wcnucleus.h"
#include "wcbattle.h"
#include "wctrans.h"
#include "wcmessage.h"
#include "wcpopnum.h"
#include "wctimer.h"
#include "wcscreenshot.h"
#include "wcinvoke.h"

#include <android\log.h>

/* Globals */
int wcnucleus_mode = WCRPG_TITLE_SCREEN;
//int wcnucleus_mode = WCRPG_MAP;
int wcnucleus_anti_auto_fire = 0;
int wcnucleus_step = 0;
int wcnucleus_current_song = 0;
texture *wcnucleus_title_tex = 0;
texture *wcnucleus_game_over_tex = 0;
menu *wcnucleus_main_menu;
float wcnucleus_from_r = 1.0f,wcnucleus_from_g = 1.0f,wcnucleus_from_b = 1.0f,wcnucleus_from_s = 1.0f;
float wcnucleus_to_r = 1.0f,wcnucleus_to_g = 1.0f,wcnucleus_to_b = 1.0f,wcnucleus_to_s = 1.0f;
float wcnucleus_tint_alpha = 0.0f;
float wcnucleus_tint_speed = 0.0f;
int wcnucleus_debug_autofire = 0;
int wcnucleus_speed_mode = 0;
int wcnucleus_info_mode = 1;

/* Get info mode */
int wcnucleus_get_debug_info_mode()
{
	return wcnucleus_info_mode;
}

/* Get speed mode */
int wcnucleus_get_debug_speed_mode()
{
	return wcnucleus_speed_mode;
}

/* Get tint red */
float wcnucleus_get_tint_red()
{
	return wcnucleus_to_r;
}

/* Get tint green */
float wcnucleus_get_tint_green()
{
	return wcnucleus_to_g;
}

/* Get tint blue */
float wcnucleus_get_tint_blue()
{
	return wcnucleus_to_b;
}

/* Get tint sat */
float wcnucleus_get_tint_sat()
{
	return wcnucleus_to_s;
}

/* Get mode */
int wcnucleus_get_mode()
{
	return wcnucleus_mode;
}

/* Set mode */
void wcnucleus_set_mode(int m)
{
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcnucleus_set_mode wcnucleus_step=0 %d %d",wcnucleus_step, m);
	wcnucleus_step = 0;
	wcnucleus_mode = m;
}

/* Handles the map preview */
void wcnucleus_map_view()
{
	float ts;
	int nid;
	treenode *tn;
	/* Not a mode to handle? */
	if(wcnucleus_mode != WCRPG_MODE_MAP_VIEW)
		return;
	/* Let the arrow keys move camera freely */
	ts = wcrpg_get_time_step();
	wclevel_move_camera(ts*4*(float)(wcinput_x()),ts*4*(float)(wcinput_y()));
	/* Next map */
	if(wcinput_cancel() && !wcnucleus_anti_auto_fire && wclevel_get_id() < wcmaptree_size()-1)
	{
		nid = wclevel_get_id();
		for(;;)
		{
			nid++;
			tn = wcmaptree_get(nid);
			if(tn->is_map())
				break;
		}
		wclevel_load(nid,0,0);
		wcnucleus_anti_auto_fire = 1;
	}
	if(wcinput_ok() && !wcnucleus_anti_auto_fire && wclevel_get_id() > 1)
	{
		nid = wclevel_get_id();
		for(;;)
		{
			nid--;
			tn = wcmaptree_get(nid);
			if(tn->is_map())
				break;
		}
		wclevel_load(nid,0,0);
		wcnucleus_anti_auto_fire = 1;
	}
	if(!wcinput_cancel() && !wcinput_ok())
		wcnucleus_anti_auto_fire = 0;
}

/* Handles the title screen */
void wcnucleus_title_screen()
{
	music *mus;
	int hs;
	if(wcnucleus_mode != WCRPG_TITLE_SCREEN)
		return;
	/* Start that music */
	if(wcnucleus_step == 0)
	{
		wcsystem_play_title_music();
		if(wcnucleus_title_tex)
			delete wcnucleus_title_tex;
		wcnucleus_title_tex = new texture(wcmaterial_request(MATERIAL_TITLE,wcsystem_get_title_graphic()),1);
		wcnucleus_title_tex->reduce();
		if(wcnucleus_game_over_tex)
			delete wcnucleus_game_over_tex;
		wcnucleus_game_over_tex = new texture(wcmaterial_request(MATERIAL_GAME_OVER,wcsystem_get_game_over_graphic()),1);
		wcnucleus_game_over_tex->reduce();
		__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcnucleus_step==0 %d wcnucleus_step=1",wcnucleus_step);
		wcnucleus_step = 1;
		wcnucleus_main_menu = wcmenu_generate_title();
	}
	/* Press start to play */
	if(wcnucleus_step == 1)
	{
		/* Get out of this title screen */
		if(wcinput_map_test())
		{
			wcnucleus_set_mode(WCRPG_MODE_MAP_VIEW);
			delete wcnucleus_title_tex;
			/* Load starting level */
			wclevel_load_start();
			wcnucleus_title_tex = 0;
		}
		/* Sound test */
		if(wcinput_sound_test())
		{
			wcsound_music_stop();
			__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcinput_sound_test %d wcnucleus_step=2",wcnucleus_step);
			wcnucleus_step = 2;
		}
		/* Open */
		if(wcinput_ok() && !wcnucleus_main_menu->is_active())
			wcnucleus_main_menu->activate();
		/* Handle menu */
		if(!wcsavemenu_active())
			wcnucleus_main_menu->handle();
		/* Calculate answer offset */
		hs = wcsave_has_game_saves();
		/* Desicion made? */
		if(wcnucleus_main_menu->get_selection()-hs == 2)
		{
			/* Exit */
			wcnucleus_main_menu->close();
			if(!wcnucleus_main_menu->is_active())
				wcrpg_exit();
		}
		/* Start the gaem! */
		if(!wcsavemenu_active())
		{
			if(wcnucleus_main_menu->get_selection()-hs == 0 || (wcrpg_neo_mode() && !*wcvocab_get(VOCAB_NEW_GAME)) || wcsave_game_loaded())
			{
				/* Start */
				delete wcnucleus_main_menu;
				wcnucleus_main_menu = 0;
				if(!wcsave_game_loaded())
					wclevel_load_start();
				wcnucleus_set_mode(WCRPG_MAP);
			}
			else
			{
				/* Load some gaem */
				if(wcnucleus_main_menu->get_selection()-hs == 1)
					wcsavemenu_open(wcnucleus_main_menu,1);
				if(hs)
				{
					if(wcnucleus_main_menu->get_selection()-hs == -1)
						wcsave_load(wcsave_get_last());
				}
			}
		}
	}
	/* Music room */
	if(wcnucleus_step == 2)
	{
		/* Next song */
		if(wcinput_x() > 0 && !wcnucleus_anti_auto_fire && wcnucleus_current_song < wcmaterial_count(MATERIAL_MUSIC)-1)
		{
			wcnucleus_current_song++;
			wcnucleus_anti_auto_fire = 1;
		}
		if(wcinput_x() < 0 && !wcnucleus_anti_auto_fire && wcnucleus_current_song > 0)
		{
			wcnucleus_current_song--;
			wcnucleus_anti_auto_fire = 1;
		}
		if(wcinput_ok())
		{
			mus = new music(wcmaterial_request(MATERIAL_MUSIC,wcnucleus_current_song));
			wcsound_music_play(mus);
			delete mus;
			wcnucleus_anti_auto_fire = 1;
		}
		if(wcinput_cancel())
		{
			wcsound_music_stop();
			wcnucleus_anti_auto_fire = 1;
		}
		if(wcinput_x() == 0 && !wcinput_ok() && !wcinput_cancel())
			wcnucleus_anti_auto_fire = 0;
	}
}

/* Handles debugger functions */
void wcnucleus_handle_debug()
{
	/* Reset autofire */
	if(!wcinput_key(WCINPUT_DEBUG_FOLDER) && !wcinput_key(WCINPUT_DEBUG_INFO))
		wcnucleus_debug_autofire = 0;
	/* Can't do these if not test play */
	if(!wcrpg_test_play())
		return;
	/* Allow instant save for test players */
	if(wcinput_key(WCINPUT_DEBUG_SAVE) && !wcsavemenu_active())
		wcsavemenu_open(0,0);
	/* Allow open game save folder */
	if(wcinput_key(WCINPUT_DEBUG_FOLDER) && !wcnucleus_debug_autofire)
	{
		wcnucleus_debug_autofire = 1;
		wcinvoke_game_save_folder();
	}
	/* Toggle info */
	if(wcinput_key(WCINPUT_DEBUG_INFO) && !wcnucleus_debug_autofire)
	{
		wcnucleus_debug_autofire = 1;
		if(wcnucleus_info_mode == 1)
			wcnucleus_info_mode = 0;
		else
			wcnucleus_info_mode = 1;
	}
	/* Speed */
	if(wcinput_key(WCINPUT_DEBUG_PLAY))
	{
		wcrpg_set_game_speed(1.0f);
		wcnucleus_speed_mode = 0;
	}
	if(wcinput_key(WCINPUT_DEBUG_SLOW))
	{
		wcrpg_set_game_speed(0.5f);
		wcnucleus_speed_mode = -1;
	}
	if(wcinput_key(WCINPUT_DEBUG_FAST))
	{
		wcrpg_set_game_speed(4.0f);
		wcnucleus_speed_mode = 1;
	}
}

/* Handles game over */
void wcnucleus_game_over()
{
	/* Handle */
	if(wcnucleus_get_mode() == WCRPG_GAME_OVER)
	{
		/* OK */
		if(wcinput_ok() && wcnucleus_step == 0 && !wctrans_active())
		{
			wctrans_erase(WCTRANS_FADE);
			__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcnucleus_game_over %d wcnucleus_step++",wcnucleus_step);
			wcnucleus_step++;
		}
		/* Wait for erased */
		if(!wctrans_active() && wctrans_get_screen_state() == WCTRANS_SCREEN_HIDDEN && wcnucleus_step == 1)
		{
			wctrans_show(WCTRANS_FADE);
			wcsave_game_reset_loaded();
			wcnucleus_set_mode(WCRPG_TITLE_SCREEN);
		}
	}
}

/* Handles the entire game for one frame */
void wcnucleus_handle()
{
	/* Handle all active modes */
	wcnucleus_map_view();
	wcnucleus_title_screen();
	wcnucleus_game_over();
	wcmap_animate();
	wcevent_animate();
	wcwindow_animate();
	/* Active mode is map exploration and transitions are not occuring */
	if(wcnucleus_get_mode() == WCRPG_MAP && !wctrans_active())
	{
		if(!wcmanage_active())
		{
			wclevel_handle();
			wccommon_handle();
		}
		wctimer_handle();
		wcpicture_handle();
		wcdialog_handle();
		wcitemmenu_handle();
		wcskillmenu_handle();
		wcquitmenu_handle();
		wcstatusmenu_handle();
		wcbattleset_handle();
		wcmanage_handle();
		wcmessage_handle();
		wcbattle_system_main();
	}
	wcsavemenu_handle(); /* Handled regardless of state of game */
	wcanimation_handle(); /* This too is handled regardless of state */
	wcpopnum_handle(); /* Pop numbers always work */
	wcfx_handle(); /* Handled no matter what */
	wctrans_handle(); /* Handled always */
	//wcscreenshot_handle(); /* Handle screenshots */
	wcnucleus_handle_debug(); /* Handle debugs */
}

/* Handle tints */
void wcnucleus_handle_tint()
{
	float dr,dg,db,ds;
	float ts;
	/* Get times */
	ts = wcrpg_get_time_step();
	/* Find distance */
	dr = wcnucleus_to_r-wcnucleus_from_r;
	dg = wcnucleus_to_g-wcnucleus_from_g;
	db = wcnucleus_to_b-wcnucleus_from_b;
	ds = wcnucleus_to_s-wcnucleus_from_s;
	/* Multiply by alpha */
	dr *= wcnucleus_tint_alpha;
	dg *= wcnucleus_tint_alpha;
	db *= wcnucleus_tint_alpha;
	ds *= wcnucleus_tint_alpha;
	/* Tint accordingly */
	wcdraw_tint_screen(wcnucleus_from_r+dr,wcnucleus_from_g+dg,wcnucleus_from_b+db,wcnucleus_from_s+ds);
	/* Slide alpha */
	wcnucleus_tint_alpha += ts*wcnucleus_tint_speed;
	if(wcnucleus_tint_alpha > 1.0f)
		wcnucleus_tint_alpha = 1.0f;
}

/* Tint */
void wcnucleus_tint(float r,float g,float b,float s,float t)
{
	float dr,dg,db,ds;
	/* Instant! */
	if(t == 0.0f)
	{
		wcnucleus_from_r = r;
		wcnucleus_from_g = g;
		wcnucleus_from_b = b;
		wcnucleus_from_s = s;
		wcnucleus_to_r = r;
		wcnucleus_to_g = g;
		wcnucleus_to_b = b;
		wcnucleus_to_s = s;
		wcnucleus_tint_alpha = 0.0f;
		wcnucleus_tint_speed = 0.0f;
		return;
	}
	/* Find distance */
	dr = wcnucleus_to_r-wcnucleus_from_r;
	dg = wcnucleus_to_g-wcnucleus_from_g;
	db = wcnucleus_to_b-wcnucleus_from_b;
	ds = wcnucleus_to_b-wcnucleus_from_s;
	/* Multiply by alpha */
	dr *= wcnucleus_tint_alpha;
	dg *= wcnucleus_tint_alpha;
	db *= wcnucleus_tint_alpha;
	ds *= wcnucleus_tint_alpha;
	/* Finalize moving */
	wcnucleus_from_r += dr;
	wcnucleus_from_g += dg;
	wcnucleus_from_b += db;
	wcnucleus_from_s += ds;
	/* Set new target */
	wcnucleus_to_r = r;
	wcnucleus_to_g = g;
	wcnucleus_to_b = b;
	wcnucleus_to_s = s;
	wcnucleus_tint_alpha = 0.0f;
	wcnucleus_tint_speed = 0.6f/t;
}

/* Draws the entire game */
void wcnucleus_draw()
{ 
	font *fon;
	/* Title screen */
	if(wcnucleus_get_mode() == WCRPG_TITLE_SCREEN)
	{
		/* Draw title screen */
		wcnucleus_title_tex->draw(0,0);
		/* Draw menu */
		if(wcnucleus_main_menu)
			wcnucleus_main_menu->draw(0,0);
		/* Draw music info */
		if(wcnucleus_step == 2)
		{
			fon = wcrpg_get_sysfont();
			fon->draw_text(0,16,"Music Test (%d songs)",wcmaterial_count(MATERIAL_MUSIC));
			fon->draw_text(0,24,"Song #%d",(wcnucleus_current_song+1));
			fon->draw_text(80,24,wcmaterial_name(MATERIAL_MUSIC,wcnucleus_current_song));
		}
	}
	/* Game.. Over */
	if(wcnucleus_get_mode() == WCRPG_GAME_OVER)
	{
		/* Draw game over screen */ 
		wcnucleus_game_over_tex->draw(0,0);
	}
	/* Draws the current level */
	if(wcnucleus_get_mode() == WCRPG_MODE_MAP_VIEW || wcnucleus_get_mode() == WCRPG_MAP)
		wclevel_draw();
	/* Battle layer */
	//wcbattle_system_draw(0);
	/* Draw screen shader */
	//wcdraw_screen_shader();
	/* Here is where the tint level is */
	//wcnucleus_handle_tint();
	/* Picture layer */
	//wcpicture_draw();
	/* Pop up numbers */
	//wcpopnum_draw();
	/* Animation layer */
	//wcanimation_draw();
	/* Effect layer */
	//wcfx_draw();
	/* Trans later */
	//wctrans_draw();
	/* Draws the menu layer */
	if(wcnucleus_get_mode() == WCRPG_MAP)
	{
		//wctimer_draw(); /* Draw all timers */
		//wcbattle_system_draw(1); /* Menus for battle system */
		//wcguibuild_draw(); /* User defined gui elements */
		//wcmanage_draw();
		//wcmessage_draw();
		//wcquitmenu_draw();
		//wcstatusmenu_draw();
		//wcitemmenu_draw();
		//wcskillmenu_draw();
	}
	//wcsavemenu_draw(); /* Drawn regardless of playing game or not */
	//wcdialog_draw();
	/* Draw debugger stuff */
	//wcscreenshot_draw();
	//wcrpg_draw_debug();
}