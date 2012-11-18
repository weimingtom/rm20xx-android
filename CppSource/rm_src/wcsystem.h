#ifndef WCSYSTEM_H
#define WCSYSTEM_H

/* Defines */
#define WCSYSTEM_PARTY_START_SIZE 4
#define WCSYSTEM_MAX_MENU_OPTIONS 8

#define WCSYSTEM_CURSOR 41
#define WCSYSTEM_DECIDE 42
#define WCSYSTEM_CANCEL 43
#define WCSYSTEM_BUZZER 44
#define WCSYSTEM_BATTLE 45
#define WCSYSTEM_ESCAPE 46
#define WCSYSTEM_ENEMY_ATTACK 47
#define WCSYSTEM_ENEMY_DAMAGE 48
#define WCSYSTEM_HERO_DAMAGE 49
#define WCSYSTEM_DODGE 50
#define WCSYSTEM_DEFEATED 51
#define WCSYSTEM_ITEM 52

/* Includes */
#include "wcsound.h"
#include "wcdraw.h"

/*
	Loads all system settings
*/
extern void wcsystem_load_all(FILE *f,int len);

/*
	Release all system settings
*/
extern void wcsystem_free_all();

/*
	Sets
*/
extern void wcsystem_set_title_music(music *m);
extern void wcsystem_set_battle_music(music *m);
extern void wcsystem_set_victory_music(music *m);
extern void wcsystem_set_gameover_music(music *m);
extern void wcsystem_set_title_graphic(char *c);
extern void wcsystem_set_game_over_graphic(char *c);
extern void wcsystem_set_graphic(char *c);
extern void wcsystem_set_graphic_tile(int en);
extern void wcsystem_set_font(int fi);
extern void wcsystem_set_gauge(char *c);

/*
	Gets
*/
extern char *wcsystem_get_title_graphic();
extern char *wcsystem_get_game_over_graphic();
extern int wcsystem_get_init_party_member(int pos);
extern texture *wcsystem_get_graphic();
extern char *wcsystem_get_graphic_name();
extern int wcsystem_get_graphic_tile();
extern int wcsystem_get_font();
extern texture *wcsystem_get_gauge();
extern int wcsystem_menu_option_count();
extern int wcsystem_menu_option(int ix);
extern int wcsystem_menu_option_to_vocab(int code);
extern int wcsystem_get_map_trans(int out);
extern int wcsystem_get_battle_trans(int out);
extern int wcsystem_get_victory_trans(int out);

/*
	Plays system sounds
*/
extern void wcsystem_play_sound(int id);

/*
	Plays system music
*/
extern void wcsystem_play_title_music();
extern void wcsystem_play_battle_music();
extern void wcsystem_play_victory_music();
extern void wcsystem_play_gameover_music();

#endif