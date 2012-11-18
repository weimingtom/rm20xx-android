/*
	The Map Tree
*/

/* Includes */
//#include <windows.h>
#include <malloc.h>
#include "wcrpg.h"
#include "wctype.h"
#include "wcmaptree.h"
#include "wclog.h"
#include "wcarray.h"

#include "../jniUtil.h"

/* Globals */
int wcmaptree_count = 0;
arraylist wcmaptree_list;
int wcmaptree_start_id[4] = {-1,-1,-1,-1};
int wcmaptree_start_x[4] = {0,0,0,0};
int wcmaptree_start_y[4] = {0,0,0,0};

/* Get size */
int wcmaptree_size()
{
	return wcarray_size(wcmaptree_list);
}

/* Get node */
treenode *wcmaptree_get(int id)
{
	int i;
	treenode *tn;
	/* Find ID */
	for(i = 0;i < wcarray_size(wcmaptree_list);i++)
	{
		tn = (treenode*)wcarray_get(wcmaptree_list,i);
		if(tn->get_id() == id)
			return tn;
	}
	/* Not found */
	wcrpg_error("No map tree node for map ID (%d)",id);
	return 0;
}

/* Loads tree */
void wcmaptree_load()
{
	char buffer[260];
	FILE *f;
	char *filetype;
	int rlen,i,code,val,len;
	treenode *node;
	/* Alloc */
	wcmaptree_list = wcarray_new();
	/* Open file */
	sprintf(buffer,"%sRPG_RT.lmt","/sdcard/wcr2kxx/assets/");
	f=fopen(buffer,"rb");
	filetype = wcstr_read_file(f,&rlen);
	wclog_write(wcrpg_get_log(),"Loading map tree (");
	wclog_write(wcrpg_get_log(),filetype);
	wclog_write(wcrpg_get_log(),") ...");
	wclog_enter(wcrpg_get_log());
	/* Find out how many */
	wcmaptree_count = wcint_read_file(f,&rlen);
	wclog_write(wcrpg_get_log(),wcmaptree_count);
	wclog_write(wcrpg_get_log()," tree nodes.");
	wclog_enter(wcrpg_get_log());
	/* Read all nodes */
	for(i = 0;i < wcmaptree_count;i++)
	{
		/* Read */
		node = new treenode(f);
		/* Add */
		wcarray_add(wcmaptree_list,node);
	}
	/* Clean extra data */
	code = 0;
	code = wcint_read_file(f,&rlen);
	wclog_write(wcrpg_get_log(),"Skipping ");
	wclog_write(wcrpg_get_log(),code);
	wclog_write(wcrpg_get_log(),"+1 numbers of trash.");
	wclog_enter(wcrpg_get_log());
	for(i = 0;i < code;i++)
		wcint_read_file(f,&rlen);
	fread(&code,1,1,f);
	/* Get the starting pos */
	for(;;)
	{
		code = 0;
		fread(&code,1,1,f);
		if(!code)
			break;
		len = wcint_read_file(f,&rlen);
		val = wcint_read_file(f,&rlen);
		/* Sort */
		if(code == 1)
		{
			wcmaptree_start_id[START_HERO] = val;
			wclog_write(wcrpg_get_log(),"Hero start map ID = ");
			wclog_write(wcrpg_get_log(),val);
			wclog_enter(wcrpg_get_log());
		}
		if(code == 2)
		{
			wcmaptree_start_x[START_HERO] = val;
			wclog_write(wcrpg_get_log(),"Hero start X = ");
			wclog_write(wcrpg_get_log(),val);
			wclog_enter(wcrpg_get_log());
		}
		if(code == 3)
		{
			wcmaptree_start_y[START_HERO] = val;
			wclog_write(wcrpg_get_log(),"Hero start Y = ");
			wclog_write(wcrpg_get_log(),val);
			wclog_enter(wcrpg_get_log());
		}
		if(code == 11)
		{
			wcmaptree_start_id[START_SKIFF] = val;
			wclog_write(wcrpg_get_log(),"Skiff start map ID = ");
			wclog_write(wcrpg_get_log(),val);
			wclog_enter(wcrpg_get_log());
		}
		if(code == 12)
		{
			wcmaptree_start_x[START_SKIFF] = val;
			wclog_write(wcrpg_get_log(),"Skiff start X = ");
			wclog_write(wcrpg_get_log(),val);
			wclog_enter(wcrpg_get_log());
		}
		if(code == 13)
		{
			wcmaptree_start_y[START_SKIFF] = val;
			wclog_write(wcrpg_get_log(),"Skiff start Y = ");
			wclog_write(wcrpg_get_log(),val);
			wclog_enter(wcrpg_get_log());
		}
		if(code == 21)
		{
			wcmaptree_start_id[START_SHIP] = val;
			wclog_write(wcrpg_get_log(),"Ship start map ID = ");
			wclog_write(wcrpg_get_log(),val);
			wclog_enter(wcrpg_get_log());
		}
		if(code == 22)
		{
			wcmaptree_start_x[START_SHIP] = val;
			wclog_write(wcrpg_get_log(),"Ship start X = ");
			wclog_write(wcrpg_get_log(),val);
			wclog_enter(wcrpg_get_log());
		}
		if(code == 23)
		{
			wcmaptree_start_y[START_SHIP] = val;
			wclog_write(wcrpg_get_log(),"Ship start Y = ");
			wclog_write(wcrpg_get_log(),val);
			wclog_enter(wcrpg_get_log());
		}
		if(code == 31)
		{
			wcmaptree_start_id[START_AIRSHIP] = val;
			wclog_write(wcrpg_get_log(),"Airship start map ID = ");
			wclog_write(wcrpg_get_log(),val);
			wclog_enter(wcrpg_get_log());
		}
		if(code == 32)
		{
			wcmaptree_start_x[START_AIRSHIP] = val;
			wclog_write(wcrpg_get_log(),"Airship start X = ");
			wclog_write(wcrpg_get_log(),val);
			wclog_enter(wcrpg_get_log());
		}
		if(code == 33)
		{
			wcmaptree_start_y[START_AIRSHIP] = val;
			wclog_write(wcrpg_get_log(),"Airship start Y = ");
			wclog_write(wcrpg_get_log(),val);
			wclog_enter(wcrpg_get_log());
		}
	}
	/* Close file */
	fclose(f);
	free(filetype);
}

