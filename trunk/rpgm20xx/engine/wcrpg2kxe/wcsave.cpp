/*
	Save Manager
*/

/* Includes */
//#include <windows.h>
#include <memory.h>
#include <stdio.h>
#include "wcsave.h"
#include "wcrpg.h"
#include "wcsaveio.h"
#include "wcsavemenu.h"
#include "wclevel.h"
#include "wcplayer.h"
#include "wcevent.h"
#include "wcmisc.h"
#include "wcmem.h"
#include "wcitem.h"
#include "wchero.h"
#include "wcmanage.h"
#include "wcnucleus.h"
#include "wcmonster.h"
#include "wcsound.h"
#include "wccommon.h"
#include "wcmove.h"
#include "wcsystem.h"

#include <string.h>

/* Globals */
savetag wcsave_tags[WCSAVE_SLOTS];
char wcsave_home[260];
char wcsave_base[260];
char wcsave_game_base[260];
char wcsave_file[260];
int wcsave_loaded_game = 0; /* Has a game been loaded? */
int wcsave_last_file = 0; /* Last file access */
int wcsave_allowed = 1; /* Is saving allowed? */
int wcsave_dest_map = 0; /* Place to put player */
int wcsave_dest_map_x = 0;
int wcsave_dest_map_y = 0;
int wcsave_dest_map_dir = 0;
music *wcsave_music = 0; /* Recalled music from records */
int wcsave_active_move_count = 0; /* Recalled move count */
int wcsave_parallax_scroll[6] = {0,0,0,0,0,0}; /* Scrolling vectors */
char *wcsave_parallax_name = 0; /* Scrolling bg */

/* Enable/Disable save from menu */
void wcsave_allow(int en)
{
	wcsave_allowed = en;
}

/* Is saving allowed from the menu? */
int wcsave_is_allowed()
{
	return wcsave_allowed;
}

/* Is there files that we can load? */
int wcsave_has_game_saves()
{
	int i;
	/* Seek */
	for(i = 0;i < WCSAVE_SLOTS;i++)
	{
		if(wcsave_tags[i].used)
			return 1; /* Yes */
	}
	/* I couldn't find any */
	return 0;
}

/* Does this dir exist? */
int wcsave_dir_exist(char *dir)
{
	///* This isn't a dir */
	//if(GetFileAttributesA(dir) == INVALID_FILE_ATTRIBUTES)
	//	return 0;
	///* This file exists */
	//return 1;
}

/* Try making a dir */
int wcsave_dir_create(char *dir)
{
	/* Return result of creation */
	//return CreateDirectoryA(dir,NULL);
}

/* Initializer */
void wcsave_init()
{
	//int i;
	//char buffer[260];
	///* Wipe */
	//memset(wcsave_tags,0,sizeof(wcsave_tags));
	//memset(wcsave_base,0,sizeof(wcsave_base));
	//memset(wcsave_home,0,sizeof(wcsave_home));
	//memset(wcsave_game_base,0,sizeof(wcsave_game_base));
	//memset(wcsave_file,0,sizeof(wcsave_file));
	//memset(buffer,0,sizeof(buffer));
	///* Load home dir */
	//GetEnvironmentVariableA("APPDATA",wcsave_home,260);
	///* Make base */
	//sprintf_s(wcsave_base,"%s\\%s\\",wcsave_home,"wcrpg");
	///* Print to log */
	//wclog_write(wcrpg_get_log(),"Save directory: ");
	//wclog_write(wcrpg_get_log(),wcsave_base);
	//wclog_enter(wcrpg_get_log());
	///* Create this directory if it does not exist */
	//if(!wcsave_dir_exist(wcsave_base))
	//{
	//	/* Create it, then */
	//	if(!wcsave_dir_create(wcsave_base))
	//	{
	//		/* Error */
	//		wcrpg_error("Could not create save directory (%s)",wcsave_base);
	//		return;
	//	}
	//	/* Log it */
	//	wclog_write(wcrpg_get_log(),"Save directory created.");
	//	wclog_enter(wcrpg_get_log());
	//}
	///* Now the directory exists for sure, whether it was there from before or not */
	///* Now, check for a folder related to the game in particular */
	//sprintf_s(buffer,sizeof(buffer),"%s",wcrpg_get_title());
	//for(i = 0;i < sizeof(buffer);i++)
	//{
	//	if(buffer[i] == ':' || buffer[i] == '*' || buffer[i] == '"' || buffer[i] == '|' || buffer[i] == '/' || buffer[i] == '\\' || buffer[i] == '?' || buffer[i] == '<' || buffer[i] == '>')
	//		buffer[i] = '-';
	//}
	//sprintf_s(wcsave_game_base,"%s%s\\",wcsave_base,buffer);
	///* Log this */
	//wclog_write(wcrpg_get_log(),"Game save directory: ");
	//wclog_write(wcrpg_get_log(),wcsave_game_base);
	//wclog_enter(wcrpg_get_log());
	///* Create this directory if it does not exist */
	//if(!wcsave_dir_exist(wcsave_game_base))
	//{
	//	/* Create it, then */
	//	if(!wcsave_dir_create(wcsave_game_base))
	//	{
	//		/* Error */
	//		wcrpg_error("Could not create save directory (%s)",wcsave_game_base);
	//		return;
	//	}
	//	/* Log it */
	//	wclog_write(wcrpg_get_log(),"Game save directory created.");
	//	wclog_enter(wcrpg_get_log());
	//}
	///* Catalog */
	//wcsave_load_tags();
}

