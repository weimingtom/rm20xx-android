/*
	Status Menu
*/

/* Includes */
#include <stdio.h>
#include "wcstatusmenu.h"
#include "wcitemmenu.h"
#include "wcskillmenu.h"
#include "wcmanage.h"
#include "wcitem.h"
#include "wcinput.h"
#include "wcsystem.h"
#include "wcwindow.h"
#include "wcheroplate.h"
#include "wcgearplate.h"
#include "wcparty.h"
#include "wcrpg.h"
#include "wchero.h"
#include "wcvocab.h"

/* Globals */
menu *wcstatusmenu_link = 0;
int wcstatusmenu_auto_fire = 0;
int wcstatusmenu_on = 0;
int wcstatusmenu_focus = 0;
int wcstatusmenu_view = 0;
int wcstatusmenu_stage = 0;
float wcstatusmenu_slide = 0.0f;
float wcstatusmenu_slide_beta = 0.0f;
text *wcstatusmenu_health_label = 0;
text *wcstatusmenu_mana_label = 0;
text *wcstatusmenu_exp_label = 0;
text *wcstatusmenu_atk_label = 0;
text *wcstatusmenu_def_label = 0;
text *wcstatusmenu_pow_label = 0;
text *wcstatusmenu_spd_label = 0;
int wcstatusmenu_equip_mode = 0;
int wcstatusmenu_equip_focus = 0;
int wcstatusmenu_removal_mode = 0;
int wcstatusmenu_skill_mode = 0;
int wcstatusmenu_skill_focus = 0;

/* Init text */
void wcstatusmenu_init()
{
	/* Make labels */
	wcstatusmenu_health_label = new text(wcvocab_get(124));
	wcstatusmenu_health_label->set_color(1);
	wcstatusmenu_mana_label = new text(wcvocab_get(125));
	wcstatusmenu_mana_label->set_color(1);
	wcstatusmenu_exp_label = new text(wcvocab_get(127));
	wcstatusmenu_exp_label->set_color(1);
	wcstatusmenu_atk_label = new text(wcvocab_get(132));
	wcstatusmenu_atk_label->set_color(1);
	wcstatusmenu_def_label = new text(wcvocab_get(133));
	wcstatusmenu_def_label->set_color(1);
	wcstatusmenu_pow_label = new text(wcvocab_get(134));
	wcstatusmenu_pow_label->set_color(1);
	wcstatusmenu_spd_label = new text(wcvocab_get(135));
	wcstatusmenu_spd_label->set_color(1);
}

/* Exit */
void wcstatusmenu_exit()
{
	delete wcstatusmenu_health_label;
	delete wcstatusmenu_mana_label;
	delete wcstatusmenu_exp_label;
	delete wcstatusmenu_atk_label;
	delete wcstatusmenu_def_label;
	delete wcstatusmenu_pow_label;
	delete wcstatusmenu_spd_label;
}

/* Open */
void wcstatusmenu_open(menu *m)
{
	wchero_refresh();
	wcstatusmenu_link = m;
	wcstatusmenu_auto_fire = 1;
	wcstatusmenu_on = 1;
	wcstatusmenu_stage = 0;
	wcstatusmenu_focus = 0;
	wcstatusmenu_equip_mode = 0;
	wcstatusmenu_skill_mode = 0;
}

/* Open equipment menu */
void wcstatusmenu_open_equip(menu *m)
{
	wcstatusmenu_open(m);
	wcstatusmenu_equip_mode = 1;
	wcstatusmenu_skill_mode = 0;
}

/* Open skill menu */
void wcstatusmenu_open_skill(menu *m)
{
	wcstatusmenu_open(m);
	wcstatusmenu_skill_mode = 1;
	wcstatusmenu_equip_mode = 0;
}

/* Close */
void wcstatusmenu_close()
{
	wcstatusmenu_on = 0;
	if(wcstatusmenu_link)
		wcstatusmenu_link->activate();
	wcmanage_return();
}

/* Active */
int wcstatusmenu_active()
{
	return wcstatusmenu_on;
}

