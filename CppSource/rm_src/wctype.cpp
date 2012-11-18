/*
	Converts from RPG ints
*/

/* Includes */
#include <malloc.h>
#include <memory.h>
#include "wctype.h"
#include "wcrpg.h"

#include <string.h>

/* Continue flag */
int wcint_continue(unsigned char b)
{
	/* True */
	if(b&(128))
		return 1;
	/* False */
	return 0;
}

/* Get actual contribution of a byte */
int wcint_get(unsigned char b,int lv)
{
	int base;
	/* Figure out the base */
	base = (1<<(7*lv));
	/* Erase 128th flag */
	b = (b&127);
	/* Add them together */
	return base*b;
}

/* Returns the length of the rpg int beginning at location */
int wcint_length(void *v)
{
	unsigned char *src;
	int i;
	/* Point to source */
	src = (unsigned char*)v;
	/* Scan the bytes */
	for(i = 0;i < WCINT_MAX_BYTES;i++)
	{
		if(!wcint_continue(src[i]))
			break;
	}
	/* Measured */
	return i+1;
}

/* Reads the rpg int beginning at the memory location given */
int wcint_read(void *v)
{
	unsigned char buffer[WCINT_MAX_BYTES];
	unsigned char *src;
	int lvls,i,accu;
	/* Point to source */
	src = (unsigned char*)v;
	memset(buffer,0,sizeof(buffer));
	/* Read the bytes */
	lvls = wcint_length(v);
	for(i = 0;i < lvls;i++)
		buffer[i] = src[i];
	/* Accumulate */
	accu = 0;
	for(i = 0;i < lvls;i++)
		accu += wcint_get(buffer[i],lvls-1-i);
	/* Done */
	return accu;
}

/* Advances the pointer to the data after int */
void *wcint_next(void *v)
{
	unsigned char *p;
	p = (unsigned char*)v;
	p += wcint_length(v);
	return p;
}

/* Reads a string */
char *wcstr_read(void *v)
{
	char *c;
	int len;
	/* Get length */
	len = wcint_read(v);
	v = wcint_next(v);
	/* Allocate */
	c = (char*)malloc(len+1);
	memset(c,0,len+1);
	/* Read */
	memcpy(c,v,len);
	/* Return */
	return c;
}

/* Reads a string of known length */
char *wcstr_read(void *v,int len)
{
	char *c;
	/* Allocate */
	c = (char*)malloc(len+1);
	memset(c,0,len+1);
	/* Read */
	memcpy(c,v,len);
	/* Return */
	return c;
}

/* Reads an rpg int from file */
int wcint_read_file(FILE *f,int *rlen)
{
	char buffer[WCINT_MAX_BYTES];
	int i;
	/* Read the entire integer */
	for(i = 0;i < WCINT_MAX_BYTES;i++)
	{
		fread(&buffer[i],1,1,f);
		if(!wcint_continue(buffer[i]))
		{
			*rlen = i+1;
			return wcint_read(buffer);
		}
	}
	/* Error */
	return 0;
}

/* Reads a string from a file */
char *wcstr_read_file(FILE *f,int *rlen)
{
	char *c;
	int len;
	/* Get len */
	len = wcint_read_file(f,rlen);
	c = (char*)malloc(len+1);
	memset(c,0,len+1);
	fread(c,1,len,f);
	*rlen += len;
	/* Return */
	return c;
}

/* Reads a string from with with length known */
char *wcstr_read_file(FILE *f,int len)
{
	char *c;
	/* Read */
	c = (char*)malloc(len+1);
	memset(c,0,len+1);
	fread(c,1,len,f);
	/* Return */
	return c;
}