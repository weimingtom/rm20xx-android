/*
	Max Field Array List
*/

/* Includes */
#include <malloc.h>
#include <memory.h>
#include <string.h>
#include <stdio.h>
#include "wcarray.h"
#include "wcrpg.h"

/* Creates a new array */
arraylist wcarray_new()
{
	arraylist a;
	a = (arraylist)malloc(sizeof(arraylist_t));
	a->cap = WCARRAY_DEFAULT_CAPACITY;
	a->pop = 0;
	a->cl = 0;
	a->list = (void**)malloc(sizeof(void*)*a->cap);
	return a;
}

/* Deletes an array */
void wcarray_delete(arraylist a)
{
	int i;
	/* Free elements */
	if(a->cl)
	{
		for(i = 0;i < a->pop;i++)
			a->cl(a->list[i]);
	}
	/* Free */
	free(a->list);
	free(a);
}

/* Set cleanup func */
void wcarray_set_clean(arraylist a,void (*cl)(void*))
{
	a->cl = cl;
}

/* Adds element */
void wcarray_add(arraylist a,void *v)
{
	void **nlist;
	int olen,nlen;
	/* Fetch more room? */
	if(a->pop >= a->cap)
	{
		/* Measure */
		olen = a->cap*sizeof(void*);
		nlen = olen*2;
		/* Copy to new */
		nlist = (void**)malloc(nlen);
		memcpy(nlist,a->list,olen);
		//memcpy_s(nlist,olen,a->list,olen);
		/* Switch */
		free(a->list);
		a->list = nlist;
		/* Now a has twice as much room */
		a->cap *= 2;
	}
	/* Add */
	a->list[a->pop] = v;
	a->pop++;
}

/* Gets size */
int wcarray_size(arraylist a)
{
	return a->pop;
}

/* Gets element */
void *wcarray_get(arraylist a,int i)
{
	/* Error */
	if(i < 0 || i >= a->pop)
	{
		wcrpg_error("Array list out of bounds when getting (index=%d)",i);
		return 0;
	}
	/* Get */
	return a->list[i];
}

/* Removes element */
void wcarray_remove(arraylist a,int i)
{
	int x;
	/* Move every data left once */
	for(x = i+1;x < wcarray_size(a);x++)
		a->list[x-1] = a->list[x];
	/* Cap it off */
	a->pop--;
}