/* Handle */
void wcstatusmenu_handle()
{
	float ts;
	hero *hr;
	ts = wcrpg_get_time_step();
	if(!wcstatusmenu_active())
	{
		wcstatusmenu_slide += (-wcstatusmenu_slide)/2.0f*ts;
		wcstatusmenu_slide_beta += (-wcstatusmenu_slide_beta)/2.0f*ts;
		return;
	}
	/* Move focus */
	if(wcstatusmenu_stage == 0)
	{
		wcstatusmenu_slide += (-wcstatusmenu_slide)/2.0f*ts;
		wcstatusmenu_slide_beta += (-wcstatusmenu_slide_beta)/2.0f*ts;
		if(wcinput_y() < 0 && !wcstatusmenu_auto_fire && wcstatusmenu_focus > 0)
		{
			wcsystem_play_sound(WCSYSTEM_CURSOR);
			wcstatusmenu_focus--;
			wcstatusmenu_auto_fire = 1;
		}
		if(wcinput_y() > 0 && !wcstatusmenu_auto_fire && wcstatusmenu_focus < wcparty_size()-1)
		{
			wcsystem_play_sound(WCSYSTEM_CURSOR);
			wcstatusmenu_focus++;
			wcstatusmenu_auto_fire = 1;
		}
		/* Autoscroll */
		if(wcstatusmenu_focus-wcstatusmenu_view > 3)
			wcstatusmenu_view++;
		if(wcstatusmenu_focus-wcstatusmenu_view < 0)
			wcstatusmenu_view--;
		wcmanage_scroll_hero_plate(56*wcstatusmenu_view);
		/* View */
		if(wcinput_ok() && !wcstatusmenu_auto_fire)
		{
			/* Open skill menu */
			if(wcstatusmenu_skill_mode)
				wcskillmenu_open((hero*)wchero_get(wcparty_get(wcstatusmenu_focus)));
			wcsystem_play_sound(WCSYSTEM_DECIDE);
			wcstatusmenu_auto_fire = 1;
			wcstatusmenu_stage++;
		}
		/* Close it */
		if(wcinput_cancel() && !wcstatusmenu_auto_fire)
		{
			wcsystem_play_sound(WCSYSTEM_CANCEL);
			wcstatusmenu_close();
			wcstatusmenu_auto_fire = 1;
		}
	}
	/* See */
	if(wcstatusmenu_stage == 1)
	{
		wcstatusmenu_slide += (1.005f-wcstatusmenu_slide)/2.0f*ts;
		wcstatusmenu_slide_beta += (-wcstatusmenu_slide_beta)/2.0f*ts;
		/* Close it */
		if(wcinput_cancel() && !wcstatusmenu_auto_fire)
		{
			/* Close skill menu */
			if(wcstatusmenu_skill_mode)
				wcskillmenu_close();
			wcsystem_play_sound(WCSYSTEM_CANCEL);
			if(!wcstatusmenu_link)
				wcmanage_return();
			wcstatusmenu_stage--;
			wcstatusmenu_auto_fire = 1;
		}
		/* Allow the selection of equipment in equipment mode */
		if(wcstatusmenu_equip_mode)
		{
			/* Pick a slot */
			if(wcinput_y() < 0 && !wcstatusmenu_auto_fire && wcstatusmenu_equip_focus > 0)
			{
				wcsystem_play_sound(WCSYSTEM_CURSOR);
				wcstatusmenu_auto_fire = 1;
				wcstatusmenu_equip_focus--;
			}
			if(wcinput_y() > 0 && !wcstatusmenu_auto_fire && wcstatusmenu_equip_focus < 4)
			{
				wcsystem_play_sound(WCSYSTEM_CURSOR);
				wcstatusmenu_auto_fire = 1;
				wcstatusmenu_equip_focus++;
			}
			/* Toggle remover mode */
			if(wcinput_x() != 0 && !wcstatusmenu_auto_fire)
			{
				wcsystem_play_sound(WCSYSTEM_CURSOR);
				wcstatusmenu_removal_mode++;
				wcstatusmenu_removal_mode = wcstatusmenu_removal_mode%2;
				wcstatusmenu_auto_fire = 1;
			}
			/* Go */
			if(wcinput_ok() && !wcstatusmenu_auto_fire)
			{
				wcstatusmenu_auto_fire = 1;
				if(!wcstatusmenu_removal_mode)
				{
					/* Adding equip */
					wcsystem_play_sound(WCSYSTEM_DECIDE);
					wcitemmenu_set_personal_filter(wcparty_get(wcstatusmenu_focus),wcstatusmenu_equip_focus+WCITEM_WEAPON);
					wcitemmenu_open(0);
					wcstatusmenu_stage++;
				}
				else
				{
					/* Removing equip */
					hr = (hero*)wchero_get(wcparty_get(wcstatusmenu_focus));
					if(hr->get_equip(wcstatusmenu_equip_focus))
					{
						wcsystem_play_sound(WCSYSTEM_DECIDE);
						hr->unequip_item_slot(wcstatusmenu_equip_focus);
						wcstatusmenu_removal_mode = 0;
					}
					else
					{
						wcsystem_play_sound(WCSYSTEM_BUZZER);
					}
				}
			}
		}
	}
	/* Browse and select item */
	if(wcstatusmenu_stage == 2)
	{
		wcstatusmenu_slide += (1.005f-wcstatusmenu_slide)/2.0f*ts;
		wcstatusmenu_slide_beta += (1.005f-wcstatusmenu_slide_beta)/2.0f*ts;
		/* Close it */
		if((wcinput_cancel() && !wcstatusmenu_auto_fire) || !wcitemmenu_active())
		{
			if(!wcstatusmenu_link)
				wcmanage_return();
			wcstatusmenu_stage--;
			wcstatusmenu_auto_fire = 1;
		}
	}
	/* Close */
	if(!wcinput_cancel() && !wcinput_y() && !wcinput_ok() && !wcinput_x())
		wcstatusmenu_auto_fire = 0;
}

