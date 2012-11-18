/*
	Items
*/

/* Globals */
#include <malloc.h>
#include <stdio.h>
#include "wcitem.h"
#include "wcarray.h"
#include "wctype.h"
#include "wcrpg.h"
#include "wcsaveio.h"
#include "wcsort.h"
#include "wcmem.h"
#include "wcparty.h"
#include "wchero.h"

#include <string>

/* Globals */
table *item_table;
arraylist items;

/* Create item */
item :: item(entry *en)
{
	int i;
	unsigned char *useptr;
	/* Init */
	id = 0;
	name = 0;
	price = 0;
	desc = 0;
	type = 0;
	held = 0;
	switch_in_field = 1;
	switch_id = 1;
	times = 1;
	attack_power = 0;
	defense_power = 0;
	special_power = 0;
	speed_power = 0;
	hero_use_num = 0;
	hero_use = 0;
	class_use_num = 0;
	class_use = 0;
	att_use_num = 0;
	att_use = 0;
	eff_use_num = 0;
	accuracy = 90;
	eff_use = 0;
	animation_id = 1;
	eff_power = 0;
	eff_dir = 0;
	/* Get ID */
	id = en->get_id();
	/* Get common data */
	if(en->has_data(0x01))
		name = wcstr_read(en->get_data(0x01),en->get_length(0x01));
	if(en->has_data(0x02))
		desc = wcstr_read(en->get_data(0x02),en->get_length(0x02));
	if(en->has_data(0x03))
		type = wcint_read(en->get_data(0x03));
	if(en->has_data(0x05))
		price = wcint_read(en->get_data(0x05));
	if(en->has_data(0x06))
		times = wcint_read(en->get_data(0x06));
	/* Get switch data */
	if(en->has_data(0x39))
		switch_in_field = wcint_read(en->get_data(0x39));
	if(en->has_data(0x37))
		switch_id = wcint_read(en->get_data(0x37));
	/* Get status data */
	if(en->has_data(0x0B))
		attack_power = wcint_read(en->get_data(0x0B));
	if(en->has_data(0x0C))
		defense_power = wcint_read(en->get_data(0x0C));
	if(en->has_data(0x0D))
		special_power = wcint_read(en->get_data(0x0D));
	if(en->has_data(0x0E))
		speed_power = wcint_read(en->get_data(0x0E));
	/* Get hero use data */
	if(en->has_data(0x3D))
	{
		hero_use_num = wcint_read(en->get_data(0x3D));
		hero_use = (unsigned char*)malloc(hero_use_num);
		useptr = (unsigned char*)en->get_data(0x3E);
		for(i = 0;i < hero_use_num;i++)
			hero_use[i] = useptr[i];
	}
	/* Get class use data */
	if(en->has_data(0x48))
	{
		class_use_num = wcint_read(en->get_data(0x48));
		class_use = (unsigned char*)malloc(class_use_num);
		useptr = (unsigned char*)en->get_data(0x49);
		for(i = 0;i < class_use_num;i++)
			class_use[i] = useptr[i];
	}
	/* Get attribute data */
	if(en->has_data(0x41))
	{
		att_use_num = wcint_read(en->get_data(0x41));
		att_use = (unsigned char*)malloc(att_use_num);
		useptr = (unsigned char*)en->get_data(0x42);
		for(i = 0;i < att_use_num;i++)
			att_use[i] = useptr[i];
	}
	/* Get effect data */
	if(en->has_data(0x3F))
	{
		eff_use_num = wcint_read(en->get_data(0x3F));
		eff_use = (unsigned char*)malloc(eff_use_num);
		useptr = (unsigned char*)en->get_data(0x40);
		for(i = 0;i < eff_use_num;i++)
			eff_use[i] = useptr[i];
	}
	if(en->has_data(0x43))
		eff_power = wcint_read(en->get_data(0x43));
	if(en->has_data(0x44))
		eff_dir = wcint_read(en->get_data(0x44));
	/* Get weapon anim data */
	if(en->has_data(0x14))
		animation_id = wcint_read(en->get_data(0x14));
	/* Get accurayc */
	if(en->has_data(0x11))
		accuracy = wcint_read(en->get_data(0x11));
}

