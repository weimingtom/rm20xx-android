/*
	Save/Loading menu
*/

/* Includes */
#include <stdio.h>
#include "wcsavemenu.h"
#include "wcdraw.h"
#include "wcparty.h"
#include "wcsave.h"
#include "wchero.h"
#include "wcmaterial.h"
#include "wcrpg.h"
#include "wcwindow.h"
#include "wcinput.h"
#include "wcmanage.h"
#include "wcvocab.h"
#include "wcsystem.h"
#include "wcplayer.h"

#include <string.h>

#include <android\log.h>

/* Globals */
texture *wcsavemenu_faces[WCSAVE_SLOTS*MAX_PARTY_SIZE]; /* All those faces */
hero *wcsavemenu_heroes[WCSAVE_SLOTS*MAX_PARTY_SIZE]; /* All those heroes */
int wcsavemenu_occupied[WCSAVE_SLOTS*MAX_PARTY_SIZE]; /* All those occupants */
text *wcsavemenu_leader_name[WCSAVE_SLOTS]; /* All those leader names */
int wcsavemenu_face_index[WCSAVE_SLOTS*MAX_PARTY_SIZE]; /* All those face index */
int wcsavemenu_on = 0; /* Is it open? */
float wcsavemenu_slide = 240.0f; /* Sliding anim */
float wcsavemenu_slide_2 = 320.0f; /* Sliding desc */
int wcsavemenu_cursor = 0; /* Selected file */
int wcsavemenu_auto_fire = 0;
int wcsavemenu_loading = 0; /* Are we loading instead? */
menu *wcsavemenu_link = 0;
text *wcsavemenu_text = 0;
text *wcsavemenu_file_label = 0;

/* Open save menu */
void wcsavemenu_open(menu *m,int loadin)
{
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcsavemenu_open");
	/* Refresh and open */
	wcsavemenu_refresh(loadin);
	wcsavemenu_on = 1;
	wcsavemenu_link = m;
	wcsavemenu_auto_fire = 1;
}

/* Close save menu */
void wcsavemenu_close()
{
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcsavemenu_close");
	int i;
	/* Free resources */
	for(i = 0;i < WCSAVE_SLOTS*MAX_PARTY_SIZE;i++)
		wcsavemenu_faces[i] = 0;
	for(i = 0;i < WCSAVE_SLOTS;i++)
	{
		if(wcsavemenu_leader_name[i])
			delete wcsavemenu_leader_name[i];
		wcsavemenu_leader_name[i] = 0;
	}
	/* Release text */
	if(wcsavemenu_text)
		delete wcsavemenu_text;
	wcsavemenu_text = 0;
	/* Close */
	wcsavemenu_on = 0;
	if(wcsavemenu_link)
		wcsavemenu_link->activate();
	wcmanage_return();
}

/* Is open? */
int wcsavemenu_active()
{
	return wcsavemenu_on;
}

