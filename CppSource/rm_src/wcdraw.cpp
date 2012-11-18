/*
	Drawing System (DirectX)
*/

/* Includes */
//#include <windows.h>
#include <memory.h>
//#include <SDL.h>
//#include <SDL_syswm.h>
#include <math.h>
//#include "resource.h"
#include "wcrpg.h"
#include "wcdraw.h"
#include "wcnucleus.h"
#include "wcarray.h"
#include "wcmaterial.h"

#include "../jniUtil.h"

#include "../gl2d.h"

#include <android\log.h>

/* Loads in a shader using resource index */
void wcdraw_load_shader(int resid){}

/* Get shader using resource index */
int wcdraw_get_shader(int resid){return -1;}

/* Add file shader */
int wcdraw_add_shader(char *file){}

/* Add intern shader */
int wcdraw_add_shader(int intres,char *name){}

/* Set index shader */
void wcdraw_set_shader(int ix){}

/* Lose all shaders */
void wcdraw_lose_all_shaders(){}

/* Reset all shaders */
void wcdraw_reset_all_shaders(){}

/* Set shader */
void wcdraw_set_shader(shader *shr){}

/* Get handles */
void shader :: get_handles(){}

/* Create shader */
shader :: shader(char *file){}

/* Create shader from resource */
shader :: shader(int resint,char *name){}

/* Delete shader */
shader :: ~shader(){}

/* Lose */
void shader :: lose(){}

/* Reset */
void shader :: reset(){}

/* Update constants */
//void shader :: update(LPDIRECT3DTEXTURE9 tex,int w,int h)
void shader :: update(int tex,int w,int h){}

/* Start shader */
void shader :: start(){}

/* End shader */
void shader :: end(){}

/* Set id */
void shader :: set_resource_id(int id){}

/* Get id */
int shader :: get_resource_id(){}

/* Grabs the screen into an image */
image *wcdraw_image_screen(){}

/* Draws the current screen shader if any */
void wcdraw_screen_shader(){}

/* Sets screen shader */
void wcdraw_set_screen_shader(int ix){}

/* Tints the screen using a software method, or the HLSL method */
void wcdraw_tint_screen(float r,float g,float b,float s){}

/* Autocalculates channel functions */
void wcdraw_calculate_channel_arithmetic(){}

/* Sets local mag */
void wcdraw_set_local_mag(float m){}

/* Resets local mag */
void wcdraw_reset_local_mag(){}

/* Starts counting triangles */
void wcdraw_triangle_count_reset()
{
	//wcdraw_tri_count = 0;
}

/* Returns the triangle count */
int wcdraw_triangle_count()
{
	//return wcdraw_tri_count;
}

/* Sets the presentation params */
void wcdraw_normal_presentation()
{
}

/* Sets the properties for default display mode */
void wcdraw_normal_display()
{	
}

/* Sets up the framebuffer */
void wcdraw_set_framebuffer()
{	
}

/* Start the graphics system */
int wcdraw_start()
{	
	return 1;
}

/* Stops the graphics system */
void wcdraw_end()
{
}

/* Renders the current game state */
void wcdraw_frame()
{	wcnucleus_draw();
}

/* Loads a texture */
//LPDIRECT3DTEXTURE9 wcdraw_load_texture(image *i)
int wcdraw_load_texture(image *i)
{
	return -1;	
}

/* Loads a texture from resource integer, pass pointers for output width and height */
//LPDIRECT3DTEXTURE9 wcdraw_load_texture(int intres)
int wcdraw_load_texture(int intres)
{
	return -1;	
}

/* Make texture */
texture :: texture(char *filename)
{
	///* Make */
	img = new image(filename);
	tex=img->texID;
	alpha = 0.0f;
	alpha2 = 0.0f;
	rot = 0.0f;
	stex = 0;
	tone_red = 1.0f;
	tone_green = 1.0f;
	tone_blue = 1.0f;
	tone_sat = 1.0f;
	ftex = 0;
	flash_active = 0;
	flash_red = 0;
	flash_green = 0;
	flash_blue = 0;
	flash = 0.0f;
}

/* Make texture */
texture :: texture(image *im)
{
	///* Make */
	img = im;
	//img = new image(filename);
	//tex=img->texID;
	tex=-1;
	alpha = 0.0f;
	alpha2 = 0.0f;
	rot = 0.0f;
	stex = 0;
	tone_red = 1.0f;
	tone_green = 1.0f;
	tone_blue = 1.0f;
	tone_sat = 1.0f;
	ftex = 0;
	flash_active = 0;
	flash_red = 0;
	flash_green = 0;
	flash_blue = 0;
	flash = 0.0f;
}

