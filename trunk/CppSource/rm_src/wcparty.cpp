/*
	Player's Hero Party
*/

/* Includes */
#include <memory.h>
#include "wcparty.h"
#include "wcsystem.h"
#include "wcrpg.h"
#include "wclog.h"
#include "wcmaterial.h"
#include "wcsaveio.h"
#include "wcplayer.h"
#include "wcwindow.h"

#include <string>

/* Globals */
int wcparty_current_size = 0; /* Current size of party */
int wcparty_party[MAX_PARTY_SIZE]; /* Just a list o' ids */
texture *wcparty_face[MAX_PARTY_SIZE]; /* List of party faces */

/* Returns texture for face */
texture *wcparty_get_face(int i)
{
	return wcparty_face[i];
}

/* Is in party? */
int wcparty_has(int id)
{
	int i;
	/* Check */
	for(i = 0;i < MAX_PARTY_SIZE;i++)
	{
		if(wcparty_party[i] == id)
			return 1; /* In party */
	}
	/* Not in party */
	return 0;
}

/* Remove member */
void wcparty_remove(int id)
{
	int i;
	/* Can't remove no one */
	if(id == WCPARTY_NOBODY)
		return;
	/* Already not in party */
	if(!wcparty_has(id))
		return;
	/* No one there */
	if(wcparty_size() <= 0)
		return;
	/* Find them */
	for(i = 0;i < MAX_PARTY_SIZE;i++)
	{
		/* Remove them */
		if(wcparty_party[i] == id)
		{
			wcparty_party[i] = WCPARTY_NOBODY;
			wclog_write(wcrpg_get_log(),wchero_get(id)->get_name());
			wclog_write(wcrpg_get_log()," has left the party.");
			wclog_enter(wcrpg_get_log());
			wcparty_current_size--;
			/* Re-arrange later ones */
			for(i = i+1;i < MAX_PARTY_SIZE;i++)
				wcparty_party[i-1] = wcparty_party[i];
			wcparty_party[MAX_PARTY_SIZE-1] = WCPARTY_NOBODY;
			return;
		}
	}
}

/* Add member */
void wcparty_add(int id)
{
	hero *hr;
	int i;
	/* Can't add no one */
	if(id == WCPARTY_NOBODY)
		return;
	/* Already there */
	if(wcparty_has(id))
		return;
	/* Too full and in old mode */
	if(!wcrpg_neo_mode() && wcparty_size() >= 4)
		return;
	/* Too full */
	if(wcparty_size() >= MAX_PARTY_SIZE)
		return;
	/* Find slot */
	for(i = 0;i < MAX_PARTY_SIZE;i++)
	{
		if(wcparty_party[i] == WCPARTY_NOBODY)
		{
			hr = wchero_get(id);
			wcparty_party[i] = id;
			wcparty_face[i] = wcwindow_pool_faceset(hr->get_faceset());
			wclog_write(wcrpg_get_log(),wchero_get(id)->get_name());
			wclog_write(wcrpg_get_log()," has joined the party!");
			wclog_enter(wcrpg_get_log());
			wcparty_current_size++;
			return;
		}
	}
}

/* Updates faces and things for the party */
void wcparty_refresh()
{
	int i;
	hero *hr;
	/* Refresh all */
	for(i = 0;i < MAX_PARTY_SIZE;i++)
	{
		/* If there is someone there */
		if(wcparty_party[i] != WCPARTY_NOBODY)
		{
			/* Delete */
			hr = wchero_get(wcparty_get(i));
			/* Refresh */
			wcparty_face[i] = wcparty_face[i] = wcwindow_pool_faceset(hr->get_faceset());
		}
	}
}

/* Initialize party */
void wcparty_init()
{
	/* Wipe textures */
	memset(wcparty_face,0,sizeof(wcparty_face));
	/* Add defaults */
	wcparty_add(wcsystem_get_init_party_member(0));
	wcparty_add(wcsystem_get_init_party_member(1));
	wcparty_add(wcsystem_get_init_party_member(2));
	wcparty_add(wcsystem_get_init_party_member(3));
}

/* Clean party */
void wcparty_exit()
{
	int i;
	for(i = 0;i < MAX_PARTY_SIZE;i++)
		wcparty_face[i] = 0;
}

/* Get leader */
hero *wcparty_leader()
{
	/* No one */
	if(wcparty_party[0] == WCPARTY_NOBODY)
		return 0;
	/* Someone */
	return wchero_get(wcparty_party[0]);
}

/* Get leader's charset */
char *wcparty_leader_charset()
{
	hero *he;
	/* Get leader */
	he = wcparty_leader();
	/* No leader */
	if(!he)
		return 0;
	/* Get charset */
	return he->get_charset();
}

/* Return party size */
int wcparty_size()
{
	return wcparty_current_size;
}

/* Gets a party member */
int wcparty_get(int i)
{
	return wcparty_party[i];
}

/* Saves party */
void wcparty_save(FILE *f)
{
	int i;
	wcsaveio_start(f);
	for(i = 0;i < MAX_PARTY_SIZE;i++)
		wcsaveio_write_num(wcparty_party[i]);
	wcsaveio_end();
}

/* Loads party */
void wcparty_load(FILE *f)
{
	int i,n;
	wcsaveio_read(f);
	wcparty_current_size = 0;
	for(i = 0;i < MAX_PARTY_SIZE;i++)
	{
		n = wcsaveio_read_num();
		wcparty_party[i] = n;
		if(n != WCPARTY_NOBODY)
			wcparty_current_size++;
	}
	wcsaveio_next();
}