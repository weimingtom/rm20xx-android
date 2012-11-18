/*
	System Settings (ones bound to the game database, not global ones)
*/

/* Includes */
#include <malloc.h>
#include "wcrpg.h"
#include "wctype.h"
#include "wcsystem.h"
#include "wclog.h"
#include "wchero.h"
#include "wcmaterial.h"
#include "wcwindow.h"

#include <android/log.h>

/* Globals */
char *wcsys_skiff; /* Vehicle graphic */
char *wcsys_ship;
char *wcsys_airship;

char *wcsys_title = 0; /* Title screen */

char *wcsys_gameover = 0; /* Gameover screen */

int wcsys_party_size; /* Initial party size */
int wcsys_party[WCSYSTEM_PARTY_START_SIZE]; /* Starting party */
int wcsys_menu_option_count; /* Number of menu options */
int wcsys_menu_option[WCSYSTEM_MAX_MENU_OPTIONS]; /* Menu options */

music *wcsys_song_title = 0; /* Common music */
music *wcsys_song_battle = 0; /* Battle music */
music *wcsys_song_victory = 0; /* Victory music */
music *wcsys_song_inn;
music *wcsys_song_skiff;
music *wcsys_song_ship;
music *wcsys_song_airship;
music *wcsys_song_gameover = 0; /* Defeat music */

sound *wcsys_snd_cursor = 0; /* Common sound effects */
sound *wcsys_snd_choice = 0;
sound *wcsys_snd_cancel = 0;
sound *wcsys_snd_buzzer = 0;
sound *wcsys_snd_battle = 0;
sound *wcsys_snd_flee = 0;
sound *wcsys_snd_attack = 0;
sound *wcsys_snd_damage = 0;
sound *wcsys_snd_ally_damage = 0;
sound *wcsys_snd_evade = 0;
sound *wcsys_snd_defeated = 0;
sound *wcsys_snd_item = 0;

texture *wcsys_system = 0; /* System graphic */
char *wcsys_system_name = 0; /* Name of system graphic */
texture *wcsys_gauge = 0; /* System gauge */
int wcsys_system_tile = 0; /* Tile? */
int wcsys_system_font = 0; /* System font? */

int wcsys_map_in = 0; /* Screen transitions */
int wcsys_map_out = 0;
int wcsys_battle_in = 0;
int wcsys_battle_out = 0;
int wcsys_victory_in = 0;
int wcsys_victory_out = 0;

/* System music */
#include "wcsystemmusic.h"

/* System sound */
#include "wcsystemsound.h"

/* Set title graphics */
void wcsystem_set_title_graphic(char *c)
{
	free(wcsys_title);
	wcsys_title = c;
	wclog_write(wcrpg_get_log(),"Title Graphic is now ");
	wclog_write(wcrpg_get_log(),c);
	wclog_enter(wcrpg_get_log());
}

/* Get title graphics */
char *wcsystem_get_title_graphic()
{
	return wcsys_title;
}

/* Set game over graphics */
void wcsystem_set_game_over_graphic(char *c)
{
	free(wcsys_gameover);
	wcsys_gameover = c;
	wclog_write(wcrpg_get_log(),"Game Over Graphic is now ");
	wclog_write(wcrpg_get_log(),c);
	wclog_enter(wcrpg_get_log());
}

/* Get game over graphics */
char *wcsystem_get_game_over_graphic()
{
	return wcsys_gameover;
}

