/*
	Equipment Plates
*/

/* Includes */
#include "wcgearplate.h"
#include "wchero.h"
#include "wcwindow.h"
#include "wcvocab.h"
#include "wcattribute.h"

/* Draw equipment */
void wcgearplate_draw(int x,int y,int ix,int s,int rm)
{
	hero *hr;
	item *it;
	text *tx;
	int i;
	/* Get the subject */
	hr = wchero_get(ix);
	/* Draw the window */
	wcwindow_draw(x,y,128,MAX_EQUIPS*16+16);
	/* Draw the selector */
	if(s)
		wcwindow_draw_cursor(x+8,y+(s-1)*16+8,128-16,16,1);
	/* Get all equips */
	for(i = 0;i < MAX_EQUIPS;i++)
	{
		/* If is removal mode */
		if(rm && i == s-1)
		{
			/* Name it red */
			tx = new text(wcvocab_get(136+i));
			tx->set_color(4);
			tx->move(x+10,y+6+i*16);
			tx->draw();
			delete tx;
		}
		else
		{
			/* If even has equipped item */
			if(hr->get_equip(i))
			{
				/* Get the item */
				it = wcitem_get(hr->get_equip(i));
				/* Create and draw text, then delete */
				tx = new text(it->get_name());
				tx->move(x+10,y+6+i*16);
				tx->draw();
				delete tx;
			}
			else
			{
				/* Name it */
				tx = new text(wcvocab_get(136+i));
				tx->set_color(3);
				tx->move(x+10,y+6+i*16);
				tx->draw();
				delete tx;
			}
		}
	}
}

/* Draw equipment expanded summary */
#include "wcrpg.h"
void wcgearplate_draw_extra(int x,int y,item *it)
{
	attribute *at;
	text *tx;
	int i;
	arraylist ls;
	/* No item */
	if(!it)
		return;
	/* Allocate */
	ls = wcarray_new();
	/* Get all atts */
	for(i = 0;i < it->get_attribute_count();i++)
	{
		/* If has attribute, add to list */
		if(it->get_attribute(i))
		{
			at = (attribute*)wcattribute_get(i+1);
			wcarray_add(ls,at->get_name());
		}
	}
	/* List too small? */
	if(wcarray_size(ls) <= 0)
		return;
	/* Draw a window that may fit the descriptions */
	wcwindow_draw(x,y,128,16+16*wcarray_size(ls));
	/* Draw the descriptions */
	for(i = 0;i < wcarray_size(ls);i++)
	{
		tx = new text((char*)wcarray_get(ls,i));
		tx->move(x+8,y+8+16*i);
		tx->draw();
		delete tx;
	}
	/* Remove */
	wcarray_delete(ls);
}