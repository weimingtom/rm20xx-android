/*
	Text dialog box,
	this is the box for Show Message
*/

/* Includes */
#include <malloc.h>
#include <memory.h>
#include <string>
#include "wcrpg.h"
#include "wcdialog.h"
#include "wcwindow.h"
#include "wcinput.h"
#include "wcplayer.h"
#include "wcdraw.h"
#include "wcmaterial.h"
#include "wcsystem.h"

/* Globals */
int dialog_active = 0;
int dialog_current_line = 0;
int dialog_auto_fire = 0;
int dialog_lock = 0;
int dialog_waiting = 0;
char *dialog_lines[WCDIALOG_LINES]; /* Lines of the actual dialog */
texture *dialog_face = 0; /* Faces to use for dialog if any */
int dialog_face_index = 0; /* Face index */
int dialog_face_right = 0; /* Face right? */
int dialog_face_flip = 0; /* Flip? */
text *dialog_text = 0; /* The actual resulting text from the source text */
float dialog_progress = 0.0f; /* Progress of text scrolling in dialog */
int dialog_no_window = 0; /* Don't show the background window? */
int dialog_auto_hide = 1; /* Move the dialog if in player's way */
int dialog_preferred_pos = WCDIALOG_POS_BOTTOM; /* Dialog preferred position */
float dialog_tempo_delay = 0.0f; /* Delay */
int dialog_choice_active = 0; /* Is the show choice menu open? */
menu *dialog_choice_menu = 0; /* The show choice menu */
int dialog_choice_default = 0; /* Option to default to */

/* Gets option choice */
int wcdialog_get_choice()
{
	return dialog_choice_menu->get_selection();
}

/* Welds the four strings together into a single string spanning four lines */
char *wcdialog_weld_lines(char *a,char *b,char *c,char *d)
{
	char *cs;
	int len,cur;
	/* Figure out how long the result will be */
	len = 1;
	if(a)
		len += (int)strlen(a);
	if(b)
		len += (int)strlen(b)+1;
	if(c)
		len += (int)strlen(c)+1;
	if(d)
		len += (int)strlen(d)+1;
	/* Allocate the room for it */
	cs = (char*)malloc(len);
	memset(cs,0,len);
	/* Copy the substrings */
	cur = 0;
	if(a)
	{
		memcpy(cs,a,strlen(a));
		cur += (int)strlen(a);
	}
	if(b)
	{
		cs[cur] = '\n';
		cur++;
		memcpy(&cs[cur],b,strlen(b));
		cur += (int)strlen(b);
	}
	if(c)
	{
		cs[cur] = '\n';
		cur++;
		memcpy(&cs[cur],c,strlen(c));
		cur += (int)strlen(c);
	}
	if(d)
	{
		cs[cur] = '\n';
		cur++;
		memcpy(&cs[cur],d,strlen(d));
		cur += (int)strlen(d);
	}
	/* Complete */
	return cs;
}

/* Updates the dialog */
void wcdialog_refresh()
{
	char *cs;
	/* Get rid of old */
	if(dialog_text)
		delete dialog_text;
	/* Make new */
	cs = wcdialog_weld_lines(dialog_lines[0],dialog_lines[1],dialog_lines[2],dialog_lines[3]);
	dialog_text = new text(cs);
	free(cs);
}

/* Clears the face */
void wcdialog_erase_face()
{
	/* Erase face */
	if(dialog_face)
		delete dialog_face;
	dialog_face = 0;
	dialog_face_right = 0;
	dialog_face_flip = 0;
}

/* Sets the face */
void wcdialog_set_face(char *face,int index,int rt,int fp)
{
	image *faceimg;
	/* Get rid of face */
	wcdialog_erase_face();
	/* Load face */
	faceimg = new image(wcmaterial_request(MATERIAL_FACESET,face));
	if(fp)
		faceimg->flip_horz();
	dialog_face = new texture(faceimg);
	dialog_face->reduce(); /* Data not required */
	dialog_face_index = index;
	dialog_face_right = rt;
	dialog_face_flip = fp;
}

/* Draws the face */
void wcdialog_draw_face(int x,int y)
{
	int sx,sy;
	/* No face */
	if(!dialog_face)
		return;
	/* Find source */
	sx = dialog_face_index%4;
	sy = dialog_face_index/4;
	sx *= 48;
	sy *= 48;
	/* Draw */
	if(!dialog_face_flip)
		dialog_face->draw(x,y,48,48,sx,sy);
	else
		dialog_face->draw(x,y,48,48,256-sx-48,sy);
}

