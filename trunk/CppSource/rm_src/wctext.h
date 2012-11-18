#ifndef WCTEXT_H
#define WCTEXT_H

/* Defines */
#define WCTEXT_CHARS_WIDE 32
#define WCTEXT_CHARS_TALL 4

/* Includes */
#include "wcdraw.h"

/* Classes */
class font
{
private:
	texture *tex; /* The texture for the sprite font */
	int cell_width,cell_height; /* Sizes of chars */
	int compression; /* Linear text spacing compression amount */
	/*
		Draws a single letter.
	*/
	void draw_letter(int x,int y,char c);
public:
	/*
		Load a font
	*/
	font(char *filename);
	/*
		Load a font from texture
	*/
	font(texture *tx);
	/*
		Load a font from internal resource
	*/
	font(int intres,int w,int h);
	~font();
	/*
		Draws a monospaced text at x and y.
	*/
	void draw_text(int x,int y,char *str);
	/*
		Draws a text with number where %d is.
	*/
	void draw_text(int x,int y,char *str,int i);
	/*
		Sets compression
	*/
	void set_compression(int amt);
	/*
		Gets attached image
	*/
	image *get_image();
	/*
		Returns the size of a character
	*/
	int get_character_width();
	int get_character_height();
};

#endif