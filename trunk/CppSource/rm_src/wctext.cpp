/*
	Text Drawing
*/

/* Includes */
//#include <windows.h>
#include <stdio.h>
#include "wctext.h"

/* Create */
font :: font(char *filename)
{
	/* Init */
	compression = 0;
	/* Create */
	tex = new texture(filename);
	/* Measure cell size */
	//cell_width = tex->get_visible_width()/WCTEXT_CHARS_WIDE;
	//cell_height = tex->get_visible_height()/WCTEXT_CHARS_TALL;
	cell_width = 8;
	cell_height = 16;
}

/* Create */
font :: font(texture *tx)
{
	/* Init */
	compression = 0;
	/* Create */
	tex = tx;
	/* Measure cell size */
	//cell_width = tex->get_visible_width()/WCTEXT_CHARS_WIDE;
	//cell_height = tex->get_visible_height()/WCTEXT_CHARS_TALL;
	cell_width = 8;
	cell_height = 16;
}

/* Create */
font :: font(int intres,int w,int h)
{
	/* Init */
	compression = 0;
	/* Create */
	tex = new texture(intres,w,h);
	/* Measure cell size */
	cell_width = tex->get_visible_width()/WCTEXT_CHARS_WIDE;
	cell_height = tex->get_visible_height()/WCTEXT_CHARS_TALL;
}

/* Free */
font :: ~font()
{
	/* Release */
	delete tex;
}

/* Draw a letter */
void font :: draw_letter(int x,int y,char c)
{
	int cx,cy;
	/* Find cell location */
	cx = (c%WCTEXT_CHARS_WIDE)*cell_width;
	cy = (c/WCTEXT_CHARS_WIDE)*cell_height;
	/* Draw it */
	tex->draw(x,y,cell_width,cell_height,cx,cy);
}

/* Draw some text */
void font :: draw_text(int x,int y,char *str)
{
	int curx,cury;
	/* Position cursor */
	curx = x;
	cury = y;
	/* Draw it */
	for(;;)
	{
		/* Return mark */
		if(*str == '\n')
		{
			/* Hit return */
			curx = x;
			cury += cell_height;
			str++;
		}
		else
		{
			/* Something to draw? */
			if(*str)
			{
				/* Draw the letter */
				draw_letter(curx,cury,*str);
				/* Advance */
				curx += cell_width-compression;
				/* Next letter */
				str++;
			}
			else
			{
				/* End of string */
				return;
			}
		}
	}
}

/* Draw text with number */
void font :: draw_text(int x,int y,char *str,int i)
{
	return;
	char buffer[260];
	sprintf(buffer,str,i);
	draw_text(x,y,buffer);
}

/* Set compression */
void font :: set_compression(int amt)
{
	compression = amt;
}

/* Get attached image */
image *font :: get_image()
{
	return tex->get_image();
}

/* Get character width */
int font :: get_character_width()
{
	return cell_width-compression;
}

/* Get character height */
int font :: get_character_height()
{
	return cell_height;
}