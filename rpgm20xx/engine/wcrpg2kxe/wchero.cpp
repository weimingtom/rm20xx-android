/*
	Heroes
*/

/* Includes */
#include <malloc.h>
#include "wcitem.h"
#include "wchero.h"
#include "wcarray.h"
#include "wctype.h"
#include "wcrpg.h"
#include "wcsaveio.h"
#include "wcplayer.h"
#include "wcattribute.h"

#include <string>

/* Globals */
table *hero_table;
arraylist heroes;

/* Create skill entry */
skillentry :: skillentry(entry *en)
{
	/* Defaults */
	level_learned = 1;
	skill_learned = 1;
	/* Get */
	if(en->has_data(0x01))
		level_learned = wcint_read(en->get_data(0x01));
	if(en->has_data(0x02))
		skill_learned = wcint_read(en->get_data(0x02));
}

/* Remove skill entry */
skillentry :: ~skillentry()
{
}

/* Get level */
int skillentry :: get_level()
{
	return level_learned;
}

/* Get skill */
int skillentry :: get_skill()
{
	return skill_learned;
}

/* Create hero */
hero :: hero(entry *en)
{
	unsigned short *sptr;
	unsigned char *bptr;
	int *cmdptr;
	int i,j,k;
	table *skill_list_table;
	skillentry *sken;
	/* Init */
	hero_id = en->get_id();
	name = 0;
	title = 0;
	charset = 0;
	baseframe = 0;
	transparent = 0;
	level = 1;
	max_level = 99;
	faceset = 0;
	faceframe = 0;
	health = 0;
	mana = 0;
	name_text = 0;
	battle_x = 0;
	battle_y = 0;
	battleset_id = 1;
	elms = 0;
	elms_size = 0;
	animation_id = 0;
	title_text = 0;
	expa = 0;
	expb = 0;
	exp = 0;
	expc = 0;
	current_class = 0;
	memset(stats,0,sizeof(stats));
	memset(stat_mod,0,sizeof(stat_mod));
	memset(equips,0,sizeof(equips));
	memset(battle_commands,0,sizeof(battle_commands));
	skill_list = wcarray_new();
	current_skills = wcarray_new();
	wcarray_set_clean(current_skills,free);
	/* Get battler */
	if(en->has_data(0x3E))
		battleset_id = wcint_read(en->get_data(0x3E));
	/* Get battle position */
	if(en->has_data(0x3B))
		battle_x = wcint_read(en->get_data(0x3B));
	if(en->has_data(0x3C))
		battle_y = wcint_read(en->get_data(0x3C));
	/* Get class */
	if(en->has_data(0x39))
		current_class = wcint_read(en->get_data(0x39));
	/* Get name */
	if(en->has_data(0x01))
		name = wcstr_read(en->get_data(0x01),en->get_length(0x01));
	/* Get charset */
	if(en->has_data(0x03))
		charset = wcstr_read(en->get_data(0x03),en->get_length(0x03));
	/* Get base frame */
	if(en->has_data(0x04))
		baseframe = wcint_read(en->get_data(0x04));
	/* Get face */
	if(en->has_data(0x0F))
		faceset = wcstr_read(en->get_data(0x0F),en->get_length(0x0F));
	/* Get face frame */
	if(en->has_data(0x10))
		faceframe = wcint_read(en->get_data(0x10));
	/* Get title */
	if(en->has_data(0x02))
		title = wcstr_read(en->get_data(0x02),en->get_length(0x02));
	/* Get levels */
	if(en->has_data(0x07))
		level = wcint_read(en->get_data(0x07));
	if(en->has_data(0x08))
		max_level = wcint_read(en->get_data(0x08));
	/* EXP */
	if(en->has_data(0x29))
		expa = wcint_read(en->get_data(0x29));
	if(en->has_data(0x2A))
		expb = wcint_read(en->get_data(0x2A));
	if(en->has_data(0x2B))
		expc = wcint_read(en->get_data(0x2B));
	/* Get elements */
	if(en->has_data(0x49))
		elms_size = wcint_read(en->get_data(0x49));
	if(en->has_data(0x4A))
	{
		elms = (unsigned char*)malloc(elms_size);
		bptr = (unsigned char*)en->get_data(0x4A);
		for(i = 0;i < elms_size;i++)
			elms[i] = bptr[i];
	}
	/* Get skill list */
	if(en->has_data(0x3F))
	{
		skill_list_table = new table(en->get_data(0x3F));
		for(i = 0;i < skill_list_table->get_count();i++)
		{
			sken = new skillentry(skill_list_table->get_entry(i));
			wcarray_add(skill_list,sken);
		}
		delete skill_list_table;
	}
	/* Get stats */
	if(en->has_data(0x1F))
	{
		/* Get ptr */
		sptr = (unsigned short*)en->get_data(0x1F);
		/* 2000 / 2003 */
		if(en->get_length(0x1F) < sizeof(unsigned short)*MAX_STATS*MAX_LEVEL)
		{
			wcrpg_set_old_mode(); /* This is one of the indicators that this is a 2000 game */
			k = 50;
		}
		else
		{
			k = 99;
		}
		/* Read */
		for(i = 0;i < MAX_STATS;i++)
		{
			/* i is stat, j is current level, k is max level */
			for(j = 0;j < k;j++)
				stats[i][j] = sptr[i*k+j];
		}
	}
	/* Get equips */
	if(en->has_data(0x33))
	{
		/* Get ptr */
		sptr = (unsigned short*)en->get_data(0x33);
		/* Read */
		for(i = 0;i < MAX_EQUIPS;i++)
			start_equips[i] = sptr[i];
	}
	/* Get commands */
	if(en->has_data(0x50))
	{
		/* Get ptr */
		cmdptr = (int*)en->get_data(0x50);
		/* Read */
		for(i = 0;i < MAX_COMMANDS;i++)
			battle_commands[i] = cmdptr[i];
	}
	/* Get animation */
	if(en->has_data(0x38))
		animation_id = wcint_read(en->get_data(0x38));
	/* Learn skills */
	learn_current_skills();
	/* Full healing */
	health = get_stat(STAT_HEALTH);
	mana = get_stat(STAT_MANA);
}

