/*
	Sorting and Filtering system
*/

/* Includes */
#include "wcsort.h"

/* Returns the right half of the input list */
arraylist wcsort_right(arraylist ls)
{
	arraylist lso;
	int div,i;
	/* Blank array begets blank array */
	if(wcarray_size(ls) <= 0)
		return wcarray_new();
	/* Single element begets that element */
	if(wcarray_size(ls) == 1)
	{
		lso = wcarray_new();
		wcarray_add(lso,wcarray_get(ls,0));
		return lso;
	}
	/* Divide */
	div = wcarray_size(ls)/2;
	lso = wcarray_new();
	for(i = div;i < wcarray_size(ls);i++)
		wcarray_add(lso,wcarray_get(ls,i));
	return lso;
}

/* Returns the left half of the input list */
arraylist wcsort_left(arraylist ls)
{
	arraylist lso;
	int div,i;
	/* Blank or single begets blank */
	if(wcarray_size(ls) <= 1)
		return wcarray_new();
	/* Divide */
	div = wcarray_size(ls)/2;
	lso = wcarray_new();
	for(i = 0;i < div;i++)
		wcarray_add(lso,wcarray_get(ls,i));
	return lso;
}

/* Merge the two */
arraylist wcsort_merge(arraylist lls,arraylist rls,int (*cmp)(void*,void*))
{
	int pl,pr;
	void *vl,*vr;
	arraylist lso;
	/* L/R pointers */
	pl = 0;
	pr = 0;
	/* Output array */
	lso = wcarray_new();
	/* Exhaust both */
	while(pl < wcarray_size(lls) && pr < wcarray_size(rls))
	{
		/* Get elements */
		vl = wcarray_get(lls,pl);
		vr = wcarray_get(rls,pr);
		/* Comp and add */
		if(cmp(vl,vr) >= 0)
		{
			wcarray_add(lso,vl);
			pl++;
		}
		else
		{
			wcarray_add(lso,vr);
			pr++;
		}
	}
	/* Exhaust left */
	while(pl < wcarray_size(lls))
	{
		/* Get elements */
		vl = wcarray_get(lls,pl);
		wcarray_add(lso,vl);
		pl++;
	}
	/* Exhaust right */
	while(pr < wcarray_size(rls))
	{
		/* Get elements */
		vr = wcarray_get(rls,pr);
		wcarray_add(lso,vr);
		pr++;
	}
	/* Delete given */
	wcarray_delete(lls);
	wcarray_delete(rls);
	/* Return result */
	return lso;
}

/* Sorts an array list using the given comparator */
arraylist wcsort_sort(arraylist ls,int (*cmp)(void*,void*))
{
	arraylist rls,lls,rlso,llso;
	/* Don't do anything if either is null */
	if(!ls || !cmp)
		return ls;
	/* This list is sorted */
	if(wcarray_size(ls) <= 1)
		return ls;
	/* Split! */
	rls = wcsort_right(ls);
	lls = wcsort_left(ls);
	wcarray_delete(ls);
	/* Sort these */
	rlso = wcsort_sort(rls,cmp);
	llso = wcsort_sort(lls,cmp);
	/* Combine */
	return wcsort_merge(llso,rlso,cmp);
}

/* Generates a filtered array based on a filter */
arraylist wcsort_filter(arraylist inls,int (*fil)(void*))
{
	arraylist ls;
	int i;
	void *v;
	/* Don't do anything if either is null */
	if(!inls || !fil)
		return 0;
	/* Make a new list */
	ls = wcarray_new();
	/* Filter */
	for(i = 0;i < wcarray_size(inls);i++)
	{
		/* Get element */
		v = wcarray_get(inls,i);
		/* If it passes through filter, add */
		if(fil(v))
			wcarray_add(ls,v);
	}
	/* Done */
	return ls;
}