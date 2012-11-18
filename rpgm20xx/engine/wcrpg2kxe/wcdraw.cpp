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

/* Globals */
//LPDIRECT3D9 d3d; /* Pointer to direct 3d interface */
//LPDIRECT3DDEVICE9 d3ddev; /* Direct 3D device */
//HWND hwnd; /* Pointer to main window */
//SDL_SysWMinfo wmi; /* Window info */
//D3DPRESENT_PARAMETERS d3dpp; /* Settings for the device */
//DWORD wcrpgvf = D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1; /* Custom vertex format */
//LPDIRECT3DVERTEXBUFFER9 d3dvb; /* Vertex buffer */
//LPDIRECT3DSURFACE9 d3ds; /* Surface */
//int wcdraw_tri_count = 0; /* Triangle counter */
//float wcdraw_local_mag = 1; /* Local magnification settings */
//unsigned char wcdraw_channel_add[256][256];
//unsigned char wcdraw_channel_sub[256][256];
//int wcdraw_reset_mode = 0; /* Do we have to go through the process of resetting? */
//int wcdraw_reset_count = 0; /* Times video device has been reset */
//int render_width = 512;
//int render_height = 512;
//LPDIRECT3DTEXTURE9 render_texture; /* The screen render texture */
//image *render_image; /* The screen render image object */
//int wcdraw_total_vram = 0; /* Total VRAM usage (may not all be hardware VRAM) */
//int wcdraw_peak_vram = 0; /* Maximum VRAM usage ever */
//shader *wcdraw_tint_shader = 0; /* Tint screen shader */
//shader *wcdraw_current_shader = 0; /* Current shader effect */
//int wcdraw_enable_shader = 1; /* Enable shaders */
//float wcdraw_fx_tint_color[4] = {1.0f,1.0f,1.0f,1.0f}; /* Local copy of shader param */
//D3DCAPS9 wcdraw_caps; /* Capabillity */
//arraylist wcdraw_shader_list; /* All shaders */
//int wcdraw_current_screen_shader = 0; /* Current screen effect */
//int wcdraw_blossom_shader = 0; /* Blossom shader */

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
	///* Create a device */
	//memset(&d3dpp,0,sizeof(d3dpp));
	//d3dpp.BackBufferWidth = WCRPG_WIDTH*wcrpg_get_mag();
	//d3dpp.BackBufferHeight = WCRPG_HEIGHT*wcrpg_get_mag();
	//d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	//d3dpp.BackBufferCount = 1;
	//d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
	//d3dpp.hDeviceWindow = hwnd;
	//d3dpp.Windowed = 1;
}

/* Sets the properties for default display mode */
void wcdraw_normal_display()
{
	///* Set render state */
	//d3ddev->SetRenderState(D3DRS_LIGHTING,0);
	//d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE,1);
	//d3ddev->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	//d3ddev->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
	//d3ddev->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE);
	///* Old school filters on */
	//d3ddev->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_NONE);
	//d3ddev->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_NONE);
}

/* Sets up the framebuffer */
void wcdraw_set_framebuffer()
{
	///* Recalc size */
	//render_width = 512*wcrpg_get_mag();
	//render_height = 512*wcrpg_get_mag();
	///* Create a framebuffer */
	//if(d3ddev->CreateOffscreenPlainSurface(WCRPG_WIDTH*wcrpg_get_mag(),WCRPG_HEIGHT*wcrpg_get_mag(),D3DFMT_X8R8G8B8,D3DPOOL_SYSTEMMEM,&d3ds,0) != D3D_OK)
	//{
	//	wcrpg_error("Could not create effect buffer");
	//	return;
	//}
	///* Create a texture */
	//if(d3ddev->CreateTexture(render_width,render_height,1,0,D3DFMT_A8R8G8B8,D3DPOOL_MANAGED,&render_texture,0) != D3D_OK)
	//{
	//	wcrpg_error("Could not create render texture");
	//	return;
	//}
	//render_image = new image(render_width,render_height);
	//render_image->reduce();
}

