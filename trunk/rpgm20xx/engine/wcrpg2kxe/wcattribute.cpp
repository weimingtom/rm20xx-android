/*
	Attributes
*/

/* Includes */
#include <malloc.h>
#include "wcattribute.h"
#include "wcarray.h"
#include "wctype.h"
#include "wcrpg.h"

/* Globals */
table *attribute_table;
arraylist attributes;

/* Create attribute */
attribute :: attribute(entry *en)
{
	/* Set defaults */
	name = 0;
	type = WCATTRIBUTE_WEAPON;
	levels[WCATTRIBUTE_A] = 300;
	levels[WCATTRIBUTE_B] = 200;
	levels[WCATTRIBUTE_C] = 100;
	levels[WCATTRIBUTE_D] = 50;
	levels[WCATTRIBUTE_E] = 0;
	/* Get name */
	if(en->has_data(0x01))
		name = wcstr_read(en->get_data(0x01),en->get_length(0x01));
	/* Get type */
	if(en->has_data(0x02))
		type = wcint_read(en->get_data(0x02));
	/* Get levels */
	if(en->has_data(0x0B))
		levels[WCATTRIBUTE_A] = wcint_read(en->get_data(0x0B));
	if(en->has_data(0x0C))
		levels[WCATTRIBUTE_B] = wcint_read(en->get_data(0x0C));
	if(en->has_data(0x0D))
		levels[WCATTRIBUTE_C] = wcint_read(en->get_data(0x0D));
	if(en->has_data(0x0E))
		levels[WCATTRIBUTE_D] = wcint_read(en->get_data(0x0E));
	if(en->has_data(0x0F))
		levels[WCATTRIBUTE_E] = wcint_read(en->get_data(0x0F));
}

/* Delete attribute */
attribute :: ~attribute()
{
	/* Release */
	free(name);
}

/* Load all */
void wcattribute_load_all(FILE *f,int len)
{
	int i;
	attribute *at;
	/* Prepare things */
	attribute_table = new table(f);
	attributes = wcarray_new();
	/* Create items */
	for(i = 0;i < attribute_table->get_count();i++)
	{
		at = new attribute(attribute_table->get_entry(i));
		wclog_write(wcrpg_get_log(),"Attribute ");
		wclog_write(wcrpg_get_log(),(i+1));
		wclog_write(wcrpg_get_log(),": ");
		wclog_write(wcrpg_get_log(),at->get_name());
		wclog_enter(wcrpg_get_log());
		wcarray_add(attributes,at);
	}
}

/* Get rid of all */
void wcattribute_free_all()
{
	int i;
	for(i = 0;i < wcarray_size(attributes);i++)
		delete (attribute*)wcarray_get(attributes,i);
	wcarray_delete(attributes);
	delete attribute_table;
}

/* Get attribute */
attribute *wcattribute_get(int id)
{
	if(id < 1 || id > wcarray_size(attributes))
	{
		wcrpg_error("Invalid attribute id (%d)",id);
		return 0;
	}
	id--;
	return (attribute*)wcarray_get(attributes,id);
}

/* Get name */
char *attribute :: get_name()
{
	return name;
}

/* Get type */
int attribute :: get_type()
{
	return type;
}

/* Get effect */
int attribute :: get_effect(int ix)
{
	return levels[ix];
}

/* Get count */
int wcattribute_count()
{
	return wcarray_size(attributes);
}