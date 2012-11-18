#ifndef WCWINDOW_H
#define WCWINDOW_H

/* Defines */
#define COMPARE_BETTER 0
#define COMPARE_DIFFERENT 1
#define COMPARE_WORSE 2
#define COMPARE_SAME 3

#define SHADOW_SMALL 1
#define SHADOW_LARGE 0

#define FONT_COLOR_COUNT 20

#define HAND_LEFT 0
#define HAND_DOWN 1

#define BAR_HP 0
#define BAR_MP 1
#define BAR_T 2

#define ARROW_UP 0
#define ARROW_DOWN 1

#define TEXT_ALL -1

/* Includes */
#include <stdio.h>
#include "wcdraw.h"
#include "wcarray.h"

/* Types */
typedef struct
{
	char *name;
	texture *tex;
}faceset;

/* Classes */
class textstring
{
private:	
	char *current; /* Current string state */
	int pos_x,pos_y; /* Position */
	int col; /* Color */
public:
	char *source; /* String source */
	/*
		Create a new text string
	*/
	textstring(char *src);
	~textstring();
	/*
		Moves this text string
	*/
	void move(int x,int y);
	/*
		Sets p characters of this text to be visible,
		this allows the animation of text scrolling you see in games.
		Use TEXT_ALL to just show all letters (default)
	*/
	void advance(int p);
	/*
		Changes text color
	*/
	void set_color(int c);
	/*
		Draws this text string
	*/
	void draw();
	/*
		Returns the pixel size of this string
	*/
	int width();
	int height();
	/*
		Returns the number of visible characters (spaces are visible too ... o.o)
	*/
	int length();
	/*
		Returns the position of this string
	*/
	int get_x();
	int get_y();
};

class icon
{
private:
	int subimage; /* Icon sub image index */
	int pos_x,pos_y; /* Position */
	int vis; /* visible? */
public:
	/*
		Creates a new icon from sub image index
	*/
	icon(int ix);
	/*
		Moves this icon
	*/
	void move(int x,int y);
	/*
		Draws this icon
	*/
	void draw();
	/*
		Returns the position of this string
	*/
	int get_x();
	int get_y();
	/*
		Sets visible or invisible (true is visible)
	*/
	void set_visible(int en);
};

class text
{
private:
	arraylist txs; /* Text strings */
	arraylist ics; /* Icons */
	arraylist ccs; /* Control strings */
	int auto_close_pos; /* When to auto close, if ever? */
public:
	/*
		Create a new text
	*/
	text(char *src);
	~text();
	/*
		Moves this text
	*/
	void move(int x,int y);
	/*
		Draws this text
	*/
	void draw();
	/*
		Advances the text so you can animate it scrolling in
		If fill is non-zero, the dialog progress will show texts all at once and iterate
		on each section of text instead of scrolling indiviual letters.
	*/
	void advance(int p,int fill);
	/*
		Returns the total character length of this text
	*/
	int length();
	/*
		Sets all the text to one color, overrides indivual coloring codes
	*/
	void set_color(int cl);
	/*
		Total tempo delay time that should have passed at p
	*/
	float tempo_time(int p);
	/*
		Current speed time that should have passed
	*/
	float current_speed(int p);
	/*
		Is the dialog being forced closed here?
	*/
	int auto_close(int p);
};

/*
	Draw a window
*/
extern void wcwindow_draw(int x,int y,int w,int h);

/*
	Draw a cursor
*/
extern void wcwindow_draw_cursor(int x,int y,int w,int h,int f);

/*
	Draw one of those item equipment compare things
*/
extern void wcwindow_draw_compare(int x,int y,int c);

/*
	Handles system graphic stuff (like animating them)
*/
extern void wcwindow_animate();

/*
	Draws a number
*/
extern void wcwindow_draw_number(int x,int y,int n);

/*
	Draws the airship shadow
*/
extern void wcwindow_draw_shadow(int x,int y,int s);

/*
	Draws text in the given color
*/
extern void wcwindow_draw_text(int x,int y,int c,char *str);

/*
	Regenerates all fonts
*/
extern void wcwindow_generate_all_fonts();

/*
	Draws the hands
*/
extern void wcwindow_draw_hand(int x,int y,int d);

/*
	Draws a bar
	p is a percent from 0 to 100
*/
extern void wcwindow_draw_bar(int x,int y,int p,int t);

/*
	Draws a bar
	p is now the actual pixel value of the bar being filled,
	and w is the pixel width of the actual bar area inside
*/
extern void wcwindow_draw_bar(int x,int y,int p,int t,int w);

/*
	Draws battle numbers
*/
extern void wcwindow_draw_battle_number(int x,int y,int n);

/*
	Draws arrows
*/
extern void wcwindow_draw_arrow(int x,int y,int d);

/*
	Draws exfont icon
*/
extern void wcwindow_draw_icon(int x,int y,int i);

/*
	Returns the size of a character of the current font
*/
extern int wcwindow_font_character_width();
extern int wcwindow_font_character_height();

/*
	Prints the result of a text division to the given file, for debug purposes
*/
extern void wcwindow_debug_text(FILE *f,char *src);

/*
	Sets the text speed
*/
extern void wcwindow_set_text_speed(int spd);

/*
	Gets the text speed
*/
extern float wcwindow_get_text_speed();

/*
	Draws a face
*/
extern void wcwindow_draw_face(texture *tex,int ix,int x,int y);

/*
	Returns a calculation of text speed
*/
extern float wcwindow_calculate_text_speed(int spd);

/*
	Clears all pooled facesets
*/
extern void wcwindow_clear_faceset();

/*
	Pool a faceset
*/
extern texture *wcwindow_pool_faceset(char *str);

/*
	Draws the timer colon
*/
extern void wcwindow_draw_colon(int x,int y);

#endif