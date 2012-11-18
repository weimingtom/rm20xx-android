/*

	Easy GL2D

	Relminator 2010
	Richard Eric M. Lope BSN RN

	http://rel.betterwebber.com


*/


#ifndef GL2D_H_INCLUDED
#define GL2D_H_INCLUDED

#include <GLES/gl.h>
#include <GLES/glext.h>
#include <stdio.h>
#include <math.h>
//#include <GL/glfw.h>

#include "SpriteBatch.h"


#define ARGB_A(u) (((u) >> 24) & 0x000000FF)
#define ARGB_R(u) (((u) >> 16) & 0x000000FF)
#define ARGB_G(u) (((u) >> 8)  & 0x000000FF)
#define ARGB_B(u) (((u) >> 0)  & 0x000000FF)
#define RGBA( r, g, b, a ) ((a) << 24 | (r) << 16 | (g) << 8 | (b) )
#define GL2D_RGBA( r, g, b, a ) RGBA( b, g, r, a)

#ifndef true
    #define true 1
    #define false 0
#endif

/******************************************************************************

                          ENUMS and STRUCTURES

******************************************************************************/

/*
  These enums are bits for flipping the sprites
  You can "|" (or) GL_FLIP_V and GL_FLIP_H to flip
  both ways.
*/
typedef enum GL2D_FLIP_MODE
{

    GL2D_FLIP_NONE 	= 1 << 0,
    GL2D_FLIP_V 	= 1 << 1,
    GL2D_FLIP_H 	= 1 << 2,
    GL2D_CENTER 	= 1 << 3,
    GL2D_CENTER_2 	= 1 << 4,
    GL2D_NO_SCALE 	= 1 << 5
} GL2D_FLIP_MODE;


typedef enum GL2D_BLEND_MODE
{

    GL2D_TRANS = 0,
	GL2D_SOLID,
	GL2D_BLENDED,
	GL2D_GLOW,
	GL2D_ALPHA,
	GL2D_BLACK


} GL2D_BLEND_MODE;

/*
  This is our struct to hold our image
  attributes. You don't need to worry about this
  if you use the texture packer.

  width -> image width
  height -> image height
  u_off/v_off -> ST/XY/UV texture offsets
  u_width/v_height -> texture atlas texture width and height
  textureID -> the texture handle in VRAM (returned by glGenTextures())
               ie. This references the actual texture stored in VRAM
*/
typedef struct glImage
{

    int		width;
    int 	height;
    float	u_off;
    float	v_off;
    float	u_width;
    float	v_height;

    GLuint	textureID;

} glImage;


void glScreenInit(const int width, const int height);

void glBlendMode(const GL2D_BLEND_MODE mode);

void glClearScreen();

void glLineGlow( const int x1, const int y1,
                 const int x2, const int y2,
                 const int width, const GLuint color
               );

void glPutPixel( int x1, int y1, const GLuint color );

void glLine( int x1, int y1, int x2, int y2, const GLuint color );

void glLineGradient( int x1, int y1, int x2, int y2,
                     const GLuint color1, const GLuint color2
                   );

void glBox(int x1, int y1, int x2, int y2, const GLuint color);

void glBoxFilled(int x1, int y1, int x2, int y2, const GLuint color);

void glBoxFilledGradient( int x1, int y1, int x2, int y2,
                          const GLuint color1, const GLuint color2,
                          const GLuint color3, const GLuint color4
                        );

void glTriangle( int x1, int y1, int x2, int y2, int x3, int y3, const GLuint color );

void glTriangleFilled( int x1, int y1, int x2, int y2, int x3, int y3, const GLuint color );

void glTriangleFilledGradient( int x1, int y1, int x2, int y2, int x3, int y3,
                               const GLuint color1, const GLuint color2, const GLuint color3
                             );

void glEllipse(int x, int y, int a, int b, int degrees, const GLuint color);

void glEllipseFilled(int x, int y, int a, int b, int degrees, const GLuint color);

void glSpriteNoScale( int x, int y, int flipmode, const glImage *spr );

/******************************************************************************

    Draws a sprite
	Parameters:
       x 		-> x position of the sprite
	   y 		-> y position of the sprite
	   flipmode -> mode for flipping (see GL_FLIP_MODE enum)
	   *spr 	-> pointer to a glImage

******************************************************************************/
void glSprite(int x, int y, int flipmode, const glImage *spr,bool rotate=false,const float angle=0.0f, const float scale1=1, const float scale2=1, float r=1, float g=1, float b=1, float a=1);

/******************************************************************************

    Draws a center rotated sprite
	Parameters:
       x 		-> x position of the sprite center
	   y 		-> y position of the sprite center
	   angle    -> is the angle to rotated (in degrees)
	   flipmode -> mode for flipping (see GL_FLIP_MODE enum)
	   *spr 	-> pointer to a glImage

******************************************************************************/
void glSpriteRotate(const int x, const int y, const float angle, const int flipmode, const glImage *spr,float r=1, float g=1, float b=1, float a=1);


/******************************************************************************

    Draws a center rotated and scaled sprite
	Parameters:
       x 		-> x position of the sprite center
	   y 		-> y position of the sprite center
	   angle    -> is the angle to rotated (-32768 to 32767)
	   scale    -> is a 20.12 FP scale value (you can use floattof32(1.45) to scale
	               and GCC would optimize it to a constant)
	   flipmode -> mode for flipping (see GL_FLIP_MODE enum)
	   *spr 	-> pointer to a glImage

******************************************************************************/
void glSpriteRotateScale(int x, int y, const float angle, const float scale, const int flipmode, const glImage *spr,float r=1, float g=1, float b=1, float a=1);

void glSpriteNoScaleCenter2( int x, int y, int flipmode, const glImage *spr,float r=1, float g=1, float b=1, float a=1 );

void glSpriteNoScaleCenter( int x, int y, int flipmode, const glImage *spr,float r=1, float g=1, float b=1, float a=1 );

void addVertex(GLuint glid, float x, float y, float uvx, float uvy, unsigned color);
void renderToScreen();

/******************************************************************************

    Initializes our spriteset with Texture Packer generated UV coordinates
	Very safe and easy to use.
	Parameters:
       filename     -> name of the file to load as texture
       *sprite 		-> pointer to an array of glImage
       width/height -> image dimensions
	   numframes 	-> number of frames in a spriteset (auto-generated by Texture Packer)
	   *texcoords 	-> Texture Packer auto-generated array of UV coords
	   textureID 	-> Texture handle returned by glGenTextures()
       filtemode    -> GL_NEAREST or GL_LINEAR

******************************************************************************/
GLuint glLoadSpriteset( const char *filename,
                        glImage *sprite,
                        const int width,
                        const int height,
                        const int numframes,
                        const unsigned int *texcoords,
                        const  GLuint filtermode
                      );


int glLoadTileSet(  const char *filename,
				   glImage              *sprite,
				   int                  tile_wid,
				   int                  tile_hei,
                   int                  bmp_wid,
				   int                  bmp_hei		  
                 );

void Init2D( const int width, const int height );

void spriteBatchBegin();
void spriteBatchEnd();

void spriteBatchDraw(int x, int y, int flipmode, const glImage *spr,bool rotate=false,const float angle=0.0f, const float scale1=1, const float scale2=1, float r=1, float g=1, float b=1, float a=1);


void spriteBatchDraw_(int texid, int x, int y, float u_off, float v_off, float u_width, float v_height, int width,int height, int flipmode, bool rotate=false,const float angle=0.0f, const float scale1=1, const float scale2=1, float r=1, float g=1, float b=1, float a=1);


#endif // GL2D_H_INCLUDED
