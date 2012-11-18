/*
	Variables and Switches
*/

/* Includes */
#include <malloc.h>
#include <memory.h>
#include "wcarray.h"
#include "wcmem.h"
#include "wctype.h"
#include "wcrpg.h"
#include "wclog.h"
#include "wcsaveio.h"
#include "wcevent.h"
#include "wclevel.h"
#include "wcmonster.h"
#include "wchero.h"
#include "wcskill.h"

#include <string>

/* Globals */
arraylist wcmem_self_list = 0;
int wcmem_switch_count = 0;
swt *wcmem_switches = 0;
int wcmem_variable_count = 0;
var *wcmem_variables = 0;
int wcmem_register_base = 1;
int (*wcmem_callback)(void*,int,int,int,int,int) = 0;
int wcmem_self_start = 0;
int wcmem_self_end = 0;

/* Set clean for self list */
void wcmem_delete_svar(void *v)
{
	svar *svr;
	/* Set */
	svr = (svar*)v;
	/* Delete strings */
	free(svr->data);
	free(svr->name);
	free(svr);
}

/* Initialize self list */
void wcmem_new_svar_list()
{
	/* Initialize */
	wcmem_self_list = wcarray_new();
	wcarray_set_clean(wcmem_self_list,wcmem_delete_svar);
}

/* Set callback for registers */
void wcmem_set_register_callback(int (*cbk)(void*,int,int,int,int,int))
{
	wcmem_callback = cbk;
}

/* Get ABCDEF base */
int wcmem_get_register_base()
{
	return wcmem_register_base;
}

/* Set ABCDEF base */
void wcmem_set_register_base(int n)
{
	wcmem_register_base = n;
}

/* Initialize */
void wcmem_load_all_switches(FILE *f,int len)
{
	int rlen,i,tem;
	/* Get count */
	wcmem_switch_count = wcint_read_file(f,&rlen);
	/* Announce */
	wclog_write(wcrpg_get_log(),"Allocating ");
	wclog_write(wcrpg_get_log(),wcmem_switch_count);
	wclog_write(wcrpg_get_log()," switches.");
	wclog_enter(wcrpg_get_log());
	/* Read all */
	for(i = 0;i < wcmem_switch_count;i++)
	{
		/* Read the ID */
		tem = wcint_read_file(f,&rlen);
		/* Read the code */
		tem = wcint_read_file(f,&rlen);
		if(tem)
		{
			/* Read the string */
			wcstr_read_file(f,&rlen);
			/* Then there's a zero */
			tem = wcint_read_file(f,&rlen);
		}
	}
	/* Alloc */
	wcmem_switches = (swt*)malloc(sizeof(swt)*wcmem_switch_count);
	memset(wcmem_switches,0,sizeof(swt)*wcmem_switch_count);
}

/* Initialize vars */
void wcmem_load_all_variables(FILE *f,int len)
{
	int rlen,i,tem;
	/* Get count */
	wcmem_variable_count = wcint_read_file(f,&rlen);
	/* Announce */
	wclog_write(wcrpg_get_log(),"Allocating ");
	wclog_write(wcrpg_get_log(),wcmem_variable_count);
	wclog_write(wcrpg_get_log()," variables.");
	wclog_enter(wcrpg_get_log());
	/* Read all */
	for(i = 0;i < wcmem_variable_count;i++)
	{
		/* Read the ID */
		tem = wcint_read_file(f,&rlen);
		/* Read the code */
		tem = wcint_read_file(f,&rlen);
		if(tem)
		{
			/* Read the string */
			wcstr_read_file(f,&rlen);
			/* Then there's a zero */
			tem = wcint_read_file(f,&rlen);
		}
	}
	/* Alloc */
	wcmem_variables = (var*)malloc(sizeof(var)*wcmem_variable_count);
	memset(wcmem_variables,0,sizeof(var)*wcmem_variable_count);
	/* Load svars */
	wcmem_new_svar_list();
}

/* Free */
void wcmem_free_all()
{
	wcarray_delete(wcmem_self_list);
	wcmem_switch_count = 0;
	free(wcmem_switches);
	wcmem_variable_count = 0;
	free(wcmem_variables);
}

/* Set switch */
void wcmem_set_switch(int id,swt s)
{
	/* Exception */
	if(id <= 0 || id > wcmem_switch_count)
	{
		/*wcrpg_error("Invalid switch set (%d)",id);*/
		return;
	}
	/* Write */
	wcmem_switches[id-1] = s;
}