/* Releaser */
void wcsave_exit()
{
	/* Delete music if had music */
	if(wcsave_music)
	{
		free(wcsave_music->get_file());
		delete wcsave_music;
	}
}

/* Get the save base */
char *wcsave_get_base()
{
	return wcsave_base;
}

/* Get the game base */
char *wcsave_get_game_base()
{
	return wcsave_game_base;
}

/* Generate save file from slot number */
char *wcsave_get_file(int slot)
{
	/* Reset and make */
	memset(wcsave_file,0,sizeof(wcsave_file));
	sprintf(wcsave_file,"%ssave%d.sav",wcsave_game_base,slot);
	return wcsave_file;
}

/* Generate tag file */
char *wcsave_get_tag_file()
{
	/* Reset and make */
	memset(wcsave_file,0,sizeof(wcsave_file));
	sprintf(wcsave_file,"%ssaveinfo.sav",wcsave_game_base);
	return wcsave_file;
}

/* Wipes out save catalog */
void wcsave_clear_tag(int slot)
{
	int j;
	free(wcsave_tags[slot].leadername);
	wcsave_tags[slot].used = 0;
	for(j = 0;j < MAX_PARTY_SIZE;j++)
	{
		free(wcsave_tags[slot].face[j]);
		wcsave_tags[slot].face[j] = 0;
		wcsave_tags[slot].party[j] = WCPARTY_NOBODY;
		wcsave_tags[slot].face_index[j] = 0;
	}
}

/* Saves tag info to the slot given */
void wcsave_tag(int slot)
{
	int i,j;
	FILE *f;
	/* Wipe out */
	wcsave_clear_tag(slot);
	/* Is now being used */
	wcsave_tags[slot].used = 1;
	/* Saved party */
	for(i = 0;i < MAX_PARTY_SIZE;i++)
	{
		wcsave_tags[slot].party[i] = wcparty_get(i);
		if(wcparty_get(i) != WCPARTY_NOBODY)
		{
			wcsave_tags[slot].face[i] = wcrpg_string_clone(wchero_get(wcparty_get(i))->get_faceset());
			wcsave_tags[slot].face_index[i] = wchero_get(wcparty_get(i))->get_faceset_number();
		}
	}
	if(wcparty_get(0) != WCPARTY_NOBODY)
		wcsave_tags[slot].leadername = wcrpg_string_clone(wchero_get(wcparty_get(0))->get_name());
	else
		wcsave_tags[slot].leadername = 0;
	/* Now write to disk */
	fopen(wcsave_get_tag_file(),"wb");
	for(i = 0;i < WCSAVE_SLOTS;i++)
	{
		wcsaveio_start(f);
		wcsaveio_write_num(wcsave_tags[i].used);
		wcsaveio_write_str(wcsave_tags[i].leadername);
		for(j = 0;j < MAX_PARTY_SIZE;j++)
		{
			wcsaveio_write_num(wcsave_tags[i].party[j]);
			wcsaveio_write_str(wcsave_tags[i].face[j]);
			wcsaveio_write_num(wcsave_tags[i].face_index[j]);
		}
		wcsaveio_end();
	}
	/* Save last used */
	wcsaveio_start(f);
	wcsaveio_write_num(wcsave_last_file);
	wcsaveio_end();
	fclose(f);
	/* Update */
	wcsavemenu_refresh(0);
}

