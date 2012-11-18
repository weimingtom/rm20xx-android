/*
	Item menu
*/

/* Includes */
#include "wcitemmenu.h"
#include "wcmanage.h"
#include "wcinput.h"
#include "wcsystem.h"
#include "wcrpg.h"
#include "wcwindow.h"

/* Globals */
menu *wcitemmenu_link = 0;
int wcitemmenu_auto_fire = 0;
int wcitemmenu_on = 0;
float wcitemmenu_slide = 0.0f;
menu *wcitemmenu_menu = 0;
int wcitemmenu_last_selection = -1;
text *wcitemmenu_desc = 0;
arraylist wcitemmenu_ls = 0;
float wcitemmenu_scroll = 0.0f;
int wcitemmenu_filter_mode = 0;
int wcitemmenu_hero_id = 0;
int wcitemmenu_equip_type = 0;

/* Set personal filter */
void wcitemmenu_set_personal_filter(int id,int t)
{
	wcitemmenu_filter_mode = WCITEMMENU_PERSONAL;
	wcitemmenu_hero_id = id;
	wcitemmenu_equip_type = t;
}

/* Update desc */
void wcitemmenu_update()
{
	item *it;
	/* Menu must exist */
	if(wcitemmenu_menu)
	{
		/* Must have enough items */
		if(wcarray_size(wcitemmenu_ls) > 0)
		{
			/* Different? */
			if(wcitemmenu_last_selection != wcitemmenu_menu->peek_selection())
			{
				/* Update */
				wcitemmenu_last_selection = wcitemmenu_menu->peek_selection();
				if(wcitemmenu_desc)
					delete wcitemmenu_desc;
				/* Make desc */
				it = (item*)wcarray_get(wcitemmenu_ls,wcitemmenu_menu->peek_selection());
				if(it->get_description())
				{
					wcitemmenu_desc = new text(it->get_description());
					wcitemmenu_desc->advance(0,0);
				}
				else
				{
					wcitemmenu_desc = 0;
				}
				wcitemmenu_scroll = 0.0f;
			}
		}
	}
}

/* Item menu filter */
int wcitemmenu_filter(void *v)
{
	item *it;
	/* You must have this item first! */
	it = (item*)v;
	if(!it->get_held())
		return 0;
	/* Let everything */
	if(wcitemmenu_filter_mode == WCITEMMENU_ALL)
		return 1;
	/* Items */
	if(wcitemmenu_filter_mode == WCITEMMENU_MEDICINE)
	{
		if(it->get_type() == WCITEM_MEDICINE)
			return 1;
	}
	/* Equips */
	if(wcitemmenu_filter_mode == WCITEMMENU_EQUIP)
	{
		if(it->get_type() == WCITEM_WEAPON)
			return 1;
		if(it->get_type() == WCITEM_SHIELD)
			return 1;
		if(it->get_type() == WCITEM_ARMOR)
			return 1;
		if(it->get_type() == WCITEM_HELMET)
			return 1;
		if(it->get_type() == WCITEM_ACC)
			return 1;
	}
	/* Special */
	if(wcitemmenu_filter_mode == WCITEMMENU_SPECIAL)
	{
		if(it->get_type() == WCITEM_SKILL)
			return 1;
		if(it->get_type() == WCITEM_SEED)
			return 1;
		if(it->get_type() == WCITEM_SCROLL)
			return 1;
	}
	/* Key */
	if(wcitemmenu_filter_mode == WCITEMMENU_ETC)
	{
		if(it->get_type() == WCITEM_COMMON)
			return 1;
		if(it->get_type() == WCITEM_SWITCH)
			return 1;
	}
	/* Personal */
	if(wcitemmenu_filter_mode == WCITEMMENU_PERSONAL)
	{
		if(it->get_type() == wcitemmenu_equip_type && it->can_use(wcitemmenu_hero_id))
			return 1;
	}
	return 0;
}

/* Open item menu */
void wcitemmenu_open(menu *m)
{
	wcitemmenu_refresh();
	wcitemmenu_link = m;
	wcitemmenu_auto_fire = 1;
	wcitemmenu_on = 1;
}

/* Close item menu */
void wcitemmenu_close()
{
	/* Delete */
	if(wcitemmenu_menu)
		delete wcitemmenu_menu;
	if(wcitemmenu_ls)
		wcarray_delete(wcitemmenu_ls);
	if(wcitemmenu_desc)
		delete wcitemmenu_desc;
	wcitemmenu_filter_mode = WCITEMMENU_ALL;
	wcitemmenu_desc = 0;
	wcitemmenu_ls = 0;
	wcitemmenu_menu = 0;
	/* Close */
	wcitemmenu_on = 0;
	if(wcitemmenu_link)
		wcitemmenu_link->activate();
	wcmanage_return();
}