/* Get switch */
swt wcmem_get_switch(int id)
{
	/* Exception */
	if(id <= 0 || id > wcmem_switch_count)
	{
		/*wcrpg_error("Invalid switch get (%d)",id);*/
		return 0;
	}
	/* Read */
	return wcmem_switches[id-1];
}

/* Set var */
void wcmem_set_variable(int id,var v,void *me)
{
	int ret;
	rpgevent *ev;
	/* Exception */
	if(id <= 0 || id > wcmem_variable_count)
	{
		wcrpg_error("Invalid variable set (%d)",id);
		return;
	}
	/* Self var */
	if(id >= wcmem_self_start && id <= wcmem_self_end && me)
	{
		ev = (rpgevent*)me;
		ev->set_var(id-wcmem_self_start,v);
	}
	/* Syscall? */
	if(wcrpg_neo_mode())
	{
		if(wcmem_callback && id == wcmem_register_base+5)
		{
			ret = wcmem_callback(me,wcmem_variables[wcmem_register_base-1],wcmem_variables[wcmem_register_base],wcmem_variables[wcmem_register_base+1],wcmem_variables[wcmem_register_base+2],wcmem_variables[wcmem_register_base+3]);
			wcmem_variables[wcmem_register_base+4] = ret;
		}
	}
	/* Write */
	wcmem_variables[id-1] = v;
}

/* Get var */
var wcmem_get_variable(int id,void *me)
{
	rpgevent *ev;
	/* Exception */
	if(id <= 0 || id > wcmem_variable_count)
	{
		wcrpg_error("Invalid variable get (%d)",id);
		return 0;
	}
	/* Self var */
	if(id >= wcmem_self_start && id <= wcmem_self_end && me)
	{
		ev = (rpgevent*)me;
		return ev->get_var(id-wcmem_self_start);
	}
	/* Read */
	return wcmem_variables[id-1];
}

/* Get var */
var wcmem_get_variable(int id)
{
	return wcmem_get_variable(id,0);
}

/* Set switch ex */
void wcmem_set_switch_ex(int fid,int tid,int op)
{
	int i;
	/* Set range */
	for(i = fid;i <= tid;i++)
	{
		/* Set on? */
		if(op == SWITCH_ON)
			wcmem_set_switch(i,1);
		if(op == SWITCH_OFF)
			wcmem_set_switch(i,0);
		if(op == SWITCH_TOGGLE)
			wcmem_set_switch(i,(wcmem_get_switch(i)+1)%2);
	}
}

/* Set variable ex */
void wcmem_set_variable_ex(int fid,int tid,int op,var val,void *me)
{
	int i;
	/* Set range */
	for(i = fid;i <= tid;i++)
	{
		/* = */
		if(op == VARIABLE_SET)
			wcmem_set_variable(i,val,me);
		/* + */
		if(op == VARIABLE_ADD)
			wcmem_set_variable(i,wcmem_get_variable(i)+val,me);
		/* - */
		if(op == VARIABLE_SUBTRACT)
			wcmem_set_variable(i,wcmem_get_variable(i)-val,me);
		/* * */
		if(op == VARIABLE_MULTIPLY)
			wcmem_set_variable(i,wcmem_get_variable(i)*val,me);
		/* / */
		if(op == VARIABLE_DIVIDE)
		{
			if(val == 0)
				return;
			wcmem_set_variable(i,wcmem_get_variable(i)/val,me);
		}
		/* % */
		if(op == VARIABLE_MODULUS)
		{
			if(val == 0)
				wcrpg_error("Modulation by zero");
			wcmem_set_variable(i,wcmem_get_variable(i)%val,me);
		}
	}
}

/* Save all switches and variables */
void wcmem_save(FILE *f)
{
	int i;
	/* First, let's go ahead and save switches */
	wcsaveio_start(f);
	for(i = 0;i < wcmem_switch_count;i++)
		wcsaveio_write(wcmem_switches[i]);
	wcsaveio_end();
	/* Then let's go ahead and save the variables */
	wcsaveio_start(f);
	for(i = 0;i < wcmem_variable_count;i++)
		wcsaveio_write_num(wcmem_variables[i]);
	/* Then save the self variable range */
	wcsaveio_write_num(wcmem_self_start);
	wcsaveio_write_num(wcmem_self_end);
	wcsaveio_end();
}

