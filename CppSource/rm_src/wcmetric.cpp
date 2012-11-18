/*
	Battle Metrics and Calculations
*/

/* Includes */
#include <stdlib.h>
#include "wcarray.h"
#include "wcmetric.h"
#include "wcbattle.h"
#include "wcitem.h"
#include "wcattribute.h"

/* Globals */
FILE *stdbattle = 0;
arraylist wcmetric_field;
int wcmetric_lowest_speed = -1;
int wcmetric_setting = WCMETRIC_DEFAULT;

/* Returns flux */
float wcmetric_roll()
{
	return ((float)(rand()%10000))/9999.0f;
}

/* Returns flux on intergal percentage */
int wcmetric_roll(int p)
{
	return ((wcmetric_roll()*100.0f) <= ((float)p));
}

/* Check lowest speed */
void wcmetric_check_lowest_speed(int spd)
{
	/* Initialize */
	if(wcmetric_lowest_speed == -1)
	{
		wcmetric_lowest_speed = spd;
		return;
	}
	/* Check */
	if(spd < wcmetric_lowest_speed)
		wcmetric_lowest_speed = spd;
}

/* Initialize */
void wcmetric_init()
{
	/* Open channel to battle log */
#ifndef NDEBUG
	//fopen("wcrpgbattle.txt","w");
#endif
#ifdef NDEBUG
	stdbattle = stdout;
#endif
	/* Make field */
	wcmetric_field = wcarray_new();
}

/* End */
void wcmetric_exit()
{
	/* Release battlers */
	wcmetric_clear_field();
	/* Release field */
	wcarray_delete(wcmetric_field);
	/* Close channel */
#ifndef NDEBUG
	fclose(stdbattle);
#endif
}

/* Spawns a battler */
battler :: battler(hero *hr,int ix)
{
	hero_link = hr;
	monster_link = 0;
	hp = 0;
	mp = 0;
	tb = 0.0f;
	x = hr->get_battle_pos_x();
	y = hr->get_battle_pos_y();
	index = ix;
}

/* Spawns a battler */
battler :: battler(monster *mn,int px,int py,int ix)
{
	hero_link = 0;
	monster_link = mn;
	hp = mn->get_max_hp();
	mp = mn->get_max_mp();
	tb = 0.0f;
	x = px;
	y = py;
	index = ix;
}

/* Delete */
battler :: ~battler()
{
}

/* Gets name */
char *battler :: get_name()
{
	if(hero_link)
		return hero_link->get_name();
	return monster_link->get_name();
}

/* Add hero to battle */
void wcmetric_add_hero(hero *hr)
{
	battler *bt;
	bt = new battler(hr,wcmetric_field_size());
	wcmetric_check_lowest_speed(bt->get_speed());
	wcarray_add(wcmetric_field,bt);
#ifndef NDEBUG
	fprintf(stdbattle,"%s has joined the fight.\n",hr->get_name());
#endif
}

/* Add monster to battle */
void wcmetric_add_monster(monster *mn,int x,int y)
{
	battler *bt;
	bt = new battler(mn,x,y,wcmetric_field_size());
	wcmetric_check_lowest_speed(bt->get_speed());
	wcarray_add(wcmetric_field,bt);
#ifndef NDEBUG
	fprintf(stdbattle,"%s has joined the fight.\n",mn->get_name());
#endif
}

/* Wipe out the field */
void wcmetric_clear_field()
{
	/* Collapse */
	while(wcarray_size(wcmetric_field) > 0)
	{
		delete (battler*)wcarray_get(wcmetric_field,0);
		wcarray_remove(wcmetric_field,0);
	}
	/* Reset speed */
	wcmetric_lowest_speed = -1;
#ifndef NDEBUG
	fprintf(stdbattle,"Field is now empty.\n");
#endif
}

/* Is hero */
int battler :: is_hero()
{
	return (hero_link != 0);
}

/* Get hero */
hero *battler :: get_hero()
{
	return hero_link;
}

/* Get monster */
monster *battler :: get_monster()
{
	return monster_link;
}

/* Get field pop */
int wcmetric_field_size()
{
	return wcarray_size(wcmetric_field);
}

