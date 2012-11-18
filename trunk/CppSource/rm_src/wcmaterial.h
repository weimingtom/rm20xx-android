#ifndef WCMATERIAL_H
#define WCMATERIAL_H

/* Defines */
#define MATERIAL_FOLDER_COUNT 20
#define MATERIAL_FOLDER_NAME_LENGTH 24

#define MATERIAL_BACKDROP 0
#define MATERIAL_BATTLE 1
#define MATERIAL_BATTLE_2 2
#define MATERIAL_BATTLE_CHARSET 3
#define MATERIAL_BATTLE_WEAPON 4
#define MATERIAL_CHARSET 5
#define MATERIAL_CHIPSET 6
#define MATERIAL_FACESET 7
#define MATERIAL_FRAME 8
#define MATERIAL_GAME_OVER 9
#define MATERIAL_MONSTER 10
#define MATERIAL_MOVIE 11
#define MATERIAL_MUSIC 12
#define MATERIAL_PANORAMA 13
#define MATERIAL_PICTURE 14
#define MATERIAL_SOUND 15
#define MATERIAL_SYSTEM 16
#define MATERIAL_SYSTEM_2 17
#define MATERIAL_TITLE 18
#define MATERIAL_EFFECT 19

/*
	Returns a folder name for material index
*/
extern char *wcmaterial_folder_name(int ix);

/*
	Prepares all resources for request and load operations
*/
extern void wcmaterial_prepare();

/*
	Closes all resources
*/
extern void wcmaterial_close();

/*
	Requests a resource filename
*/
extern char *wcmaterial_request(int type,char *name);

/*
	Requests a resource filename by id
*/
extern char *wcmaterial_request(int type,int id);

/*
	Requests a resource filename by file name index
*/
extern char *wcmaterial_request_filename_index(int type,int ix);

/*
	Count number of resources
*/
extern int wcmaterial_count(int type);

/*
	Gets the basic name of a resource at index given
*/
extern char *wcmaterial_name(int type,int id);

#endif