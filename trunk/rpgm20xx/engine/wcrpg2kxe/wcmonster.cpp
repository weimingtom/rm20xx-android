/*
	Enemies
*/

/* Includes */
#include <malloc.h>
#include <memory.h>
#include "wcrpg.h"
#include "wcarray.h"
#include "wcmonster.h"
#include "wctype.h"
#include "wcmaterial.h"
#include "wcbattleset.h"
#include "wcattribute.h"

#include <string>

/* Globals */
table *monster_table;
arraylist monsters;
arraylist monster_pics = 0;
int monster_auto_charset = 0;

/* Enable/Disable auto battle charset */
void wcmonster_enable_auto_battle_charset(int en)
{
	monster_auto_charset = en;
}

/* Get state of auto anim */
int wcmonster_get_auto_battle_charset()
{
	return monster_auto_charset;
}

/* Erase entry */
void wcmonster_free_picture(void *v)
{
	monsterpic *mp;
	mp = (monsterpic*)v;
	free(mp->name);
	delete mp->tex;
	free(v);
}

/* Reset pools */
void wcmonster_reset_picture_pool()
{
	/* Erase old */
	if(monster_pics)
		wcarray_delete(monster_pics);
	/* Make new and set cleaner */
	monster_pics = wcarray_new();
	wcarray_set_clean(monster_pics,wcmonster_free_picture);
}

/* Pool */
texture *wcmonster_pool_picture(char *name)
{
	int i;
	monsterpic *mp;
	/* Find the name */
	for(i = 0;i < wcarray_size(monster_pics);i++)
	{
		mp = (monsterpic*)wcarray_get(monster_pics,i);
		if(!strcmp(mp->name,name))
		{
			/* Match found, return tex */
			return mp->tex;
		}
	}
	/* Not found, alloc */
	mp = (monsterpic*)malloc(sizeof(monsterpic));
	mp->name = wcrpg_string_clone(name);
	mp->tex = new texture(wcmaterial_request(MATERIAL_MONSTER,name));
	mp->tex->enable_flash();
	wcarray_add(monster_pics,mp);
	/* Return result */
	return mp->tex;
}

/* Create monster */
monster :: monster(entry *en)
{
	int i;
	unsigned char *bptr;
	/* Defaults */
	name = 0;
	pic = 0;
	hp = 10;
	mp = 10;
	battleset_id = WCMONSTER_DEFAULT_BATTLESET;
	elms = 0;
	elms_size = 0;
	exp_win = 0;
	money_win = 0;
	item_id = 0;
	item_chance = 100;
	crit_chance = 30;
	crit_has = 0;
	flying = 0;
	trans = 0;
	often_miss = 0;
	/* Load */
	if(en->has_data(0x01))
		name = wcstr_read(en->get_data(0x01),en->get_length(0x01));
	if(en->has_data(0x02))
		pic = wcstr_read(en->get_data(0x02),en->get_length(0x02));
	if(en->has_data(0x04))
		hp = wcint_read(en->get_data(0x04));
	if(en->has_data(0x05))
		mp = wcint_read(en->get_data(0x05));
	/* Get stats */
	if(en->has_data(0x06))
		attack = wcint_read(en->get_data(0x06));
	if(en->has_data(0x07))
		defense = wcint_read(en->get_data(0x07));
	if(en->has_data(0x08))
		power = wcint_read(en->get_data(0x08));
	if(en->has_data(0x09))
		speed = wcint_read(en->get_data(0x09));
	/* Get loots */
	if(en->has_data(0x0B))
		exp_win = wcint_read(en->get_data(0x0B));
	if(en->has_data(0x0C))
		money_win = wcint_read(en->get_data(0x0C));
	if(en->has_data(0x0D))
		item_id = wcint_read(en->get_data(0x0D));
	if(en->has_data(0x0E))
		item_chance = wcint_read(en->get_data(0x0E));
	/* Get crits */
	if(en->has_data(0x15))
		crit_has = wcint_read(en->get_data(0x15));
	if(en->has_data(0x16))
		crit_chance = wcint_read(en->get_data(0x16));
	/* Get special */
	if(en->has_data(0x1A))
		often_miss = wcint_read(en->get_data(0x1A));
	if(en->has_data(0x1C))
		flying = wcint_read(en->get_data(0x1C));
	if(en->has_data(0x0A))
		trans = wcint_read(en->get_data(0x0A));
	/* Get elements */
	if(en->has_data(0x21))
		elms_size = wcint_read(en->get_data(0x21));
	if(en->has_data(0x22))
	{
		elms = (unsigned char*)malloc(elms_size);
		bptr = (unsigned char*)en->get_data(0x22);
		for(i = 0;i < elms_size;i++)
			elms[i] = bptr[i];
	}
}

/* Delete monster */
monster :: ~monster()
{
	free(name);
	free(pic);
	free(elms);
}

/* Get element */
int monster :: get_element_effect(int ix)
{
	attribute *at;
	at = wcattribute_get(ix);
	/* Out of range returns default */
	if(ix-1 < 0 || ix-1 >= elms_size)
		return at->get_effect(WCATTRIBUTE_C);
	return at->get_effect(elms[ix-1]);
}

