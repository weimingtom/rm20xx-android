/*
	RPG Database
*/

/* Includes */
////#include <windows.h>
#include <string.h>

#include <malloc.h>
#include <stdio.h>
#include "wcrpg.h"
#include "wctype.h"
#include "wcdatabase.h"
#include "wclog.h"
#include "wcarray.h"
#include "wcsystem.h"
#include "wchero.h"
#include "wcitem.h"
#include "wcvocab.h"
#include "wcmem.h"
#include "wccommon.h"
#include "wcanimation.h"
#include "wcbattleset.h"
#include "wcattribute.h"
#include "wcskill.h"
#include "wcmonster.h"
#include "wcgroup.h"
#include "wcterrain.h"
#include "wcbattlecommand.h"

#include "../jniUtil.h"

/* Globals */
arraylist tilesets;
char wcdatabase_unknown_section_buffer[260];

/* Returns */
int wcdatabase_is_junk(int code)
{
	if(code == 0x1A)
		return 1;
	if(code == 0x1B)
		return 1;
	if(code == 0x1C)
		return 1;
	if(code == 0x1F)
		return 1;
	return 0;
}

/* Returns the name of the section */
char *wcdatabase_identify_section(int code)
{
	/* Code is known */
	switch(code)
	{
	case DATA_HEROES:
		return "Heroes";
	case DATA_SKILLS:
		return "Skills";
	case DATA_ITEMS:
		return "Items";
	case DATA_MONSTER:
		return "Monsters";
	case DATA_GROUP:
		return "Groups";
	case DATA_TERRAIN:
		return "Terrain";
	case DATA_ATTRIBUTE:
		return "Attribute";
	case DATA_CONDITIONS:
		return "Conditions";
	case DATA_ANIMATION:
		return "Animations";
	case DATA_TILESETS:
		return "Tilesets";
	case DATA_VOCAB:
		return "Vocabulary";
	case DATA_SETTINGS:
		return "Settings";
	case DATA_SWITCHES:
		return "Switches";
	case DATA_VARIABLES:
		return "Variables";
	case DATA_COMMON_EVENTS:
		return "Common Events";
	case DATA_BATTLESETS:
		return "Battlesets";
	case DATA_CLASSES:
		return "Classes";
	case DATA_COMMANDS:
		return "Commands";
	}
	/* Code is unknown */
	memset(wcdatabase_unknown_section_buffer,0,sizeof(wcdatabase_unknown_section_buffer));
	sprintf(wcdatabase_unknown_section_buffer,"Unknown %X",code);
	return wcdatabase_unknown_section_buffer;
}

/* Dump section */
void wcdatabase_dump_section(unsigned char *data,int len,char *name)
{
	char buffer[260];
	FILE *f;
	sprintf(buffer,"%s.dat",name);
	//fopen(buffer,"wb");
	//fwrite(data,1,len,f);
	//fclose(f);
}

/* Parse section if supported */
int wcdatabase_process_section(int code,FILE *f,int len)
{
	/* Mark */
	wclog_write(wcrpg_get_log(),"Processing ");
	wclog_write(wcrpg_get_log(),wcdatabase_identify_section(code));
	wclog_write(wcrpg_get_log()," (");
	wclog_write(wcrpg_get_log(),len);
	wclog_write(wcrpg_get_log()," bytes).");
	wclog_enter(wcrpg_get_log());
	/* Load heroes */
	if(code == DATA_HEROES)
	{
		__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcdatabase_process_section [0]");
		wchero_load_all(f,len);
		return 1;
	}
	/* Load skills */
	if(code == DATA_SKILLS)
	{
		__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcdatabase_process_section [1]");
		wcskill_load_all(f,len);
		return 1;
	}
	/* Load items */
	if(code == DATA_ITEMS)
	{
		__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcdatabase_process_section [2]");
		wcitem_load_all(f,len);
		return 1;
	}
	/* Load monsters */
	if(code == DATA_MONSTER)
	{
		__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcdatabase_process_section [3]");
		wcmonster_load_all(f,len);
		return 1;
	}
	/* Load battle commands */
	if(code == DATA_COMMANDS)
	{
		__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcdatabase_process_section [4]");
		wcbattlecommand_load_all(f,len);
		return 1;
	}
	/* Load groups */
	if(code == DATA_GROUP)
	{
		__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcdatabase_process_section [5]");
		wcgroup_load_all(f,len);
		return 1;
	}
	/* Load terrains */
	if(code == DATA_TERRAIN)
	{
		__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcdatabase_process_section [6]");
		wcterrain_load_all(f,len);
		return 1;
	}
	/* Load attributes */
	if(code == DATA_ATTRIBUTE)
	{
		__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcdatabase_process_section [7]");
		wcattribute_load_all(f,len);
		return 1;
	}
	/* Load tilesets */
	if(code == DATA_TILESETS)
	{
		__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcdatabase_process_section [8]");
		tilesets = wctileset_load_all(f,len);
		return 1;
	}
	/* Load system */
	if(code == DATA_SETTINGS)
	{
		__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcdatabase_process_section [9]");
		wcsystem_load_all(f,len);
		return 1;
	}
	/* Load vocab */
	if(code == DATA_VOCAB)
	{
		__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcdatabase_process_section [10]");
		wcvocab_load_all(f,len);
		return 1;
	}
	/* Load switches */
	if(code == DATA_SWITCHES)
	{
		__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcdatabase_process_section [11]");
		wcmem_load_all_switches(f,len);
		return 1;
	}
	/* Load variables */
	if(code == DATA_VARIABLES)
	{
		__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcdatabase_process_section [12]");
		wcmem_load_all_variables(f,len);
		return 1;
	}
	/* Load common */
	if(code == DATA_COMMON_EVENTS)
	{
		__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcdatabase_process_section [13]");
		wccommon_load_all(f,len);
		return 1;
	}
	/* Load animations */
	if(code == DATA_ANIMATION)
	{
		__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcdatabase_process_section [14]");
		wcanimation_load_all(f,len);
		return 1;
	}
	/* Load battlesets */
	if(code == DATA_BATTLESETS)
	{
		__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcdatabase_process_section [15]");
		wcbattleset_load_all(f,len);
		return 1;
	}
	/* Unsupported feature */
	return 0;
}