/* Make texture */
texture :: texture(int intres,int w,int h)
{
	///* Make */
	if(intres==116)
		img = new image("sysfont.png");
	tex=img->texID;

	alpha = 0.0f;
	alpha2 = 0.0f;
	rot = 0.0f;
	stex = 0;
	tone_red = 1.0f;
	tone_green = 1.0f;
	tone_blue = 1.0f;
	tone_sat = 1.0f;
	ftex = 0;
	flash_active = 0;
	flash_red = 0;
	flash_green = 0;
	flash_blue = 0;
	flash = 0.0f;
}

/* Make dekeyed texture */
texture :: texture(char *filename,int nocolorkey)
{
	///* Make */
	img = new image(filename);
	
	//if(nocolorkey)
	//	img->de_color_key();
	tex = wcdraw_load_texture(img);
	//glLoadTileSet("CharSet/hero2.png",spr,24,32,256,256);
	tex=img->texID;
	alpha = 0.0f;
	alpha2 = 0.0f;
	rot = 0.0f;
	stex = 0;
	tone_red = 1.0f;
	tone_green = 1.0f;
	tone_blue = 1.0f;
	tone_sat = 1.0f;
	ftex = 0;
	flash_active = 0;
	flash_red = 0;
	flash_green = 0;
	flash_blue = 0;
	flash = 0.0f;
}

/* Tone texture */
void texture :: tone(float r,float g,float b,float s){}

/* Free texture */
texture :: ~texture()
{
	/* Delete */
	/*tex->Release();
	tex = 0;
	if(stex)
		stex->Release();
	if(ftex)
		ftex->Release();
	ftex = 0;
	stex = 0;
	wcdraw_total_vram -= img->get_width()*img->get_height()*4;
	delete img;*/
}

/* Draw texture full */
//void wcdraw_texture(LPDIRECT3DTEXTURE9 tex,image *img,int x,int y,int w,int h,int sx,int sy,int dw,int dh,float a,float a2,float r,LPDIRECT3DTEXTURE9 ftex,int fle,int flr,int flg,int flb,float fla)
void wcdraw_texture(int tex,image *img,int x,int y,int w,int h,int sx,int sy,int dw,int dh,float a,float a2,float r,int ftex,int fle,int flr,int flg,int flb,float fla){}

/* Draw texture */
void texture :: draw(int x,int y){
	int w=img->get_width();
	int h=img->get_height();
	float u=w/w;
	float v=h/h;

	spriteBatchDraw_(img->texID,x,y,0,0,u,v,w,h,0);
}

/* Draw texture cutout */
void texture :: draw(int x,int y,int w,int h,int sx,int sy)
{	
	//__android_log_print(ANDROID_LOG_INFO, "adMOB", "x %d y %d w %d h %d sx %d sy %d",x,y,w,h,sx,sy);
	int w_=img->get_width();
	int h_=img->get_height();
	float u=(float)w/(float)w_;
	float v=(float)h/(float)h_;
	float u_off=(float)sx/(float)w_;
	float v_off=(float)sy/(float)h_;

	//__android_log_print(ANDROID_LOG_INFO, "adMOB", "u_off %f v_off %f u %f v %f w %d h %d",u_off,v_off,u,v,w,h);

	spriteBatchDraw_(img->texID,x,y,u_off,v_off,u,v,w,h,0);
}

/* Draw texture with destination rect */
void texture :: draw(int x,int y,int w,int h,int sx,int sy,int dw,int dh)
{
	int w_=img->get_width();
	int h_=img->get_height();
	float u=(float)w/(float)w_;
	float v=(float)h/(float)h_;
	float u_off=(float)sx/(float)w_;
	float v_off=(float)sy/(float)h_;

	spriteBatchDraw_(img->texID,x,y,u_off,v_off,u,v,w,h,0,false,0,dw/w,dh/h);
}

/* Return width */
int texture :: get_width()
{
	return img->get_width();
}

/* Return height */
int texture :: get_height()
{
	return img->get_height();
}

/* Return visible width */
int texture :: get_visible_width()
{
	return img->get_visible_width();
}

/* Return visible height */
int texture :: get_visible_height()
{
	return img->get_visible_height();
}

/* Returns resolution magnification */
int texture :: get_res_mag(int n)
{
	return img->get_width()/n;
}

/* Returns attached image */
image *texture :: get_image()
{
	return img;
}

/* Reduce */
void texture :: reduce()
{
	//img->reduce();
}

/* Set trans */
void texture :: set_transparency(float a,float a2)
{
	alpha = a;
	alpha2 = a2;
}

/* Set rot */
void texture :: set_rotation(float r)
{
	rot = r;
}

/* Enable the flash */
void texture :: enable_flash(){}

/* Set the flash */
void texture :: set_flash(float a,int r,int g,int b){}

/* Get peak VRAM */
int wcdraw_get_peak_vram(){}