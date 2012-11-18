#ifndef WCDRAW_H
#define WCDRAW_H

/* Defines */
#define WCDRAW_CHANNEL_CLAMP(v) if((v) < 0)(v)=0;if((v) > 0xFF)(v)=0xFF

/* Includes */
//#include <d3d9.h>
//#include <d3dx9.h>
#include "wcimage.h"

/* Types */
typedef struct
{
	float x,y,z; /* X, Y, and Z coords */
	float w; /* Homogeneous W coord */
	//D3DCOLOR c; /* Color/Alpha blend value */
	int c; /* Color/Alpha blend value */
	float u,v; /* Texture coords */
}vertex;

typedef struct
{
	unsigned char b,g,r,a; /* Pixel splitter */
}pixel32;

/* Classes */
class shader
{
private:
	char *fname;
	//LPD3DXEFFECT eff;
	//LPD3DXBUFFER msg;
	//D3DXHANDLE handle_texture;
	//D3DXHANDLE handle_tint;
	//D3DXHANDLE handle_size;

	int eff;
	int msg;
	int handle_texture;
	int handle_tint;
	int handle_size;

	int resource_index; /* Associated resource index */
	/*
		Gets handles
	*/
	void get_handles();
public:
	/*
		Create a shader from a text file
	*/
	shader(char *file);
	/*
		Create a shader from internal file
	*/
	shader(int resint,char *name);
	~shader();
	/*
		Starts the shader
	*/
	void start();
	/*
		Ends the shader
	*/
	void end();
	/*
		Updates constants
	*/
	//void update(LPDIRECT3DTEXTURE9 tex,int w,int h);
	void update(int tex,int w,int h);
	/*
		Lose the data attached to shader so device may reset
	*/
	void lose();
	/*
		Reset shader
	*/
	void reset();
	/*
		Set resource index
	*/
	void set_resource_id(int id);
	/*
		Get resource index
	*/
	int get_resource_id();
};
class texture
{
private:
	image *img;
	//LPDIRECT3DTEXTURE9 tex;
	//LPDIRECT3DTEXTURE9 stex; /* Source texture when in software processing mode */
	//LPDIRECT3DTEXTURE9 ftex; /* Flashing texture */

	int tex;
	int stex; /* Source texture when in software processing mode */
	int ftex; /* Flashing texture */

	float alpha; /* 0.0 = opaque ~ 1.0 = transparent */
	float alpha2; /* Same thing but lower half's transparency */
	float rot; /* Rotation angle */
	float tone_red,tone_green,tone_blue,tone_sat; /* Tones */
	float flash; /* Flash strength */
	int flash_red,flash_green,flash_blue; /* Flash color */
	int flash_active; /* Is flash on? */
public:
	/*
		Creates a texture from an image
	*/
	texture(image *im);
	/*
		Creates a texture from a resource integer to bitmap resource
	*/
	texture(int intres,int w,int h);
	/*
		Creates a texture from the file given.
	*/
	texture(char *filename);
	/*
		Creates a texture from the file given, with the option of disabling color keying
	*/
	texture(char *filename,int nocolorkey);
	~texture();
	/*
		Draws the entire texture at x and y on the screen.
	*/
	void draw(int x,int y);
	/*
		Draws a cutout of the texture at x and y on the screen.
		w is the width, h is the height,
		sx and sy is the source location INSIDE the texture to take from
	*/
	void draw(int x,int y,int w,int h,int sx,int sy);
	/*
		Draws a cutout of the textre at x and y on the screen.
		w is the width, h is the height,
		sx and sy is the source location INSIDE the texture to take from,
		dw and dh is the destination rectangle size for min/mag
	*/
	void draw(int x,int y,int w,int h,int sx,int sy,int dw,int dh);
	/*
		Returns the size of this texture
	*/
	int get_width();
	int get_height();
	/*
		Returns the visible size of this texture
	*/
	int get_visible_width();
	int get_visible_height();
	/*
		Returns the resolution magnification with the normal width given
	*/
	int get_res_mag(int n);
	/*
		Returns the attached image
	*/
	image *get_image();
	/*
		Reduces the texture's software component such that the image data
		only resides wheverever DirectX put it
	*/
	void reduce();
	/*
		Sets the new transparency of this texture
	*/
	void set_transparency(float a,float a2);
	/*
		Sets the rotation of this texture
	*/
	void set_rotation(float r);
	/*
		Tones this texture
	*/
	void tone(float r,float g,float b,float s);
	/*
		Enables this texture to flash colors
	*/
	void enable_flash();
	/*
		Sets the flash
	*/
	void set_flash(float a,int r,int g,int b);
};

/*
	Starts the drawing system.
*/
extern int wcdraw_start();

/*
	Stops the drawing system.
*/
extern void wcdraw_end();

/*
	Draws one frame of the system.
*/
extern void wcdraw_frame();

/*
	Creates a texture from an image
*/
//extern LPDIRECT3DTEXTURE9 wcdraw_load_texture(image *i);
extern int wcdraw_load_texture(image *i);

/*
	Draws a texture on the screen 2D style
*/
//extern void wcdraw_texture(LPDIRECT3DTEXTURE9 tex,image *img,int x,int y,int w,int h,int sx,int sy,int dw,int dh,float a,float a2,float r,LPDIRECT3DTEXTURE9 ftex,int fle,int flr,int flg,int flb,float fla);
extern void wcdraw_texture(int tex,image *img,int x,int y,int w,int h,int sx,int sy,int dw,int dh,float a,float a2,float r,int ftex,int fle,int flr,int flg,int flb,float fla);

/*
	Returns the triangle count
*/
extern int wcdraw_triangle_count();

/*
	Resets the triangle count
*/
extern void wcdraw_triangle_count_reset();

/*
	Sets the local resolution mag
*/
extern void wcdraw_set_local_mag(float m);

/*
	Resets the local resolution mag
*/
extern void wcdraw_reset_local_mag();

/*
	Tints the screen, the screen can be drawn to afterwards for objects not to be effected
*/
extern void wcdraw_tint_screen(float r,float g,float b,float s);

/*
	Draws the current screen shader if any
*/
extern void wcdraw_screen_shader();

/*
	Sets the current screen shader (using shader index given by the create function)
*/
extern void wcdraw_set_screen_shader(int ix);

/*
	Get peak vram
*/
extern int wcdraw_get_peak_vram();

/*
	Set shader for all next draw texture commands to use
*/
extern void wcdraw_set_shader(shader *shr);

/*
	Adds a new shader
*/
extern int wcdraw_add_shader(char *file);

/*
	Adds a new shader from resource
*/
extern int wcdraw_add_shader(int intres,char *name);

/*
	Gets shader
*/
extern void wcdraw_set_shader(int ix);

/*
	Gets shader using resource index
*/
extern int wcdraw_get_shader(int resid);

/*
	Loads shader
*/
extern void wcdraw_load_shader(int resid);

/*
	Returns an image of the screen
*/
extern image *wcdraw_image_screen();

#endif