/*
	Image Resource
*/

/* Includes */
//#include <SDL.h>
//#include <SDL_image.h>
//#include <windows.h>
#include <malloc.h>
#include <memory.h>
#include <stdio.h>
#include "wcrpg.h"
#include "wcimage.h"
#include "wclog.h"
#include <android\log.h>

#include <string.h>

#include "../jniUtil.h"

/* Extend image */
void image :: extend(int nw,int nh)
{
	int x,y;
	color *ndata;
	/* Allocate dest */
	ndata = (color*)malloc(sizeof(color)*nw*nh);
	memset(ndata,0xFF,sizeof(color)*nw*nh);
	/* Copy old */
	for(y = 0;y < height;y++)
		for(x = 0;x < width;x++)
			ndata[x+(y+nh-height)*nw] = data[x+y*width];
	/* Swap */
	width = nw;
	height = nh;
	free(data);
	data = ndata;
}

/* Find the nearest power of 2 greater than the number */
int wcimage_next_power(int n)
{
	int i;
	/* Set */
	for(i = 0;i < WCIMAGE_EXPAND_MAX;i++)
	{
		/* Fit found */
		if(n <= (1<<i))
			return (1<<i);
	}
	/* Error */
	wcrpg_error("Value too big for image power expansion (%d)",n);
	return n;
}

/* Expand image */
void image :: expand()
{
	/* Extend expand */
	extend(wcimage_next_power(width),wcimage_next_power(height));
}

/* Init */
void image :: init(int w,int h)
{
	int len;
	/* Error */
	if(w < 0 || h < 0)
	{
		wcrpg_error("Can't make an image with negative dimensions.");
		return;
	}
	/* Alloc */
	len = sizeof(color)*w*h;
	width = w;
	height = h;
	vwidth = w;
	vheight = h;
	data = (color*)malloc(len);
	memset(data,0,len);
}

/* Destroy */
image :: ~image()
{
	/* Release */
	//free(data);
}

/* Create black */
image :: image(int w,int h)
{
	/* Init these */
	//init(w,h);
}

/* Create color */
image :: image(int w,int h,color c)
{
	//int x,y;
	///* Init */
	//init(w,h);
	///* Fill */
	//for(y = 0;y < height;y++)
	//	for(x = 0;x < width;x++)
	//		data[x+y*width] = c;
}

/* Create */
image :: image(char *filename)
{
	texID=JNIUtil::jni_load_texture(filename);
	width=JNIUtil::jni_getWidth();
	height=JNIUtil::jni_getHeight();
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "%s %d,%d ",filename, width,height);
	//SDL_Surface *surface;
	//SDL_Color *ckey;
	//int x,y,rank8,rank32,pos;
	//unsigned char *src8;
	//unsigned long *src32;
	//unsigned char *dest;
	///* Nullify */
	//if(!filename)
	//{
	//	wclog_write(wcrpg_get_log(),"Using blank stub image");
	//	wclog_enter(wcrpg_get_log());
	//	init(32,32);
	//	return;
	//}
	///* Announce */
	//wclog_write(wcrpg_get_log(),"Loading image: ");
	//wclog_write(wcrpg_get_log(),filename);
	//wclog_write(wcrpg_get_log(),"...");
	//wclog_enter(wcrpg_get_log());
	///* Create the surface */
	//surface = IMG_Load(filename);
	///* Error? */
	//if(!surface)
	//{
	//	wcrpg_error("Could not load image %s (%s)",filename,IMG_GetError());
	//	init(16,16);
	//	return;
	//}
	///* Get info */
	//init(surface->w,surface->h);
	///* Announce info */
	//wclog_write(wcrpg_get_log(),vwidth);
	//wclog_write(wcrpg_get_log(),"x");
	//wclog_write(wcrpg_get_log(),vheight);
	//wclog_write(wcrpg_get_log()," bpp: ");
	//wclog_write(wcrpg_get_log(),surface->format->BitsPerPixel);
	//wclog_enter(wcrpg_get_log());
	///* Lock surface */
	//if(SDL_LockSurface(surface) == -1)
	//{
	//	wcrpg_error("Could not lock image %s (%s)",filename,SDL_GetError());
	//	return;
	//}
	//rank8 = surface->pitch;
	//rank32 = surface->pitch/4;
	//dest = (unsigned char*)data;
	//src8 = (unsigned char*)surface->pixels;
	//src32 = (unsigned long*)surface->pixels;
	///* Copy pixels */
	//for(y = 0;y < height;y++)
	//{
	//	for(x = 0;x < width;x++)
	//	{
	//		/* Position */
	//		pos = x*sizeof(color)+(height-1-y)*width*sizeof(color);
	//		/* Color Indexed */
	//		if(surface->format->BitsPerPixel == 8)
	//		{
	//			/* Get first color of palette */
	//			ckey = &surface->format->palette->colors[0];
	//			/* 8-bit case */
	//			SDL_GetRGBA(src8[x+y*rank8],surface->format,&dest[pos+2],&dest[pos+1],&dest[pos],&dest[pos+3]);
	//			/* Matches color? */
	//			if(ckey->r == dest[pos+2] && ckey->g == dest[pos+1] && ckey->b == dest[pos])
	//				dest[pos+3] = 0x00; /* Blank it out in alpha map */
	//		}
	//		/* Full 32 Color */
	//		if(surface->format->BitsPerPixel == 32)
	//		{
	//			/* Direct copy */
	//			SDL_GetRGBA(src32[x+y*rank32],surface->format,&dest[pos+2],&dest[pos+1],&dest[pos],&dest[pos+3]);
	//		}
	//	}
	//}
	///* Done, unlock */
	//SDL_UnlockSurface(surface);
	///* Now release */
	//SDL_FreeSurface(surface);
	///* Expand */
	//expand();
}

