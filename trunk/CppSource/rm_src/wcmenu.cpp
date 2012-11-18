/*
	Basic menu tool
*/

/* Includes */
#include <malloc.h>
#include <stdio.h>
#include "wcinput.h"
#include "wcrpg.h"
#include "wcmenu.h"
#include "wcwindow.h"
#include "wcvocab.h"
#include "wcsystem.h"
#include "wcsave.h"

#include <android\log.h>

/* Create menu */
menu :: menu(int w,int os)
{
	/* Initialize */
	width = w;
	options = os;
	ls = wcarray_new();
	sls = wcarray_new();
	dls = wcarray_new();
	nls = wcarray_new();
	wcarray_set_clean(sls,free);
	wcarray_set_clean(dls,free);
	wcarray_set_clean(nls,free);
	selected = 0;
	accepted = 0;
	view = 0;
	closing = 0;
	active = 0;
	autofire = 0;
	introdist = 0.0f;
	numspace = WCMENU_DEFAULT_NUMBER_SPACING;
}

/* Release menu */
menu :: ~menu()
{
	int i;
	/* Delete all */
	for(i = 0;i < wcarray_size(ls);i++)
		delete ((text*)wcarray_get(ls,i));
	wcarray_delete(ls);
	wcarray_delete(sls);
	wcarray_delete(dls);
	wcarray_delete(nls);
}

/* Add entry to menu */
void menu :: add(char *str)
{
	text *tx;
	int *dis;
	/* Add */
	dis = (int*)malloc(sizeof(int));
	*dis = 0;
	wcarray_add(dls,dis);
	wcarray_add(sls,str);
	tx = new text(str);
	tx->advance(TEXT_ALL,0);
	wcarray_add(ls,tx);
	wcarray_add(nls,0);
}

/* Add entry plus a number quantity to menu */
void menu :: add(char *str,int amt)
{
	text *tx;
	int *dis;
	char buffer[32];
	/* Add */
	dis = (int*)malloc(sizeof(int));
	*dis = 0;
	wcarray_add(dls,dis);
	wcarray_add(sls,str);
	tx = new text(str);
	tx->advance(TEXT_ALL,0);
	wcarray_add(ls,tx);
	sprintf(buffer,"%d",amt);
	wcarray_add(nls,wcrpg_string_clone(buffer));
}

/* Closes the menu */
void menu :: close()
{
	if(!closing)
	{
		__android_log_print(ANDROID_LOG_INFO, "adMOB", "menu :: close()");
		active = 1;
		closing = 1;
	}
}

/* Draw menu */
void menu :: draw(int x,int y)
{
	text *tx;
	char *nstr;
	int i,showlen,half;
	/* Inactive AND folded */
	if(!active && introdist <= 0.0f)
		return;

	//__android_log_print(ANDROID_LOG_INFO, "adMOB", "menu :: draw()");

	/* Half size */
	half = (options*wcwindow_font_character_height()+16)/2-8;
	/* Draw background */
	wcwindow_draw(x,y+(int)(half*(1.0f-introdist)),width,(int)((options*wcwindow_font_character_height())*introdist)+16);
	/* Draw texts */
	if(introdist >= 1.0f)
	{
		/* Show selected */
		wcwindow_draw_cursor(x+8,y+8+(selected-view)*wcwindow_font_character_height(),width-16,wcwindow_font_character_height(),active);
		/* Draw text */
		showlen = wcarray_size(ls)-view;
		if(showlen > options)
			showlen = options;
		//__android_log_print(ANDROID_LOG_INFO, "adMOB", "menu :: draw() %d", showlen);
		for(i = 0;i < showlen;i++)
		{
			/* Draw text */
			tx = (text*)wcarray_get(ls,i+view);
			tx->move(x+10,y+6+i*wcwindow_font_character_height());
			tx->draw();
			/* Draw number */
			nstr = (char*)wcarray_get(nls,i+view);
			if(nstr)
				wcwindow_draw_text(x+width-numspace,y+6+i*wcwindow_font_character_height(),0,nstr);
		}
	}
	/* Draw arrows */
	if(view > 0)
		wcwindow_draw_arrow(x+width/2-8,y,0);
	if(view < wcarray_size(ls)-options)
		wcwindow_draw_arrow(x+width/2-8,y+options*16+16-8,1);
}

/* Opens menu */
void menu :: activate()
{
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "menu :: activate()");
	active = 1;
	autofire = 1;
	closing = 0;
	accepted = 0;
}

/* Returns if active */
int menu :: is_active()
{
	return active;
}

/* Make selection for user */
void menu :: set_selection(int s)
{
	selected = s;
	accepted = 1;
	closing = 1;
}

/* Move selection for user */
void menu :: move_selection(int s)
{
	selected = s;
}

/* Cursor up */
void menu :: cursor_up()
{
	if(selected > 0)
	{
		wcsystem_play_sound(WCSYSTEM_CURSOR);
		selected--;
		autofire = 1;
		if(selected < view)
			view--;
	}
}

