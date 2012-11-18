/*
	Menu for qutting
*/

/* Include */
#include "wcrpg.h"
#include "wcquitmenu.h"
#include "wcmanage.h"
#include "wcsystem.h"
#include "wcinput.h"
#include "wcwindow.h"
#include "wcvocab.h"

/* Globals */
menu *wcquitmenu_link = 0;
int wcquitmenu_auto_fire = 0;
int wcquitmenu_on = 0;
float wcquitmenu_slide = 0.0f;
text *wcquitmenu_msg = 0;
menu *wcquitmenu_menu = 0;

/* Open quit menu */
void wcquitmenu_open(menu *m)
{
	wcquitmenu_link = m;
	wcquitmenu_auto_fire = 1;
	wcquitmenu_on = 1;
	if(!wcquitmenu_msg)
		wcquitmenu_msg = new text(wcvocab_get(151));
	if(!wcquitmenu_menu)
	{
		wcquitmenu_menu = new menu(80,2);
		wcquitmenu_menu->add(wcvocab_get(152));
		wcquitmenu_menu->add(wcvocab_get(153));
	}
	wcquitmenu_menu->activate();
}

/* Close item menu */
void wcquitmenu_close()
{
	wcquitmenu_on = 0;
	if(wcquitmenu_link)
		wcquitmenu_link->activate();
	wcmanage_return();
}

/* Active? */
int wcquitmenu_active()
{
	return wcquitmenu_on;
}

/* Handle */
void wcquitmenu_handle()
{
	float ts;
	/* Slide */
	ts = wcrpg_get_time_step();
	if(!wcquitmenu_active())
	{
		wcquitmenu_slide += (-wcquitmenu_slide)/2.0f*ts;
		return;
	}
	wcquitmenu_slide += (1.005f-wcquitmenu_slide)/2.0f*ts;
	if(wcquitmenu_menu)
	{
		wcquitmenu_menu->handle();
		if(wcquitmenu_menu->get_selection() == 0)
			wcrpg_exit();
		if(wcquitmenu_menu->get_selection() == 1)
			wcquitmenu_close();
	}
	/* Close it */
	if(wcinput_cancel() && !wcquitmenu_auto_fire)
	{
		wcsystem_play_sound(WCSYSTEM_CANCEL);
		wcquitmenu_close();
		wcquitmenu_auto_fire = 1;
	}
	/* Close */
	if(!wcinput_cancel())
		wcquitmenu_auto_fire = 0;
}

/* Draw */
void wcquitmenu_draw()
{
	wcwindow_draw(0,-32+(int)(32*wcquitmenu_slide),WCRPG_WIDTH,32);
	if(wcquitmenu_msg)
	{
		wcquitmenu_msg->move(8,-32+(int)(32*wcquitmenu_slide)+8);
		wcquitmenu_msg->draw();
	}
	if(wcquitmenu_menu && wcquitmenu_active())
		wcquitmenu_menu->draw(WCRPG_WIDTH/2-40,WCRPG_HEIGHT/2-24);
}