/* Delete */
hero :: ~hero()
{
	int i;
	wcarray_delete(current_skills);
	for(i = 0;i < wcarray_size(skill_list);i++)
		delete (skillentry*)wcarray_get(skill_list,i);
	wcarray_delete(skill_list);
	free(name);
	free(charset);
	free(faceset);
	free(elms);
	free(title);
	if(title_text)
		delete title_text;
	if(name_text)
		delete name_text;
}

/* Learn all relevant skills */
void hero :: learn_current_skills()
{
	int i;
	skillentry *sken;
	/* Check all skills */
	for(i = 0;i < wcarray_size(skill_list);i++)
	{
		/* Seek */
		sken = (skillentry*)wcarray_get(skill_list,i);
		/* Add if can know it */
		if(sken->get_level() <= level)
			add_skill(sken->get_skill());
	}
}

/* Load all */
void wchero_load_all(FILE *f,int len)
{
	int i;
	hero *he;
	/* Prepare things */
	hero_table = new table(f);
	heroes = wcarray_new();
	/* Create heroes */
	for(i = 0;i < hero_table->get_count();i++)
	{
		he = new hero(hero_table->get_entry(i));
		wcarray_add(heroes,he);
	}
}

/* Validate all */
void wchero_validate_all_equipment()
{
	int i;
	hero *he;
	/* Load all and validate all */
	for(i = 0;i < wcarray_size(heroes);i++)
	{
		he = (hero*)wcarray_get(heroes,i);
		he->validate_equipment(i+1);
	}
}

/* Get rid of the tables and list */
void wchero_free_all()
{
	int i;
	for(i = 0;i < wcarray_size(heroes);i++)
		delete (hero*)wcarray_get(heroes,i);
	wcarray_delete(heroes);
	delete hero_table;
}

/* Get hero */
hero *wchero_get(int id)
{
	if(id < 1 || id > wcarray_size(heroes))
	{
		wcrpg_error("Invalid hero id (%d)",id);
		return 0;
	}
	id--;
	return (hero*)wcarray_get(heroes,id);
}

/* Get name */
char *hero :: get_name()
{
	return name;
}

/* Set name */
void hero :: set_name(char *str)
{
	free(name);
	name = wcrpg_string_clone(str);
}

/* Get charset */
char *hero :: get_charset()
{
	return charset;
}

/* Set charset */
void hero :: set_charset(char *str,int fr,int tr)
{
	free(charset);
	charset = wcrpg_string_clone(str);
	baseframe = fr;
	transparent = tr;
}

/* Get charset number */
int hero :: get_charset_number()
{
	return baseframe;
}

/* Get faceset */
char *hero :: get_faceset()
{
	return faceset;
}

/* Get faceset number */
int hero :: get_faceset_number()
{
	return faceframe;
}

/* Get transparency */
int hero :: is_transparent()
{
	return transparent;
}

/* Set face */
void hero :: set_faceset(char *str,int fr)
{
	free(faceset);
	faceset = wcrpg_string_clone(str);
	faceframe = fr;
}

