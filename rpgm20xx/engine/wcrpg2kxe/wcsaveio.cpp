/*
	Save I/O
*/

/* Includes */
#include <malloc.h>
#include <memory.h>
#include <string.h>
#include "wcsaveio.h"
#include "wcarray.h"

/* Globals */
arraylist wcsaveio_buffer = 0;
FILE *wcsaveio_file = 0;
int wcsaveio_max = 0;

/* Starts a save block */
void wcsaveio_start(FILE *f)
{
	/* Create new buffer */
	wcsaveio_buffer = wcarray_new();
	/* Assign file pointer */
	wcsaveio_file = f;
}

/* Ends a save block */
void wcsaveio_end()
{
	unsigned char *b;
	int i,len;
	/* Write the length of the buffer first */
	len = wcarray_size(wcsaveio_buffer);
	fwrite(&len,4,1,wcsaveio_file);
	/* Allocate write buffer */
	b = (unsigned char*)malloc(len);
	/* Load it */
	//for(i = 0;i < len;i++)
	//	b[i] = (unsigned char)wcarray_get(wcsaveio_buffer,i);
	/* Flush it */
	fwrite(b,1,len,wcsaveio_file);
	/* Done, erase data */
	free(b);
	wcarray_delete(wcsaveio_buffer);
	wcsaveio_buffer = 0;
}

/* Writes to save block */
void wcsaveio_write(unsigned char b)
{
	wcarray_add(wcsaveio_buffer,(void*)b);
}

/* Writes number */
void wcsaveio_write_num(int n)
{
	unsigned char *b;
	b = (unsigned char*)&n;
	wcsaveio_write(b[0]);
	wcsaveio_write(b[1]);
	wcsaveio_write(b[2]);
	wcsaveio_write(b[3]);
}

/* Writes float */
void wcsaveio_write_float(float f)
{
	unsigned char *b;
	b = (unsigned char*)&f;
	wcsaveio_write(b[0]);
	wcsaveio_write(b[1]);
	wcsaveio_write(b[2]);
	wcsaveio_write(b[3]);
}

/* Writes string */
void wcsaveio_write_str(char *str)
{
	int len,i;
	if(!str)
	{
		/* Null string */
		wcsaveio_write_num(0);
		return;
	}
	len = (int)strlen(str);
	wcsaveio_write_num(len);
	for(i = 0;i < len;i++)
		wcsaveio_write(str[i]);
}

/* Start read */
void wcsaveio_read(FILE *f)
{
	/* Assigne file and read length */
	wcsaveio_file = f;
	fread(&wcsaveio_max,4,1,f);
}

/* Reads but a single byte */
unsigned char wcsaveio_read_byte()
{
	unsigned char b;
	/* Out of space */
	if(wcsaveio_max < 1)
		return 0;
	fread(&b,1,1,wcsaveio_file);
	wcsaveio_max--;
	return b;
}

/* Reads number */
int wcsaveio_read_num()
{
	int n;
	/* Out of space */
	if(wcsaveio_max < 4)
		return 0;
	fread(&n,4,1,wcsaveio_file);
	wcsaveio_max -= 4;
	return n;
}

/* Reads float */
float wcsaveio_read_float()
{
	float f;
	/* Out of space */
	if(wcsaveio_max < 4)
		return 0.0f;
	fread(&f,4,1,wcsaveio_file);
	wcsaveio_max -= 4;
	return f;
}

/* Reads and allocates string */
char *wcsaveio_read_str()
{
	char *str;
	int len;
	/* Get len */
	len = wcsaveio_read_num();
	if(!len)
		return 0;
	/* Allocate */
	str = (char*)malloc(len+1);
	memset(str,0,len+1);
	/* Can we read all? */
	if(wcsaveio_max >= len)
	{
		fread(str,1,len,wcsaveio_file);
		wcsaveio_max -= len;
	}
	else
	{
		fread(str,1,wcsaveio_max,wcsaveio_file);
		wcsaveio_max = 0;
	}
	/* Done */
	return str;
}

/* Finish reading */
void wcsaveio_next()
{
	unsigned char *b;
	/* Our work here is done */
	if(wcsaveio_max <= 0)
		return;
	/* If not, read rest */
	b = (unsigned char*)malloc(wcsaveio_max);
	fread(b,1,wcsaveio_max,wcsaveio_file);
	free(b);
	wcsaveio_max = 0;
}