/* Get battler */
battler *wcmetric_get_battler(int ix)
{
	return (battler*)wcarray_get(wcmetric_field,ix);
}

/* Set time */
void battler :: set_time_bar(float n)
{
	tb = n;
}

/* Get time */
float battler :: get_time_bar()
{
	return tb;
}

/* Get speed */
int battler :: get_speed()
{
	if(is_hero())
		return hero_link->get_stat(STAT_SPEED);
	return monster_link->get_speed();
}

/* Get attack */
int battler :: get_attack()
{
	if(is_hero())
		return hero_link->get_stat(STAT_ATTACK);
	return monster_link->get_attack();
}

/* Get defense */
int battler :: get_defense()
{
	if(is_hero())
		return hero_link->get_stat(STAT_DEFENSE);
	return monster_link->get_defense();
}

/* Get lowest speed */
int wcmetric_get_lowest_speed()
{
	return wcmetric_lowest_speed;
}

/* Get health */
int battler :: get_health()
{
	if(is_hero())
		return hero_link->get_health();
	return hp;
}

/* Get mana */
int battler :: get_mana()
{
	if(is_hero())
		return hero_link->get_mana();
	return mp;
}

/* Get battler by section and count */
battler *wcmetric_get_battler(int hr,int ix)
{
	int ct,i;
	/* Seek the whole field */
	ct = 0;
	for(i = 0;i < wcmetric_field_size();i++)
	{
		/* Found a hero */
		if(wcmetric_get_battler(i)->is_hero() == hr)
		{
			/* This is the one */
			if(ct == ix)
			{
				if(hr || !wcmetric_get_battler(i)->is_dead())
					return wcmetric_get_battler(i);
			}
			else
			{
				/* Next */
				ct++;
			}
		}
	}
	/* No one found */
	return 0;
}

/* Get position */
int battler :: get_pos_x()
{
	return x;
}
int battler :: get_pos_y()
{
	return y;
}

/* Get index */
int battler :: get_index()
{
	return index;
}

/* Get hurt */
void battler :: damage(int amt)
{
	int fhp;
	/* Get health */
	fhp = get_health();
	/* Sub */
	fhp -= amt;
	/* Zero out */
	if(fhp < 0)
		fhp = 0;
	/* Set back */
	if(hero_link)
		hero_link->set_health(fhp);
	else
		hp = fhp;
}

/* Get element effect */
int battler :: get_element_effect(int ix)
{
	if(is_hero())
		return hero_link->get_element_effect(ix);
	return monster_link->get_element_effect(ix);
}

/* Returns hit rate on target */
int wcmetric_hit_rate(battler *a,battler *b)
{
	item *it;
	float aga,agb,agc,agf,agw;
	/* RPG Maker Standard METRIC */
	if(wcmetric_setting == WCMETRIC_DEFAULT)
	{
		/* Figure out agi component */
		aga = (float)a->get_speed();
		agb = (float)b->get_speed();
		agc = agb/aga;
		agc = agc-1.0f;
		agc /= 2.0f;
		agc += 1.0f;
		/* Figure out weapon component */
		agw = 0.0f;
		/* Must be hero */
		if(a->is_hero())
		{
			/* Must be equipped */
			if(a->get_hero()->get_equip(0))
			{
				it = wcitem_get(a->get_hero()->get_equip(0));
				agw = 100.0f-((float)it->get_accuracy());
				agw /= 100.0f;
			}
		}
		/* Final */
		agf = 1.0f-agw*agc;
	}
	/* Return result */
	return (int)(agf*100.0f);
}