/* Get title */
char *hero :: get_title()
{
	return title;
}

/* Set title */
void hero :: set_title(char *str)
{
	free(title);
	title = wcrpg_string_clone(str);
}

/* Get EXP */
int hero :: gain_exp(int amt)
{
	int ch;
	/* Change */
	exp += amt;
	ch = 0;
	/* Can't have fewer than no points */
	if(exp < 0)
		exp = 0;
	/* Increase in level */
	while(exp > get_exp_for_next())
	{
		level++;
		ch = 1;
	}
	/* Decrease in level */
	if(level > 1)
	{
		for(;;)
		{
			/* Go down one */
			level--;
			/* Can we level back up? */
			if(exp > get_exp_for_next())
			{
				/* Level up and stop */
				level++;
				break;
			}
		}
	}
	/* Change */
	if(ch)
		learn_current_skills();
	/* Did the player level up? */
	return ch;
}

/* Refresh! */
void hero :: full_restore()
{
	health = get_stat(STAT_HEALTH);
	mana = get_stat(STAT_MANA);
}

/* Get stat */
int hero :: get_stat(int st)
{
	int ret;
	/* Errors */
	if(st < 0 || st >= MAX_STATS)
	{
		wcrpg_error("Invalid stat number (%d)",st);
		return 0;
	}
	/* Get */
	ret = stats[st][level-1];
	/* Mod */
	ret += stat_mod[st];
	/* Clip */
	if(ret < 0)
		return 1;
	return ret;
}

/* Get level */
int hero :: get_level()
{
	return level;
}

/* Set level */
void hero :: set_level(int lv)
{
	/* Clamp */
	if(lv < 1)
		lv = 1;
	if(lv > max_level)
		lv = max_level;
	/* Set */
	if(lv > 1)
	{
		/* EXP since then */
		level = lv-1;
		exp = get_exp_for_next();
		level++;
	}
	else
	{
		/* EXP zero */
		exp = 0;
		level = lv;
	}
	/* Learn skillz */
	learn_current_skills();
}

/* Get health */
int hero :: get_health()
{
	return health;
}

/* Get mana */
int hero :: get_mana()
{
	return mana;
}

/* Set health */
void hero :: set_health(int amt)
{
	if(amt < 0)
		amt = 0;
	if(amt > get_stat(STAT_HEALTH))
		amt = get_stat(STAT_HEALTH);
	health = amt;
}

/* Set mana */
void hero :: set_mana(int amt)
{
	if(amt < 0)
		amt = 0;
	if(amt > get_stat(STAT_MANA))
		amt = get_stat(STAT_MANA);
	mana = amt;
}

/* Get name text */
text *hero :: get_name_text()
{
	return name_text;
}

/* Get title text */
text *hero :: get_title_text()
{
	return title_text;
}

/* Get exp */
int hero :: get_exp()
{
	return exp;
}

/* Set exp */
void hero :: set_exp(int ex)
{
	exp = ex;
}

/* Get offset */
int hero :: get_stat_offset(int st)
{
	/* Errors */
	if(st < 0 || st >= MAX_STATS)
	{
		wcrpg_error("Invalid stat offset number (%d)",st);
		return 0;
	}
	return stat_mod[st];
}

/* Mod stat */
void hero :: modify_stat(int st,int amt)
{
	/* Errors */
	if(st < 0 || st >= MAX_STATS)
	{
		wcrpg_error("Invalid stat modification number (%d)",st);
		return;
	}
	stat_mod[st] += amt;
}

/* Set stat */
void hero :: set_stat(int st,int amt)
{
	/* Errors */
	if(st < 0 || st >= MAX_STATS)
	{
		wcrpg_error("Invalid stat modification number (%d)",st);
		return;
	}
	stat_mod[st] = amt;
}

/* Refresh text */
void hero :: refresh()
{
	if(title_text)
		delete title_text;
	if(name_text)
		delete name_text;
	if(title)
		title_text = new text(title);
	else
		title_text = 0;
	if(name)
		name_text = new text(name);
	else
		name_text = 0;
}

/* Get EXP for next level */
int hero :: get_exp_for_next()
{
	int val,i,j;
	/* A * level */
	val = expa*level;
	/* B expo level */
	j = 0;
	for(i = 0;i <= level;i++)
		j += i;
	val += expb*j;
	/* C * level */
	val += expc*level;
	return val;
}

/* Get equipment */
int hero :: get_equip(int slot)
{
	return equips[slot];
}

/* Set equipment */
void hero :: set_equip(int slot,int id)
{
	equips[slot] = id;
}

/* Get class */
int hero :: get_class()
{
	return current_class;
}