/* Load all switches and variables */
void wcmem_load(FILE *f)
{
	int i,n;
	unsigned char b;
	/* First, load the switches */
	wcsaveio_read(f);
	for(i = 0;i < wcmem_switch_count;i++)
	{
		b = wcsaveio_read_byte();
		wcmem_switches[i] = b;
	}
	wcsaveio_next();
	/* Then, let's do the variables */
	wcsaveio_read(f);
	for(i = 0;i < wcmem_variable_count;i++)
	{
		n = wcsaveio_read_num();
		wcmem_variables[i] = n;
	}
	/* Load up self ranges */
	wcmem_self_start = wcsaveio_read_num();
	wcmem_self_end = wcsaveio_read_num();
	wcsaveio_next();
}

/* Set self var range */
void wcmem_set_self_var_range(int f,int t)
{
	/* Set */
	wcmem_self_start = f;
	wcmem_self_end = t;
	/* Announce */
	wclog_write(wcrpg_get_log(),"Set self variable range to ");
	wclog_write(wcrpg_get_log(),wcmem_self_start);
	wclog_write(wcrpg_get_log()," - ");
	wclog_write(wcrpg_get_log(),wcmem_self_end);
	wclog_enter(wcrpg_get_log());
}

/* Returns an allocated lump of variables */
var *wcmem_new_self_vars()
{
	var *rv;
	int len;
	/* No alloc */
	if(wcmem_self_start == 0 || wcmem_self_end == 0)
		return 0;
	/* Alloc */
	len = sizeof(var)*(wcmem_self_end-wcmem_self_start+1);
	rv = (var*)malloc(len);
	memset(rv,0,len);
	return rv;
}

/* Releases a lump of self vars */
void wcmem_delete_self_vars(var *vs)
{
	free(vs);
}

/* Takes an event and generates a self variable entry */
svar *wcmem_generate_svar(void *me)
{
	rpgevent *ev;
	svar *svr;
	int i;
	/* Must be valid */
	if(!wcmem_self_start)
	{
		wcrpg_error("Cannot generate self save variable if self variables are not enabled.");
		return 0;
	}
	/* Get me */
	ev = (rpgevent*)me;
	/* Generate */
	svr = (svar*)malloc(sizeof(svar));
	svr->name = wcrpg_string_clone(ev->get_name());
	svr->id = ev->get_id();
	svr->len = wcmem_self_end-wcmem_self_start+1;
	svr->data = (var*)malloc(sizeof(var)*svr->len);
	svr->mid = wclevel_get_id();
	/* Load data */
	for(i = 0;i < svr->len;i++)
		svr->data[i] = wcmem_get_variable(wcmem_self_start+i,me);
	/* Return result */
	return svr;
}

/* Takes a self variable entry and applies it to this event */
void wcmem_set_svar(svar *svr,void *me)
{
	rpgevent *ev;
	int i,len;
	/* Must be valid */
	if(!wcmem_self_start)
	{
		wcrpg_error("Cannot set self save variables if self variables are not enabled.");
		return;
	}
	/* Get me */
	ev = (rpgevent*)me;
	/* Find out len */
	len = wcmem_self_end-wcmem_self_start+1;
	if(len > svr->len)
		len = svr->len;
	/* Load */
	for(i = 0;i < len;i++)
		wcmem_set_variable_ex(wcmem_self_start+i,wcmem_self_start+i,VARIABLE_SET,svr->data[i],me);
}

/* Saves my self vars */
void wcmem_save_svar(void *me)
{
	int i;
	rpgevent *ev;
	svar *svr;
	svar *nsvr;
	/* Get me */
	ev = (rpgevent*)me;
	/* Seek my ID */
	for(i = 0;i < wcarray_size(wcmem_self_list);i++)
	{
		svr = (svar*)wcarray_get(wcmem_self_list,i);
		/* ID found */
		if(svr->id == ev->get_id() && svr->mid == wclevel_get_id())
		{
			/* Make new self var */
			nsvr = wcmem_generate_svar(me);
			/* Boot old one */
			wcarray_remove(wcmem_self_list,i);
			/* Add new one */
			wcarray_add(wcmem_self_list,nsvr);
			/* Report */
			wclog_write(wcrpg_get_log(),"Updated ");
			wclog_write(wcrpg_get_log(),ev->get_name());
			wclog_write(wcrpg_get_log()," to self variable record (");
			wclog_write(wcrpg_get_log(),wcarray_size(wcmem_self_list));
			wclog_write(wcrpg_get_log()," total)");
			wclog_enter(wcrpg_get_log());
			return;
		}
	}
	/* Brand new */
	nsvr = wcmem_generate_svar(me);
	/* Add */
	wcarray_add(wcmem_self_list,nsvr);
	/* Report */
	wclog_write(wcrpg_get_log(),"Added ");
	wclog_write(wcrpg_get_log(),ev->get_name());
	wclog_write(wcrpg_get_log()," to self variable record (");
	wclog_write(wcrpg_get_log(),wcarray_size(wcmem_self_list));
	wclog_write(wcrpg_get_log()," total)");
	wclog_enter(wcrpg_get_log());
}

