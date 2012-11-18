/*
	Terrains
*/

/* Includes */
#include "wcdraw.h"
#include "wcrpg.h"
#include "wcterrain.h"
#include "wctype.h"
#include "wcmaterial.h"

#include <stdlib.h>

/* Globals */
table *terrain_table;
arraylist terrains;
texture *terrain_tex = 0;
terrain *terrain_current = 0;
int terrain_standing = 0;

/* Draw terrain */
void terrain :: draw()
{
	if(terrain_current != this)
		set();
	terrain_tex->draw(0,0);
}

/* Set terrain */
void terrain :: set()
{
	/* Erase old */
	if(terrain_tex)
		delete terrain_tex;
	/* Make new */
	terrain_tex = new texture(wcmaterial_request(MATERIAL_BACKDROP,file));
	terrain_current = this;
}

/* Create terrain */
terrain :: terrain(entry *en)
{
	/* Defaults */
	name = 0;
	file = 0;
	/* Get name and file */
	if(en->has_data(0x01))
		name = wcstr_read(en->get_data(0x01),en->get_length(0x01));
	if(en->has_data(0x04))
		file = wcstr_read(en->get_data(0x04),en->get_length(0x04));
}

/* Delete terrain */
terrain :: ~terrain()
{
	free(name);
	free(file);
}

/* Load all terrains */
void wcterrain_load_all(FILE *f,int len)
{
	int i;
	terrain *tr;
	/* Read in */
	terrain_table = new table(f);
	terrains = wcarray_new();
	/* Load all */
	for(i = 0;i < terrain_table->get_count();i++)
	{
		tr = new terrain(terrain_table->get_entry(i));
		wcarray_add(terrains,tr);
	}
}

/* Free terrains */
void wcterrain_free_all()
{
	int i;
	for(i = 0;i < wcarray_size(terrains);i++)
		delete (terrain*)wcarray_get(terrains,i);
	wcarray_delete(terrains);
	delete terrain_table;
	if(terrain_tex)
		delete terrain_tex;
}

/* Get terrain */
terrain *wcterrain_get(int id)
{
	if(id <= 0 || id > wcarray_size(terrains))
	{
		wcrpg_error("Invalid terrain ID (%d)",id);
		return 0;
	}
	id--;
	return (terrain*)wcarray_get(terrains,id);
}