/* Dump sections */
#ifndef NDEBUG
void wcdatabase_dump_sections(FILE *f)
{
	int code,len,rlen;
	unsigned char *data;
	/* In end-user performance check mode, they do not care about binary dumps */
#ifdef WCPERFORM
	return;
#endif
	/* Read codes until common events */
	for(;;)
	{
		/* Read code and length */
		code = wcint_read_file(f,&rlen);
		len = wcint_read_file(f,&rlen);
		/* Report code and length */
		wclog_write(wcrpg_get_log(),wcdatabase_identify_section(code));
		wclog_write(wcrpg_get_log()," (");
		wclog_write(wcrpg_get_log(),len);
		wclog_write(wcrpg_get_log()," bytes)");
		wclog_enter(wcrpg_get_log());
		/* Read it in */
		data = (unsigned char*)malloc(len);
		fread(data,1,len,f);
		/* Dump it */
		if(!wcdatabase_is_junk(code))
			wcdatabase_dump_section(data,len,wcdatabase_identify_section(code));
		/* Discard if not using */
		free(data);
		/* Was that the end? */
		if(code == DATA_BATTLESETS && !wcrpg_old_mode())
			break;
		if(code == DATA_COMMON_EVENTS && wcrpg_old_mode())
			break;
		/* Unexpected end of file */
		if(feof(f))
		{
			wcrpg_set_old_mode();
			break;
		}
	}
}
#endif
#ifdef NDEBUG
void wcdatabase_dump_sections(FILE *f){} /* Stub it */
#endif

/* Read section */
void wcdatabase_load_sections(FILE *f)
{
	int code,len,rlen;
	unsigned char *data;
	/* Read codes until common events */
	for(;;)
	{
		/* Read code and length */
		code = wcint_read_file(f,&rlen);
		len = wcint_read_file(f,&rlen);
		/* Process */
		__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcdatabase_load_sections [0]");
		if(!wcdatabase_process_section(code,f,len))
		{
			__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcdatabase_load_sections [1]");
			/* Skipping */
			wclog_write(wcrpg_get_log(),"Skipping ");
			wclog_write(wcrpg_get_log(),wcdatabase_identify_section(code));
			wclog_enter(wcrpg_get_log());
			/* Read it in */
			__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcdatabase_load_sections [2]");
			data = (unsigned char*)malloc(len);
			__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcdatabase_load_sections [3]");
			fread(data,1,len,f);
			__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcdatabase_load_sections [4]");
			/* Discard if not using */
			free(data);
			__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcdatabase_load_sections [5]");
		}
		/* Was that the end? */
		if(code == DATA_BATTLESETS && !wcrpg_old_mode())
			break;
		if(code == DATA_COMMON_EVENTS && wcrpg_old_mode())
			break;
		/* Unexpected end of file */
		__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcdatabase_load_sections [6]");
		if(feof(f))
		{
			__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcdatabase_load_sections [7]");
			wcrpg_set_old_mode();
			break;
		}
		__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcdatabase_load_sections [8]");
	}
}

/* Read database */
void wcdatabase_load()
{
	char buffer[260];
	FILE *f;
	char *filetype;
	int rlen;
	/* Open channel to file */
	sprintf(buffer,"%sRPG_RT.ldb","/sdcard/wcr2kxx/assets/");
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcdatabase %s",buffer);
	f=fopen(buffer,"rb");
	if(!f)
	{
		wcrpg_error("No such game found in %s",wcrpg_get_root());
		return;
	}
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcdatabase [1]");
	filetype = wcstr_read_file(f,&rlen);
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcdatabase [2]");
	//wcdatabase_dump_sections(f);
	fclose(f);
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcdatabase [3]");
	f=fopen(buffer,"rb");
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcdatabase [4]");
	/* Read type */
	filetype = wcstr_read_file(f,&rlen);
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcdatabase [5]");
	wclog_write(wcrpg_get_log(),"Loading database... (");
	wclog_write(wcrpg_get_log(),filetype);
	wclog_write(wcrpg_get_log(),")");
	wclog_enter(wcrpg_get_log());
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcdatabase [6]");
	/* Go */
	wcdatabase_load_sections(f);
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcdatabase [1]");
	/* End */
	wclog_write(wcrpg_get_log(),"Finished reading database.");
	wclog_enter(wcrpg_get_log());
	/* Finish */
	free(filetype);
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcdatabase [1]");
	fclose(f);
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcdatabase [1]");
}

/* Free database */
void wcdatabase_free()
{
	wcbattlecommand_free_all();
	wcgroup_free_all();
	wcmonster_free_all();
	wcattribute_free_all();
	wcbattleset_free_all();
	wcanimation_free_all();
	wccommon_free_all();
	wctileset_free_all(tilesets);
	wcsystem_free_all();
	wchero_free_all();
	wcvocab_free_all();
	wcmem_free_all();
	wcterrain_free_all();
}

/* Request a tileset */
tileset *wcdatabase_get_tileset(int id)
{
	/* Invalid request */
	if(id <= 0 || id > wcarray_size(tilesets))
	{
		wcrpg_error("Tileset request out of range (%d)",id);
		return 0;
	}
	/* Request */
	return (tileset*)wcarray_get(tilesets,id-1);
}