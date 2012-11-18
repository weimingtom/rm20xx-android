/*
	RPG manager
*/

/* Includes */
#include "wcrpg.h"
#include "wcmanage.h"
#include "wcmenu.h"
#include "wcinput.h"
#include "wcheroplate.h"
#include "wcdialog.h"
#include "wcsavemenu.h"
#include "wcitemmenu.h"
#include "wcquitmenu.h"
#include "wcstatusmenu.h"
#include "wcsystem.h"
#include "wcsave.h"
#include "wcwindow.h"
#include "wcmisc.h"
#include "wcvocab.h"
#include "wccommon.h"
#include "wcsaveio.h"
#include "wcplayer.h"

#include <string>

/* Globals */
menu *manage_menu;
int manage_remap[8];
int manage_override_disable[8];
int manage_active = 0;
int manage_autofire = 0;
int manage_allowed = 1;
int manage_submenu = 0;
int manage_hero_plate_pos = 0;
float manage_hero_slide = 0.0f;
float manage_slide = 0.0f;
text *manage_money_text = 0;

/* Saves remap options */
void wcmanage_save(FILE *f)
{
	int i;
	wcsaveio_start(f);
	for(i = 0;i < sizeof(manage_remap)/sizeof(int);i++)
	{
		wcsaveio_write_num(manage_remap[i]);
		wcsaveio_write_num(manage_override_disable[i]);
	}
	wcsaveio_end();
}

/* Loads remap options */
void wcmanage_load(FILE *f)
{
	int i;
	wcsaveio_read(f);
	for(i = 0;i < sizeof(manage_remap)/sizeof(int);i++)
	{
		manage_remap[i] = wcsaveio_read_num();
		manage_override_disable[i] = wcsaveio_read_num();
	}
	wcsaveio_next();
}

/* Remaps a menu option to a common event */
void wcmanage_remap(int cmd,int com,int en)
{
	manage_remap[cmd-1] = com;
	if(!en)
		manage_override_disable[cmd-1] = 1;
	else
		manage_override_disable[cmd-1] = 0;
}

/* Set hero plate offset */
void wcmanage_scroll_hero_plate(int pos)
{
	manage_hero_plate_pos = pos;
}

/* Get pixel perfect scrolling offset */
int wcmanage_get_hero_scroll_pos()
{
	return (int)manage_hero_slide;
}

/* Enable/disable manager */
void wcmanage_enable(int en)
{
	manage_allowed = en;
}

/* Initialize */
void wcmanage_init()
{
	manage_menu = wcmenu_generate_main();
	memset(manage_remap,0,sizeof(manage_remap));
	memset(manage_override_disable,0,sizeof(manage_override_disable));
}

/* Refresh options with the main menu */
void wcmanage_refresh()
{
	int i;
	char buffer[32];
	/* Check all opts */
	for(i = 0;i < wcsystem_menu_option_count();i++)
	{
		/* Allow/Disallow save */
		if(wcsystem_menu_option(i) == 4 && !wcsave_is_allowed())
			manage_menu->set_disable(i,1);
		else
			manage_menu->set_disable(i,0);
		/* Allow/Disallow override */
		if(manage_override_disable[wcsystem_menu_option(i)-1])
			manage_menu->set_disable(i,1);
		else
			manage_menu->set_disable(i,0);
	}
	/* Money */
	if(manage_money_text)
		delete manage_money_text;
	sprintf(buffer,"%s %d",wcvocab_get(95),wcmisc_get_money());
	manage_money_text = new text(buffer);
}

/* Open main menu */
void wcmanage_open()
{
	wcsystem_play_sound(WCSYSTEM_DECIDE);
	wcmanage_refresh();
	wcheroplate_refresh();
	wchero_refresh();
	manage_active = 1;
	manage_menu->activate();
	manage_autofire = 1;
}

/* Is sub menu active? */
int wcmanage_submenu_active()
{
	if(wcsavemenu_active())
		return 1;
	if(wcitemmenu_active())
		return 1;
	if(wcquitmenu_active())
		return 1;
	if(wcstatusmenu_active())
		return 1;
	return 0;
}