/* Handle save menu */
void wcsavemenu_handle()
{
	if(!wcsavemenu_active())
	{
		/* Slide away */
		wcsavemenu_slide += (241.0f-wcsavemenu_slide)/2.0f*wcrpg_get_time_step();
		wcsavemenu_slide_2 += (321.0f-wcsavemenu_slide_2)/2.0f*wcrpg_get_time_step();
		return;
	}
	/* Slide open */
	wcsavemenu_slide += ((-wcsavemenu_cursor*WCSAVEMENU_HEIGHT)-wcsavemenu_slide)/2.0f*wcrpg_get_time_step();
	wcsavemenu_slide_2 += (-wcsavemenu_slide_2)/2.0f*wcrpg_get_time_step();
	/* Up/Down */
	if(wcinput_y() < 0 && !wcsavemenu_auto_fire && wcsavemenu_cursor > 0)
	{
		wcsystem_play_sound(WCSYSTEM_CURSOR);
		wcsavemenu_cursor--;
		wcsavemenu_auto_fire = 1;
	}
	if(wcinput_y() > 0 && !wcsavemenu_auto_fire && wcsavemenu_cursor < WCSAVE_SLOTS-1)
	{
		wcsystem_play_sound(WCSYSTEM_CURSOR);
		wcsavemenu_cursor++;
		wcsavemenu_auto_fire = 1;
	}
	/* Save! */
	if(wcinput_ok() && !wcsavemenu_auto_fire && !wcsavemenu_loading)
	{
		wcsystem_play_sound(WCSYSTEM_DECIDE);
		wcsave_save(wcsavemenu_cursor);
		wcsavemenu_slide = 240.0f;
		wcsavemenu_auto_fire = 1;
	}
	/* Load! */
	if(wcinput_ok() && !wcsavemenu_auto_fire && wcsavemenu_loading)
	{
		/* OK, this has to be loaded, you know */
		if(wcsave_get_tag(wcsavemenu_cursor)->used)
		{
			wcsystem_play_sound(WCSYSTEM_DECIDE);
			wcsave_load(wcsavemenu_cursor);
			wcsavemenu_close();
			wcplayer_unlock();
		}
		else
		{
			wcsystem_play_sound(WCSYSTEM_BUZZER);
		}
		wcsavemenu_auto_fire = 1;
	}
	/* Closing? */
	if(wcinput_cancel() && !wcsavemenu_auto_fire)
	{
		wcsystem_play_sound(WCSYSTEM_CANCEL);
		wcsavemenu_close();
		wcsavemenu_auto_fire = 1;
	}
	if(!wcinput_cancel() && wcinput_y() == 0 && !wcinput_ok())
		wcsavemenu_auto_fire = 0;
}

/* Draw save menu */
void wcsavemenu_draw()
{
	int i,ybase,j,fi;
	char buffer[4];
	/* Draw the windowframes */
	for(i = 0;i < WCSAVE_SLOTS;i++)
	{
		/* Draw if in view */
		if(!((int)wcsavemenu_slide+i*WCSAVEMENU_HEIGHT+32 < -WCSAVEMENU_HEIGHT || (int)wcsavemenu_slide+i*WCSAVEMENU_HEIGHT+32 > 240))
		{
			/* Set y */
			ybase = (int)(wcsavemenu_slide)+i*WCSAVEMENU_HEIGHT+32;
			/* Draw base */
			wcwindow_draw(WCRPG_WIDTH-WCSAVEMENU_WIDTH,ybase,WCSAVEMENU_WIDTH,WCSAVEMENU_HEIGHT);
			/* Draw label */
			if(wcsavemenu_file_label)
			{
				wcsavemenu_file_label->move(8+WCRPG_WIDTH-WCSAVEMENU_WIDTH,ybase+4);
				wcsavemenu_file_label->draw();
			}
			/* Draw number */
			sprintf(buffer,"%d",i+1);
			wcwindow_draw_text(8+32+8+WCRPG_WIDTH-WCSAVEMENU_WIDTH,ybase+4,0,buffer);
			/* Draw leader's name */
			if(wcsavemenu_leader_name[i])
			{
				wcsavemenu_leader_name[i]->move(8+32+32+8+WCRPG_WIDTH-WCSAVEMENU_WIDTH,ybase+4);
				wcsavemenu_leader_name[i]->draw();
			}
			/* Draw faces */
			for(j = 0;j < MAX_PARTY_SIZE;j++)
			{
				/* If have face draw that face */
				if(wcsavemenu_faces[i*MAX_PARTY_SIZE+j])
				{
					/* Get face index */
					fi = wcsavemenu_face_index[i*MAX_PARTY_SIZE+j];
					/* Draw with face index */
					wcwindow_draw_face(wcsavemenu_faces[i*MAX_PARTY_SIZE+j],fi,8+j*56+WCRPG_WIDTH-WCSAVEMENU_WIDTH,ybase+16+6);
				}
			}
		}
	}
	/* Draw arrows */
	if(wcsavemenu_active())
	{
		if(wcsavemenu_cursor != 0)
			wcwindow_draw_arrow(WCSAVEMENU_WIDTH/2-8,32,0);
		if(wcsavemenu_cursor < WCSAVE_SLOTS-2)
			wcwindow_draw_arrow(WCSAVEMENU_WIDTH/2-8,240-8,1);
	}
	/* Draw the desc */
	wcwindow_draw((int)(wcsavemenu_slide_2)+WCRPG_WIDTH-WCSAVEMENU_WIDTH,0,320,32);
	if(wcsavemenu_text)
	{
		wcsavemenu_text->move((int)(wcsavemenu_slide_2)+8+WCRPG_WIDTH-WCSAVEMENU_WIDTH,8);
		wcsavemenu_text->draw();
	}
}

