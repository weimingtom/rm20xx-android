#ifndef WCMEM_H
#define WCMEM_H

/* Defines */
#define SWITCH_ON 0
#define SWITCH_OFF 1
#define SWITCH_TOGGLE 2

#define VARIABLE_SET 0
#define VARIABLE_ADD 1
#define VARIABLE_SUBTRACT 2
#define VARIABLE_MULTIPLY 3
#define VARIABLE_DIVIDE 4
#define VARIABLE_MODULUS 5

#define LOAD_DATA_MONSTER 6
#define LOAD_DATA_SKILL 2

#define LOAD_LIST_HERO_SKILL 0

/* Includes */
#include <stdio.h>

/* Types */
typedef unsigned char swt;
typedef int var;
typedef struct
{
	char *name;
	var *data;
	int id;
	int len;
	int mid;
}svar;

/*
	Read switches
*/
extern void wcmem_load_all_switches(FILE *f,int len);

/*
	Read variables
*/
extern void wcmem_load_all_variables(FILE *f,int len);

/*
	Release memory
*/
extern void wcmem_free_all();

/*
	Sets a switch
*/
extern void wcmem_set_switch(int id,swt s);

/*
	Gets a switch
*/
extern swt wcmem_get_switch(int id);

/*
	Sets a variable
*/
extern void wcmem_set_variable(int id,var v,void *me);

/*
	Gets a variable, and can get self variables
*/
extern var wcmem_get_variable(int id,void *me);

/*
	Gets a variable
*/
extern var wcmem_get_variable(int id);

/*
	Sets switches with more options
	From switch fid to switch tid,
	you can set on, set off or set toggle
*/
extern void wcmem_set_switch_ex(int fid,int tid,int op);

/*
	Sets variables with more options
	From variable fid to variable tid,
	you can specify the operation and the value
*/
extern void wcmem_set_variable_ex(int fid,int tid,int op,var val,void *me);

/*
	Gets the base of the numerical registers
*/
extern int wcmem_get_register_base();

/*
	Sets the base of the numberical register
*/
extern void wcmem_set_register_base(int n);

/*
	Sets the callback for memory
*/
extern void wcmem_set_register_callback(int (*cbk)(void*,int,int,int,int,int));

/*
	Saves switches and variables to file
*/
extern void wcmem_save(FILE *f);

/*
	Loads switches and variables from file
*/
extern void wcmem_load(FILE *f);

/*
	Sets the active range of self switches
*/
extern void wcmem_set_self_var_range(int f,int t);

/*
	Allocates and returns an event's self vars
*/
extern var *wcmem_new_self_vars();

/*
	Releases an event's self vars
*/
extern void wcmem_delete_self_vars(var *vs);

/*
	Generates a save var from event
*/
extern svar *wcmem_generate_svar(void *me);

/*
	Applies a save var to event
*/
extern void wcmem_set_svar(svar *svr,void *me);

/*
	Saves my self vars
*/
extern void wcmem_save_svar(void *me);

/*
	Gets the count of svar records
*/
extern int wcmem_get_svar_count();

/*
	Restores the self vars of an event if applicable
*/
extern void wcmem_restore_svar(void *me);

/*
	Saves all svars
*/
extern void wcmem_save_all_svar(FILE *f);

/*
	Loads all svars
*/
extern void wcmem_load_all_svar(FILE *f);

/*
	Loads data from database into vars
*/
extern void wcmem_load_data(int source,int id,int v);

/*
	Loads list elements from database into a single var
*/
extern void wcmem_load_list(int source,int id,int v,int n);

#endif