/* Generate file in memory */
void *image :: generate_bmp()
{
	//unsigned char *d;
	//BITMAPFILEHEADER bfh;
	//BITMAPINFOHEADER bih;
	///* Fill out file header */
	//bfh.bfType = 0x4D42;
	//bfh.bfSize = sizeof(color)*width*height;
	//bfh.bfReserved1 = 0;
	//bfh.bfReserved2 = 0;
	//bfh.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
	///* Fill out info header */
	//bih.biSize = sizeof(BITMAPINFOHEADER);
	//bih.biWidth = width;
	//bih.biHeight = height;
	//bih.biPlanes = 1;
	//bih.biBitCount = 32;
	//bih.biCompression = BI_RGB;
	//bih.biSizeImage = sizeof(color)*width*height;
	//bih.biXPelsPerMeter = 4200;
	//bih.biYPelsPerMeter = 4200;
	//bih.biClrUsed = 0;
	//bih.biClrImportant = 0;
	///* Allocate the container */
	//d = (unsigned char*)malloc(sizeof(bfh)+sizeof(bih)+sizeof(color)*width*height);
	//memcpy_s(d,sizeof(bfh),&bfh,sizeof(bfh));
	//memcpy_s(&d[sizeof(bfh)],sizeof(bih),&bih,sizeof(bih));
	//memcpy_s(&d[sizeof(bfh)+sizeof(bih)],sizeof(color)*width*height,data,sizeof(color)*width*height);
	///* Return it */
	//return d;
}

/* Save file */
void image :: save(char *filename)
{
	FILE *f;
	void *v;
	if(!data)
	{
		wcrpg_error("Can't save a reduced image to %s",filename);
		return;
	}
	/* Generate */
	v = generate_bmp();
	/* Open */
	//fopen(filename,"wb");
	if(!f)
	{
		wcrpg_error("Could not save .BMP %s",filename);
		return;
	}
	/* Write out */
	//fwrite(v,1,size_of_bmp(),f);
	/* End */
	//(f);
	/* Release */
	free(v);
}

/* Size of bmp */
int image :: size_of_bmp()
{
	//return sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+sizeof(color)*width*height;
}

/* Width */
int image :: get_width()
{
	return width;
}

/* Height */
int image :: get_height()
{
	return height;
}

/* Visible Width */
int image :: get_visible_width()
{
	return vwidth;
}

/* Visible Height */
int image :: get_visible_height()
{
	return vheight;
}

/* Remove color key */
void image :: de_color_key()
{
	int x,y;
	if(!data)
	{
		wcrpg_error("Can't remove a color key from a reduced image");
		return;
	}
	for(y = 0;y < height;y++)
		for(x = 0;x < width;x++)
			data[x+y*width].a = 0xFF;
}