/* Frees tree */
void wcmaptree_free()
{
	int i;
	for(i = 0;i < wcmaptree_count;i++)
		delete ((treenode*)wcarray_get(wcmaptree_list,i));
	/* Release */
	wcarray_delete(wcmaptree_list);
}

/* Apply node property */
int treenode :: apply(FILE *f,int code,int len)
{
	int rlen;
	if(code == 0x01)
	{
		/* Name type */
		name = wcstr_read_file(f,len);
		wclog_write(wcrpg_get_log(),name);
		wclog_write(wcrpg_get_log()," ");
		return 1;
	}
	if(code == 0x04)
	{
		/* Node type */
		type = wcint_read_file(f,&rlen);
		wclog_write(wcrpg_get_log(),"T=");
		wclog_write(wcrpg_get_log(),type);
		wclog_write(wcrpg_get_log()," ");
		return 1;
	}
	if(code == 0x02)
	{
		/* Parent to a node */
		parent = wcint_read_file(f,&rlen);
		wclog_write(wcrpg_get_log(),"^=");
		wclog_write(wcrpg_get_log(),parent);
		wclog_write(wcrpg_get_log()," ");
		return 1;
	}
	if(code == 0x0B)
	{
		/* Music mode */
		music_mode = wcint_read_file(f,&rlen);
		wclog_write(wcrpg_get_log(),"MMode=");
		wclog_write(wcrpg_get_log(),music_mode);
		wclog_write(wcrpg_get_log()," ");
		return 1;
	}
	if(code == 0x0C)
	{
		/* Music to play */
		mus = new music(f);
		wclog_write(wcrpg_get_log(),"M=");
		wclog_write(wcrpg_get_log(),mus->get_file());
		wclog_write(wcrpg_get_log()," ");
		return 1;
	}
	if(code == 0x03)
	{
		/* Depth */
		depth = wcint_read_file(f,&rlen);
		return 1;
	}
	if(code == 0x07)
	{
		/* Children */
		has_children = wcint_read_file(f,&rlen);
		return 1;
	}
	if(code == 0x15)
	{
		/* Battle back Id */
		battle_background = wcint_read_file(f,&rlen);
		return 1;
	}
	if(code == 0x16)
	{
		/* Battle file */
		battle_background_file = wcstr_read_file(f,len);
		wclog_write(wcrpg_get_log(),"BK=");
		wclog_write(wcrpg_get_log(),battle_background_file);
		wclog_write(wcrpg_get_log()," ");
		return 1;
	}
	if(code == 0x1F)
	{
		/* Teleport */
		teleport_option = wcint_read_file(f,&rlen);
		return 1;
	}
	if(code == 0x20)
	{
		/* Escape */
		escape_option = wcint_read_file(f,&rlen);
		return 1;
	}
	if(code == 0x21)
	{
		/* Save */
		save_option = wcint_read_file(f,&rlen);
		return 1;
	}
	if(code == 0x2C)
	{
		/* Encounter */
		encounter_steps = wcint_read_file(f,&rlen);
		return 1;
	}
	return 0;
}