/* Start the graphics system */
int wcdraw_start()
{
	//HRESULT r;
	///* Get the window HWND */
	//if(!SDL_GetWMInfo(&wmi))
	//{
	//	wcrpg_error("Could not get window info (%s)",SDL_GetError());
	//	return 0;
	//}
	//hwnd = (HWND)wmi.window;
	///* Initialize D3D */
	//d3d = Direct3DCreate9(D3D_SDK_VERSION);
	//if(!d3d)
	//{
	//	wcrpg_error("Could not start DirectX, check your DirectX for updates");
	//	return 0;
	//}
	///* Normal present */
	//wcdraw_normal_presentation();
	//r = d3d->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hwnd,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&d3dpp,&d3ddev);
	//if(r != D3D_OK)
	//{
	//	switch(r)
	//	{
	//	case D3DERR_DEVICELOST:
	//		wcrpg_error("Direct3D device lost");
	//		break;
	//	case D3DERR_INVALIDCALL:
	//		wcrpg_error("Invalid Direct3D device creation call");
	//		break;
	//	case D3DERR_NOTAVAILABLE:
	//		wcrpg_error("Direct3D device requested is not availiable");
	//		break;
	//	case D3DERR_OUTOFVIDEOMEMORY:
	//		wcrpg_error("There isn't enough video memory to create Direct3D device");
	//		break;
	//	}
	//	return 0;
	//}
	///* Create a vertex buffer */
	//d3ddev->SetVertexShader(NULL);
	//d3ddev->SetFVF(wcrpgvf);
	//if(d3ddev->CreateVertexBuffer(sizeof(vertex)*4,NULL,wcrpgvf,D3DPOOL_MANAGED,&d3dvb,NULL) != D3D_OK)
	//{
	//	wcrpg_error("Could not create vertex buffer.");
	//	return 0;
	//}
	//if(d3ddev->SetStreamSource(0,d3dvb,0,sizeof(vertex)) != D3D_OK)
	//{
	//	wcrpg_error("Could not set vertex stream source.");
	//	return 0;
	//}
	///* Normal */
	//wcdraw_normal_display();
	///* Calculate color channel arithmetic */
	//wcdraw_calculate_channel_arithmetic();
	//wcdraw_set_framebuffer();
	///* Figure out caps */
	//memset(&wcdraw_caps,0,sizeof(wcdraw_caps));
	//d3ddev->GetDeviceCaps(&wcdraw_caps);
	///* Report shader model */
	//wclog_write(wcrpg_get_log(),"System uses pixel shader model ");
	//wclog_write(wcrpg_get_log(),(wcdraw_caps.PixelShaderVersion&0xFFFF)>>8);
	//wclog_enter(wcrpg_get_log());
	//if((wcdraw_caps.PixelShaderVersion&0xFFFF)>>8 < 2)
	//	wcdraw_enable_shader = 0;
	//wclog_write(wcrpg_get_log(),"Shader enable switch set to ");
	//wclog_write(wcrpg_get_log(),wcdraw_enable_shader);
	//wclog_enter(wcrpg_get_log());
	///* Load shaders */
	//wcdraw_shader_list = wcarray_new();
	//if(wcdraw_enable_shader)
	//	wcdraw_tint_shader = new shader(fx_tint,"fx_tint");
	return 1;
}

/* Stops the graphics system */
void wcdraw_end()
{
	//int i;
	///* Release shaders */
	//for(i = 0;i < wcarray_size(wcdraw_shader_list);i++)
	//	delete (shader*)wcarray_get(wcdraw_shader_list,i);
	//wcarray_delete(wcdraw_shader_list);
	///* Release tint shader */
	//delete wcdraw_tint_shader;
	///* Release vertex data */
	//d3ddev->SetStreamSource(0,NULL,0,0);
	//d3dvb->Release();
	//d3ds->Release();
	//render_texture->Release();
	//delete render_image;
	///* Release D3D */
	//d3ddev->Release();
	//d3d->Release();
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
	//LPDIRECT3DTEXTURE9 tex;
	//void *v;
	///* Generate */
	//v = i->generate_bmp();
	///* Load */
	//if(FAILED(D3DXCreateTextureFromFileInMemory(d3ddev,v,i->size_of_bmp(),&tex)))
	//{
	//	wcrpg_error("Could not create texture.");
	//	return 0;
	//}
	///* Done */
	//free(v);
	//wcdraw_total_vram += i->get_width()*i->get_height()*4;
	//if(wcdraw_total_vram > wcdraw_peak_vram)
	//	wcdraw_peak_vram = wcdraw_total_vram;
	//return tex;
}