/* Delete item */
item :: ~item()
{
	/* Release */
	free(name);
	free(desc);
	/* Release use */
	free(hero_use);
	free(class_use);
	free(att_use);
	free(eff_use);
}

/* Load all */
void wcitem_load_all(FILE *f,int len)
{
	int i;
	item *it;
	/* Prepare things */
	item_table = new table(f);
	items = wcarray_new();
	/* Create items */
	for(i = 0;i < item_table->get_count();i++)
	{
		it = new item(item_table->get_entry(i));
		wcarray_add(items,it);
	}
}

/* Get rid of all */
void wcitem_free_all()
{
	int i;
	for(i = 0;i < wcarray_size(items);i++)
		delete (item*)wcarray_get(items,i);
	wcarray_delete(items);
	delete item_table;
}

/* Get item */
item *wcitem_get(int id)
{
	if(!wcitem_valid_id(id))
	{
		wcrpg_error("Invalid item id (%d)",id);
		return 0;
	}
	id--;
	return (item*)wcarray_get(items,id);
}

/* Get name */
char *item :: get_name()
{
	return name;
}

/* Get desc */
char *item :: get_description()
{
	return desc;
}

/* Get price */
int item :: get_price()
{
	return price;
}

/* Get type */
int item :: get_type()
{
	return type;
}

/* Get times usable */
int item :: get_times()
{
	return times;
}

/* Get held */
int item :: get_held()
{
	/* Infinite */
	if(times == 0 && held > 0)
		return 1;
	/* Infinite but do not have */
	if(times == 0 && held <= 0)
		return 0;
	/* Last batch */
	if(times > 1 && held < times && held > 0)
		return 1;
	/* Standard */
	return held/times;
}

/* Get equipped */
int item :: get_equipped()
{
	int i,j,total;
	hero *he;
	/* Begin count */
	total = 0;
	/* Scan party */
	for(i = 0;i < wcparty_size();i++)
	{
		he = wchero_get(wcparty_get(i));
		/* Check equipment */
		for(j = 0;j < MAX_EQUIPS;j++)
		{
			/* Match found */
			if(he->get_equip(j) == id)
				total++;
		}
	}
	/* Return result */
	return total;
}

/* Get field use */
int item :: usable_in_field()
{
	/* Usable items */
	if(type == WCITEM_MEDICINE || type == WCITEM_SEED)
		return 1; /* These may be used */
	/* May be used in field */
	if(type == WCITEM_SWITCH)
	{
		/* Only if usable on the field */
		if(switch_in_field)
			return 1;
	}
	/* Unusable */
	return 0;
}

/* Set held */
void item :: set_held(int h)
{
	held = h;
	/* Can't ever have negative items */
	if(held < 0)
		held = 0;
}

/* Use */
void item :: use(hero *hr)
{
	/* If switch */
	if(type == WCITEM_SWITCH)
		wcmem_set_switch(switch_id,1);
	/* If equipment */
	if(type >= WCITEM_WEAPON && type <= WCITEM_ACC)
		hr->equip_item(id);
	/* Reduce held */
	if(times != 0)
	{
		held--;
		if(held < 0)
			held = 0;
	}
}

/* Checks for equip */
int item :: can_use(int id)
{
	hero *hr;
	/* Reduce ID */
	id--;
	/* No hero equip data */
	if(!hero_use)
		return 1; /* Then any shmuck may use */
	/* Hero equip override */
	if(!hero_use[id])
		return 0;
	/* Stop here if using old editor */
	if(wcrpg_old_mode())
		return 1;
	/* Stop here if no class use pointer */
	if(!class_use)
		return 1;
	/* Hero can use, but also can their class? */
	hr = (hero*)wchero_get(id+1);
	return class_use[hr->get_class()];
}

/* Get attack power */
int item :: get_attack_power()
{
	return attack_power;
}

/* Get defense power */
int item :: get_defense_power()
{
	return defense_power;
}

/* Get special power */
int item :: get_special_power()
{
	return special_power;
}

/* Get speed power */
int item :: get_speed_power()
{
	return speed_power;
}

