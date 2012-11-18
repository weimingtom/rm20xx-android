/*
	Vocabulary
*/

/* Includes */
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include "wcrpg.h"
#include "wcarray.h"
#include "wcvocab.h"
#include "wctype.h"
#include "wclog.h"

#include <string.h>

/* Globals */
arraylist wcvocab_list;

/* Creates vocabulary from file */
vocabulary *wcvocab_read(FILE *f,int *tlen)
{
	int len,code,rlen;
	vocabulary *v;
	/* Total length */
	*tlen = 0;
	/* Get code */
	/*code = 0;
	fread(&code,1,1,f);*/
	code = wcint_read_file(f,&rlen);
	*tlen += rlen;
	/* Stop */
	if(!code)
		return 0;
	/* Get len */
	len = wcint_read_file(f,&rlen);
	*tlen += rlen;
	/* Alloc */
	v = (vocabulary*)malloc(sizeof(vocabulary));
	v->id = code;
	v->str = (char*)malloc(len+1);
	memset(v->str,0,len+1);
	/* Read */
	fread(v->str,1,len,f);
	*tlen += len;
	/* Return */
	return v;
}

/* Releases vocabulary */
void wcvocab_free(void *ptr)
{
	vocabulary *v;
	v = (vocabulary*)ptr;
	free(v->str);
	free(v);
}

/* Loads all vocabulary */
void wcvocab_load_all(FILE *f,int len)
{
	int rlen;
	vocabulary *v;
	wcvocab_list = wcarray_new();
	wcarray_set_clean(wcvocab_list,wcvocab_free);
	/* Read all strings */
	while(len > 0)
	{
		v = wcvocab_read(f,&rlen);
		len -= rlen;
		if(!v)
			break;
		wclog_write(wcrpg_get_log(),v->id);
		wclog_write(wcrpg_get_log(),":");
		wclog_write(wcrpg_get_log(),v->str);
		wclog_enter(wcrpg_get_log());
		wcarray_add(wcvocab_list,v);
	}
	/* Error? */
	if(len != 0)
		wcrpg_error("Garbage data detected in vocabulary (%d)",len);
}

/* Releases vocab */
void wcvocab_free_all()
{
	wcarray_delete(wcvocab_list);
}

/* Requests a vocabulary */
char *wcvocab_get(int code)
{
	int i;
	vocabulary *v;
	/* Seek */
	for(i = 0;i < code;i++)
	{
		v = (vocabulary*)wcarray_get(wcvocab_list,i);
		/* Found */
		if(v->id == code)
			return v->str;
	}
	/* Unfound */
	wcrpg_error("Could not find vocabulary string (%d)",code);
	return 0;
}