/* Process entry */
int wcsystem_process(FILE *f,int code,int len)
{
	int rlen,i;
	unsigned short u16hero;
	unsigned short u16menuitem;
	if(code == 17)
	{
		//__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcsystem_process [0]");
		wcsystem_set_title_graphic(wcstr_read_file(f,len));
		return 1;
	}
	if(code == 18)
	{
		//__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcsystem_process [1]");
		wcsystem_set_game_over_graphic(wcstr_read_file(f,len));
		return 1;
	}
	if(code == 19)
	{
		//__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcsystem_process [2]");
		wcsystem_set_graphic(wcstr_read_file(f,len));
		return 1;
	}
	/* Music goes here */
	if(code == 31)
	{
		//__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcsystem_process [3]");
		wcsystem_set_title_music(new music(f));
		return 1;
	}
	if(code == 32)
	{
		//__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcsystem_process [4]");
		wcsystem_set_battle_music(new music(f));
		return 1;
	}
	if(code == 33)
	{
		//__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcsystem_process [5]");
		wcsystem_set_victory_music(new music(f));
		return 1;
	}
	if(code == 38)
	{
		//__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcsystem_process [6]");
		wcsystem_set_gameover_music(new music(f));
		return 1;
	}
	/* Music ends here */
	if(code == 20)
	{
		//__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcsystem_process [7]");
		wcsystem_set_gauge(wcstr_read_file(f,len));
		return 1;
	}
	if(code == 21)
	{
		//__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcsystem_process [8]");
		wcsys_party_size = wcint_read_file(f,&rlen);
		wclog_write(wcrpg_get_log(),"Initial party size is ");
		wclog_write(wcrpg_get_log(),wcsys_party_size);
		wclog_enter(wcrpg_get_log());
		return 1;
	}
	if(code == 22)
	{
		//__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcsystem_process [9]");
		/* OK read them */
		for(i = 0;i < len/2;i++)
		{
			fread(&u16hero,2,1,f);
			wcsys_party[i] = u16hero;
			wclog_write(wcrpg_get_log(),"Hero #");
			wclog_write(wcrpg_get_log(),(i+1));
			wclog_write(wcrpg_get_log(),": ");
			wclog_write(wcrpg_get_log(),wchero_get(wcsys_party[i])->get_name());
			wclog_enter(wcrpg_get_log());
		}
		return 1;
	}
	/* If 2003, */
	if(!wcrpg_old_mode())
	{
		//__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcsystem_process [10]");
		if(code == 27)
		{
			/* Menu options */
			wcsys_menu_option_count = len/2;
			for(i = 0;i < len/2;i++)
			{
				fread(&u16menuitem,2,1,f);
				wcsys_menu_option[i] = (int)u16menuitem;
				wclog_write(wcrpg_get_log(),"Menu option ");
				wclog_write(wcrpg_get_log(),(i+1));
				wclog_write(wcrpg_get_log()," is ");
				wclog_write(wcrpg_get_log(),(int)u16menuitem);
				wclog_enter(wcrpg_get_log());
			}
			return 1;
		}
	}
	else
	{
		//__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcsystem_process [11]");
		/* Not 2003 */
		wcsys_menu_option_count = 4;
		for(i = 0;i < 4;i++)
			wcsys_menu_option[i] = i+1;
	}
	if(code == 71)
	{
		//__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcsystem_process [12]");
		wcsystem_set_graphic_tile(wcint_read_file(f,&rlen));
		return 1;
	}
	if(code == 72)
	{
		//__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcsystem_process [13]");
		wcsystem_set_font(wcint_read_file(f,&rlen));
		return 1;
	}
	/* Trans */
	if(code == 61)
	{
		//__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcsystem_process [14]");
		wcsys_map_in = wcint_read_file(f,&rlen);
		return 1;
	}
	if(code == 62)
	{
		//__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcsystem_process [15]");
		wcsys_map_out = wcint_read_file(f,&rlen);
		return 1;
	}
	if(code == 63)
	{
		//__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcsystem_process [16]");
		wcsys_battle_in = wcint_read_file(f,&rlen);
		return 1;
	}
	if(code == 64)
	{
		//__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcsystem_process [17]");
		wcsys_battle_out = wcint_read_file(f,&rlen);
		return 1;
	}
	if(code == 65)
	{
		//__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcsystem_process [18]");
		wcsys_victory_in = wcint_read_file(f,&rlen);
		return 1;
	}
	if(code == 66)
	{
		//__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcsystem_process [19]");
		wcsys_victory_out = wcint_read_file(f,&rlen);
		return 1;
	}
	/* Pass off to sound */
	if(wcsystem_process_sound(f,code,len)){
		//__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcsystem_process [20]");
		return 1;
	}
	return 0;
}

