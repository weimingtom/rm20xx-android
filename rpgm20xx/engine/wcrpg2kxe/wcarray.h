#ifndef WCARRAY_H
#define WCARRAY_H

/* Defines */
#define WCARRAY_DEFAULT_CAPACITY 16

/* Types */
typedef struct
{
	void **list; /* Actual list pointers */
	void (*cl)(void*); /* Cleanup function */
	int cap; /* Capacity */
	int pop; /* Current usage */
}arraylist_t;

/* Typedefs */
typedef arraylist_t *arraylist;

/*
	Creates a new array list.
*/
extern arraylist wcarray_new();

/*
	Deletes an array list.
*/
extern void wcarray_delete(arraylist a);

/*
	Sets the cleanup function for an array list to be run on each element at deletion
*/
extern void wcarray_set_clean(arraylist a,void (*cl)(void*));

/*
	Adds an element to this array list
*/
extern void wcarray_add(arraylist a,void *v);

/*
	Returns the size of this array list
*/
extern int wcarray_size(arraylist a);

/*
	Gets an element in the list
*/
extern void *wcarray_get(arraylist a,int i);

/*
	Removes an element
*/
extern void wcarray_remove(arraylist a,int i);

#endif