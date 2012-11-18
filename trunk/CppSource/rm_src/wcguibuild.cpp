/*
	GUI-Builder, allows the user of the editor to compose custom menu systems
	using the basic system graphics
*/

/* Include */
#include <malloc.h>
#include "wcguibuild.h"
#include "wcwindow.h"
#include "wcarray.h"
#include "wcbattleset.h"
#include "wcmonster.h"
#include "wcrpg.h"
#include "wcmaterial.h"
#include "wcguiname.h"

/* Globals */
arraylist wcguibuild_list;
arraylist wcguibuild_image_pool = 0;

/* Release pooled image */
void wcguibuild_free_image(void *v)
{
	imageentry *ie;
	ie = (imageentry*)v;
	delete ie->tex;
	free(ie->name);
	ie->resid = 0;
	free(ie);
}

/* Initialize image pool */
void wcguibuild_reset_image_pool()
{
	/* Release old */
	if(wcguibuild_image_pool)
		wcarray_delete(wcguibuild_image_pool);
	/* Make new and set cleaner */
	wcguibuild_image_pool = wcarray_new();
	wcarray_set_clean(wcguibuild_image_pool,wcguibuild_free_image);
}

/* Delete image pool */
void wcguibuild_delete_image_pool()
{
	/* Release old */
	if(wcguibuild_image_pool)
		wcarray_delete(wcguibuild_image_pool);
	/* Zero set */
	wcguibuild_image_pool = 0;
}

/* Initialize */
void wcguibuild_init()
{
	/* Make new */
	wcguibuild_list = wcarray_new();
	/* Reset image pool */
	wcguibuild_reset_image_pool();
}

/* Pool an image */
texture *wcguibuild_pool_image(int resid)
{
	int i;
	imageentry *ie;
	/* First, see if an existing resid may be found */
	for(i = 0;i < wcarray_size(wcguibuild_image_pool);i++)
	{
		ie = (imageentry*)wcarray_get(wcguibuild_image_pool,i);
		/* If found, return resource */
		if(ie->resid == resid)
			return ie->tex;
	}
	/* Create */
	ie = (imageentry*)malloc(sizeof(imageentry));
	ie->resid = resid;
	ie->name = wcrpg_string_clone(wcmaterial_request_filename_index(MATERIAL_PICTURE,resid));
	ie->tex = new texture(ie->name);
	/* Add */
	wcarray_add(wcguibuild_image_pool,ie);
	/* Report */
	wclog_write(wcrpg_get_log(),"Loaded particle ");
	wclog_write(wcrpg_get_log(),ie->name);
	wclog_enter(wcrpg_get_log());
	/* Return resource */
	return ie->tex;
}

/* Exit */
void wcguibuild_exit()
{
	/* Delete images */
	wcguibuild_delete_image_pool();
	/* Delete */
	wcarray_delete(wcguibuild_list);
}

/* Draw all the elements to the screen and flush them */
void wcguibuild_draw()
{
	guielement *ge;
	/* Consume all the waiting elements */
	while(wcarray_size(wcguibuild_list) > 0)
	{
		/* Get and draw */
		ge = (guielement*)wcarray_get(wcguibuild_list,0);
		ge->draw();
		/* Delete and remove */
		delete ge;
		wcarray_remove(wcguibuild_list,0);
	}
}

/* Add to drawing list */
void wcguibuild_add(guielement *ge)
{
	/* Add */
	wcarray_add(wcguibuild_list,ge);
}

/* Make new element */
guielement :: guielement(int t,int x,int y)
{
	/* Set */
	type = t;
	pos_x = x;
	pos_y = y;
	num = 0;
	option = 0;
	frame = wcbattleset_get_frame();
	mirror = wcbattleset_get_reverse();
}

/* Set number */
void guielement :: set_number(int n)
{
	num = n;
}

/* Set option */
void guielement :: set_option(int o)
{
	option = o;
}

/* Set size */
void guielement :: set_size(int w,int h)
{
	width = w;
	height = h;
}

/* Draws this element */
void guielement :: draw()
{
	texture *tex;
	/* Number */
	if(type == GUI_BUILDER_NUMBER)
	{
		/* Either normal or battle number */
		if(option == 0)
			wcwindow_draw_number(pos_x,pos_y,num);
		else
			wcwindow_draw_battle_number(pos_x,pos_y,num);
	}
	/* Window */
	if(type == GUI_BUILDER_WINDOW)
	{
		/* Draw that window */
		wcwindow_draw(pos_x,pos_y,width,height);
	}
	/* Health bar */
	if(type == GUI_BUILDER_HEALTH_BAR)
	{
		/* Draw the health */
		wcwindow_draw_bar(pos_x,pos_y,num,0,width);
	}
	/* Mana bar */
	if(type == GUI_BUILDER_MANA_BAR)
	{
		/* Draw the mana */
		wcwindow_draw_bar(pos_x,pos_y,num,1,width);
	}
	/* Time bar */
	if(type == GUI_BUILDER_TIME_BAR)
	{
		/* Draw the mana */
		wcwindow_draw_bar(pos_x,pos_y,num,2,width);
	}
	/* Battle character */
	if(type == GUI_BUILDER_BATTLE_CHARACTER)
	{
		/* Draw the character */
		wcbattleset_set_option(frame,mirror);
		wcbattleset_draw(pos_x,pos_y,num-1,option-1);
	}
	/* Battle attack right */
	if(type == GUI_BUILDER_BATTLE_ATTACK_RIGHT)
	{
		/* Draw the character */
		wcbattleset_set_option(frame,mirror);
		wcbattleset_draw_attack(pos_x,pos_y,num-1,option-1,0);
	}
	/* Battle attack left */
	if(type == GUI_BUILDER_BATTLE_ATTACK_LEFT)
	{
		/* Draw the character */
		wcbattleset_set_option(frame,mirror);
		wcbattleset_draw_attack(pos_x,pos_y,num-1,option-1,1);
	}
	/* Monster */
	if(type == GUI_BUILDER_MONSTER)
	{
		/* Draw the monster */
		wcbattleset_set_option(frame,mirror);
		wcmonster_draw(num,pos_x,pos_y,option-1);
	}
	/* Random image */
	if(type == GUI_BUILDER_IMAGE)
	{
		/* Draw the image */
		tex = wcguibuild_pool_image(num);
		tex->set_transparency(((float)option)/100.0f,((float)option)/100.0f);
		tex->draw(pos_x,pos_y);
	}
	/* Name */
	if(type == GUI_BUILDER_NAME)
	{
		/* Draw the name */
		wcguiname_draw(option,num,pos_x,pos_y);
	}
}