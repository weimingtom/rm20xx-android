#ifndef WCSORT_H
#define WCSORT_H

/* Require */
#include "wcarray.h"

/*
	Sorts the array list using the given comparator
*/
extern arraylist wcsort_sort(arraylist ls,int (*cmp)(void*,void*));

/*
	Returns a filtered view of the input array list
	DO NOT FREE ANY ELEMENTS OR SET ANY CLEANUP FUNCTION,
	delete the returned arraylist using only the keyword "delete"
*/
extern arraylist wcsort_filter(arraylist inls,int (*fil)(void*));

#endif