/* Get count */
int wcmem_get_svar_count()
{
	return wcarray_size(wcmem_self_list);
}

/* Restore variables to an event */
void wcmem_restore_svar(void *me)
{
	int i;
	rpgevent *ev;
	svar *svr;
	/* Must be active at all */
	if(!wcmem_self_start)
		return;
	/* Assign */
	ev = (rpgevent*)me;
	/* Seek */
	for(i = 0;i < wcarray_size(wcmem_self_list);i++)
	{
		/* Get self var */
		svr = (svar*)wcarray_get(wcmem_self_list,i);
		/* Examine map id and id and name */
		if(svr->mid == wclevel_get_id() && svr->id == ev->get_id() && !strcmp(svr->name,ev->get_name()))
		{
			/* Restore */
			wcmem_set_svar(svr,me);
			/* Report */
			wclog_write(wcrpg_get_log(),"Restored self variables for ");
			wclog_write(wcrpg_get_log(),svr->name);
			wclog_enter(wcrpg_get_log());
			return;
		}
	}
}

/* Save all vars */
void wcmem_save_all_svar(FILE *f)
{
	int i,j;
	svar *svr;
	/* Save a block of svars */
	wcsaveio_start(f);
	/* Write the number of svars */
	wcsaveio_write_num(wcarray_size(wcmem_self_list));
	/* For each svar */
	for(i = 0;i < wcarray_size(wcmem_self_list);i++)
	{
		/* Get one */
		svr = (svar*)wcarray_get(wcmem_self_list,i);
		/* Write MID, ID, name, and length */
		wcsaveio_write_num(svr->mid);
		wcsaveio_write_num(svr->id);
		wcsaveio_write_str(svr->name);
		wcsaveio_write_num(svr->len);
		/* Now write actual svars */
		for(j = 0;j < svr->len;j++)
		{
			/* Write index and value only if nonzero */
			if(svr->data[j])
			{
				wcsaveio_write_num(j);
				wcsaveio_write_num(svr->data[j]);
			}
		}
		/* Cap it though so it knows when is next */
		if(!svr->data[svr->len-1])
		{
			wcsaveio_write_num(svr->len-1);
			wcsaveio_write_num(0);
		}
	}
	/* Done */
	wcsaveio_end();
}

/* Load all vars */
void wcmem_load_all_svar(FILE *f)
{
	int count,i,ix;
	var val;
	svar *svr;
	/* Begin reads */
	wcsaveio_read(f);
	/* Count */
	count = wcsaveio_read_num();
	/* For all svar */
	for(i = 0;i < count;i++)
	{
		/* Allocate svar */
		svr = (svar*)malloc(sizeof(svar));
		/* Load heading */
		svr->mid = wcsaveio_read_num();
		svr->id = wcsaveio_read_num();
		svr->name = wcsaveio_read_str();
		svr->len = wcsaveio_read_num();
		/* Allocate */
		svr->data = (var*)malloc(sizeof(var)*svr->len);
		memset(svr->data,0,sizeof(var)*svr->len);
		/* Read all */
		for(;;)
		{
			/* Get next entry */
			ix = wcsaveio_read_num();
			val = (var)wcsaveio_read_num();
			/* Set next entry */
			svr->data[ix] = val;
			/* Was end? */
			if(ix == svr->len-1)
				break;
		}
		/* Add */
		wcarray_add(wcmem_self_list,svr);
	}
	/* End */
	wcsaveio_next();
}

/* Loads data */
void wcmem_load_data(int source,int id,int v)
{
	var *pv;
	/* Get ptr */
	pv = (var*)&wcmem_variables[v-1];
	/* Load data */
	if(source == LOAD_DATA_MONSTER)
		wcmonster_get(id)->access_data(pv);
	if(source == LOAD_DATA_SKILL)
		wcskill_get(id)->access_data(pv);
}

/* Loads list elements */
void wcmem_load_list(int source,int id,int v,int n)
{
	var *pv;
	/* Get ptr */
	pv = (var*)&wcmem_variables[v-1];
	/* Load list */
	if(source == LOAD_LIST_HERO_SKILL)
	{
		if(!n)
			*pv = wchero_get(id)->get_skill_count();
		else
			*pv = wchero_get(id)->get_skill(n-1);
	}
}