/* Draw typical number stats */
void wcstatusmenu_typical(hero *hr,int bx,int by)
{
	char buffer[32];
	wcwindow_draw(bx,by,128,64);
	wcstatusmenu_health_label->move(bx+8,by+8);
	wcstatusmenu_health_label->draw();
	wcstatusmenu_mana_label->move(bx+8,by+8+16);
	wcstatusmenu_mana_label->draw();
	wcstatusmenu_exp_label->move(bx+8,by+8+32);
	wcstatusmenu_exp_label->draw();
	sprintf(buffer,"%d",hr->get_health());
	wcwindow_draw_text(bx+48,by+8,0,buffer);
	wcwindow_draw_text(bx+48+28,by+8,0,"/");
	sprintf(buffer,"%d",hr->get_stat(STAT_HEALTH));
	wcwindow_draw_text(bx+48+32+8,by+8,0,buffer);
	sprintf(buffer,"%d",hr->get_mana());
	wcwindow_draw_text(bx+48,by+8+16,0,buffer);
	wcwindow_draw_text(bx+48+28,by+8+16,0,"/");
	sprintf(buffer,"%d",hr->get_stat(STAT_MANA));
	wcwindow_draw_text(bx+48+32+8,by+8+16,0,buffer);
	sprintf(buffer,"%d",hr->get_exp());
	wcwindow_draw_text(bx+48,by+8+32,0,buffer);
	/*wcwindow_draw_text(bx+48+28,by+8+32,0,"/");
	sprintf_s(buffer,sizeof(buffer),"%d",hr->get_exp_for_next());
	wcwindow_draw_text(bx+48+32+8,by+8+32,0,buffer);*/
}

/* Draw title and possibly class */
void wcstatusmenu_title(hero *hr,int bx,int by)
{
	if(!hr->get_title_text())
		return;
	wcwindow_draw(bx,by,64+32,32);
	hr->get_title_text()->move(bx+8,by+8);
	hr->get_title_text()->draw();
}

/* Actual stats */
void wcstatusmenu_stats(hero *hr,int bx,int by)
{
	char buffer[32];
	wcwindow_draw(bx,by,128,80);
	/* Labels */
	wcstatusmenu_atk_label->move(bx+8,by+8);
	wcstatusmenu_atk_label->draw();
	wcstatusmenu_def_label->move(bx+8,by+8+16);
	wcstatusmenu_def_label->draw();
	wcstatusmenu_pow_label->move(bx+8,by+8+32);
	wcstatusmenu_pow_label->draw();
	wcstatusmenu_spd_label->move(bx+8,by+8+48);
	wcstatusmenu_spd_label->draw();
	/* Stats */
	sprintf(buffer,"%d",hr->get_stat(STAT_ATTACK));
	wcwindow_draw_text(bx+8+80,by+8,0,buffer);
	sprintf(buffer,"%d",hr->get_stat(STAT_DEFENSE));
	wcwindow_draw_text(bx+8+80,by+8+16,0,buffer);
	sprintf(buffer,"%d",hr->get_stat(STAT_POWER));
	wcwindow_draw_text(bx+8+80,by+8+32,0,buffer);
	sprintf(buffer,"%d",hr->get_stat(STAT_SPEED));
	wcwindow_draw_text(bx+8+80,by+8+48,0,buffer);
}