/* Cursor down */
void menu :: cursor_down()
{
	if(selected < wcarray_size(ls)-1)
	{
		wcsystem_play_sound(WCSYSTEM_CURSOR);
		selected++;
		autofire = 1;
		if(selected > view+options-1)
			view++;
	}
}

/* Handles menu */
void menu :: handle()
{
	float ts;
	int *dis;
	/* Handle only if active */
	if(!active)
		return;
	/* Intro */
	ts = wcrpg_get_time_step();
	if(!closing)
	{
		if(introdist < 1.0f)
		{
			introdist += ts*0.4f;
			if(introdist > 1.0f)
				introdist = 1.0f;
			return;
		}
		introdist = 1.0f;
	}
	else
	{
		if(introdist > 0.0f)
		{
			introdist -= ts*0.4f;
			if(introdist < 0.0f)
				introdist = 0.0f;
			return;
		}
		introdist = 0.0f;
		active = 0;
		return;
	}
	/* Move up/down */
	if(wcinput_y() < 0 && !autofire)
		cursor_up();
	if(wcinput_y() > 0 && !autofire)
		cursor_down();
	if(wcinput_scroll_y() < 0 && !autofire)
	{
		cursor_up();cursor_up();cursor_up();cursor_up();cursor_up();cursor_up();cursor_up();cursor_up();
	}
	if(wcinput_scroll_y() > 0 && !autofire)
	{
		cursor_down();cursor_down();cursor_down();cursor_down();cursor_down();cursor_down();cursor_down();cursor_down();
	}
	/* Select */
	if(wcinput_ok() && !autofire)
	{
		/* Nothing to select? */
		if(wcarray_size(ls) <= 0)
		{
			wcsystem_play_sound(WCSYSTEM_BUZZER);
		}
		else
		{
			/* Normal */
			dis = (int*)wcarray_get(dls,selected);
			if(!*dis)
			{
				wcsystem_play_sound(WCSYSTEM_DECIDE);
				accepted = 1;
				active = 0;
			}
			else
			{
				wcsystem_play_sound(WCSYSTEM_BUZZER);
			}
		}
		autofire = 1;
	}
	/* Release */
	if(!wcinput_y() && !wcinput_ok() && !wcinput_scroll_y())
		autofire = 0;
}

/* Set disable */
void menu :: set_disable(int id,int d)
{
	int *dis;
	text *tx;
	int i;
	/* Get */
	dis = (int*)wcarray_get(dls,id);
	/* No change */
	if(*dis == d)
		return;
	/* Disable */
	if(d)
	{
		/* Disable and color disable */
		*dis = 1;
		tx = (text*)wcarray_get(ls,id);
		tx->set_color(3);
		return;
	}
	/* Set flag */
	*dis = 0;
	/* Else enable. First, wipe out old list */
	for(i = 0;i < wcarray_size(ls);i++)
		delete ((text*)wcarray_get(ls,i));
	wcarray_delete(ls);
	/* Add back new list */
	ls = wcarray_new();
	for(i = 0;i < wcarray_size(sls);i++)
		wcarray_add(ls,new text((char*)wcarray_get(sls,i)));
	/* Check disables */
	for(i = 0;i < wcarray_size(dls);i++)
	{
		dis = (int*)wcarray_get(dls,i);
		if(*dis)
		{
			/* Disable and color disable */
			tx = (text*)wcarray_get(ls,i);
			tx->set_color(3);
		}
	}
}

/* Get selection */
int menu :: get_selection()
{
	/* If accepted then get selection */
	if(accepted)
		return selected;
	/* Otherwise, user has not decided yet */
	return -1;
}

/* Peek selection */
int menu :: peek_selection()
{
	return selected;
}

/* Get size */
int menu :: size()
{
	return options;
}

/* Generates title menu */
menu *wcmenu_generate_title()
{
	menu *mn;
	char buffer[128];
	if(!wcsave_has_game_saves())
		mn = new menu(WCMENU_MAIN_WIDTH,3);
	else
		mn = new menu(WCMENU_MAIN_WIDTH,4);
	if(wcsave_has_game_saves())
	{
		sprintf(buffer,"(%s %d)",wcvocab_get(148),wcsave_get_last()+1);
		mn->add(wcrpg_string_clone(buffer));
	}
	mn->add(wcrpg_string_clone(wcvocab_get(114)));
	mn->add(wcrpg_string_clone(wcvocab_get(115)));
	mn->add(wcrpg_string_clone(wcvocab_get(117)));
	if(!wcsave_has_game_saves())
		mn->set_disable(1,1);
	return mn;
}

/* Generates main menu */
menu *wcmenu_generate_main()
{
	menu *mn;
	int i;
	mn = new menu(WCMENU_MAIN_WIDTH,wcsystem_menu_option_count()+1);
	/* Add all */
	for(i = 0;i < wcsystem_menu_option_count();i++)
		mn->add(wcvocab_get(wcsystem_menu_option_to_vocab(wcsystem_menu_option(i))));
	mn->add(wcvocab_get(112));
	return mn;
}