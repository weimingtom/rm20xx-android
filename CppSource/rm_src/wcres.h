#ifndef WCRES_H
#define WCRES_H

/* Defines */
#define WCRES_RTP_FOLDER "C:/Program Files/rpg2003/RTP/"
#define WCRES_OLD_RTP_FOLDER "C:/Program Files/ASCII/RPG2000/RTP/"
#define WCRES_JAPN_RTP_FOLDER "C:/Program Files/Enterbrain/RPG2003/RTP/"
#define WCRES_RTP_FOLDER_64 "C:/Program Files (x86)/rpg2003/RTP/"
#define WCRES_OLD_RTP_FOLDER_64 "C:/Program Files (x86)/ASCII/RPG2000/RTP/"
#define WCRES_JAPN_RTP_FOLDER_64 "C:/Program Files (x86)/Enterbrain/RPG2003/RTP/"

/* Includes */
#include "wcarray.h"
#include "wclog.h"

/* Classes */
class reslist
{
private:
	char *source;
	arraylist list;
public:
	/*
		Creates a new resource list from folder given.
	*/
	reslist(char *folder);
	~reslist();
	/*
		Returns the number of iles in this resource list.
	*/
	int size();
	/*
		Reports a summary of this object to the log.
	*/
	void report(elog l);
	/*
		Returns a file name with just the resource name of the file.
		Ex: ChipSet/tiles.png is returned if tiles is provided.
	*/
	char *request(char *name);
	/*
		Returns a file name with an id in this list, usually alphabetically listed (but not always)
		Ex: id = 3 is the 4th file in the list
	*/
	char *get(int id);
	/*
		Gets the basic name of a resource
	*/
	char *get_name(int id);
	/*
		Returns a file name beginning with the number passed to this method
		Ex: Effect/0.fx is returned if 0 is provided.
		*DOES NOT HANDLE -2X REPLACEMENTS*
	*/
	char *request_index(int ix);
};

/*
	Register fallbacks
*/
extern void wcres_register_rtp();

/*
	Unregister fallbacks
*/
extern void wcres_unregister_rtp();

#endif