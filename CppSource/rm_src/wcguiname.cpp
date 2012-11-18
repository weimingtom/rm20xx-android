/*
	Name-Drawing commands for the GUI builder
*/

/* Includes */
#include "wcrpg.h"
#include "wcguiname.h"
#include "wcwindow.h"
#include "wchero.h"
#include "wcskill.h"
#include "wcitem.h"
#include "wcmonster.h"
#include "wcattribute.h"
#include "wcvocab.h"

/* Draw it */
void wcguiname_draw(int source,int id,int x,int y)
{
	text *txt;
	txt = 0;
	/* Hero name */
	if(source == GUI_NAME_HERO)
		txt = new text(wchero_get(id)->get_name());
	if(source == GUI_NAME_TITLE)
		txt = new text(wchero_get(id)->get_title());
	if(source == GUI_NAME_SKILL)
		txt = new text(wcskill_get(id)->get_name());
	if(source == GUI_NAME_SKILL_DESC)
		txt = new text(wcskill_get(id)->get_description());
	if(source == GUI_NAME_ITEM)
		txt = new text(wcitem_get(id)->get_name());
	if(source == GUI_NAME_ITEM_DESC)
		txt = new text(wcitem_get(id)->get_description());
	if(source == GUI_NAME_MONSTER)
		txt = new text(wcmonster_get(id)->get_name());
	if(source == GUI_NAME_ATTRIBUTE)
		txt = new text(wcattribute_get(id)->get_name());
	if(source == GUI_NAME_STRING)
		txt = new text(wcvocab_get(id));
	/* Draw it */
	if(txt)
	{
		txt->move(x,y);
		txt->draw();
		delete txt;
	}
}