/* Get attribute */
int item :: get_attribute(int id)
{
	if(id < 0 || id >= att_use_num)
		return 0;
	return att_use[id];
}

/* Get all */
int item :: get_attribute_count()
{
	return att_use_num;
}

/* Save all items */
void wcitem_save(FILE *f)
{
	int i;
	item *it;
	wcsaveio_start(f);
	for(i = 0;i < wcarray_size(items);i++)
	{
		it = (item*)wcarray_get(items,i);
		wcsaveio_write_num(it->get_held());
	}
	wcsaveio_end();
}

/* Load all items */
void wcitem_load(FILE *f)
{
	int i,n;
	item *it;
	wcsaveio_read(f);
	for(i = 0;i < wcarray_size(items);i++)
	{
		n = wcsaveio_read_num();
		it = (item*)wcarray_get(items,i);
		it->set_held(n);
	}
	wcsaveio_next();
}

/* Add N of item I */
void wcitem_add(int i,int n)
{
	item *it;
	it = (item*)wcarray_get(items,i);
	/* Report */
	wclog_write(wcrpg_get_log(),"Added ");
	wclog_write(wcrpg_get_log(),n);
	wclog_write(wcrpg_get_log()," " );
	wclog_write(wcrpg_get_log(),it->get_name());
	wclog_write(wcrpg_get_log()," to inventory");
	wclog_enter(wcrpg_get_log());
	if(it->get_times() == 0)
	{
		it->set_held(1);
		return;
	}
	it->set_held(it->get_held()+n*it->get_times());
}

/* Remove N of item I */
void wcitem_remove(int i,int n)
{
	item *it;
	it = (item*)wcarray_get(items,i);
	/* Report */
	wclog_write(wcrpg_get_log(),"Removed ");
	wclog_write(wcrpg_get_log(),n);
	wclog_write(wcrpg_get_log()," " );
	wclog_write(wcrpg_get_log(),it->get_name());
	wclog_write(wcrpg_get_log()," from inventory");
	wclog_enter(wcrpg_get_log());
	if(it->get_times() == 0)
	{
		it->set_held(0);
		return;
	}
	it->set_held(it->get_held()-n*it->get_times());
	if(it->get_held() < 0)
		it->set_held(0);
}

/* Is a valid name character */
int wcitem_valid_char(char c)
{
	if(c >= '0' && c <= '9')
		return 1;
	if(c >= 'A' && c <= 'Z')
		return 1;
	if(c >= 'a' && c <= 'z')
		return 1;
	return 0;
}

/* Compare two items */
int wcitem_sort(void *a,void *b)
{
	item *ai,*bi;
	char *adesc,*bdesc;
	int ap,bp;
	/* Reference */
	ai = (item*)a;
	bi = (item*)b;
	ap = 0;
	bp = 0;
	adesc = ai->get_name();
	bdesc = bi->get_name();
	/* Seek first valid char */
	return -strcmp(adesc,bdesc);
}

/* Get filtered view of items */
arraylist wcitem_filter(int (*fil)(void*))
{
	arraylist lss,lso;
	lss = wcsort_filter(items,fil);
	lso = wcsort_sort(lss,wcitem_sort);
	return lso;
}

/* Enables only field usable items */
void wcitem_enable_field_usable_items(arraylist lso,menu *m)
{
	int i;
	item *it;
	/* All items */
	for(i = 0;i < wcarray_size(lso);i++)
	{
		it = (item*)wcarray_get(lso,i);
		/* Set enable/disable */
		if(it->usable_in_field())
			m->set_disable(i,0); /* May be used in field */
		else
			m->set_disable(i,1); /* May not be used in field */
	}
}

/* Enables all items */
void wcitem_enable_all(arraylist lso,menu *m)
{
	int i;
	/* All items */
	for(i = 0;i < wcarray_size(lso);i++)
		m->set_disable(i,0); /* May be used in field */
}

/* Returns true if the id is even valid */
int wcitem_valid_id(int id)
{
	if(id < 1 || id > wcarray_size(items))
		return 0;
	return 1;
}

/* Get animation */
int item :: get_animation()
{
	return animation_id;
}

/* Get acc */
int item :: get_accuracy()
{
	return accuracy;
}