/* Validate equipment */
void hero :: validate_equipment(int id)
{
	int i;
	item *it;
	for(i = 0;i < MAX_EQUIPS;i++)
	{
		/* Get item */
		if(start_equips[i])
		{
			/* Valid item? */
			if(wcitem_valid_id(start_equips[i]))
			{
				it = (item*)wcitem_get(start_equips[i]);
				if(it->can_use(id))
					equip_item(start_equips[i]);
			}
		}
	}
}

/* Remove item on this hero */
void hero :: unequip_item(int id)
{
	item *it;
	int sl;
	/* Get item */
	it = (item*)wcitem_get(id);
	/* Get type converted to slot */
	sl = it->get_type()-WCITEM_WEAPON;
	/* Unequip an item slot */
	unequip_item_slot(sl);
}

/* Remove item on this hero using a slot */
void hero :: unequip_item_slot(int sl)
{
	item *it;
	/* Must be something there */
	if(!equips[sl])
		return;
	/* Get item */
	it = (item*)wcitem_get(equips[sl]);
	/* Report this */
	wclog_write(wcrpg_get_log(),"Removed ");
	wclog_write(wcrpg_get_log(),it->get_name());
	wclog_write(wcrpg_get_log()," from ");
	wclog_write(wcrpg_get_log(),get_name());
	wclog_enter(wcrpg_get_log());
	/* Then take it off */
	wcitem_add(equips[sl]-1,1);
	equips[sl] = 0;
	/* Modify stats */
	stat_mod[STAT_ATTACK] -= it->get_attack_power();
	stat_mod[STAT_DEFENSE] -= it->get_defense_power();
	stat_mod[STAT_POWER] -= it->get_special_power();
	stat_mod[STAT_SPEED] -= it->get_speed_power();
}

/* Equip item on this hero */
void hero :: equip_item(int id)
{
	item *it;
	int sl;
	/* Get item */
	it = (item*)wcitem_get(id);
	/* Must be equipment */
	if(it->get_type() < WCITEM_WEAPON || it->get_type() > WCITEM_ACC)
		return;
	if(!it->can_use(hero_id))
		return;
	/* Get type converted to slot */
	sl = it->get_type()-WCITEM_WEAPON;
	/* Is there anything there before? */
	if(equips[sl])
		unequip_item(equips[sl]);
	/* Now set it */
	equips[sl] = id;
	/* Modify stats */
	stat_mod[STAT_ATTACK] += it->get_attack_power();
	stat_mod[STAT_DEFENSE] += it->get_defense_power();
	stat_mod[STAT_POWER] += it->get_special_power();
	stat_mod[STAT_SPEED] += it->get_speed_power();
	/* Report this */
	wclog_write(wcrpg_get_log(),"Equipped ");
	wclog_write(wcrpg_get_log(),it->get_name());
	wclog_write(wcrpg_get_log()," on ");
	wclog_write(wcrpg_get_log(),get_name());
	wclog_enter(wcrpg_get_log());
}

/* Skill count */
int hero :: get_skill_count()
{
	return wcarray_size(current_skills);
}

/* Skill get */
int hero :: get_skill(int ix)
{
	int *ret;
	ret = (int*)wcarray_get(current_skills,ix);
	return *ret;
}

/* Skill has */
int hero :: has_skill(int id)
{
	int i;
	int skid;
	/* Seek */
	for(i = 0;i < get_skill_count();i++)
	{
		skid = get_skill(i);
		if(id == skid)
			return 1; /* I have this one */
	}
	/* Does not have */
	return 0;
}

/* Equip has */
int hero :: has_equip(int id)
{
	int i;
	/* Seek */
	for(i = 0;i < MAX_EQUIPS;i++)
	{
		if(get_equip(i) == id)
			return 1; /* Equipped */
	}
	/* Not equipped */
	return 0;
}

/* Skill add */
void hero :: add_skill(int id)
{
	int i;
	int skid;
	int *nskid;
	/* Seek */
	for(i = 0;i < get_skill_count();i++)
	{
		skid = get_skill(i);
		if(id == skid)
			return; /* I already have this one */
	}
	/* I dont have this */
	nskid = (int*)malloc(sizeof(int));
	*nskid = id;
	wcarray_add(current_skills,nskid);
}

/* Skill remove */
void hero :: remove_skill(int id)
{
	int i;
	int skid;
	/* Seek */
	for(i = 0;i < get_skill_count();i++)
	{
		skid = get_skill(i);
		/* Found! remove it */
		if(id == skid)
		{
			wcarray_remove(current_skills,i);
			return;
		}
	}
}