/* Handle */
void wcmanage_handle()
{
	float ts;
	int ch;
	/* Get the time step */
	ts = wcrpg_get_time_step();
	/* Always slide plates */
	manage_hero_slide += (manage_hero_plate_pos-manage_hero_slide)/2.0f*ts;
	if(!wcinput_menu() && !manage_submenu)
		manage_autofire = 0;
	/* Handle the root menu */
	manage_menu->handle();
	/* Open? */
	if(wcinput_menu() && !manage_submenu && !manage_autofire && !manage_active && manage_allowed && !wcdialog_active() && wcplayer_is_allowed_to_move())
	{
		wcmanage_open();
	}
	/* Handle */
	if(!manage_active)
	{
		/* Slide closed */
		if(!wcstatusmenu_active())
			manage_slide += (-manage_slide/2.0f)*ts;
		else
			manage_slide += ((1-manage_slide)/2.0f)*ts;
		return;
	}
	/* Slide open */
	manage_slide += ((1-manage_slide)/2.0f)*ts;
	/* Stop the base menu if a submenu is open */
	if(!wcmanage_submenu_active())
	{
		/* Make a choice */
		if(manage_menu->get_selection() != -1)
		{
			/* Route choice */
			ch = wcsystem_menu_option(manage_menu->get_selection());
			/* Quit option */
			if(manage_menu->get_selection() == manage_menu->size()-1)
			{
				wcquitmenu_open(manage_menu);
				/* Up */
				manage_submenu = 1;
			}
			else
			{
				/* Remap option */
				if(manage_remap[ch-1])
				{
					/* Execute! */
					wccommon_run(manage_remap[ch-1]);
					/* Close manager without sound or warning */
					manage_autofire = 1;
					manage_menu->close();
					manage_active = 0;
				}
				else
				{
					/* Save option */
					if(ch == 4)
						wcsavemenu_open(manage_menu,0);
					/* Status option */
					if(ch == 5)
						wcstatusmenu_open(manage_menu);
					/* Item option */
					if(ch == 1)
						wcitemmenu_open(manage_menu);
					/* Equipment option */
					if(ch == 3)
						wcstatusmenu_open_equip(manage_menu);
					/* Skill option */
					if(ch == 2)
						wcstatusmenu_open_skill(manage_menu);
					/* Up */
					manage_submenu = 1;
				}
			}
		}
		/* Close */
		if(wcinput_menu() && !manage_autofire && !manage_submenu)
		{
			wcsystem_play_sound(WCSYSTEM_CANCEL);
			manage_autofire = 1;
			manage_menu->close();
			manage_active = 0;
		}
	}
}

/* Forces the manager closed */
void wcmanage_close()
{
	wcsystem_play_sound(WCSYSTEM_CANCEL);
	manage_autofire = 1;
	manage_menu->close();
	manage_active = 0;
	manage_submenu = 0;
}

/* Return from sub menu */
void wcmanage_return()
{
	manage_submenu = 0;
	manage_autofire = 1;
}

/* Stacks a submenu on top */
void wcmanage_stack()
{
	manage_submenu = 1;
}

/* Returns true if active */
int wcmanage_active()
{
	/* Also active if any sub menus are */
	if(wcmanage_submenu_active())
		return 1;
	/* Main menu is active */
	return manage_active;
}

/* Draw */
void wcmanage_draw()
{
	/* Draw menu */
	manage_menu->draw(0,0);
	wcheroplate_draw_party((WCRPG_WIDTH-(320-WCMENU_MAIN_WIDTH))+(int)(WCRPG_WIDTH*(1-manage_slide)),-(int)manage_hero_slide);
	/* Draw money window */
	if(manage_money_text)
	{
		wcwindow_draw(0,WCRPG_HEIGHT-(int)((manage_slide)*32),80,32);
		manage_money_text->move(8,WCRPG_HEIGHT-(int)((manage_slide)*32)+6);
		manage_money_text->draw();
	}
}

/* Clear */
void wcmanage_exit()
{
	delete manage_menu;
}