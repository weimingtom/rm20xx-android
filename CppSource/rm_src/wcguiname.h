#ifndef WCGUINAME_H
#define WCGUINAME_H

/* Defines */
#define GUI_NAME_HERO 0
#define GUI_NAME_TITLE 1
#define GUI_NAME_SKILL 2
#define GUI_NAME_SKILL_DESC 3
#define GUI_NAME_ITEM 4
#define GUI_NAME_ITEM_DESC 5
#define GUI_NAME_MONSTER 6
#define GUI_NAME_ATTRIBUTE 7
#define GUI_NAME_CONDITION 8
#define GUI_NAME_STRING 9

/*
	Draws the name of some object in the database
*/
extern void wcguiname_draw(int source,int id,int x,int y);

#endif