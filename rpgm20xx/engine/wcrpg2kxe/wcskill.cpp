/*
	Skills
*/

/* Includes */
#include "wcskill.h"
#include "wctype.h"
#include "wcrpg.h"

#include <stdlib.h>

/* Globals */
table *skill_table = 0;
arraylist skills = 0;

/* Create skill */
skill :: skill(entry *en)
{
	/* Init */
	name = 0;
	desc = 0;
	mp_need = 0;
	mp_percent = 1;
	mp_is_percent = 0;
	type = WCSKILL_NORMAL;
	target = WCSKILL_TARGET_ENEMY;
	effect_rating = 0;
	success = 100;
	hp_effect = 0;
	mp_effect = 0;
	attack_effect = 0;
	defense_effect = 0;
	intellect_effect = 0;
	speed_effect = 0;
	absorb = 0;
	ignore_defense = 0;
	physical_influence = 0;
	ether_influence = 3;
	variance = 4;
	reverse_treatment = 0;
	attribute_defense = 0;
	animation_id = 1;
	/* Get names */
	if(en->has_data(0x01))
		name = wcstr_read(en->get_data(0x01),en->get_length(0x01));
	if(en->has_data(0x02))
		desc = wcstr_read(en->get_data(0x02),en->get_length(0x02));
	/* Get MP needed */
	if(en->has_data(0x0A))
		mp_percent = wcint_read(en->get_data(0x0A));
	if(en->has_data(0x0B))
		mp_need = wcint_read(en->get_data(0x0B));
	if(en->has_data(0x09))
		mp_is_percent = wcint_read(en->get_data(0x09));
	/* Get type */
	if(en->has_data(0x08))
		type = wcint_read(en->get_data(0x08));
	/* Get target */
	if(en->has_data(0x0C))
		target = wcint_read(en->get_data(0x0C));
	/* Get strength */
	if(en->has_data(0x18))
		effect_rating = wcint_read(en->get_data(0x18));
	/* Get % working */
	if(en->has_data(0x19))
		success = wcint_read(en->get_data(0x19));
	/* Get effect targets */
	if(en->has_data(0x1F))
		hp_effect = wcint_read(en->get_data(0x1F));
	if(en->has_data(0x20))
		mp_effect = wcint_read(en->get_data(0x20));
	if(en->has_data(0x21))
		attack_effect = wcint_read(en->get_data(0x21));
	if(en->has_data(0x22))
		defense_effect = wcint_read(en->get_data(0x22));
	if(en->has_data(0x23))
		intellect_effect = wcint_read(en->get_data(0x23));
	if(en->has_data(0x24))
		speed_effect = wcint_read(en->get_data(0x24));
	/* Get special effect flags */
	if(en->has_data(0x25))
		absorb = wcint_read(en->get_data(0x25));
	if(en->has_data(0x26))
		ignore_defense = wcint_read(en->get_data(0x26));
	/* Get influences */
	if(en->has_data(0x15))
		physical_influence = wcint_read(en->get_data(0x15));
	if(en->has_data(0x16))
		ether_influence = wcint_read(en->get_data(0x16));
	if(en->has_data(0x17))
		variance = wcint_read(en->get_data(0x17));
	/* Get reverse treatment */
	if(en->has_data(0x14))
		reverse_treatment = wcint_read(en->get_data(0x14));
	/* Get attribute defense */
	if(en->has_data(0x2D))
		attribute_defense = wcint_read(en->get_data(0x2D));
	/* Get animation */
	if(en->has_data(0x0E))
		animation_id = wcint_read(en->get_data(0x0E));
}

/* Delete skill */
skill :: ~skill()
{
	/* Release */
	free(name);
	free(desc);
}

/* Get type */
int skill :: get_type()
{
	return type;
}

/* Get mp need */
int skill :: get_mp_need()
{
	return mp_need;
}

/* Get name */
char *skill :: get_name()
{
	return name;
}

/* Get description */
char *skill :: get_description()
{
	return desc;
}

/* Load all */
void wcskill_load_all(FILE *f,int len)
{
	int i;
	skill *sk;
	/* Prepare things */
	skill_table = new table(f);
	skills = wcarray_new();
	/* Create skills */
	for(i = 0;i < skill_table->get_count();i++)
	{
		sk = new skill(skill_table->get_entry(i));
		wcarray_add(skills,sk);
	}
}

/* Get rid of all */
void wcskill_free_all()
{
	int i;
	for(i = 0;i < wcarray_size(skills);i++)
		delete (skill*)wcarray_get(skills,i);
	wcarray_delete(skills);
	delete skill_table;
}

/* Get skill */
skill *wcskill_get(int id)
{
	if(id < 1 || id > wcarray_size(skills))
	{
		wcrpg_error("Invalid skill id (%d)",id);
		return 0;
	}
	id--;
	return (skill*)wcarray_get(skills,id);
}

/* Access data */
void skill :: access_data(var *base)
{
	base[0] = type;
	if(mp_is_percent)
		base[1] = mp_percent;
	else
		base[1] = mp_need;
	base[2] = mp_is_percent;
	base[3] = target;
	base[4] = effect_rating;
	base[5] = success;
	base[6] = hp_effect;
	base[7] = mp_effect;
	base[8] = attack_effect;
	base[9] = defense_effect;
	base[10] = intellect_effect;
	base[11] = speed_effect;
	base[12] = absorb;
	base[13] = ignore_defense;
	base[14] = physical_influence;
	base[15] = ether_influence;
	base[16] = variance;
	base[17] = reverse_treatment;
	base[18] = attribute_defense;
	base[19] = animation_id;
}