/* Get stats */
int monster :: get_attack()
{
	return attack;
}
int monster :: get_defense()
{
	return defense;
}
int monster :: get_power()
{
	return power;
}
int monster :: get_speed()
{
	return speed;
}

/* Get hp */
int monster :: get_max_hp()
{
	return hp;
}

/* Get mp */
int monster :: get_max_mp()
{
	return mp;
}

/* Load all */
void wcmonster_load_all(FILE *f,int len)
{
	int i;
	monster *mn;
	/* Prepare things */
	monster_table = new table(f);
	monsters = wcarray_new();
	/* Create monsters */
	for(i = 0;i < monster_table->get_count();i++)
	{
		mn = new monster(monster_table->get_entry(i));
		wcarray_add(monsters,mn);
	}
	/* Reset pool */
	wcmonster_reset_picture_pool();
}

/* Free all */
void wcmonster_free_all()
{
	int i;
	for(i = 0;i < wcarray_size(monsters);i++)
		delete (monster*)wcarray_get(monsters,i);
	wcarray_delete(monsters);
	delete monster_table;
	wcarray_delete(monster_pics);
}

/* Get monster by id */
monster *wcmonster_get(int id)
{
	if(id <= 0 || id > wcarray_size(monsters))
	{
		wcrpg_error("Invalid monster id (%d)",id);
		return 0;
	}
	id--;
	return (monster*)wcarray_get(monsters,id);
}

/* Draw monster */
void monster :: draw(int x,int y)
{
	texture *tex;
	/* Get picture */
	tex = wcmonster_pool_picture(pic);
	/* Draw picture */
	tex->draw(x,y);
}

/* Draw monster with flash */
void monster :: draw(int x,int y,float a,int r,int g,int b,float alpha)
{
	texture *tex;
	/* Get picture */
	tex = wcmonster_pool_picture(pic);
	/* Draw picture */
	tex->set_flash(a,r,g,b);
	tex->set_transparency(alpha,alpha);
	tex->draw(x,y);
	tex->set_transparency(0.0f,0.0f);
	tex->set_flash(0.0f,0,0,0);
}

/* Get battle id */
int monster :: get_battleset_id()
{
	return battleset_id;
}

/* Set battle id */
void monster :: set_battleset_id(int id)
{
	battleset_id = id;
}

/* Get name */
char *monster :: get_name()
{
	return name;
}

/* Has battleset */
int wcmonster_valid_battleset(monster *mn)
{
	/* True? */
	if(monster_auto_charset)
	{
		/* Is it valid? */
		if(mn->get_battleset_id() != WCMONSTER_NO_BATTLESET)
			return 1;
	}
	return 0;
}

/* Find battle set ID */
void wcmonster_find_battleset_id(monster *mn)
{
	int idt;
	/* There might be a battle charset */
	if(monster_auto_charset)
	{
		/* Alright do we already have battleset id? */
		if(mn->get_battleset_id() == WCMONSTER_DEFAULT_BATTLESET)
		{
			/* No, try finding one */
			idt = wcbattleset_seek(mn->get_name());
			/* Set it */
			mn->set_battleset_id(idt);
		}
	}
}

/* Draw monster */
void wcmonster_draw(int id,int x,int y,int p)
{
	monster *mn;
	/* Get monster */
	mn = (monster*)wcmonster_get(id);
	/* There might be a battle charset */
	wcmonster_find_battleset_id(mn);
	if(wcmonster_valid_battleset(mn))
	{
		wcbattleset_draw(x,y,mn->get_battleset_id(),p);
		return;
	}
	/* If there is no battle charset */
	mn->draw(x,y);
}

/* Draw monster with flash */
void wcmonster_draw(int id,int x,int y,int p,float a,int r,int g,int b,float alpha)
{
	monster *mn;
	/* Get monster */
	mn = (monster*)wcmonster_get(id);
	/* There might be a battle charset */
	wcmonster_find_battleset_id(mn);
	if(wcmonster_valid_battleset(mn))
	{
		wcbattleset_draw(x,y,mn->get_battleset_id(),p);
		return;
	}
	/* If there is no battle charset */
	mn->draw(x,y,a,r,g,b,alpha);
}

/* Get monster size */
int monster :: get_width()
{
	/* Get set */
	wcmonster_find_battleset_id(this);
	/* Battleset */
	if(wcmonster_valid_battleset(this))
		return 48;
	return wcmonster_pool_picture(pic)->get_visible_width();
}
int monster :: get_height()
{
	/* Get set */
	wcmonster_find_battleset_id(this);
	/* Battleset */
	if(wcmonster_valid_battleset(this))
		return 48;
	return wcmonster_pool_picture(pic)->get_visible_height();
}

/* Access data */
void monster :: access_data(var *base)
{
	base[0] = hp;
	base[1] = mp;
	base[2] = attack;
	base[3] = defense;
	base[4] = power;
	base[5] = speed;
	base[6] = exp_win;
	base[7] = money_win;
	base[8] = item_id;
	base[9] = item_chance;
	base[10] = crit_has;
	base[11] = crit_chance;
	base[12] = often_miss;
	base[13] = trans;
	base[14] = flying;
}