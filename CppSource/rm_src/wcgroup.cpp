/*
	Monster Group
*/

/* Includes */
#include <malloc.h>
#include "wctype.h"
#include "wcgroup.h"
#include "wcrpg.h"
#include "wcmetric.h"
#include "wcmonster.h"
#include "wcbattleset.h"

/* Group */
table *monster_group_table;
arraylist monster_groups;

/* Make monster group */
monstergroup :: monstergroup(entry *en)
{
	int i;
	table *list_table;
	entry *lsen;
	monsterentry *men;
	/* Defaults */
	name = 0;
	monster_list = wcarray_new();
	wcarray_set_clean(monster_list,free);
	/* Get name */
	if(en->has_data(0x01))
		name = wcstr_read(en->get_data(0x01),en->get_length(0x01));
	/* Get list */
	if(en->has_data(0x02))
	{
		/* Make table */
		list_table = new table(en->get_data(0x02));
		/* Add */
		for(i = 0;i < list_table->get_count();i++)
		{
			/* Allocate and default */
			men = (monsterentry*)malloc(sizeof(monsterentry));
			men->hidden = 0;
			men->x = 0;
			men->y = 0;
			men->id = 1;
			men->flash_dist = 0.0f;
			men->flash_a = 0.0f;
			men->red = 0;
			men->green = 0;
			men->blue = 0;
			men->dead = 0;
			men->fade = 0.0f;
			/* Set */
			lsen = list_table->get_entry(i);
			/* ID */
			if(lsen->has_data(0x01))
				men->id = wcint_read(lsen->get_data(0x01));
			/* X and Y */
			if(lsen->has_data(0x02))
				men->x = wcint_read(lsen->get_data(0x02));
			if(lsen->has_data(0x03))
				men->y = wcint_read(lsen->get_data(0x03));
			/* Hidden */
			if(lsen->has_data(0x04))
				men->hidden = wcint_read(lsen->get_data(0x04));
			/* Add */
			wcarray_add(monster_list,men);
		}
		/* Release table */
		delete list_table;
	}
}

/* Delete monster group */
monstergroup :: ~monstergroup()
{
	wcarray_delete(monster_list);
	free(name);
}

/* Load all */
void wcgroup_load_all(FILE *f,int len)
{
	int i;
	monstergroup *mg;
	/* Allocate */
	monster_group_table = new table(f);
	monster_groups = wcarray_new();
	/* Load them */
	for(i = 0;i < monster_group_table->get_count();i++)
	{
		mg = new monstergroup(monster_group_table->get_entry(i));
		wcarray_add(monster_groups,mg);
	}
}

/* Free all */
void wcgroup_free_all()
{
	int i;
	for(i = 0;i < wcarray_size(monster_groups);i++)
		delete (monstergroup*)wcarray_get(monster_groups,i);
	wcarray_delete(monster_groups);
	delete monster_group_table;
}

/* Get monster group */
monstergroup *wcgroup_get(int id)
{
	if(id <= 0 || id > wcarray_size(monster_groups))
	{
		wcrpg_error("Invalid monster group ID (%d)",id);
		return 0;
	}
	id--;
	return (monstergroup*)wcarray_get(monster_groups,id);
}

/* Get entry */
monsterentry *monstergroup :: get_entry(int ix)
{
	return (monsterentry*)wcarray_get(monster_list,ix);
}

/* Get count */
int monstergroup :: get_count()
{
	return wcarray_size(monster_list);
}

/* Add all monsters */
void monstergroup :: add_to_field()
{
	int i;
	monsterentry *men;
	for(i = 0;i < get_count();i++)
	{
		men = get_entry(i);
		wcmetric_add_monster(wcmonster_get(men->id),men->x,men->y);
	}
}

/* Get name */
char *monstergroup :: get_name()
{
	return name;
}

/* Draw */
void monstergroup :: draw()
{
	int i;
	monsterentry *men;
	monster *mn;
	for(i = 0;i < get_count();i++)
	{
		/* Get monster */
		men = get_entry(i);
		/* Draw all visible enemies */
		if(!men->hidden)
		{
			mn = (monster*)wcmonster_get(men->id);
			wcbattleset_set_option(WCBATTLESET_FRAME_AUTO,1);
			wcmonster_draw(men->id,men->x-mn->get_width()/2,men->y-mn->get_height()/2,0,men->flash_dist*men->flash_a,men->red,men->green,men->blue,men->fade);
		}
	}
}

/* Flash entry */
void monstergroup :: flash(int ix,float a,int r,int g,int b)
{
	monsterentry *men;
	if(ix < 0 || ix >= get_count())
		return;
	men = get_entry(ix);
	men->flash_dist = 1.0f;
	men->flash_a = a;
	men->green = g;
	men->blue = b;
	men->red = r;
}

/* Handle groups */
void monstergroup :: handle()
{
	int i;
	float ts;
	monsterentry *men;
	ts = wcrpg_get_time_step();
	for(i = 0;i < get_count();i++)
	{
		/* Get monster */
		men = get_entry(i);
		/* Handle */
		men->flash_dist -= ts*0.1f;
		if(men->flash_dist < 0.0f)
			men->flash_dist = 0.0f;
		if(men->dead)
			men->fade += ts*0.1f;
		if(men->fade > 1.0f)
			men->fade = 1.0f;
	}
}

/* Defeat monster */
void monstergroup :: defeat(int ix)
{
	monsterentry *men;
	if(ix < 0 || ix >= get_count())
		return;
	men = get_entry(ix);
	men->dead = 1;
}