/* Simulate the effect of an item on the target battler */
int wcmetric_item_element_effect(item *it,battler *t)
{
	int i,welm,melm,eff,welm_use,melm_use;
	attribute *at;
	eff = 0;
	/* RPG Maker Standard METRIC */
	if(wcmetric_setting == WCMETRIC_DEFAULT)
	{
		/* Set weapon and magic effects to zero */
		welm = 0;
		melm = 0;
		welm_use = 0;
		melm_use = 0;
		/* Get max weapon effect */
		for(i = 0;i < wcattribute_count();i++)
		{
			/* Had attribute? */
			if(it->get_attribute(i))
			{
				/* Get at */
				at = wcattribute_get(i+1);
				/* Branch on type */
				if(at->get_type() == WCATTRIBUTE_WEAPON)
				{
					welm_use = 1;
					if(t->get_element_effect(i+1) > welm)
						welm = t->get_element_effect(i+1);
				}
				else
				{
					melm_use = 1;
					if(t->get_element_effect(i+1) > melm)
						melm = t->get_element_effect(i+1);
				}
			}
		}
		/* Total result */
		if(!welm_use)
			welm = 100;
		if(!melm_use)
			melm = 100;
		eff = (int)((((float)melm)/100.0f)*(((float)welm)/100.0f)*100.0f);
	}
	/* Return result */
	return eff;
}

/* Simulate attack based on current METRIC where a attacks b */
int wcmetric_attack(battler *a,battler *b)
{
	int eff;
	float dmg;
	float rdmg;
	float fdmg;
	dmg = 0.0f;
	rdmg = 0.0f;
	fdmg = 0.0f;
	/* RPG Maker Standard METRIC */
	if(wcmetric_setting == WCMETRIC_DEFAULT)
	{
		/* Basic damage */
		dmg += ((float)a->get_attack())/2.0f;
		dmg -= ((float)a->get_defense())/4.0f;
		/* Roll */
		rdmg = wcmetric_roll()*0.4f; /* 40% spread */
		rdmg = dmg*rdmg;
		/* Base damage is 80% */
		dmg *= 0.8f;
		/* Final damage */
		fdmg = dmg+rdmg;
		fdmg *= 2.0f;
	}
	/* Adjust for elements */
	if(a->is_hero())
	{
		if(a->get_hero()->get_equip(0))
		{
			eff = wcmetric_item_element_effect(wcitem_get(a->get_hero()->get_equip(0)),b);
#ifndef NDEBUG
			fprintf(stdbattle,"Effect of %s is %d%% - ",wcitem_get(a->get_hero()->get_equip(0))->get_name(),eff);
#endif
			fdmg *= ((float)eff)/100.0f;
		}
	}
	/* Hurt target */
	b->damage((int)fdmg);
	/* Reel back target if hero */
	if(b->is_hero())
		wcbattle_reel_hero(b->get_hero());
	/* Announce */
#ifndef NDEBUG
	fprintf(stdbattle,"%s attacks %s for %d damage! (Hit %d%%)\n",a->get_name(),b->get_name(),(int)fdmg,wcmetric_hit_rate(a,b));
#endif
	/* Return result */
	return (int)fdmg;
}

/* Get death */
int battler :: is_dead()
{
	if(get_health() <= 0)
		return 1;
	return 0;
}

/* Get enemy cound */
int wcmetric_enemy_count()
{
	int i;
	int ct;
	ct = 0;
	/* Count enemies */
	for(i = 0;i < wcmetric_field_size();i++)
	{
		if(!wcmetric_get_battler(i)->is_dead() && !wcmetric_get_battler(i)->is_hero())
			ct++;
	}
	/* Return result */
	return ct;
}

/* Get next alive enemy */
int wcmetric_next_enemy(int ix)
{
	int i;
	/* Go */
	ix++;
	for(i = ix;i < wcmetric_field_size();i++)
	{
		/* Find the next alive monster */
		if(!wcmetric_get_battler(i)->is_dead() && !wcmetric_get_battler(i)->is_hero())
			return i;
	}
	/* If not found return self */
	return ix;
}

/* Get next alive enemy */
battler *wcmetric_next_enemy(battler *cur)
{
	int ix;
	/* Get my id */
	ix = cur->get_index();
	/* Return next */
	return wcmetric_get_battler(wcmetric_next_enemy(ix));
}

/* Are all controllable players dead? */
int wcmetric_game_over()
{
	int i;
	battler *bt;
	/* Seek all */
	for(i = 0;i < wcmetric_field_size();i++)
	{
		bt = wcmetric_get_battler(i);
		/* Just need one left standing */
		if(bt->is_hero() && bt->get_health() > 0)
			return 0;
	}
	/* No one is alive */
	return 1;
}