/* Loads tag info if it exists */
void wcsave_load_tags()
{
	FILE *f;
	int i,j;
	/* Open */
	fopen(wcsave_get_tag_file(),"rb");
	/* Doesn't exist? */
	if(!f)
	{
		/* Report */
		wclog_write(wcrpg_get_log(),"No save catalog found");
		wclog_enter(wcrpg_get_log());
		return;
	}
	/* Load all */
	for(i = 0;i < WCSAVE_SLOTS;i++)
	{
		wcsaveio_read(f);
		wcsave_tags[i].used = wcsaveio_read_num();
		wcsave_tags[i].leadername = wcsaveio_read_str();
		for(j = 0;j < MAX_PARTY_SIZE;j++)
		{
			wcsave_tags[i].party[j] = wcsaveio_read_num();
			wcsave_tags[i].face[j] = wcsaveio_read_str();
			wcsave_tags[i].face_index[j] = wcsaveio_read_num();
		}
		wcsaveio_next();
	}
	/* Load last used */
	wcsaveio_read(f);
	wcsave_last_file = wcsaveio_read_num();
	wcsaveio_next();
	/* Close */
	fclose(f);
	/* Report */
	wclog_write(wcrpg_get_log(),"Save catalog loaded");
	wclog_enter(wcrpg_get_log());
	/* Update */
	wcsavemenu_refresh(1);
}

/* Saves header information into the game save */
void wcsave_save_header(FILE *f)
{
	int scrollv[6],i;
	rpgevent *ev;
	wcsaveio_start(f);
	/* Save player's current map ID, X, and Y positions */
	wcsaveio_write_num(wclevel_get_id());
	ev = wcplayer_get();
	wcsaveio_write_num(ev->get_pos_x());
	wcsaveio_write_num(ev->get_pos_y());
	wcsaveio_write_num(ev->get_direction());
	/* Save player's current money */
	wcsaveio_write_num(wcmisc_get_money());
	/* Save the save count? */
	wcsaveio_write_num(wcmisc_get_save_count());
	/* Save the current screen tint */
	wcsaveio_write_float(wcnucleus_get_tint_red());
	wcsaveio_write_float(wcnucleus_get_tint_green());
	wcsaveio_write_float(wcnucleus_get_tint_blue());
	wcsaveio_write_float(wcnucleus_get_tint_sat());
	/* Save auto anim */
	wcsaveio_write_num(wcmonster_get_auto_battle_charset());
	/* Save speed */
	if(wcplayer_get())
		wcplayer_get()->memorize_speed();
	wcsaveio_write_num(wcmisc_get_player_speed());
	/* Save current song */
	wcsaveio_write_str(wcsound_get_music_name());
	wcsaveio_write_num(wcsound_get_music_volume());
	wcsaveio_write_num(wcsound_get_music_tempo());
	wcsaveio_write_num(wcsound_get_music_balance());
	/* Save active moving events */
	wcsaveio_write_num(wcmove_get_active());
	/* Save parallaxing */
	wcsaveio_write_str(wclevel_get_parallax_name());
	wclevel_get_parallax_scroll(scrollv);
	for(i = 0;i < 6;i++)
		wcsaveio_write_num(scrollv[i]);
	/* Save graphic and fonts */
	wcsaveio_write_str(wcsystem_get_graphic_name());
	wcsaveio_write_num(wcsystem_get_graphic_tile());
	wcsaveio_write_num(wcsystem_get_font());
	wcsaveio_end();
}

