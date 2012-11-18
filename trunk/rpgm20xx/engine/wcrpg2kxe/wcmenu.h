#ifndef WCMENU_H
#define WCMENU_H

/* Defines */
#define WCMENU_MAIN_WIDTH 96
#define WCMENU_DEFAULT_NUMBER_SPACING 24

/* Requirements */
#include "wcarray.h"

/* Classes */
class menu
{
private:
	arraylist ls; /* Entries */
	arraylist sls; /* Source Entries */
	arraylist dls; /* Enable/Disable */
	arraylist nls; /* Numbers */
	int numspace; /* Space from the left side of the number to the right side of menu */
	int width; /* Width of the menu in pixels */
	int options; /* Number of options */
	int selected; /* Selected option */
	int accepted; /* Has selected? */
	int view; /* Current view position */
	int active; /* Activated? */
	int autofire; /* Anti-autofire? */
	int closing; /* Is closing? */
	float introdist; /* Intro animation distance */
	/*
		Move cursor up
	*/
	void cursor_up();
	/*
		Move cursor down
	*/
	void cursor_down();
public:
	/*
		Creates a new menu with widths and number of options
	*/
	menu(int w,int os);
	~menu();
	/*
		Adds an entry to this menu
	*/
	void add(char *str);
	/*
		Adds a numbered entry to this menu
	*/
	void add(char *str,int amt);
	/*
		Draws this menu
	*/
	void draw(int x,int y);
	/*
		Handles this menu
	*/
	void handle();
	/*
		Activates this menu and it will begin handling user input
	*/
	void activate();
	/*
		Returns whether or not this menu is active
	*/
	int is_active();
	/*
		Closes the menu
	*/
	void close();
	/*
		Enables/Disables an entry so that it can't be selected
	*/
	void set_disable(int id,int d);
	/*
		Returns the currently selected item or -1 if not decided yet
	*/
	int get_selection();
	/*
		Returns the size of this menu in visible selections
	*/
	int size();
	/*
		Forces a selection as though the user did it
	*/
	void set_selection(int s);
	/*
		Gets the currently highlighted choice even though the menu may still be active
	*/
	int peek_selection();
	/*
		Forces a selection, but doesn't hit OK
	*/
	void move_selection(int s);
};

/*
	Generates title menu
*/
extern menu *wcmenu_generate_title();

/*
	Generates main menu
*/
extern menu *wcmenu_generate_main();

#endif