/* Attempts to aquire the dialog lock and returns true if it was granted */
int wcdialog_request(int id)
{
	/* Request */
	if(!dialog_lock)
	{
		/* Permission granted, you have the lock */
		dialog_lock = id;
		dialog_waiting = 0;
		return 1;
	}
	/* You do not have permission to open a dialog box as there is another already open */
	dialog_waiting = 1; /* But i'll let them know you're waiting on dialog */
	return 0;
}

/* Returns 1 if this event id should wait for dialog to end */
int wcdialog_halt(int id)
{
	if(id == dialog_lock)
		return 1;
	return 0;
}

/* Adds a line of dialog to be shown */
void wcdialog_add_line(char *ln)
{
	/* Advanced too far? */
	if(dialog_current_line >= WCDIALOG_LINES)
		return;
	/* Initalizer? */
	if(dialog_current_line == 0)
		memset(dialog_lines,0,sizeof(dialog_lines));
	/* Set line */
	if(ln)
		dialog_lines[dialog_current_line] = ln;
	else
		dialog_lines[dialog_current_line] = "";
	/* Refresh */
	wcdialog_refresh();
	/* Advance */
	dialog_current_line++;
}

/* Resets dialog stack */
void wcdialog_reset_line()
{
	int i;
	dialog_current_line = 0;
	for(i = 0;i < WCDIALOG_LINES;i++)
		dialog_lines[i] = 0;
}

/* Open the dialog box */
void wcdialog_open()
{
	/* Trip the flag */
	dialog_active = 1;
	dialog_auto_fire = 1; /* Prevent the OK button from quickskipping dialog */
	dialog_progress = 0.0f; /* Dialog progress */
	dialog_tempo_delay = 0.0f; /* Dialog tempo progress */
	wcwindow_set_text_speed(10); /* Default speed */
}

/* Open a show choice menu */
void wcdialog_open_choice(char *str)
{
	/* Trip the flag */
	dialog_choice_active = 1;
	if(dialog_choice_menu)
		delete dialog_choice_menu;
	dialog_choice_menu = wcdialog_make_choice_menu(str);
	dialog_choice_menu->activate();
}

/* Set choice default */
void wcdialog_set_choice_default(int d)
{
	dialog_choice_default = d;
}

/* Draws the dialog layer */
void wcdialog_draw()
{
	int posoff;
	/* Only if open */
	if(!dialog_active && !dialog_choice_active)
		return;
	/* Find out if the dialog box is to be moved */
	if(dialog_text)
	{
		posoff = 0; /* -160 is the top -80 is middle */
		/* Set position */
		if(dialog_preferred_pos == WCDIALOG_POS_TOP)
			posoff = -160;
		if(dialog_preferred_pos == WCDIALOG_POS_MIDDLE)
			posoff = -80;
		if(dialog_preferred_pos == WCDIALOG_POS_BOTTOM)
			posoff = 0;
		/* Auto position */
		if(dialog_auto_hide)
		{
			/* Bottom */
			posoff = 0;
			/* And then top if in the way */
			if(wcplayer_get()->get_screen_y() > 120)
				posoff = -160;
		}
		/* Draw the dialog box */
		if(!dialog_no_window)
			wcwindow_draw(0,240-80+posoff,WCRPG_WIDTH,80);
		/* Draw the face */
		if(!dialog_face_right)
			wcdialog_draw_face(16,240-80+16+posoff);
		else
			wcdialog_draw_face(320-16-48,240-80+16+posoff);
		/* Draw the text */
		if(dialog_text)
		{
			if(dialog_face && !dialog_face_right)
			{
				dialog_text->move(8+64+8,240-80+8+posoff);
				dialog_text->draw();
			}
			else
			{
				dialog_text->move(8,240-80+8+posoff);
				dialog_text->draw();
			}
		}
		/* Draw next arrow */
		if((int)dialog_progress >= dialog_text->length())
			wcwindow_draw_arrow(WCRPG_WIDTH/2-8,240-8+posoff,1);
	}
	/* Draw the choice menu */
	if(dialog_choice_menu && dialog_choice_active)
	{
		posoff = 0; /* -160 is the top -80 is middle */
		/* Set position */
		if(dialog_preferred_pos == WCDIALOG_POS_TOP)
			posoff = -160;
		if(dialog_preferred_pos == WCDIALOG_POS_MIDDLE)
			posoff = -80;
		if(dialog_preferred_pos == WCDIALOG_POS_BOTTOM)
			posoff = 0;
		/* Auto position */
		if(dialog_auto_hide)
		{
			/* Bottom */
			posoff = 0;
			/* And then top if in the way */
			if(wcplayer_get()->get_screen_y() > 120)
				posoff = -160;
		}
		/* Draw it */
		if(dialog_face && !dialog_face_right)
			dialog_choice_menu->draw(64,240-(dialog_choice_menu->size()+1)*16+posoff);
		else
			dialog_choice_menu->draw(0,240-(dialog_choice_menu->size()+1)*16+posoff);
	}
}

