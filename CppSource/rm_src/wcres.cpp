/*
	Resource List and Request
*/

/* Includes */
//#include <windows.h>
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include "wcrpg.h"
#include "wcres.h"

#include <android\log.h>

#include <vector>
#include <string>

#include <dirent.h>

#include <sstream>


using namespace std;




/* Globals */
arraylist wcres_rtp_folders;

/* Register fallbacks */
void wcres_register_rtp()
{
	wcres_rtp_folders = wcarray_new();
	wcarray_set_clean(wcres_rtp_folders,free);
	wcarray_add(wcres_rtp_folders,wcrpg_string_clone(WCRES_RTP_FOLDER));
	wcarray_add(wcres_rtp_folders,wcrpg_string_clone(WCRES_OLD_RTP_FOLDER));
	wcarray_add(wcres_rtp_folders,wcrpg_string_clone(WCRES_JAPN_RTP_FOLDER));
	wcarray_add(wcres_rtp_folders,wcrpg_string_clone(WCRES_RTP_FOLDER_64));
	wcarray_add(wcres_rtp_folders,wcrpg_string_clone(WCRES_OLD_RTP_FOLDER_64));
	wcarray_add(wcres_rtp_folders,wcrpg_string_clone(WCRES_JAPN_RTP_FOLDER_64));
}

/* Unregister fallbacks */
void wcres_unregister_rtp()
{
	wcarray_delete(wcres_rtp_folders);
}

/* Create */
reslist :: reslist(char *folder)
{
	
	char buffer[260];
	char addbuffer[260];	
	sprintf(buffer,"%s%s/","/sdcard/wcr2kxx/assets/",folder);
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "reslist [%s]",buffer);
	list = wcarray_new();

	DIR *dir;
	struct dirent *ent;
	dir = opendir (buffer);
	if (dir != NULL) {

		/* print all the files and directories within directory */
		while ((ent = readdir (dir)) != NULL) {
			if (strcmp(ent->d_name, ".") &&
				strcmp(ent->d_name, "..")){
					__android_log_print(ANDROID_LOG_INFO, "adMOB","%s", ent->d_name);
					//sprintf(addbuffer,"%s%s/%s","/sdcard/wcr2kxx/assets/",folder,ent->d_name);
					sprintf(addbuffer,"%s/%s",folder,ent->d_name);
					wcarray_add(list,wcrpg_string_clone(addbuffer));
			}
		}
		closedir (dir);  
	} 


	//travelDirectoryRecursive(buffer);

	//char buffer[260];
	//char addbuffer[260];
	//WIN32_FIND_DATAA fd;
	//HANDLE hf;
	//int i;
	///* Make name */
	//source = folder;
	//sprintf_s(buffer,sizeof(buffer),"%s%s/*",wcrpg_get_root(),folder);
	///* Allocate */
	//list = wcarray_new();
	///* Find the first file */
	//hf = FindFirstFileA(buffer,&fd);
	//if(hf == INVALID_HANDLE_VALUE)
	//{
	//	/* No files I guess */
	//	wclog_write(wcrpg_get_log(),"Nothing found for: ");
	//	wclog_write(wcrpg_get_log(),buffer);
	//	wclog_enter(wcrpg_get_log());
	//}
	//else
	//{
	//	/* Go through all possible candidates */
	//	for(;;)
	//	{
	//		/* If it's a valid file name and not a directory, add */
	//		if((fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) == 0)
	//		{
	//			/* Don't use hidden files */
	//			if((fd.dwFileAttributes&FILE_ATTRIBUTE_HIDDEN) == 0)
	//			{
	//				sprintf_s(addbuffer,sizeof(addbuffer),"%s%s/%s",wcrpg_get_root(),folder,fd.cFileName);
	//				wcarray_add(list,wcrpg_string_clone(addbuffer));
	//			}
	//		}
	//		/* Get next and done */
	//		if(!FindNextFileA(hf,&fd))
	//			break;
	//	}
	//}
	///* Then index the RTP AFTER so that it falls back */
	//for(i = 0;i < wcarray_size(wcres_rtp_folders);i++)
	//{
	//	sprintf_s(buffer,sizeof(buffer),"%s%s/*",(char*)wcarray_get(wcres_rtp_folders,i),folder);
	//	hf = FindFirstFileA(buffer,&fd);
	//	if(hf != INVALID_HANDLE_VALUE)
	//	{
	//		/* Go through all possible candidates */
	//		for(;;)
	//		{
	//			/* If it's a valid file name and not a directory, add */
	//			if((fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) == 0)
	//			{
	//				/* Don't use hidden files */
	//				if((fd.dwFileAttributes&FILE_ATTRIBUTE_HIDDEN) == 0)
	//				{
	//					sprintf_s(addbuffer,sizeof(addbuffer),"%s%s/%s",(char*)wcarray_get(wcres_rtp_folders,i),folder,fd.cFileName);
	//					wcarray_add(list,wcrpg_string_clone(addbuffer));
	//				}
	//			}
	//			/* Get next and done */
	//			if(!FindNextFileA(hf,&fd))
	//				break;
	//		}
	//	}
	//}
}

/* Free */
reslist :: ~reslist()
{
	/* Release */
	wcarray_delete(list);
}

/* Size */
int reslist :: size()
{
	return wcarray_size(list);
}

