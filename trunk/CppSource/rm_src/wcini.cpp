/*
	.ini File Reader
*/

/* Includes */
//#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <memory.h>
#include "wcini.h"
#include "wcrpg.h"

#include <android\log.h>

#include "../jniUtil.h"

/* Counts the lines in a file */
int wcini_line_count(char *file)
{
	char buffer[260];
	FILE *f;
	int l;
	/* Open */
	f=fopen(file,"r");
	if(!f)
		return -1; /* Error */
	/* Count lines */
	l = 0;
	while(fgets(buffer,sizeof(buffer),f)){
		__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcini_line_count %s",buffer);
		l++;
	}
	fclose(f);
	/* Return that count */
	return l;
}

/* Is alpha */
int wcini_is_alpha(char c)
{
	/* Valid char ranges */
	if(c >= 'a' && c <= 'z')
		return 1;
	if(c >= 'A' && c <= 'Z')
		return 1;
	if(c >= '0' && c <= '9')
		return 1;
	/* Not a valid char */
	return 0;
}

/* Is entry */
int wcini_is_entry(char *c)
{
	int ec,len,i;
	/* Must start with legal char */
	if(!wcini_is_alpha(*c))
		return 0;
	/* Must have ONE equals sign */
	ec = 0;
	len = (int)strlen(c);
	for(i = 0;i < len;i++)
	{
		if(c[i] == '=')
			ec++;
	}
	if(ec != 1)
		return 0;
	/* Is entry */
	return 1;
}

/* Remove instances of c in str */
void wcini_remove_chars(char c,char *str)
{
	int i;
	for(i = 0;i < (int)strlen(str);i++)
	{
		if(str[i] == c)
			str[i] = 0;
	}
}

/* Clones field */
char *wcini_to_field(char *c)
{
	char *cc;
	int i;
	cc = wcrpg_string_clone(c);
	for(i = 0;i < (int)strlen(cc);i++)
	{
		if(cc[i] == '=')
		{
			cc[i] = 0;
			break;
		}
	}
	return cc;
}

/* Clones value */
char *wcini_to_value(char *c)
{
	int i;
	for(i = 0;i < (int)strlen(c);i++)
	{
		if(c[i] == '=')
			return wcrpg_string_clone(&c[i+1]);
	}
	return 0;
}

/* Create new */
ini :: ini(char *file)
{
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "ini[1]");
	char buffer[260];
	FILE *f;
	int i;
	/* Source */
	source = file;
	/* Count them lines */
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "ini[2]");
	lines = wcini_line_count(file);
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "ini[3] %d",lines);
	if(lines == -1)
	{
		wcrpg_error("Could not open %s",file);
		return;
	}
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "ini[4]");
	/* Prepare list */
	data = wcarray_new();
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "ini[5]");
	wcarray_set_clean(data,free);
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "ini[6] %s",file);
	/* Read */
	f=fopen(file,"r");
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "ini[7]");
	while(fgets(buffer,sizeof(buffer),f))
	{
		__android_log_print(ANDROID_LOG_INFO, "adMOB", "ini[8]");
		wcini_remove_chars('\n',buffer);
		wcarray_add(data,wcrpg_string_clone(buffer));
	}
	fclose(f);
	/* Parse data */
	fields = wcarray_new();
	wcarray_set_clean(fields,free);
	values = wcarray_new();
	wcarray_set_clean(values,free);
	for(i = 0;i < wcarray_size(data);i++)
	{
		/* Split */
		if(wcini_is_entry((char*)wcarray_get(data,i)))
		{
			wcarray_add(fields,wcini_to_field((char*)wcarray_get(data,i)));
			wcarray_add(values,wcini_to_value((char*)wcarray_get(data,i)));
		}
	}
}

/* Release */
ini :: ~ini()
{
	/* Release the lists */
	wcarray_delete(values);
	wcarray_delete(fields);
	wcarray_delete(data);
}

/* Get entry count */
int ini :: entries()
{
	return wcarray_size(fields);
}

/* Report */
void ini :: report(elog l)
{
	wclog_write(l,source);
	wclog_write(l," : ");
	wclog_write(l,entries());
	wclog_write(l," entries.");
	wclog_enter(l);
}

/* Get field */
char *ini :: get_value(char *field)
{
	int i;
	char *c;
	/* Seek */
	for(i = 0;i < wcarray_size(fields);i++)
	{
		c = (char*)wcarray_get(fields,i);
		/* Found? */
		if(!strcmp(c,field))
			return (char*)wcarray_get(values,i);
	}
	/* Not found */
	return 0;
}