/* Active? */
int wcitemmenu_active()
{
	return wcitemmenu_on;
}

/* Handle */
void wcitemmenu_handle()
{
	float ts;
	item *it;
	ts = wcrpg_get_time_step();
	if(!wcitemmenu_active())
	{
		wcitemmenu_slide += (-wcitemmenu_slide)/2.0f*ts;
		return;
	}
	wcitemmenu_slide += (1.005f-wcitemmenu_slide)/2.0f*ts;
	wcitemmenu_scroll += ts*3.0f;
	if(wcitemmenu_desc)
		wcitemmenu_desc->advance((int)wcitemmenu_scroll,0);
	wcitemmenu_menu->handle();
	wcitemmenu_update();
	/* Close it */
	if(wcinput_cancel() && !wcitemmenu_auto_fire)
	{
		wcsystem_play_sound(WCSYSTEM_CANCEL);
		wcitemmenu_close();
		wcitemmenu_auto_fire = 1;
	}
	else
	{
		/* Cannot switch filter if in personal mode */
		if(wcitemmenu_filter_mode != WCITEMMENU_PERSONAL)
		{
			/* Switch filter */
			if(wcinput_x() > 0 && !wcitemmenu_auto_fire && wcitemmenu_filter_mode < 4)
			{
				wcsystem_play_sound(WCSYSTEM_CURSOR);
				wcitemmenu_filter_mode++;
				wcitemmenu_refresh();
				wcitemmenu_auto_fire = 1;
			}
			if(wcinput_x() < 0 && !wcitemmenu_auto_fire && wcitemmenu_filter_mode > 0)
			{
				wcsystem_play_sound(WCSYSTEM_CURSOR);
				wcitemmenu_filter_mode--;
				wcitemmenu_refresh();
				wcitemmenu_auto_fire = 1;
			}
		}
		/* Accepted */
		if(wcitemmenu_menu->get_selection() != -1)
		{
			/* Item chosen */
			it = (item*)wcarray_get(wcitemmenu_ls,wcitemmenu_menu->get_selection());
			/* Use up that item */
			if(wcitemmenu_filter_mode != WCITEMMENU_PERSONAL)
				it->use(0);
			else
				it->use((hero*)wchero_get(wcitemmenu_hero_id));
			wcitemmenu_close();
			if(wcitemmenu_link)
				wcmanage_close();
			wcitemmenu_auto_fire = 1;
		}
	}
	/* Close it */
	if(!wcinput_cancel() && !wcinput_x())
		wcitemmenu_auto_fire = 0;
}

/* Draw */
void wcitemmenu_draw()
{
	/* Draw the desc window */
	wcwindow_draw(0,-32+(int)(wcitemmenu_slide*32),WCRPG_WIDTH,32);
	if(wcitemmenu_desc)
	{
		wcitemmenu_desc->move(8,-32+(int)(wcitemmenu_slide*32)+8);
		wcitemmenu_desc->draw();
	}
	/* Draw the menu */
	if(wcitemmenu_menu)
		wcitemmenu_menu->draw(WCRPG_WIDTH-WCITEMMENU_WIDTH,32);
}

/* Refresh */
void wcitemmenu_refresh()
{
	arraylist ls;
	int i;
	item *it;
	/* Refresh desc */
	wcitemmenu_last_selection = -1;
	/* Delete old */
	if(wcitemmenu_ls)
		wcarray_delete(wcitemmenu_ls);
	if(wcitemmenu_menu)
		delete wcitemmenu_menu;
	/* Get list */
	ls = wcitem_filter(wcitemmenu_filter);
	/* Make menu */
	if(wcarray_size(ls) < WCITEMMENU_MAX_VISIBLE)
		wcitemmenu_menu = new menu(WCITEMMENU_WIDTH,wcarray_size(ls));
	else
		wcitemmenu_menu = new menu(WCITEMMENU_WIDTH,WCITEMMENU_MAX_VISIBLE);
	for(i = 0;i < wcarray_size(ls);i++)
	{
		it = (item*)wcarray_get(ls,i);
		wcitemmenu_menu->add(wcrpg_string_clone(it->get_name()),it->get_held());
	}
	/* Filter by usabillity */
	if(wcitemmenu_filter_mode != WCITEMMENU_PERSONAL)
		wcitem_enable_field_usable_items(ls,wcitemmenu_menu);
	else
		wcitem_enable_all(ls,wcitemmenu_menu);
	/* Activate menu */
	wcitemmenu_menu->activate();
	wcitemmenu_ls = ls;
}

/* Get highlight item */
item *wcitemmenu_get_selected()
{
	/* Null if no list */
	if(wcarray_size(wcitemmenu_ls) <= 0)
		return 0;
	return (item*)wcarray_get(wcitemmenu_ls,wcitemmenu_menu->peek_selection());
}