/* Loads a texture from resource integer, pass pointers for output width and height */
//LPDIRECT3DTEXTURE9 wcdraw_load_texture(int intres)
int wcdraw_load_texture(int intres)
{
	return -1;
	//LPDIRECT3DTEXTURE9 tex;
	//HGLOBAL hbitmap;
	//void *vres;
	//int bsize;
	//BITMAPFILEHEADER *bfh;
	///* Find the file */
	//hbitmap = LoadResource(0,FindResource(0,MAKEINTRESOURCE(intres),RT_BITMAP));
	//if(!hbitmap)
	//{
	//	wcrpg_error("Could not find resource ID ",intres);
	//	return 0;
	//}
	///* Map out the bitmap file header and get the size */
	//vres = LockResource(hbitmap);
	//bfh = (BITMAPFILEHEADER*)vres;
	//if(!vres)
	//{
	//	wcrpg_error("Could not lock internal resource with ID ",intres);
	//	return 0;
	//}
	//bsize = bfh->bfSize;
	///* Load */
	//if(FAILED(D3DXCreateTextureFromFileInMemory(d3ddev,vres,bsize,&tex)))
	//{
	//	wcrpg_error("Could not create internal texture with ID ",intres);
	//	return 0;
	//}
	///* Done */
	//UnlockResource(hbitmap);
	//FreeResource(hbitmap);
	///* Return result */
	//return tex;
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
		img = new image("sysfont.bmp");
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
void texture :: tone(float r,float g,float b,float s)
{
	//D3DLOCKED_RECT d3dr_src,d3dr_dst;
	//unsigned char *dst,*src;
	//float sinv;
	//int x,y,dstw,srcw,dsta,srca;
	//int addr,addg,addb;
	//int subr,subg,subb;
	//int tempr,tempg,tempb;
	//int enr,eng,enb;
	//int denr,deng,denb;
	///* Check and see if there is any change */
	//if(tone_red == r && tone_green == g && tone_blue == b && tone_sat == s)
	//	return; /* Nothing needs to be done here */
	///* Initialize color changers */
	//addr = 0;
	//addg = 0;
	//addb = 0;
	//subr = 0;
	//subg = 0;
	//subb = 0;
	///* Find color changes */
	//if(r > 1.0f)
	//	addr = (int)((r-1.0f)*0xFF);
	//if(g > 1.0f)
	//	addg = (int)((g-1.0f)*0xFF);
	//if(b > 1.0f)
	//	addb = (int)((b-1.0f)*0xFF);
	//if(r < 1.0f)
	//	subr = (int)((1.0f-r)*0xFF);
	//if(g < 1.0f)
	//	subg = (int)((1.0f-g)*0xFF);
	//if(b < 1.0f)
	//	subb = (int)((1.0f-b)*0xFF);
	///* Memorize */
	//tone_red = r;
	//tone_green = g;
	//tone_blue = b;
	//tone_sat = s;
	///* Check and see if this has been toned before */
	//if(!stex)
	//{
	//	/* Swap pointers */
	//	stex = tex;
	//	tex = 0;
	//	/* Create a new blank texture */
	//	if(FAILED(d3ddev->CreateTexture(img->get_width(),img->get_height(),1,0,D3DFMT_A8R8G8B8,D3DPOOL_MANAGED,&tex,NULL)))
	//	{
	//		wcrpg_error("Could not create tone texture.");
	//		return;
	//	}
	//}
	///* Lock both */
	//if(FAILED(stex->LockRect(0,&d3dr_src,NULL,D3DLOCK_READONLY)))
	//{
	//	wcrpg_error("Could not lock source tone texture.");
	//	return;
	//}
	//if(FAILED(tex->LockRect(0,&d3dr_dst,NULL,0)))
	//{
	//	wcrpg_error("Could not lock destination tone texture.");
	//	return;
	//}
	///* Process the destination based on the source, using only the visible area to save processor power */
	//dst = (unsigned char*)d3dr_dst.pBits;
	//src = (unsigned char*)d3dr_src.pBits;
	//dstw = d3dr_dst.Pitch;
	//srcw = d3dr_src.Pitch;
	//sinv = 1.0f-s;
	//for(y = 0;y < img->get_visible_height();y++)
	//{
	//	/* Calculate y ranks */
	//	dsta = dstw*y;
	//	srca = srcw*y;
	//	/* Process the line */
	//	for(x = 0;x < img->get_visible_width();x++)
	//	{
	//		/* Get source color */
	//		tempr = src[(x<<2)+srca+2];
	//		tempg = src[(x<<2)+srca+1];
	//		tempb = src[(x<<2)+srca];
	//		/* Sat/Desat */
	//		if(s != 1.0f)
	//		{
	//			/* Find the entropic color */
	//			enr = (tempr+tempg+tempb)/3;
	//			eng = enr;
	//			enb = enr;
	//			/* Calculate entropic distance */
	//			denr = enr-tempr;
	//			deng = eng-tempg;
	//			denb = enb-tempb;
	//			/* Scale entropic distance */
	//			tempr = tempr+(int)(denr*sinv);
	//			WCDRAW_CHANNEL_CLAMP(tempr);
	//			tempg = tempg+(int)(deng*sinv);
	//			WCDRAW_CHANNEL_CLAMP(tempg);
	//			tempb = tempb+(int)(denb*sinv);
	//			WCDRAW_CHANNEL_CLAMP(tempb);
	//		}
	//		/* Effect color */
	//		dst[(x<<2)+dsta] = wcdraw_channel_add[wcdraw_channel_sub[tempb][subb]][addb];
	//		dst[(x<<2)+dsta+1] = wcdraw_channel_add[wcdraw_channel_sub[tempg][subg]][addg];
	//		dst[(x<<2)+dsta+2] = wcdraw_channel_add[wcdraw_channel_sub[tempr][subr]][addr];
	//		dst[(x<<2)+dsta+3] = src[(x<<2)+srca+3];
	//	}
	//}
	///* Unlock both */
	//if(FAILED(tex->UnlockRect(0)))
	//{
	//	wcrpg_error("Could not unlock destination tone texture.");
	//	return;
	//}
	//if(FAILED(stex->UnlockRect(0)))
	//{
	//	wcrpg_error("Could not unlock source tone texture.");
	//	return;
	//}
}

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
void wcdraw_texture(int tex,image *img,int x,int y,int w,int h,int sx,int sy,int dw,int dh,float a,float a2,float r,int ftex,int fle,int flr,int flg,int flb,float fla)
{
	//vertex *verts;
	//int i,xm,ym;
	//int red,green,blue;
	//float u1,v1,u2,v2;
	//float fw,fh;
	//float nrx,nry;
	//float uw,vh;
	//unsigned char alphabyte,alphabyte2;
	///* Boot shader */
	//if(wcdraw_current_shader)
	//{
	//	/* Update constants */
	//	wcdraw_current_shader->update(tex,img->get_width(),img->get_height());
	//	/* Set shader */
	//	wcdraw_current_shader->start();
	//}
	///* Default color */
	//red = 0xFF;
	//green = 0xFF;
	//blue = 0xFF;
	///* Color is defined */
	//if(fle && !ftex)
	//{
	//	red = flr;
	//	green = flg;
	//	blue = flb;
	//}
	///* Find alpha */
	//alphabyte = 0xFF;
	//if(a != 0.0f)
	//	alphabyte = (unsigned char)((1.0f-a)*0xFF);
	//alphabyte2 = 0xFF;
	//if(a2 != 0.0f)
	//	alphabyte2 = (unsigned char)((1.0f-a2)*0xFF);
	///* Set coords to match mags */
	//xm = x;
	//ym = y;
	//xm *= wcrpg_get_mag();
	//ym *= wcrpg_get_mag();
	///* Figure out texture coordinates */
	//uw = (float)(w)/(float)(img->get_width());
	//vh = (float)(h)/(float)(img->get_height());
	//u1 = (float)(sx)/(float)(img->get_width());
	//v1 = (float)(sy)/(float)(img->get_height());
	//u2 = u1+uw;
	//v2 = v1+vh;
	///* Figure out dimensions */
	//fw = (float)dw*wcrpg_get_mag()*wcdraw_local_mag*0.5f;
	//fh = (float)dh*wcrpg_get_mag()*wcdraw_local_mag*0.5f;
	///* Lock the buffer */
	//d3dvb->Lock(0,0,(void**)&verts,NULL);
	///* Modify the verticies in the sprite quad */
	///* Top left */
	//verts[0].c = D3DCOLOR_ARGB(alphabyte,red,green,blue);
	///*verts[0].x = (float)x-0.5f;
	//verts[0].y = (float)y-0.5f;*/
	//verts[0].x = -fw;
	//verts[0].y = -fh;
	//verts[0].z = 0.0f;
	//verts[0].w = 1.0f;
	//verts[0].u = u1;
	//verts[0].v = v1;
	///* Top right */
	//verts[1].c = D3DCOLOR_ARGB(alphabyte,red,green,blue);
	///*verts[1].x = (float)(x+dw*wcrpg_get_mag()*wcdraw_local_mag)-0.5f;
	//verts[1].y = (float)y-0.5f;*/
	//verts[1].x = fw;
	//verts[1].y = -fh;
	//verts[1].z = 0.0f;
	//verts[1].w = 1.0f;
	//verts[1].u = u2;
	//verts[1].v = v1;
	///* Bottom right */
	//verts[2].c = D3DCOLOR_ARGB(alphabyte2,red,green,blue);
	///*verts[2].x = (float)(x+dw*wcrpg_get_mag()*wcdraw_local_mag)-0.5f;
	//verts[2].y = (float)(y+dh*wcrpg_get_mag()*wcdraw_local_mag)-0.5f;*/
	//verts[2].x = fw;
	//verts[2].y = fh;
	//verts[2].z = 0.0f;
	//verts[2].w = 1.0f;
	//verts[2].u = u2;
	//verts[2].v = v2;
	///* Bottom left */
	//verts[3].c = D3DCOLOR_ARGB(alphabyte2,red,green,blue);
	///*verts[3].x = (float)x-0.5f;
	//verts[3].y = (float)(y+dh*wcrpg_get_mag()*wcdraw_local_mag)-0.5f;*/
	//verts[3].x = -fw;
	//verts[3].y = fh;
	//verts[3].z = 0.0f;
	//verts[3].w = 1.0f;
	//verts[3].u = u1;
	//verts[3].v = v2;
	///* Rotate */
	//if(r != 0.0f)
	//{
	//	for(i = 0;i < 4;i++)
	//	{
	//		nrx = verts[i].x*cos(r)-verts[i].y*sin(r);
	//		nry = verts[i].x*sin(r)+verts[i].y*cos(r);
	//		verts[i].x = nrx;
	//		verts[i].y = nry;
	//	}
	//}
	///* Translate */
	//for(i = 0;i < 4;i++)
	//{
	//	verts[i].x += (float)xm-0.5f+fw;
	//	verts[i].y += (float)ym-0.5f+fh;
	//}
	///* Unlock buffer */
	//d3dvb->Unlock();
	///* Set texture */
	//d3ddev->SetTexture(0,tex);
	//d3ddev->DrawPrimitive(D3DPT_TRIANGLEFAN,0,2);
	///* Count triangles */
	//wcdraw_tri_count += 2;
	///* Draw flash image (if any */
	//if(ftex && fle)
	//	wcdraw_texture(ftex,img,x,y,w,h,sx,sy,dw,dh,1.0f-fla,1.0f-fla,r,0,1,flr,flg,flb,0.0f);
	///* End shader */
	//if(wcdraw_current_shader)
	//	wcdraw_current_shader->end();
}

/* Draw texture */
void texture :: draw(int x,int y)
{
	//int w=img->get_visible_width();
	//int h=img->get_visible_height();
	//float u=w/img->get_visible_width();
	//float v=h/img->get_visible_height();

	int w=img->get_width();
	int h=img->get_height();
	float u=w/w;
	float v=h/h;
	
	//spriteBatchDraw_(img->texID,x,y,0,0,u,v,w,h,0);
	//glSprite(x,y,0,&spr[0]);
	//wcdraw_texture(tex,img,x,y,img->get_visible_width(),img->get_visible_height(),0,0,img->get_visible_width(),img->get_visible_height(),alpha,alpha2,rot,ftex,flash_active,flash_red,flash_green,flash_blue,flash);
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

	//glSprite(x,y,0,&spr[0]);
	//wcdraw_texture(tex,img,x,y,w,h,sx,sy,w,h,alpha,alpha2,rot,ftex,flash_active,flash_red,flash_green,flash_blue,flash);
}

/* Draw texture with destination rect */
void texture :: draw(int x,int y,int w,int h,int sx,int sy,int dw,int dh)
{
	int w_=img->get_width();
	int h_=img->get_height();
	float u=w_/w_;
	float v=h_/h_;

	//spriteBatchDraw_(img->texID,x,y,0,0,u,v,w_,h_,0);

	//glSprite(x,y,0,&spr[0]);
	//wcdraw_texture(tex,img,x,y,w,h,sx,sy,dw,dh,alpha,alpha2,rot,ftex,flash_active,flash_red,flash_green,flash_blue,flash);
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
void texture :: enable_flash()
{
	//image *fimg;
	///* Already has flash */
	//if(ftex)
	//	return;
	///* Get white pane */
	//fimg = img->create_flash_image();
	///* Load it as texture */
	//ftex = wcdraw_load_texture(fimg);
	///* Delete source */
	//delete fimg;
}

/* Set the flash */
void texture :: set_flash(float a,int r,int g,int b)
{
	///* Error */
	//if(!ftex)
	//{
	//	wcrpg_error("Cannot set the flash for a texture without flash enabled.");
	//	return;
	//}
	///* Set off */
	//if(a == 0.0f)
	//{
	//	flash_active = 0;
	//	return;
	//}
	///* Set */
	//flash_active = 1;
	//flash = a;
	//flash_red = r;
	//flash_green = g;
	//flash_blue = b;
}

/* Get peak VRAM */
int wcdraw_get_peak_vram()
{
	//return wcdraw_peak_vram;
}