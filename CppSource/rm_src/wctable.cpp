/*
	Database Table
*/

/* Includes */
#include <malloc.h>
#include "wcrpg.h"
#include "wctable.h"
#include "wctype.h"

/* Loads entry from data */
entry :: entry(void *v)
{
	unsigned char *bv,*d;
	int *coded,*lend;
	int code,len,i,needle;
	/* Use byte size */
	size = 0;
	bv = (unsigned char*)v;
	/* Beginning */
	needle = 0;
	/* Alloc */
	codes = wcarray_new();
	lens = wcarray_new();
	datas = wcarray_new();
	/* Free data */
	wcarray_set_clean(lens,free);
	wcarray_set_clean(codes,free);
	wcarray_set_clean(datas,free);
	/* Get ID */
	id = wcint_read(bv);
	size += wcint_length(bv);
	bv += wcint_length(bv);
	/* Begin reading codes */
	for(;;)
	{
		/* Get code */
		code = 0;
		code = wcint_read(bv);
		size += wcint_length(bv);
		bv += wcint_length(bv);
		/* Last one? */
		if(!code)
			break; /* Finished */
		/* Get len */
		len = wcint_read(bv);
		size += wcint_length(bv);
		bv += wcint_length(bv);
		/* Read data */
		d = (unsigned char*)malloc(len);
		for(i = 0;i < len;i++)
			d[i] = bv[i];
		size += len;
		bv += len;
		/* Add to list */
		coded = (int*)malloc(sizeof(int));
		lend = (int*)malloc(sizeof(int));
		*coded = code;
		*lend = len;
		wcarray_add(codes,coded);
		wcarray_add(lens,lend);
		wcarray_add(datas,d);
	}
}

/* Gets the size */
int entry :: get_size()
{
	return size;
}

/* Loads entry */
entry :: entry(FILE *f)
{
	int code,len,rlen;
	int *coded,*lend;
	unsigned char *d;
	/* Zero size */
	size = 0;
	/* Alloc */
	codes = wcarray_new();
	lens = wcarray_new();
	datas = wcarray_new();
	/* Free data */
	wcarray_set_clean(lens,free);
	wcarray_set_clean(codes,free);
	wcarray_set_clean(datas,free);
	/* Get ID */
	id = wcint_read_file(f,&rlen);
	size += rlen;
	/* Begin reading codes */
	for(;;)
	{
		/* Get code */
		code = 0;
		fread(&code,1,1,f);
		size++;
		/* Last one? */
		if(!code)
			break; /* Finished */
		/* Get len */
		len = wcint_read_file(f,&rlen);
		size += rlen;
		/* Read data */
		d = (unsigned char*)malloc(len);
		fread(d,1,len,f);
		size += len;
		/* Add to list */
		coded = (int*)malloc(sizeof(int));
		lend = (int*)malloc(sizeof(int));
		*coded = code;
		*lend = len;
		wcarray_add(codes,coded);
		wcarray_add(lens,lend);
		wcarray_add(datas,d);
	}
}

/* Frees entry */
entry :: ~entry()
{
	/* Release all */
	wcarray_delete(datas);
	wcarray_delete(lens);
	wcarray_delete(codes);
}

/* Get id */
int entry :: get_id_from_code(int code)
{
	int i,coded;
	/* Seek */
	for(i = 0;i < wcarray_size(codes);i++)
	{
		/* Check */
		coded = *((int*)wcarray_get(codes,i));
		/* Found */
		if(coded == code)
			return i;
	}
	/* Not found */
	return -1;
}

/* Get length */
int entry :: get_length(int code)
{
	int i;
	i = get_id_from_code(code);
	/* Error */
	if(i == -1)
	{
		wcrpg_error("Invalid table length request (%d)",code);
		return 0;
	}
	/* Get */
	return *((int*)wcarray_get(lens,i));
}

/* Get data */
unsigned char *entry :: get_data(int code)
{
	int i;
	i = get_id_from_code(code);
	/* Error */
	if(i == -1)
	{
		wcrpg_error("Invalid table data request (%d)",code);
		return 0;
	}
	/* Get */
	return (unsigned char*)wcarray_get(datas,i);
}

/* Get id */
int entry :: get_id()
{
	return id;
}

/* Has data? */
int entry :: has_data(int code)
{
	if(get_id_from_code(code) != -1)
		return 1;
	return 0;
}

/* Make table */
table :: table(FILE *f)
{
	entry *en;
	int i,rlen;
	/* Allocate */
	entries = wcarray_new();
	size = 0;
	/* Get count */
	count = wcint_read_file(f,&rlen);
	size += rlen;
	/* Read body */
	for(i = 0;i < count;i++)
	{
		en = new entry(f);
		size += en->get_size();
		wcarray_add(entries,en);
	}
}

/* Get size */
int table :: get_size()
{
	return size;
}

/* Make table */
table :: table(void *v)
{
	entry *en;
	int i;
	unsigned char *bv;
	bv = (unsigned char*)v;
	/* Allocate */
	entries = wcarray_new();
	size = 0;
	/* Get count */
	count = wcint_read(bv);
	bv += wcint_length(bv);
	/* Read body */
	for(i = 0;i < count;i++)
	{
		en = new entry(bv);
		bv += en->get_size();
		wcarray_add(entries,en);
	}
}

/* Delete table */
table :: ~table()
{
	int i;
	/* Collapse list */
	for(i = 0;i < wcarray_size(entries);i++)
		delete (entry*)wcarray_get(entries,i);
	wcarray_delete(entries);
}

/* Gets entry */
entry *table :: get_entry(int id)
{
	/* Wrong id */
	if(id < 0 || id >= count)
	{
		wcrpg_error("Invalid id request for an entry in table (%d)",id);
		return 0;
	}
	/* Get */
	return (entry*)wcarray_get(entries,id);
}


/* Gets count */
int table :: get_count()
{
	return count;
}