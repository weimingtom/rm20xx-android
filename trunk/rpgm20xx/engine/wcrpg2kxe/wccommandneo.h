/* Codes */
#define X_COMMAND_STOP 0
#define X_COMMAND_DRAW_NUMBER 6
#define X_COMMAND_DRAW_WINDOW 8
#define X_COMMAND_REMAP_MENU 18
#define X_COMMAND_DRAW_HEALTH_BAR 12
#define X_COMMAND_DRAW_MANA_BAR 13
#define X_COMMAND_DRAW_TIME_BAR 14
#define X_COMMAND_ANIMATE_ENEMIES 16
#define X_COMMAND_SET_SELF_VARIABLE_RANGE 19
#define X_COMMAND_DRAW_BATTLE_CHARACTER 20
#define X_COMMAND_DRAW_BATTLE_ATTACK_RIGHT 21
#define X_COMMAND_DRAW_BATTLE_ATTACK_LEFT 22
#define X_COMMAND_OPEN_SUB_MENU 23
#define X_COMMAND_DRAW_MONSTER 24
#define X_COMMAND_BATTLE_CHARACTER_OPTION 25
#define X_COMMAND_SAVE_SELF 26
#define X_COMMAND_SCREEN_SHADER 27
#define X_COMMAND_EVENT_SHADER 28
#define X_COMMAND_TILE_SHADER 29
#define X_COMMAND_DRAW_IMAGE 30
#define X_COMMAND_DRAW_NAME 31
#define X_COMMAND_LOAD_DATA 32
#define X_COMMAND_LOAD_LIST 33

/* Includes */
#include "wcguibuild.h"
#include "wcitemmenu.h"
#include "wcstatusmenu.h"
#include "wcsavemenu.h"
#include "wcmonster.h"
#include "wcbattleset.h"
#include "wcdraw.h"