/* Loads header information from game save */
void wcsave_load_header(FILE *f)
{
	char *name;
	int i,j,k;
	float af,bf,cf,df;
	wcsaveio_read(f);
	/* Reads map ID, X, and Y to load up onto */
	wcsave_dest_map = wcsaveio_read_num(); /* ID */
	wcsave_dest_map_x = wcsaveio_read_num(); /* X */
	wcsave_dest_map_y = wcsaveio_read_num(); /* Y */
	/* Make the player face */
	wcsave_dest_map_dir = wcsaveio_read_num(); /* FACING */
	/* Now load the player's money */
	i = wcsaveio_read_num(); /* AMT */
	wcmisc_add_money(i);
	/* Now load save count.. */
	i = wcsaveio_read_num(); /* COUNT */
	for(j = 0;j < i;j++)
		wcmisc_increment_save_count();
	/* Now load the tint */
	af = wcsaveio_read_float();
	bf = wcsaveio_read_float();
	cf = wcsaveio_read_float();
	df = wcsaveio_read_float();
	wcnucleus_tint(af,bf,cf,df,0.0f);
	/* Load auto anim */
	i = wcsaveio_read_num();
	wcmonster_enable_auto_battle_charset(i);
	/* Load speed */
	i = wcsaveio_read_num();
	wcmisc_set_player_speed(i);
	/* Load current song */
	name = wcsaveio_read_str();
	i = wcsaveio_read_num();
	j = wcsaveio_read_num();
	k = wcsaveio_read_num();
	wcsave_music = new music(name,i,j,k,0);
	/* Load active events */
	wcsave_active_move_count = wcsaveio_read_num();
	/* Load parallaxing */
	wcsave_parallax_name = wcsaveio_read_str();
	for(i = 0;i < 6;i++)
		wcsave_parallax_scroll[i] = wcsaveio_read_num();
	/* Load graphic and fonts */
	name = wcsaveio_read_str();
	i = wcsaveio_read_num();
	j = wcsaveio_read_num();
	wcsystem_set_graphic(name);
	wcsystem_set_graphic_tile(i);
	wcsystem_set_font(j);
	wcwindow_generate_all_fonts();
	wcsaveio_next();
}

/* Saves the current state of the game into the slot given */
void wcsave_save(int slot)
{
	FILE *f;
	/* Increment it */
	wcmisc_increment_save_count();
	/* Tag it */
	wcsave_last_file = slot;
	wcsave_tag(slot);
	/* Open channel */
	fopen(wcsave_get_file(slot),"wb");
	/* Save header */
	wcsave_save_header(f);
	/* Save all data */
	wcparty_save(f);
	wcmem_save(f);
	wcitem_save(f);
	wchero_save(f);
	wcmanage_save(f);
	wcmem_save_all_svar(f);
	wclevel_save_state(f);
	wccommon_save(f);
	wcplayer_save(f);
	/* Close channel */
	fclose(f);
	/* Report */
	wclog_write(wcrpg_get_log(),"Saved game ");
	wclog_write(wcrpg_get_log(),slot);
	wclog_enter(wcrpg_get_log());
}

/* Loads a state of game from the slot given */
void wcsave_load(int slot)
{
	FILE *f;
	/* Save tags */
	wcsave_loaded_game = 1;
	/* Open channel */
	fopen(wcsave_get_file(slot),"rb");
	/* Load header */
	wcsave_load_header(f);
	/* Load all data */
	wcparty_load(f);
	wcmem_load(f);
	wcitem_load(f);
	wchero_load(f);
	wcmanage_load(f);
	wcmem_load_all_svar(f);
	/* Resave tags */
	wcsave_tag(slot);
	/* Put the player there */
	wclevel_load(wcsave_dest_map,wcsave_dest_map_x,wcsave_dest_map_y);
	wcplayer_get()->face(wcsave_dest_map_dir);
	/* Load BG */
	wclevel_change_parallax(wcsave_parallax_name,wcsave_parallax_scroll[0],wcsave_parallax_scroll[1],wcsave_parallax_scroll[2],wcsave_parallax_scroll[3],wcsave_parallax_scroll[4],wcsave_parallax_scroll[5]);
	free(wcsave_parallax_name);
	wcsave_parallax_name = 0;
	/* Reload party */
	wcparty_refresh();
	wcplayer_refresh();
	/* Play music if have music */
	if(wcsave_music->get_file())
		wcsound_music_play(wcsave_music);
	/* Now load the level state */
	wclevel_load_state(f);
	wccommon_load(f);
	wcplayer_load(f);
	wcmove_set_active(wcsave_active_move_count);
	/* Close channel */
	fclose(f);
	/* Report */
	wcsave_last_file = slot;
	wclog_write(wcrpg_get_log(),"Loaded game ");
	wclog_write(wcrpg_get_log(),slot);
	wclog_enter(wcrpg_get_log());
}

/* Gets a save tag */
savetag *wcsave_get_tag(int slot)
{
	return &wcsave_tags[slot];
}

/* Has loaded game */
int wcsave_game_loaded()
{
	return wcsave_loaded_game;
}

/* Reset loaded gaem */
void wcsave_game_reset_loaded()
{
	wcsave_loaded_game = 0;
}

/* Get last file */
int wcsave_get_last()
{
	return wcsave_last_file;
}