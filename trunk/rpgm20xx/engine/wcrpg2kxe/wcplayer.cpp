/*
	Player Object in Maps
*/

/* Includes */
#include "wcparty.h"
#include "wcplayer.h"
#include "wchero.h"
#include "wcinput.h"
#include "wclevel.h"
#include "wcdialog.h"
#include "wcmanage.h"
#include "wccommon.h"
#include "wcsaveio.h"
#include "wcrpg.h"

/* Globals */
rpgevent *wcplayer_event = 0;
int wcplayer_last_move = 0;
int wcplayer_examine_auto_fire = 0;
int wcplayer_control_lock = 0;
int wcplayer_lock_delay = 0;

/* Create player object at x and y */
void wcplayer_start(int tx,int ty)
{
	texture *tex;
	hero *he;
	/* Delete old */
	if(wcplayer_event)
	{
		wcplayer_event->memorize_speed();
		delete wcplayer_event;
	}
	/* Make new */
	wcplayer_event = new rpgevent();
	/* Recall speed */
	wcplayer_event->recall_speed();
	/* Prepare charset */
	tex = 0;
	he = wcparty_leader();
	if(wcparty_leader_charset())
	{
		tex = wcevent_pool_charset(wcparty_leader_charset());
		wcplayer_event->change_graphics(tex,he->get_charset_number());
	}
	/* Move player */
	wcplayer_event->place(tx,ty);
}

/* Draws the player */
void wcplayer_draw(int x,int y)
{
	wcplayer_event->draw(x,y);
}

/* Gets the player */
rpgevent *wcplayer_get()
{
	return wcplayer_event;
}

/* Is the player allowed to move? */
int wcplayer_is_allowed_to_move()
{
	return (wcplayer_event->get_state() != EVENT_MOVING && !wcdialog_active() && !wcplayer_control_lock && !wccommon_auto_active() && !wcmanage_submenu_active());
}

/* Handles the player */
void wcplayer_handle()
{
	int x,y,bx,by,odir;
	/* Can't handle the player if they don't exist! */
	if(!wcplayer_event)
		return;
	/* Slowly undo lock */
	if(wcplayer_lock_delay > 0)
		wcplayer_lock_delay--;
	if(wcplayer_lock_delay == 0)
		wcplayer_control_lock = 0;
	/* Handle me */
	wcplayer_event->handle();
	/* Put the camera at me */
	wcplayer_event->focus();
	/* Remember */
	odir = wcplayer_event->get_direction();
	/* Handle my moves */
	if(wcplayer_event->get_route())
	{
		/* Execute */
		wcplayer_event->get_route()->run(wcplayer_event);
		/* End */
		if(wcplayer_event->get_route()->is_finished())
		{
			if(!wcplayer_event->walking())
			{
				wcmove_remove_active();
				wcplayer_event->get_route()->reset();
				wcplayer_event->set_move_list(0);
			}
		}
		return;
	}
	/* Move me if i am moving AND there is no dialog box */
	if(wcplayer_is_allowed_to_move())
	{
		/* Do I want to move? */
		if(wcinput_x() != 0 || wcinput_y() != 0)
		{
			/* Record old position */
			bx = wcplayer_event->get_pos_x();
			by = wcplayer_event->get_pos_y();
			/* X major */
			if(wcinput_x() != 0 && wcinput_y() == 0)
			{
				wcplayer_last_move = 0;
				if(wcinput_x() > 0)
					wcplayer_event->face(FACE_RIGHT);
				if(wcinput_x() < 0)
					wcplayer_event->face(FACE_LEFT);
				wcplayer_event->move(wcinput_x(),0);
			}
			/* Y major */
			if(wcinput_x() == 0 && wcinput_y() != 0)
			{
				wcplayer_last_move = 1;
				if(wcinput_y() > 0)
					wcplayer_event->face(FACE_DOWN);
				if(wcinput_y() < 0)
					wcplayer_event->face(FACE_UP);
				wcplayer_event->move(0,wcinput_y());
			}
			/* Ambiguity */
			if(wcinput_x() != 0 && wcinput_y() != 0)
			{
				/* Tilt */
				if(wcplayer_last_move == 0)
				{
					if(wcinput_y() < 0)
						wcplayer_event->face(FACE_UP);
					if(wcinput_y() > 0)
						wcplayer_event->face(FACE_DOWN);
					wcplayer_event->move(0,wcinput_y());
				}
				else
				{
					if(wcinput_x() > 0)
						wcplayer_event->face(FACE_RIGHT);
					if(wcinput_x() < 0)
						wcplayer_event->face(FACE_LEFT);
					wcplayer_event->move(wcinput_x(),0);
				}
			}
		}
	}
	/* Dialog can't be active for this to happen */
	if(!wcdialog_active())
	{
		/* Allow me to examine things */
		if(wcinput_ok() && !wcplayer_examine_auto_fire)
		{
			/* Get position */
			x = wcplayer_event->get_pos_x();
			y = wcplayer_event->get_pos_y();
			/* Under feet AND in front */
			wclevel_examine(x,y);
			/* Look in front */
			switch(wcplayer_event->get_direction())
			{
			case FACE_UP:
				y--;
				break;
			case FACE_DOWN:
				y++;
				break;
			case FACE_RIGHT:
				x++;
				break;
			case FACE_LEFT:
				x--;
				break;
			}
			/* Examine */
			wclevel_examine(x,y);
			/* Trigger */
			wcplayer_examine_auto_fire = 1;
		}
		if(!wcinput_ok())
			wcplayer_examine_auto_fire = 0;
	}
	/* Remember if lock facing */
	if(wcplayer_event->get_direction_fix())
		wcplayer_event->face(odir);
}

/* Trips auto fire */
void wcplayer_trip_examine_auto_fire()
{
	wcplayer_examine_auto_fire = 1;
}

/* Locks up the player's controls */
void wcplayer_lock()
{
	wcplayer_control_lock = 1;
	wcplayer_lock_delay = -1;
}

/* Unlocks the player's controls */
void wcplayer_unlock()
{
	wcplayer_lock_delay = 2;
}

/* Refresh hero sprite */
void wcplayer_refresh()
{
	hero *hr;
	/* Get whoever is leader */
	hr = wcparty_leader();
	/* Was that anyone? */
	if(!hr)
		return;
	/* Change charset, frame, and trans */
	wcplayer_event->change_graphics(wcevent_pool_charset(hr->get_charset()),hr->get_charset_number());
	wcplayer_event->set_transparent(hr->is_transparent());
}

/* Save player state */
void wcplayer_save(FILE *f)
{
	if(!wcplayer_event)
	{
		wcrpg_error("Attempted to save the player when it doesn't exist yet.");
		return;
	}
	wcsaveio_start(f);
	wcsaveio_write_num(1);
	wcsaveio_end();
	wcplayer_event->save_state(f);
}

/* Load player state */
void wcplayer_load(FILE *f)
{
	if(!wcplayer_event)
	{
		wcrpg_error("Attempted to load the player when it doesn't exist yet.");
		return;
	}
	wcsaveio_read(f);
	if(!wcsaveio_read_num())
	{
		wcsaveio_next();
		return;
	}
	wcsaveio_next();
	wcplayer_event->load_state(f);
}