/* Projected stats */
void wcstatusmenu_projected_stats(hero *hr,item *rit,item *ait,int bx,int by)
{
	char buffer[32];
	wcwindow_draw(bx,by,128,80);
	int proj_attack,proj_defense,proj_special,proj_speed;
	int color_attack,color_defense,color_special,color_speed;
	/* First, get original stats */
	proj_attack = hr->get_stat(STAT_ATTACK);
	proj_defense = hr->get_stat(STAT_DEFENSE);
	proj_special = hr->get_stat(STAT_POWER);
	proj_speed = hr->get_stat(STAT_SPEED);
	/* Defaulted colors */
	color_attack = 0;
	color_defense = 0;
	color_special = 0;
	color_speed = 0;
	/* Equipment to remove? */
	if(rit)
	{
		/* Remove bonuses */
		proj_attack -= rit->get_attack_power();
		proj_defense -= rit->get_defense_power();
		proj_special -= rit->get_special_power();
		proj_speed -= rit->get_speed_power();
	}
	/* Now add bonuses */
	if(ait)
	{
		proj_attack += ait->get_attack_power();
		proj_defense += ait->get_defense_power();
		proj_special += ait->get_special_power();
		proj_speed += ait->get_speed_power();
	}
	/* Color things */
	if(proj_attack > hr->get_stat(STAT_ATTACK))
		color_attack = 2;
	if(proj_attack < hr->get_stat(STAT_ATTACK))
		color_attack = 3;
	if(proj_defense > hr->get_stat(STAT_DEFENSE))
		color_defense = 2;
	if(proj_defense < hr->get_stat(STAT_DEFENSE))
		color_defense = 3;
	if(proj_special > hr->get_stat(STAT_POWER))
		color_special = 2;
	if(proj_special < hr->get_stat(STAT_POWER))
		color_special = 3;
	if(proj_speed > hr->get_stat(STAT_SPEED))
		color_speed = 2;
	if(proj_speed < hr->get_stat(STAT_SPEED))
		color_speed = 3;
	/* Labels */
	wcstatusmenu_atk_label->move(bx+8,by+8);
	wcstatusmenu_atk_label->draw();
	wcstatusmenu_def_label->move(bx+8,by+8+16);
	wcstatusmenu_def_label->draw();
	wcstatusmenu_pow_label->move(bx+8,by+8+32);
	wcstatusmenu_pow_label->draw();
	wcstatusmenu_spd_label->move(bx+8,by+8+48);
	wcstatusmenu_spd_label->draw();
	/* Stats */
	sprintf(buffer,"%d",proj_attack);
	wcwindow_draw_text(bx+8+80,by+8,color_attack,buffer);
	sprintf(buffer,"%d",proj_defense);
	wcwindow_draw_text(bx+8+80,by+8+16,color_defense,buffer);
	sprintf(buffer,"%d",proj_special);
	wcwindow_draw_text(bx+8+80,by+8+32,color_special,buffer);
	sprintf(buffer,"%d",proj_speed);
	wcwindow_draw_text(bx+8+80,by+8+48,color_speed,buffer);
}