/* Load all settings */
void wcsystem_load_all(FILE *f,int len)
{
	int code,slen,rlen;
	unsigned char *d;
	wclog_write(wcrpg_get_log(),"Loading game settings...");
	wclog_enter(wcrpg_get_log());
	while(len > 0)
	{
		/* Get code */
		code = 0;
		fread(&code,1,1,f);
		len--;
		/* Zero code */
		if(!code)
			break;
		/* Get length */
		slen = wcint_read_file(f,&rlen);
		len -= rlen;
		if(!wcsystem_process(f,code,slen))
		{
			/* Skip over it */
			d = (unsigned char*)malloc(slen);
			fread(d,1,slen,f);
			free(d);
		}
		len -= slen;
	}
	if(len != 0)
		wcrpg_error("Garbage data detected in game settings (%d)",len);
}

/* Release all data */
void wcsystem_free_all()
{
	/* Free music */
	if(wcsys_song_title)
		delete wcsys_song_title;
	/* Free sound */
	wcsystem_free_sound();
	free(wcsys_title);
}

/* Get starting party */
int wcsystem_get_init_party_member(int pos)
{
	/* Err */
	if(pos < 0 || pos >= 4)
	{
		wcrpg_error("Initial party member request out of range (%d)",pos);
		return 0;
	}
	/* Get */
	return wcsys_party[pos];
}

/* Set system graphic */
void wcsystem_set_graphic(char *c)
{
	/* Erase old */
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcsystem_set_graphic [0]");	
	if(wcsys_system)
		delete wcsys_system;
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcsystem_set_graphic [1]");	
	/* Load it */
	wcsys_system = new texture(wcmaterial_request(MATERIAL_SYSTEM,c));
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcsystem_set_graphic [2]");	
	wcwindow_generate_all_fonts();
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcsystem_set_graphic [3]");	
	/* Set name */
	if(wcsys_system_name)
		free(wcsys_system_name);
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcsystem_set_graphic [4]");	
	wcsys_system_name = wcrpg_string_clone(c);
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcsystem_set_graphic [5]");	
}

/* Get system graphic name */
char *wcsystem_get_graphic_name()
{
	return wcsys_system_name;
}

/* Get system graphic */
texture *wcsystem_get_graphic()
{
	return wcsys_system;
}

/* Set tiling mode */
void wcsystem_set_graphic_tile(int en)
{
	wcsys_system_tile = en;
}

/* Get tiling mode */
int wcsystem_get_graphic_tile()
{
	return wcsys_system_tile;
}

/* Set font */
void wcsystem_set_font(int fi)
{
	wcsys_system_font = fi;
}

/* Get font */
int wcsystem_get_font()
{
	return wcsys_system_font;
}

/* Set gauge */
void wcsystem_set_gauge(char *c)
{
	/* Erase old */
	if(wcsys_gauge)
		delete wcsys_gauge;
	/* Load it */
	wcsys_gauge = new texture(wcmaterial_request(MATERIAL_SYSTEM_2,c));
}

/* Get gauge */
texture *wcsystem_get_gauge()
{
	return wcsys_gauge;
}

/* Get menu option count */
int wcsystem_menu_option_count()
{
	return wcsys_menu_option_count;
}

/* Get menu option code */
int wcsystem_menu_option(int ix)
{
	return wcsys_menu_option[ix];
}

/* Turn menu option code to vocab code */
int wcsystem_menu_option_to_vocab(int code)
{
	/* Return menu code */
	if(code == 1)
		return 106; /* Item */
	if(code == 2)
		return 107; /* Skill */
	if(code == 3)
		return 108; /* Equip */
	if(code == 4)
		return 110; /* Save */
	if(code == 5)
		return 118; /* Status */
	if(code == 6)
		return 119; /* Row */
	if(code == 7)
		return 120; /* Order */
	if(code == 8)
		return 122; /* Active/Wait */
	/* Error */
	return 0;
}

/* Default map trans */
int wcsystem_get_map_trans(int out)
{
	if(out)
		return wcsys_map_out;
	return wcsys_map_in;
}

/* Default battle trans */
int wcsystem_get_battle_trans(int out)
{
	if(out)
		return wcsys_battle_out;
	return wcsys_battle_in;
}

/* Default victory trans */
int wcsystem_get_victory_trans(int out)
{
	if(out)
		return wcsys_victory_out;
	return wcsys_victory_in;
}