/* Decode and execute 20XX command */
int wccommand_neo_run(void *me,int a,int b,int c,int d,int e)
{
	guielement *ge;
	rpgevent *ev;
	ev = (rpgevent*)me;
	/* The stop command */
	if(e == X_COMMAND_STOP)
		wcrpg_exit();
	/* The draw number command */
	if(e == X_COMMAND_DRAW_NUMBER)
	{
		/* Prepare */
		ge = new guielement(GUI_BUILDER_NUMBER,a,b);
		ge->set_number(c);
		ge->set_option(d);
		/* Add */
		wcguibuild_add(ge);
	}
	/* The draw image command */
	if(e == X_COMMAND_DRAW_IMAGE)
	{
		/* Prepare */
		ge = new guielement(GUI_BUILDER_IMAGE,b,c);
		ge->set_number(a);
		ge->set_option(d);
		/* Add */
		wcguibuild_add(ge);
	}
	/* The draw window command */
	if(e == X_COMMAND_DRAW_WINDOW)
	{
		/* Prepare */
		ge = new guielement(GUI_BUILDER_WINDOW,a,b);
		ge->set_size(c,d);
		/* Add */
		wcguibuild_add(ge);
	}
	/* The remap command */
	if(e == X_COMMAND_REMAP_MENU)
	{
		/* Remap! */
		wcmanage_remap(a,b,c);
	}
	/* The draw health bar command */
	if(e == X_COMMAND_DRAW_HEALTH_BAR)
	{
		/* Prepare */
		ge = new guielement(GUI_BUILDER_HEALTH_BAR,a,b);
		ge->set_number(c);
		ge->set_size(d,0);
		/* Add */
		wcguibuild_add(ge);
	}
	/* The draw mana bar command */
	if(e == X_COMMAND_DRAW_MANA_BAR)
	{
		/* Prepare */
		ge = new guielement(GUI_BUILDER_MANA_BAR,a,b);
		ge->set_number(c);
		ge->set_size(d,0);
		/* Add */
		wcguibuild_add(ge);
	}
	/* The draw time bar command */
	if(e == X_COMMAND_DRAW_TIME_BAR)
	{
		/* Prepare */
		ge = new guielement(GUI_BUILDER_TIME_BAR,a,b);
		ge->set_number(c);
		ge->set_size(d,0);
		/* Add */
		wcguibuild_add(ge);
	}
	/* Set range */
	if(e == X_COMMAND_SET_SELF_VARIABLE_RANGE)
	{
		/* Prepare */
		wcmem_set_self_var_range(a,b);
	}
	/* Draw battle char */
	if(e == X_COMMAND_DRAW_BATTLE_CHARACTER)
	{
		/* Preapre */
		ge = new guielement(GUI_BUILDER_BATTLE_CHARACTER,a,b);
		ge->set_number(c);
		ge->set_option(d);
		/* Add */
		wcguibuild_add(ge);
	}
	/* Draw battle attack right */
	if(e == X_COMMAND_DRAW_BATTLE_ATTACK_RIGHT)
	{
		/* Preapre */
		ge = new guielement(GUI_BUILDER_BATTLE_ATTACK_RIGHT,a,b);
		ge->set_number(c);
		ge->set_option(d);
		/* Add */
		wcguibuild_add(ge);
	}
	/* Draw battle attack left */
	if(e == X_COMMAND_DRAW_BATTLE_ATTACK_LEFT)
	{
		/* Preapre */
		ge = new guielement(GUI_BUILDER_BATTLE_ATTACK_LEFT,a,b);
		ge->set_number(c);
		ge->set_option(d);
		/* Add */
		wcguibuild_add(ge);
	}
	/* Open sub menu */
	if(e == X_COMMAND_OPEN_SUB_MENU)
	{
		/* Find which menu to open */
		if(a == 1)
			wcitemmenu_open(0);
		if(a == 3)
			wcstatusmenu_open_equip(0);
		if(a == 4)
			wcsavemenu_open(0,0);
		if(a == 5)
			wcstatusmenu_open(0);
		if(a == 9)
			wcsavemenu_open(0,1);
		wcmanage_stack();
	}
	/* Draw monster */
	if(e == X_COMMAND_DRAW_MONSTER)
	{
		/* Prepare */
		ge = new guielement(GUI_BUILDER_MONSTER,a,b);
		ge->set_number(c);
		ge->set_option(d);
		/* Add */
		wcguibuild_add(ge);
	}
	/* Animate enemies */
	if(e == X_COMMAND_ANIMATE_ENEMIES)
	{
		/* Set */
		wcmonster_enable_auto_battle_charset(a);
	}
	/* Set options */
	if(e == X_COMMAND_BATTLE_CHARACTER_OPTION)
	{
		/* Set */
		wcbattleset_set_option(a,b);
	}
	/* Save myself */
	if(e == X_COMMAND_SAVE_SELF)
	{
		/* Save me */
		wcmem_save_svar(me);
	}
	/* Set screen shader */
	if(e == X_COMMAND_SCREEN_SHADER)
	{
		if(b == 0)
		{
			/* Erase shader */
			wcdraw_set_screen_shader(0);
		}
		else
		{
			/* Set shader */
			wcdraw_load_shader(a);
			wcdraw_set_screen_shader(wcdraw_get_shader(a));
		}
	}
	/* Set event shader */
	if(e == X_COMMAND_EVENT_SHADER)
	{
		if(b == 0)
		{
			/* Erase shader */
			ev->set_shader(0);
		}
		else
		{
			/* Set shader */
			wcdraw_load_shader(a);
			ev->set_shader(wcdraw_get_shader(a));
		}
	}	
	/* Set tile shader */
	if(e == X_COMMAND_TILE_SHADER)
	{
		if(b == 0)
		{
			/* Erase shader */
			wctileset_set_shader(c,0);
		}
		else
		{
			/* Set shader */
			wcdraw_load_shader(a);
			wctileset_set_shader(c,wcdraw_get_shader(a));
		}
	}
	/* Draw name */
	if(e == X_COMMAND_DRAW_NAME)
	{
		/* Prepare */
		ge = new guielement(GUI_BUILDER_NAME,c,d);
		ge->set_option(a);
		ge->set_number(b);
		/* Add */
		wcguibuild_add(ge);
	}
	/* Load data */
	if(e == X_COMMAND_LOAD_DATA)
	{
		/* Load data */
		wcmem_load_data(a,b,c);
	}
	/* Load list */
	if(e == X_COMMAND_LOAD_LIST)
	{
		/* Load list */
		wcmem_load_list(a,b,c,d);
	}
	/* Always return zero normally */
	return 0;
}