/* Set */
void image :: set(int x,int y,color c)
{
	if(!data)
	{
		wcrpg_error("Can't set a pixel inside a reduced image");
		return;
	}
	/* Bound check */
	if(x < 0 || x >= width)
		return;
	if(y < 0 || y >= height)
		return;
	/* Set */
	data[x+(height-1-y)*width] = c;
}

/* Get */
color image :: get(int x,int y)
{
	//color nullcol;
	///* Prepare null color */
	//nullcol.a = 0;
	//nullcol.b = 0;
	//nullcol.g = 0;
	//nullcol.r = 0;
	//if(!data)
	//{
	//	wcrpg_error("Can't get a pixel from a reduced image");
	//	return nullcol;
	//}
	///* Check */
	//if(x < 0 || x >= width)
	//	return nullcol;
	//if(y < 0 || y >= height)
	//	return nullcol;
	///* Get */
	//return data[x+(height-1-y)*width];
}

/* Blit */
void image :: blit(image *src,int sx,int sy,int w,int h,int dx,int dy)
{
	//int x,y;
	//if(!data)
	//{
	//	wcrpg_error("Can't perform a blit with a reduced image");
	//	return;
	//}
	///* Copy pixels */
	//for(y = 0;y < h;y++)
	//	for(x = 0;x < w;x++)
	//		set(dx+x,dy+y,src->get(sx+x,sy+y));
}

/* Tile */
void image :: blit_tile(image *src,int sx,int sy,int w,int h)
{
	int x,y;
	if(!data)
	{
		wcrpg_error("Can't tile a reduced image");
		return;
	}
	for(y = 0;y < height;y += h)
		for(x = 0;x < width;x += w)
			blit(src,sx,sy,w,h,x,y);
}

/* Multiply */
void image :: multiply(image *src)
{
	//int x,y;
	//color s,d;
	//float sa,sr,sg,sb;
	//float da,dr,dg,db;
	//if(!data)
	//{
	//	wcrpg_error("Can't multiply a reduced image");
	//	return;
	//}
	///* Multiply together */
	//for(y = 0;y < height;y++)
	//{
	//	for(x = 0;x < width;x++)
	//	{
	//		/* Get data */
	//		s = src->get(x,y);
	//		d = get(x,y);
	//		/* To sample space */
	//		sa = (float)s.a/255.0f;
	//		sb = (float)s.b/255.0f;
	//		sg = (float)s.g/255.0f;
	//		sr = (float)s.r/255.0f;
	//		da = (float)d.a/255.0f;
	//		db = (float)d.b/255.0f;
	//		dg = (float)d.g/255.0f;
	//		dr = (float)d.r/255.0f;
	//		/* Multiply */
	//		da = sa*da;
	//		dr = sr*dr;
	//		dg = sg*dg;
	//		db = sb*db;
	//		/* Return to the original */
	//		d.a = (int)(da*255);
	//		d.g = (int)(dg*255);
	//		d.b = (int)(db*255);
	//		d.r = (int)(dr*255);
	//		/* Put back */
	//		set(x,y,d);
	//	}
	//}
}

/* Release memory */
void image :: reduce()
{
	//free(data);
	//data = 0;
}

/* Flip horizontal */
void image :: flip_horz()
{
	int x,y;
	int len;
	color *ndata;
	/* Make a new canvas */
	len = sizeof(color)*width*height;
	ndata = (color*)malloc(len);
	/* Flip copy */
	for(y = 0;y < height;y++)
		for(x = 0;x < width;x++)
			ndata[(width-1-x)+y*width] = data[x+y*width];
	/* Kill old */
	free(data);
	/* Make new */
	data = ndata;
}

/* Create flash image */
image *image :: create_flash_image()
{
	int x,y;
	image *whiteimg;
	color srcc,dstc;
	/* Make a new blank white image */
	whiteimg = new image(get_width(),get_height());
	/* Multiply only the source alpha */
	for(y = 0;y < height;y++)
	{
		for(x = 0;x < width;x++)
		{
			/* Get source */
			srcc = get(x,y);
			/* White, but alpha matches source */
			dstc.r = 255;
			dstc.g = 255;
			dstc.b = 255;
			dstc.a = srcc.a;
			/* Put */
			whiteimg->set(x,y,dstc);
		}
	}
	/* Return the result */
	return whiteimg;
}