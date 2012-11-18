/*
	Skill menu
*/

/* Includes */
#include "wcskillmenu.h"
#include "wcrpg.h"

/* Globals */
hero *wcskillmenu_focus = 0;
menu *wcskillmenu_menu = 0;
int wcskillmenu_is_open = 0;
text *wcskillmenu_desc_text = 0;
float wcskillmenu_scroll = 0.0f;
float wcskillmenu_slide = 0.0f;
int wcskillmenu_last_select = 0;

/* Refresh description */
void wcskillmenu_refresh()
{
	skill *sk;
	int ski;
	/* If menu */
	if(!wcskillmenu_menu)
		return;
	/* Delete old text */
	if(wcskillmenu_desc_text)
		delete wcskillmenu_desc_text;
	/* If not empty */
	if(wcskillmenu_focus->get_skill_count() <= 0)
	{
		wcskillmenu_desc_text = new text("");
		return;
	}
	/* Find out which item is highlighted */
	ski = wcskillmenu_menu->peek_selection();
	/* Get the skill */
	sk = (skill*)wcskill_get(wcskillmenu_focus->get_skill(ski));
	/* Then make the text */
	wcskillmenu_desc_text = new text(sk->get_description());
	wcskillmenu_desc_text->advance(0,0);
	wcskillmenu_scroll = 0.0f;
}

/* Update description */
void wcskillmenu_update()
{
	if(!wcskillmenu_menu)
		return;
	/* Refresh when different */
	if(wcskillmenu_menu->peek_selection() != wcskillmenu_last_select)
	{
		wcskillmenu_refresh();
		wcskillmenu_last_select = wcskillmenu_menu->peek_selection();
	}
}

/* Opens the skill menu */
void wcskillmenu_open(hero *hr)
{
	int os;
	int i;
	skill *sk;
	wcskillmenu_focus = hr;
	/* Delete old */
	if(wcskillmenu_menu)
		delete wcskillmenu_menu;
	/* Measure height of menu */
	if(hr->get_skill_count() > WCSKILLMENU_MAX_VISIBLE)
		os = WCSKILLMENU_MAX_VISIBLE;
	else
		os = hr->get_skill_count();
	/* Make the menu */
	wcskillmenu_menu = new menu(WCSKILLMENU_WIDTH,os);
	/* Add stuff to the menu */
	for(i = 0;i < hr->get_skill_count();i++)
	{
		sk = wcskill_get(hr->get_skill(i));
		wcskillmenu_menu->add(wcrpg_string_clone(sk->get_name()),sk->get_mp_need());
	}
	/* Disable everything */
	for(i = 0;i < hr->get_skill_count();i++)
		wcskillmenu_menu->set_disable(i,1);
	/* Open up */
	wcskillmenu_menu->activate();
	wcskillmenu_is_open = 1;
	/* Refresh */
	wcskillmenu_refresh();
}

/* Closes the skill menu */
void wcskillmenu_close()
{
	wcskillmenu_menu->close();
	wcskillmenu_is_open = 0;
}

/* Handles skill menu */
void wcskillmenu_handle()
{
	float ts;
	ts = wcrpg_get_time_step();
	/* Handle menu */
	if(wcskillmenu_menu)
		wcskillmenu_menu->handle();
	if(!wcskillmenu_is_open)
	{
		wcskillmenu_slide += (-wcskillmenu_slide)/2.0f*ts;
		return;
	}
	wcskillmenu_slide += (1.0f-wcskillmenu_slide)/2.0f*ts;
	/* Scroll text */
	wcskillmenu_scroll += 3.0f*ts;
	if(wcskillmenu_desc_text)
		wcskillmenu_desc_text->advance((int)wcskillmenu_scroll,0);
	/* Update */
	wcskillmenu_update();
}

/* Draws skill menu */
void wcskillmenu_draw()
{
	/* Draw it if open */
	if(wcskillmenu_menu)
		wcskillmenu_menu->draw(WCRPG_WIDTH-WCSKILLMENU_WIDTH,32);
	/* Always drawing description window */
	wcwindow_draw(0,-32+(int)(32*wcskillmenu_slide),WCRPG_WIDTH,32);
	if(!wcskillmenu_is_open)
		return;
	/* Draw description */
	if(wcskillmenu_desc_text)
	{
		wcskillmenu_desc_text->move(8,8-32+(int)(32*wcskillmenu_slide));
		wcskillmenu_desc_text->draw();
	}
}