/* Refresh save menu */
void wcsavemenu_refresh(int loadin)
{
	int i,j;
	savetag *stg;
	hero *he;
	/* Set loading */
	wcsavemenu_loading = loadin;
	/* Delete old names */
	for(i = 0;i < WCSAVE_SLOTS;i++)
	{
		if(wcsavemenu_leader_name[i])
			delete wcsavemenu_leader_name[i];
	}
	/* Rewipe */
	memset(wcsavemenu_faces,0,sizeof(wcsavemenu_faces));
	memset(wcsavemenu_heroes,0,sizeof(wcsavemenu_heroes));
	memset(wcsavemenu_occupied,0,sizeof(wcsavemenu_occupied));
	memset(wcsavemenu_leader_name,0,sizeof(wcsavemenu_leader_name));
	memset(wcsavemenu_face_index,0,sizeof(wcsavemenu_face_index));
	/* Reload text */
	if(wcsavemenu_text)
		delete wcsavemenu_text;
	/* Load all tags in */
	for(i = 0;i < WCSAVE_SLOTS;i++)
	{
		/* Get tag */
		stg = wcsave_get_tag(i);
		/* Index heroes */
		if(stg->used)
		{
			for(j = 0;j < MAX_PARTY_SIZE;j++)
			{
				if(stg->party[j] != WCPARTY_NOBODY)
				{
					/* Get hero */
					he = (hero*)wchero_get(stg->party[j]);
					/* Load face resource */
					wcsavemenu_faces[i*MAX_PARTY_SIZE+j] = wcwindow_pool_faceset(he->get_faceset());
					wcsavemenu_face_index[i*MAX_PARTY_SIZE+j] = stg->face_index[j];
					if(wcsavemenu_faces[i*MAX_PARTY_SIZE+j])
						wcsavemenu_faces[i*MAX_PARTY_SIZE+j]->reduce();
					/* Ref hero */
					wcsavemenu_heroes[i*MAX_PARTY_SIZE+j] = he;
				}
			}
			/* Name */
			if(wcsavemenu_heroes[0])
				wcsavemenu_leader_name[i] = new text(stg->leadername);
		}
	}
	/* Reload */
	if(loadin)
		wcsavemenu_text = new text(wcvocab_get(147));
	else
		wcsavemenu_text = new text(wcvocab_get(146));
	/* Load once */
	if(!wcsavemenu_file_label)
		wcsavemenu_file_label = new text(wcvocab_get(148));
}

/* Reload save menu */
void wcsavemenu_init()
{
	/* Wipe out all */
	memset(wcsavemenu_faces,0,sizeof(wcsavemenu_faces));
	memset(wcsavemenu_heroes,0,sizeof(wcsavemenu_heroes));
	memset(wcsavemenu_occupied,0,sizeof(wcsavemenu_occupied));
	memset(wcsavemenu_leader_name,0,sizeof(wcsavemenu_leader_name));
	memset(wcsavemenu_face_index,0,sizeof(wcsavemenu_face_index));
}