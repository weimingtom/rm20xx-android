/*
	Hero plates
*/

/* Includes */
#include <stdio.h>
#include "wcheroplate.h"
#include "wchero.h"
#include "wcparty.h"
#include "wcwindow.h"
#include "wcmenu.h"
#include "wcvocab.h"

/* Globals */
text *wcheroplate_level_label = 0;
text *wcheroplate_health_label = 0;
text *wcheroplate_normal = 0;
text *wcheroplate_mana_label = 0;

/* Initialize hero plates */
void wcheroplate_init()
{
	wcheroplate_level_label = new text(wcvocab_get(128));
	wcheroplate_level_label->set_color(1);
	wcheroplate_health_label = new text(wcvocab_get(129));
	wcheroplate_health_label->set_color(1);
	wcheroplate_normal = new text(wcvocab_get(126));
	wcheroplate_mana_label = new text(wcvocab_get(130));
	wcheroplate_mana_label->set_color(1);
}

/* Free hero plates */
void wcheroplate_exit()
{
	delete wcheroplate_level_label;
	delete wcheroplate_health_label;
	delete wcheroplate_normal;
	delete wcheroplate_mana_label;
}

/* Refresh plates */
void wcheroplate_refresh()
{
	wcheroplate_exit();
	wcheroplate_init();
}

/* Draws party of the entire party */
void wcheroplate_draw_party_entry(int x,int y,int ix)
{
	char buffer[32];
	hero *hr;
	texture *ftex;
	hr = wchero_get(wcparty_get(ix));
	ftex = wcparty_get_face(ix);
	/* First line */
	wcwindow_draw_face(ftex,hr->get_faceset_number(),x,y);
	if(hr->get_name_text())
	{
		hr->get_name_text()->move(x+56,y-4);
		hr->get_name_text()->draw();
	}
	if(hr->get_title_text())
	{
		hr->get_title_text()->move(x+56+80,y-4);
		hr->get_title_text()->draw();
	}
	/* Second line */
	wcheroplate_level_label->move(x+56,y-4+16);
	wcheroplate_level_label->draw();
	sprintf(buffer,"%d",hr->get_level());
	wcwindow_draw_text(x+56+16,y-4+16,0,buffer);
	wcheroplate_normal->move(x+56+16+24,y-4+16);
	wcheroplate_normal->draw();
	/* HP */
	wcheroplate_health_label->move(x+56+16+24+48,y-4+16);
	wcheroplate_health_label->draw();
	wcwindow_draw_text(x+56+16+24+48+32,y-4+16,0,"/");
	sprintf(buffer,"%d",hr->get_health());
	wcwindow_draw_text(x+56+16+24+48+12,y-4+16,0,buffer);
	sprintf(buffer,"%d",hr->get_stat(STAT_HEALTH));
	wcwindow_draw_text(x+56+16+24+48+8+32,y-4+16,0,buffer);
	/* MP */
	wcheroplate_mana_label->move(x+56+16+24+48,y-4+32);
	wcheroplate_mana_label->draw();
	wcwindow_draw_text(x+56+16+24+48+32,y-4+32,0,"/");
	sprintf(buffer,"%d",hr->get_mana());
	wcwindow_draw_text(x+56+16+24+48+12,y-4+32,0,buffer);
	sprintf(buffer,"%d",hr->get_stat(STAT_MANA));
	wcwindow_draw_text(x+56+16+24+48+8+32,y-4+32,0,buffer);
	/* EXP */
	wcwindow_draw_text(x+56+16+24,y-4+32,0,"/");
	sprintf(buffer,"%d",hr->get_exp());
	wcwindow_draw_text(x+56,y-4+32,0,buffer);
	sprintf(buffer,"%d",hr->get_exp_for_next());
	wcwindow_draw_text(x+56+16+24+8,y-4+32,0,buffer);
}

/* Draw the entire party */
void wcheroplate_draw_party(int x,int y)
{
	int i;
	/* Draw the background window */
	wcwindow_draw(x,y,320-WCMENU_MAIN_WIDTH,16+56*wcparty_size());
	/* Draw all entries */
	for(i = 0;i < wcparty_size();i++)
		wcheroplate_draw_party_entry(x+10,y+10+56*i,i);
}