/* Report */
void reslist :: report(elog l)
{
	wclog_write(l,source);
	wclog_write(l," : ");
	wclog_write(l,size());
	wclog_write(l," files.");
	wclog_enter(l);
}

/* Request */
char *reslist :: get(int id)
{
	return (char*)wcarray_get(list,id);
}

/* Return basic name */
char *reslist :: get_name(int id)
{
	int i,n;
	char *c;
	c = (char*)wcarray_get(list,id);
	n = 0;
	for(i = 0;i < (int)strlen(c);i++)
	{
		if(c[i] == '/' || c[i] == '\\')
			n = i+1;
	}
	return &c[n];
}

/* Returns a lower case version */
char wcres_lower(char c)
{
	/* Is upper alpha */
	if(c >= 'A' && c <= 'Z')
	{
		c = c-'A'; /* Normalize */
		c += 'a';
	}
	/* Passthrough */
	return c;
}

/* String compare ignore case */
int wcres_strcmp(char *a,char *b)
{
	char *ac,*bc;
	int i,len,r;
	/* Go ahead and make copies.. */
	ac = wcrpg_string_clone(a);
	bc = wcrpg_string_clone(b);
	/* Lower case them all */
	len = (int)strlen(ac);
	for(i = 0;i < len;i++)
		ac[i] = wcres_lower(ac[i]);
	len = (int)strlen(bc);
	for(i = 0;i < len;i++)
		bc[i] = wcres_lower(bc[i]);
	/* THEN strcmp them */
	r = strcmp(ac,bc);
	free(ac);
	free(bc);
	return r;
}

/* Request */
char *reslist :: request(char *name)
{
	int i,j,enh;
	int namestart;
	char buffer[260];
	char *cc;
	/* Nullify */
	if(!name)
		return 0;
	/* Make name looking for 20XX file */
	sprintf(buffer,"%s-2x",name);
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "res::request [%s]",buffer);
	/* Loop */
	for(enh = 1;enh >= 0;enh--)
	{
		for(i = 0;i < wcarray_size(list);i++)
		{
			/* Get and clone */
			cc = wcrpg_string_clone((char*)wcarray_get(list,i));
			/* Does it compare with full path? */
			if(!wcres_strcmp(name,cc))
			{
				free(cc);
				return (char*)wcarray_get(list,i);
			}
			/* Find last instance of / or \\ */
			namestart = 0;
			for(j = 0;j < (int)strlen(cc);j++)
			{
				/* Set name start */
				if(cc[j] == '/' || cc[j] == '\\')
					namestart = j+1;
			}
			/* Delete last dot from the right */
			for(j = (int)strlen(cc)-1;j >= 0;j--)
			{
				if(cc[j] == '.')
				{
					cc[j] = 0;
					break;
				}
			}
			/* Compare */
			if(enh)
			{
				if(!wcres_strcmp(buffer,&cc[namestart]))
				{
					/* Matched 20XX enhanced file */
					free(cc);
					return (char*)wcarray_get(list,i);
				}
			}
			else
			{
				if(!wcres_strcmp(name,&cc[namestart]))
				{
					/* Matched */
					free(cc);
					return (char*)wcarray_get(list,i);
				}
			}
			/* Continue on */
			free(cc);
		}
	}
	/* None found */
	wcrpg_error("Could not find resource %s (in %s)",name,source);
	return 0;
}

/* Check requesting */
int wcres_compare_index_filename(int ix,char *name)
{
	int len,i,ret;
	char *cc,*cn;
	/* Make a blank copy matching the given name string */
	len = (int)strlen(name)+1;
	cc = (char*)malloc(len);
	memset(cc,0,len);
	/* Add up all alpha characters */
	for(i = 0;i < len;i++)
	{
		if(name[i] >= '0' && name[i] <= '9')
			cc[i] = name[i];
		else
			break;
	}
	/* Create a string copy of ix */
	cn = (char*)malloc(len);
	memset(cn,0,len);
	sprintf(cn,"%d",ix);
	/* Return the compare */
	ret = !strcmp(cn,cc);
	/* Free */
	free(cn);
	free(cc);
	/* Return result */
	return ret;
}

/* Request index */
char *reslist :: request_index(int ix)
{
	int i,j;
	int namestart;
	char *cc;
	/* Loop */
	for(i = 0;i < wcarray_size(list);i++)
	{
		/* Get and clone */
		cc = wcrpg_string_clone((char*)wcarray_get(list,i));
		/* Find last instance of / or \\ */
		namestart = 0;
		for(j = 0;j < (int)strlen(cc);j++)
		{
			/* Set name start */
			if(cc[j] == '/' || cc[j] == '\\')
				namestart = j+1;
		}
		/* Delete last dot from the right */
		for(j = (int)strlen(cc)-1;j >= 0;j--)
		{
			if(cc[j] == '.')
			{
				cc[j] = 0;
				break;
			}
		}
		/* Compare */
		if(wcres_compare_index_filename(ix,&cc[namestart]))
		{
			/* Matched */
			free(cc);
			return (char*)wcarray_get(list,i);
		}
		/* Continue on */
		free(cc);
	}
	/* None found */
	wcrpg_error("Could not find file name indexed resource %d",ix);
	wcrpg_error(".. Which was located in %s",source);
	return 0;
}