/* Save heroes */
void wchero_save(FILE *f)
{
	int i,j,count;
	hero *he;
	count = wcarray_size(heroes);
	/* Write the count */
	wcsaveio_start(f);
	wcsaveio_write_num(count);
	wcsaveio_end();
	/* Write heroes */
	for(i = 0;i < count;i++)
	{
		wcsaveio_start(f);
		he = (hero*)wcarray_get(heroes,i);
		/* Save name */
		wcsaveio_write_str(he->get_name());
		/* Save appearence */
		wcsaveio_write_str(he->get_charset());
		wcsaveio_write_num(he->get_charset_number());
		wcsaveio_write_num(he->is_transparent());
		wcsaveio_write_str(he->get_faceset());
		wcsaveio_write_num(he->get_faceset_number());
		/* Save title */
		wcsaveio_write_str(he->get_title());
		/* Save level */
		wcsaveio_write_num(he->get_level());
		/* Save HP/MP */
		wcsaveio_write_num(he->get_health());
		wcsaveio_write_num(he->get_mana());
		/* Save EXP */
		wcsaveio_write_num(he->get_exp());
		/* Save stats */
		for(j = 0;j < MAX_STATS;j++)
			wcsaveio_write_num(he->get_stat_offset(j));
		/* Save equips */
		for(j = 0;j < MAX_EQUIPS;j++)
			wcsaveio_write_num(he->get_equip(j));
		/* Save skills */
		wcsaveio_write_num(he->get_skill_count());
		for(j = 0;j < he->get_skill_count();j++)
			wcsaveio_write_num(he->get_skill(j));
		wcsaveio_end();
	}
}

/* Load heroes */
void wchero_load(FILE *f)
{
	int i,count,j,k,ii;
	char *str;
	hero *he;
	/* Get count */
	count = 0;
	wcsaveio_read(f);
	count = wcsaveio_read_num();
	wcsaveio_next();
	/* Read heroes */
	for(i = 0;i < count;i++)
	{
		wcsaveio_read(f);
		if(i < wcarray_size(heroes))
		{
			he = (hero*)wcarray_get(heroes,i);
			/* Set name */
			str = wcsaveio_read_str();
			he->set_name(str);
			/* Set appearence */
			str = wcsaveio_read_str();
			j = wcsaveio_read_num();
			k = wcsaveio_read_num();
			he->set_charset(str,j,k);
			str = wcsaveio_read_str();
			j = wcsaveio_read_num();
			he->set_faceset(str,j);
			/* Get title */
			str = wcsaveio_read_str();
			he->set_title(str);
			/* Get level */
			j = wcsaveio_read_num();
			he->set_level(j);
			/* Get HP/MP */
			j = wcsaveio_read_num();
			k = wcsaveio_read_num();
			he->set_health(j);
			he->set_mana(k);
			/* Get EXP */
			j = wcsaveio_read_num();
			he->set_exp(j);
			/* Get stats */
			for(ii = 0;ii < MAX_STATS;ii++)
			{
				j = wcsaveio_read_num();
				he->set_stat(ii,j);
			}
			/* Get equips */
			for(ii = 0;ii < MAX_EQUIPS;ii++)
			{
				j = wcsaveio_read_num();
				he->set_equip(ii,j);
			}
			/* Get skills */
			k = wcsaveio_read_num();
			for(ii = 0;ii < k;ii++)
			{
				j = wcsaveio_read_num();
				he->add_skill(j);
			}
		}
		wcsaveio_next();
	}
}

/* Refresh all heroes */
void wchero_refresh()
{
	int i;
	hero *hr;
	for(i = 0;i < wcarray_size(heroes);i++)
	{
		hr = (hero*)wcarray_get(heroes,i);
		hr->refresh();
	}
}

/* Get battle position */
int hero :: get_battle_pos_x()
{
	return battle_x;
}
int hero :: get_battle_pos_y()
{
	return battle_y;
}

/* Get battleset */
int hero :: get_battleset()
{
	return battleset_id;
}

/* Get command */
int hero :: get_command(int ix)
{
	return battle_commands[ix];
}

/* Get animation */
int hero :: get_battle_animation()
{
	/* Get hands */
	if(!get_equip(0))
		return animation_id;
	/* Return the animation of equipped weapon */
	return wcitem_get(get_equip(0))->get_animation();
}

/* Get element effect */
int hero :: get_element_effect(int ix)
{
	attribute *at;
	at = wcattribute_get(ix);
	/* Out of range returns default */
	if(ix-1 < 0 || ix-1 >= elms_size)
		return at->get_effect(WCATTRIBUTE_C);
	return at->get_effect(elms[ix-1]);
}