/* Returns true if dialog is currently active */
int wcdialog_active()
{
	return (dialog_active||dialog_choice_active);
}

/* Handles dialog */
void wcdialog_handle()
{
	float ts,nprog,ttime,cspeed;
	int i;
	/* Handle show choice, which overrides control of dialog boxes */
	if(dialog_choice_active)
	{
		/* Handle */
		dialog_choice_menu->handle();
		/* Select out */
		if(!dialog_choice_menu->is_active())
		{
			/* Release and return */
			dialog_lock = 0;
			dialog_auto_fire = 1;
			if(!dialog_waiting)
				dialog_active = 0;
			dialog_choice_active = 0;
			wcplayer_trip_examine_auto_fire();
		}
		/* Defaulting? */
		if(dialog_choice_default && wcinput_cancel())
		{
			/* Force a selection */
			if(dialog_choice_default != 4)
			{
				/* Some option */
				dialog_choice_menu->set_selection(dialog_choice_default-1);
				wcsystem_play_sound(WCSYSTEM_CANCEL); /* Make a fart noise and close */
				dialog_choice_default = 0;  /* Get rid of default */
			}
			else
			{
				/* Custom handler */
				dialog_choice_menu->set_selection(4);
				wcsystem_play_sound(WCSYSTEM_CANCEL); /* Make a fart noise and close */
				dialog_choice_default = 0;  /* Get rid of default */
			}
		}
		return;
	}
	/* Only if open */
	if(!dialog_active)
		return;
	/* Regressive advancement (to help fix JARRING text on unstable speed systems) */
	for(i = 0;i < 8;i++)
	{
		/* Make the text reflect the current position */
		dialog_text->advance((int)dialog_progress,0);
		/* Get the speed */
		cspeed = dialog_text->current_speed((int)dialog_progress);
		/* Find out where the dialog text will go next */
		ts = wcrpg_get_time_step();
		nprog = dialog_progress+(ts*6.0f*wcwindow_get_text_speed()*cspeed)/8.0f;
		/* Get the tempo of that time */
		ttime = dialog_text->tempo_time((int)nprog);
		/* Can we advance? */
		if(ttime > dialog_tempo_delay)
		{
			/* We must wait. */
			dialog_tempo_delay += (ts*6.0f)/8.0f;
			/* Fill out to current position */
			dialog_text->advance((int)dialog_progress,1);
		}
		else
		{
			/* We may advance */
			dialog_progress = nprog;
		}
	}
	/* Advance if OK is pressed */
	if((wcinput_ok() && !dialog_auto_fire) || dialog_text->auto_close((int)dialog_progress))
	{
		if((int)dialog_progress >= dialog_text->length())
		{
			/* Release permission */
			dialog_lock = 0;
			dialog_auto_fire = 1;
			if(!dialog_waiting)
				dialog_active = 0;
			wcplayer_trip_examine_auto_fire();
		}
		else
		{
			/* Instant show (skip text scrolling for current dialog) */
			dialog_progress = (float)dialog_text->length();
			dialog_auto_fire = 1;
		}
	}
	/* Release auto fire */
	if(!wcinput_ok())
		dialog_auto_fire = 0;
}

/* Set options */
void wcdialog_options(int trans,int pos,int autopos,int nowait)
{
	/* Set trans */
	dialog_no_window = trans;
	/* Set pos */
	dialog_preferred_pos = pos;
	/* Set auto hide */
	dialog_auto_hide = autopos;
}

/* Generate a menu based on the input show choice string */
menu *wcdialog_make_choice_menu(char *str)
{
	char *cstr;
	menu *mn;
	int slen,i,count;
	/* Measure the length of input string */
	slen = (int)strlen(str);
	/* Make / -> 0 copy */
	cstr = wcrpg_string_clone(str);
	count = 1;
	for(i = 0;i < slen;i++)
	{
		if(cstr[i] == '/')
		{
			count++;
			cstr[i] = 0;
		}
	}
	/* Create menu */
	if(dialog_face)
		mn = new menu(WCRPG_WIDTH-64,count);
	else
		mn = new menu(WCRPG_WIDTH,count);
	/* Add options */
	i = 0;
	for(;;)
	{
		/* End of options */
		if(i >= slen)
			break;
		/* Add option */
		mn->add(wcrpg_string_clone(&cstr[i]));
		/* Seek to next option */
		while(cstr[i])
			i++;
		i++;
	}
	/* Done with clone */
	free(cstr);
	/* Return result */
	return mn;
}