/* Draw */
void wcstatusmenu_draw()
{
	int wid,ritid;
	texture *tex;
	hero *hr;
	wid = 320-WCMENU_MAIN_WIDTH;
	item *rit;
	/* End here if not drawing */
	if(!wcstatusmenu_active())
		return;
	/* Select a hero */
	wcwindow_draw_cursor(WCRPG_WIDTH-wid+8-2,8+wcstatusmenu_focus*56-wcmanage_get_hero_scroll_pos()-2,wid-16+4,56,(wcstatusmenu_stage == 0));
	wcheroplate_draw_party_entry(WCRPG_WIDTH-wid+8+4-2,8+wcstatusmenu_focus*56-wcmanage_get_hero_scroll_pos()+4-2,wcstatusmenu_focus);
	/* Draw face plate if not viewing skills */
	if(!wcstatusmenu_skill_mode)
	{
		tex = wcparty_get_face(wcstatusmenu_focus);
		hr = wchero_get(wcparty_get(wcstatusmenu_focus));
		wcwindow_draw(0,-64+(int)(64*wcstatusmenu_slide),64,64);
		wcwindow_draw_face(tex,hr->get_faceset_number(),8,-64+(int)(64*wcstatusmenu_slide)+8);
	}
	/* Display stats */
	if(!wcstatusmenu_equip_mode && !wcstatusmenu_skill_mode)
	{
		/* Draw name plate */
		wcwindow_draw(-(64+32)+(int)((64+32)*wcstatusmenu_slide),64,64+32,32);
		if(hr->get_name_text())
		{
			hr->get_name_text()->move(-(64+32)+(int)((64+32)*wcstatusmenu_slide)+8,64+8);
			hr->get_name_text()->draw();
		}
		/* Draw arrows? */
		if(wcstatusmenu_view > 0)
			wcwindow_draw_arrow(WCRPG_WIDTH-wid+8+wid/2-8,0,0);
		if(wcstatusmenu_view < wcparty_size()-4)
			wcwindow_draw_arrow(WCRPG_WIDTH-wid+8+wid/2-8,240-8,1);
		/* Draw typcial status */
		wcstatusmenu_typical(hr,64+(WCRPG_WIDTH-64)-(int)((WCRPG_WIDTH-64)*wcstatusmenu_slide),0);
		/* Draw title plate(s) */
		wcstatusmenu_title(hr,64+32,WCRPG_HEIGHT-(int)((WCRPG_HEIGHT-64)*wcstatusmenu_slide));
		/* Draw stats */
		wcstatusmenu_stats(hr,-128+(int)(128*wcstatusmenu_slide),64+32);
		/* Draw equipment */
		wcgearplate_draw(WCRPG_WIDTH-128+(int)(128*(1.0f-wcstatusmenu_slide)),64,wcparty_get(wcstatusmenu_focus),0,0);
	}
	if(wcstatusmenu_equip_mode)
	{
		/* Draw equipment */
		wcgearplate_draw(-128+(int)(128*wcstatusmenu_slide),64+48+32,wcparty_get(wcstatusmenu_focus),wcstatusmenu_equip_focus+1,wcstatusmenu_removal_mode);
		/* Decide if projected or standard stats will show */
		if(!wcitemmenu_active() && !wcstatusmenu_removal_mode)
		{
			/* If item menu is closed, always standard */
			wcstatusmenu_stats(hr,-128+(int)(128*wcstatusmenu_slide),64-(int)(32*wcstatusmenu_slide_beta));
		}
		/* Show projections of equipping items */
		if(wcitemmenu_active() && !wcstatusmenu_removal_mode)
		{
			/* Get removing item */
			ritid = hr->get_equip(wcstatusmenu_equip_focus);
			rit = 0;
			if(ritid)
				rit = (item*)wcitem_get(ritid);
			/* Display */
			wcstatusmenu_projected_stats(hr,rit,wcitemmenu_get_selected(),-128+(int)(128*wcstatusmenu_slide),64-(int)(32*wcstatusmenu_slide_beta));
			wcgearplate_draw_extra(-128+(int)(128*wcstatusmenu_slide),64-(int)(32*wcstatusmenu_slide_beta)+80,wcitemmenu_get_selected());
		}
		/* Show projections of removing items */
		if(!wcitemmenu_active() && wcstatusmenu_removal_mode)
		{
			/* Get removing item */
			ritid = hr->get_equip(wcstatusmenu_equip_focus);
			rit = 0;
			if(ritid)
				rit = (item*)wcitem_get(ritid);
			/* Display */
			wcstatusmenu_projected_stats(hr,rit,0,-128+(int)(128*wcstatusmenu_slide),64-(int)(32*wcstatusmenu_slide_beta));
		}
	}
}