/* Create node */
treenode :: treenode(FILE *f)
{
	int code,len,rlen;
	unsigned char *d;
	/* Nullify */
	mus = 0;
	name = 0;
	battle_background_file = 0;
	parent = 0;
	has_children = 0;
	depth = 0;
	encounter_steps = 0;
	wclog_write(wcrpg_get_log(),"(");
	id = wcint_read_file(f,&rlen);
	wclog_write(wcrpg_get_log(),"#");
	wclog_write(wcrpg_get_log(),id);
	wclog_write(wcrpg_get_log()," ");
	/* Read until stop code */
	for(;;)
	{
		/* Get code */
		code = 0;
		fread(&code,1,1,f);
		/* Stop? */
		if(!code)
			break;
		/* Length */
		len = wcint_read_file(f,&rlen);
		if(!apply(f,code,len))
		{
			/* Skip Data */
			d = (unsigned char*)malloc(len);
			fread(d,1,len,f);
			free(d);
		}
	}
	/* Finish entry */
	wclog_write(wcrpg_get_log(),")");
	wclog_enter(wcrpg_get_log());
}

/* Delete node */
treenode :: ~treenode()
{
	/* Release */
	if(mus)
		delete mus;
	free(name);
	free(battle_background_file);
}

/* Am I a map? */
int treenode :: is_map()
{
	return (type == NODE_MAP);
}

/* Get music */
music *treenode :: get_music()
{
	treenode *tn;
	/* I can inherit properties! */
	if(music_mode == NODE_INHERIT && parent)
	{
		tn = wcmaptree_get(parent);
		return tn->get_music();
	}
	/* Don't do a thing */
	if(music_mode == NODE_UNCHANGED)
		return 0;
	/* Mode specify */
	return mus;
}

/* Get ID */
int treenode :: get_id()
{
	return id;
}

/* Get start map ID */
int wcmaptree_get_start_id(int kind)
{
	return wcmaptree_start_id[kind];
}

/* Get start map X */
int wcmaptree_get_start_x(int kind)
{
	return wcmaptree_start_x[kind];
}

/* Get start map Y */
int wcmaptree_get_start_y(int kind)
{
	return wcmaptree_start_y[kind];
}