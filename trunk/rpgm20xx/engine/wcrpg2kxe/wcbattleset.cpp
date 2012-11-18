/*
	Character Sets in Battle
*/

/* Includes */
#include "wcrpg.h"
#include "wcmaterial.h"
#include "wcbattleset.h"
#include "wctype.h"

#include <stdlib.h>

/* Globals */
table *battleset_table;
arraylist battlechar_pool = 0;
arraylist battleweapon_pool = 0;
arraylist battlesets;
float battleset_clock = 0.0f;
int battleset_frame = WCBATTLESET_FRAME_AUTO;
int battleset_reverse = 0;

/* Set options for next drawing */
void wcbattleset_set_option(int f,int m)
{
	battleset_frame = f;
	battleset_reverse = m;
}

/* Get frame */
int wcbattleset_get_frame()
{
	return battleset_frame;
}

/* Get reverse */
int wcbattleset_get_reverse()
{
	return battleset_reverse;
}

/* Handle battle sets */
void wcbattleset_handle()
{
	float ts;
	/* Advance the clock */
	ts = wcrpg_get_time_step();
	battleset_clock += ts*0.3f;
	/* Animation time nuke prevention (see A-Bomb in Oblivion) */
	if(ts > 1024.0f)
		ts = 0;
}

/* Make new battleframe */
battleframe :: battleframe(entry *en,int wp)
{
	/* Defaults */
	source = 0;
	tex = 0;
	charset = 0;
	use_animation = 0;
	animation_id = 1;
	is_weapon = wp;
	/* Get source */
	if(en->has_data(0x02))
		source = wcstr_read(en->get_data(0x02),en->get_length(0x02));
	/* Get charset */
	if(en->has_data(0x03))
		charset = wcint_read(en->get_data(0x03));
	/* Use anim? */
	if(en->has_data(0x04))
		use_animation = wcint_read(en->get_data(0x04));
	/* Anim ID */
	if(en->has_data(0x05))
		animation_id = wcint_read(en->get_data(0x05));
}

/* Draws a battle frame */
void battleframe :: draw(int x,int y,int s)
{
	int sx;
	/* Load */
	if(!tex)
	{
		if(is_weapon)
			tex = wcbattleset_pool_battleweapon(source,battleset_reverse);
		else
			tex = wcbattleset_pool_battlechar(source,battleset_reverse);
	}
	/* Fix s */
	if(battleset_frame != WCBATTLESET_FRAME_AUTO)
		s = battleset_frame-1;
	/* Draw at location */
	if(!is_weapon)
	{
		sx = s*48;
		if(battleset_reverse)
			sx = tex->get_width()-sx-48;
	}
	else
	{
		sx = s*64;
		if(battleset_reverse)
			sx = tex->get_width()-sx-64;
	}
	if(!is_weapon)
		tex->draw(x,y,48,48,sx,charset*48,48,48);
	else
		tex->draw(x,y,64,64,sx,charset*64,64,64);
}

/* Delete battleframe */
battleframe :: ~battleframe()
{
	/* Free file name */
	free(source);
}

/* Make new battleset */
battleset :: battleset(entry *en)
{
	int i;
	table *body_table;
	table *weapon_table;
	/* Defaults */
	speed = WCBATTLESET_SLOW;
	bodies = wcarray_new();
	weapons = wcarray_new();
	/* Get speed */
	if(en->has_data(0x02))
		speed = wcint_read(en->get_data(0x02));
	/* Get name */
	if(en->has_data(0x01))
		name = wcstr_read(en->get_data(0x01),en->get_length(0x01));
	/* Get bodies */
	if(en->has_data(0x0A))
	{
		body_table = new table(en->get_data(0x0A));
		for(i = 0;i < body_table->get_count();i++)
			wcarray_add(bodies,new battleframe(body_table->get_entry(i),0));
		delete body_table;
	}
	/* Get weapons */
	if(en->has_data(0x0B))
	{
		weapon_table = new table(en->get_data(0x0B));
		for(i = 0;i < weapon_table->get_count();i++)
			wcarray_add(weapons,new battleframe(weapon_table->get_entry(i),1));
		delete weapon_table;
	}
}

/* Returns name */
char *battleset :: get_name()
{
	return name;
}

/* Returns true if the given pose number is cyclical */
int wcbattleset_is_cycle(int p)
{
	if(p == 0) /* Stand */
		return 1;
	if(p == 6) /* Weakened */
		return 1;
	if(p == 8) /* Walk left */
		return 1;
	if(p == 9) /* Walk right */
		return 1;
	return 0;
}

/* Draw battleset */
void battleset :: draw(int x,int y,int p)
{
	battleframe *bf;
	float as;
	int fr;
	/* Just draw my body */
	bf = (battleframe*)wcarray_get(bodies,p);
	/* Figure out speed */
	as = battleset_clock;
	if(speed == WCBATTLESET_SLOW)
		as = battleset_clock*0.5f;
	if(speed == WCBATTLESET_QUICK)
		as = battleset_clock*1.5f;
	/* Set proper frame */
	if(!wcbattleset_is_cycle(p))
	{
		/* Figure out frame */
		fr = (int)(as);
		fr = fr%3;
	}
	else
	{
		/* Figure out cycle frame */
		fr = (int)(as);
		fr = fr%4;
		if(fr == 3)
			fr = 1;
	}
	/* Draw it */
	bf->draw(x,y,fr);
}

