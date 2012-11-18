/*
	Material Data Indexer
*/

/* Includes */
#include "wcrpg.h"
#include "wcmaterial.h"
#include "wcres.h"
#include "wcarray.h"

/* Globals */
char res_folder[MATERIAL_FOLDER_COUNT][MATERIAL_FOLDER_NAME_LENGTH] = {"Backdrop",
                                                                       "Battle",
																	   "Battle2",
																	   "BattleCharSet",
																	   "BattleWeapon",
																	   "CharSet",
																	   "ChipSet",
																	   "FaceSet",
																	   "Frame",
																	   "GameOver",
																	   "Monster",
																	   "Movie",
																	   "Music",
																	   "Panorama",
																	   "Picture",
																	   "Sound",
																	   "System",
																	   "System2",
																	   "Title",
																	   "Effect"};
reslist *res[MATERIAL_FOLDER_COUNT];

/* Get folder name */
char *wcmaterial_folder_name(int ix)
{
	return res_folder[ix];
}

/* Index all resources */
void wcmaterial_prepare()
{
	int i;
	/* Announce */
	wclog_write(wcrpg_get_log(),"Indexing resources...");
	wclog_enter(wcrpg_get_log());
	/* Load fallbacks */
	//wcres_register_rtp();
	/* Load all */
	for(i = 0;i < MATERIAL_FOLDER_COUNT;i++)
	{
		/* Index */
		res[i] = new reslist(wcmaterial_folder_name(i));
		/* Report */
		//res[i]->report(wcrpg_get_log());
	}
}

/* Release all resources */
void wcmaterial_close()
{
	int i;
	/* Release fallbacks */
	//wcres_unregister_rtp();
	/* Release all */
	for(i = 0;i < MATERIAL_FOLDER_COUNT;i++)
	{
		if(res[i])
			delete res[i];
		res[i] = 0;
	}
}

/* Request a resource */
char *wcmaterial_request(int type,char *name)
{
	/* Error type */
	if(type < 0 || type >= MATERIAL_FOLDER_COUNT)
	{
		wcrpg_error("Invalid material type (%d)",type);
		return 0;
	}
	/* Request */
	return res[type]->request(name);
}

/* Request a file name indexed resource */
char *wcmaterial_request_filename_index(int type,int ix)
{
	/* Error type */
	if(type < 0 || type >= MATERIAL_FOLDER_COUNT)
	{
		wcrpg_error("Invalid material type (%d)",type);
		return 0;
	}
	/* Request index */
	return res[type]->request_index(ix);
}

/* Request a resource by index */
char *wcmaterial_request(int type,int id)
{
	/* Error type */
	if(type < 0 || type >= MATERIAL_FOLDER_COUNT)
	{
		wcrpg_error("Invalid material type (%d)",type);
		return 0;
	}
	/* Request */
	return res[type]->get(id);
}

/* Request the count of resources */
int wcmaterial_count(int type)
{
	/* Error type */
	if(type < 0 || type >= MATERIAL_FOLDER_COUNT)
	{
		wcrpg_error("Invalid material type (%d)",type);
		return 0;
	}
	/* Request */
	return res[type]->size();
}

/* Get basic name */
char *wcmaterial_name(int type,int id)
{
	/* Error type */
	if(type < 0 || type >= MATERIAL_FOLDER_COUNT)
	{
		wcrpg_error("Invalid material type (%d)",type);
		return 0;
	}
	/* Request */
	return res[type]->get_name(id);
}