/* Draw weapon */
void battleset :: draw(int x,int y,int w,int h)
{
	battleframe *bf;
	float as;
	int fr;
	/* Get the weapon */
	bf = (battleframe*)wcarray_get(weapons,w);
	/* Figure out speed */
	as = battleset_clock;
	if(speed == WCBATTLESET_SLOW)
		as = battleset_clock*0.5f;
	if(speed == WCBATTLESET_QUICK)
		as = battleset_clock*1.5f;
	/* Set proper frame */
	fr = (int)(as);
	fr = fr%3;
	/* Draw first if right */
	if(h == WCBATTLESET_RIGHT_HAND)
		bf->draw(x-8,y-8,fr);
	/* Draw my body as either left or right handed */
	draw(x,y,1+h);
	/* Draw next if left */
	if(h == WCBATTLESET_LEFT_HAND)
		bf->draw(x-8,y-8,fr);
}

/* Delete battleset */
battleset :: ~battleset()
{
	int i;
	/* Release name */
	free(name);
	/* Get rid of lists */
	if(bodies)
	{
		wcarray_delete(bodies);
		for(i = 0;i < wcarray_size(bodies);i++)
			delete (battleframe*)wcarray_get(bodies,i);
	}
	if(weapons)
	{
		wcarray_delete(weapons);
		for(i = 0;i < wcarray_size(weapons);i++)
			delete (battleframe*)wcarray_get(weapons,i);
	}
}

/* Delete battle charset */
void wcbattleset_release_battlechar(void *v)
{
	battlechar *bc;
	bc = (battlechar*)v;
	free(bc->name);
	delete bc->tex;
	delete bc->im;
	free(v);
}

/* Delete battle weapon */
void wcbattleset_release_battleweapon(void *v)
{
	battleweapon *bw;
	bw = (battleweapon*)v;
	free(bw->name);
	delete bw->tex;
	delete bw->im;
	free(v);
}

/* Load battle char */
texture *wcbattleset_pool_battlechar(char *name,int mir)
{
	int i;
	battlechar *bc;
	image *im;
	/* Find an existing one */
	for(i = 0;i < wcarray_size(battlechar_pool);i++)
	{
		bc = (battlechar*)wcarray_get(battlechar_pool,i);
		if(!strcmp(bc->name,name) && bc->mir == mir)
			return bc->tex;
	}
	/* Make new */
	bc = (battlechar*)malloc(sizeof(battlechar));
	bc->name = wcrpg_string_clone(name);
	bc->mir = mir;
	im = new image(wcmaterial_request(MATERIAL_BATTLE_CHARSET,name));
	if(mir)
		im->flip_horz();
	bc->tex = new texture(im);
	im->reduce();
	bc->im = im;
	/*delete im;*/
	/* Add and return */
	wcarray_add(battlechar_pool,bc);
	return bc->tex;
}

/* Load battle weapon */
texture *wcbattleset_pool_battleweapon(char *name,int mir)
{
	int i;
	battleweapon *bw;
	/* Find an existing one */
	for(i = 0;i < wcarray_size(battleweapon_pool);i++)
	{
		bw = (battleweapon*)wcarray_get(battleweapon_pool,i);
		if(!strcmp(bw->name,name) && bw->mir == mir)
			return bw->tex;
	}
	/* Make new */
	bw = (battleweapon*)malloc(sizeof(battleweapon));
	bw->name = wcrpg_string_clone(name);
	bw->mir = mir;
	bw->im = new image(wcmaterial_request(MATERIAL_BATTLE_WEAPON,name));
	if(mir)
		bw->im->flip_horz();
	bw->tex = new texture(bw->im);
	bw->im->reduce();
	/* Add and return */
	wcarray_add(battleweapon_pool,bw);
	return bw->tex;
}

/* Reload pool */
void wcbattleset_reload_all_set()
{
	/* Delete the old pools */
	if(battlechar_pool)
		wcarray_delete(battlechar_pool);
	if(battleweapon_pool)
		wcarray_delete(battleweapon_pool);
	/* Make new pools */
	battlechar_pool = wcarray_new();
	battleweapon_pool = wcarray_new();
	wcarray_set_clean(battlechar_pool,wcbattleset_release_battlechar);
	wcarray_set_clean(battleweapon_pool,wcbattleset_release_battleweapon);
}

/* Load all battle sets */
void wcbattleset_load_all(FILE *f,int len)
{
	int i;
	battleset *bs;
	/* Load table */
	battleset_table = new table(f);
	battlesets = wcarray_new();
	/* Create sets */
	for(i = 0;i < battleset_table->get_count();i++)
	{
		bs = new battleset(battleset_table->get_entry(i));
		wcarray_add(battlesets,bs);
	}
	/* Init pool */
	wcbattleset_reload_all_set();
}

/* Get rid of all */
void wcbattleset_free_all()
{
	int i;
	/* Delete pool */
	wcarray_delete(battlechar_pool);
	wcarray_delete(battleweapon_pool);
	/* Delete list */
	for(i = 0;i < wcarray_size(battlesets);i++)
		delete (battleset*)wcarray_get(battlesets,i);
	wcarray_delete(battlesets);
}

/* Draw a battler */
void wcbattleset_draw(int x,int y,int c,int p)
{
	battleset *bs;
	/* Get battle set */
	bs = (battleset*)wcarray_get(battlesets,c);
	/* Draw battle set */
	bs->draw(x,y,p);
}

/* Draw an attack */
void wcbattleset_draw_attack(int x,int y,int c,int w,int h)
{
	battleset *bs;
	/* Get battle set */
	bs = (battleset*)wcarray_get(battlesets,c);
	/* Draw battle set */
	bs->draw(x,y,w,h);
}

/* Find battleset */
int wcbattleset_seek(char *name)
{
	int i;
	battleset *bs;
	for(i = 0;i < wcarray_size(battlesets);i++)
	{
		bs = (battleset*)wcarray_get(battlesets,i);
		if(!strcmp(bs->get_name(),name))
			return i; /* Found! */
	}
	/* Not found */
	return 0;
}