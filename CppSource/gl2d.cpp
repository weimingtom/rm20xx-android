/*

	Easy GL2D

	Relminator 2010
	Richard Eric M. Lope BSN RN

	http://rel.betterwebber.com


*/


#include "gl2d.h"
#include "jniUtil.h"
#include <android/log.h>

#include "gmUtil.h"

// mingw header excluding GL_BGRA in windows
#ifndef GL_BGRA
#define GL_BGRA  0x80E1
#endif

#include <map>


/******************************************************************************

    IMPLEMENTATION

******************************************************************************/


/******************************************************************************

    Private Functions

******************************************************************************/



/******************************************************************************

   Private!!!
   Sets up OpenGL for 2d rendering

******************************************************************************/
int gWidth;
int gHeight;


static GLuint gCurrentTexture = 0;

struct Vertex
{
	short v[2];
	unsigned color;
	float uv[2];
};

struct VertexInfo
{
	Vertex vertex[1000];
	int _vertexCount;
};

typedef std::map<GLuint, VertexInfo> VertexMap;
VertexMap vertices;

SpriteBatch *spriteBatch;

void Init2D( const int width, const int height )
{
	gCurrentTexture = 0;
	gWidth=width;
	gHeight=height;

	glDisable( GL_CULL_FACE);

	//glEnable( GL_ALPHA_TEST );
	//glAlphaFunc( GL_LEQUAL ,0.1 );

    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ); 

	 
    glDisable( GL_STENCIL_TEST );
    glDisable( GL_LIGHTING );
    glDisable( GL_DITHER );
    glDisable( GL_FOG );

    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity(); 
	//glOrthof( 0, width, height, 0, -1, 1 );
	glOrthof( 0, width, height, 0, -1000, 1000 );
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glLoadIdentity();
    glTranslatef( 0.375, 0.375, 0.0 );

	spriteBatch = new GLES11SpriteBatch(width, height);
}

/******************************************************************************


******************************************************************************/

void glScreenInit(const int width, const int height)
{
    double FOVy;
    double aspect;
    double znear;
    double zfar;

    // using screen info w and h as params
    glViewport( 0, 0, width, height );

    // Set current Mode to projection(ie: 3d)
    glMatrixMode( GL_PROJECTION );

    // Load identity matrix to projection matrix
    glLoadIdentity();

    // Set gluPerspective params
    FOVy = 90 / 2;                                     //45 deg fovy
    aspect = width / height;
    znear = 1;                                       //Near clip
    zfar = 500;                                      //far clip

    // use glu Perspective to set our 3d frustum dimension up
    //gluPerspective( FOVy, aspect, znear, zfar );

    // Modelview mode
    // ie. Matrix that does things to anything we draw
    // as in lines, points, tris, etc.
    glMatrixMode( GL_MODELVIEW );

    // load identity(clean) matrix to modelview
    glLoadIdentity();

    //glShadeModel( GL_SMOOTH );                 //'set shading to smooth(try GL_FLAT)
    glClearColor( 0.0, 0.0, 0.0, 1.0 );        //'set Clear color to BLACK
	//glClearDepthf( 1.0 );                       //'Set Depth buffer to 1(z-Buffer)
	
	//'compare each incoming pixel z value with the z value present in the depth buffer
    //'LEQUAL means than pixel is drawn if the incoming z value is less than
    //'or equal to the stored z value
	//glEnable( GL_DEPTH_TEST );
	//glDepthFunc( GL_LEQUAL );
	//glClearDepthf(1.0);


    //'have one or more material parameters track the current color
    //'Material is your 3d model
    //glEnable( GL_COLOR_MATERIAL );

	//Enable Texturing
    glEnable( GL_TEXTURE_2D );

    //'Tell openGL that we want the best possible perspective transform
    //glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

    //'Disable Backface culling
    glDisable ( GL_CULL_FACE );

    //glDisable( GL_DEPTH_TEST );
    glDisable( GL_STENCIL_TEST );
    glDisable( GL_LIGHTING );
    glDisable( GL_DITHER );
    glDisable( GL_FOG );

    Init2D( width, height );
}


void glClearScreen()
{

    //glClearColor( 0.0, 0.0f, 0.0f, 0.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}


void renderToScreen()
{
	glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	VertexMap::iterator it = vertices.begin();
    for (; it != vertices.end(); it++)
    {

      	VertexInfo *vertexInfo = &it->second;
    	Vertex *vert = vertexInfo->vertex;

		//if(test<1){
		//	__android_log_print(ANDROID_LOG_INFO, "adMOB","glid %d z: %f",(*it).first, vert->v[2]);
		//}

    	if ( (*it).first != gCurrentTexture )
    	{
    		glBindTexture( GL_TEXTURE_2D, (*it).first );
    		gCurrentTexture = (*it).first;
    	}

		glVertexPointer(2, GL_SHORT, sizeof(Vertex), &vert[0].v);
    	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &vert[0].uv);
    	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex), &vert[0].color);
    	glDrawArrays(GL_TRIANGLES, 0, vertexInfo->_vertexCount);
    	vertexInfo->_vertexCount = 0;
    }

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

void addVertex(GLuint glid, float x, float y, float uvx, float uvy, unsigned color)
{
	
	VertexInfo *vertexInfo = &vertices[glid];
	//if(test<1)
	//	__android_log_print(ANDROID_LOG_INFO, "adMOB","glid %d vertexInfo->_vertexCount %d %f",glid, vertexInfo->_vertexCount, z);
	Vertex *vert = &vertexInfo->vertex[vertexInfo->_vertexCount];
    vert->v[0] = x;
    vert->v[1] = y;
    vert->uv[0] = uvx;
    vert->uv[1] = uvy;
    vert->color = color;
    vertexInfo->_vertexCount++;
}

/******************************************************************************

    Draws a sprite
	Parameters:
       x 		-> x position of the sprite
	   y 		-> y position of the sprite
	   flipmode -> mode for flipping (see GL_FLIP_MODE enum)
 *spr 	-> pointer to a glImage

 ******************************************************************************/
void glSprite( int x, int y, int flipmode, const glImage *spr,bool rotate,const float angle, const float scale1, const float scale2,float r, float g, float b, float a)
{
	spriteBatchDraw(x,y,flipmode,spr,rotate,angle,scale1,scale2,r,g,b,a);
	return;

	int glid=spr->textureID;

	VertexMap::iterator it = vertices.find(glid);
	if (it == vertices.end()){
		vertices[glid]._vertexCount = 0;
	}

	float scaleX;
	float scaleY;

	if(flipmode & GL2D_NO_SCALE){
		scaleX=gWidth/800.0f;
		scaleY=gHeight/480.0f;
	}else{
		scaleX=gWidth/GMUtil::SCREEN_WIDTH;
		scaleY=gHeight/GMUtil::SCREEN_HEIGHT;
	}

	float topLeftX;
	float topLeftY;
	float topRightX;
	float topRightY;
	float bottomLeftX;
	float bottomLeftY;
	float bottomRightX;
	float bottomRightY;


	if(rotate){
		float sinTheta = sinf(angle) * spr->height* .5f;
		float cosTheta = cosf(angle) * spr->width * .5f;

		topLeftX = x - cosTheta +  sinTheta;
		topLeftY = y - sinTheta -  cosTheta;

		topRightX = x + cosTheta +  sinTheta;
		topRightY = y + sinTheta -  cosTheta;

		bottomLeftX = x - cosTheta -  sinTheta;
		bottomLeftY = y - sinTheta +  cosTheta;

		bottomRightX = x + cosTheta -  sinTheta;
		bottomRightY = y + sinTheta +  cosTheta;
	}else{
		int x1=x;
		int y1=y;
		int x2=x+spr->width;
		int y2=y+spr->height;
		if(flipmode & GL2D_CENTER_2){
			x1 =  x-spr->width;
			y1 =  y-spr->height;
			x2 =  x;
			y2 =  y;
		}else if(flipmode & GL2D_CENTER){
			x1 =  x-spr->width*scale1/2;
			y1 =  y-spr->height*scale2/2;
			x2 =  x+spr->width*scale1/2;
			y2 =  y+spr->height*scale2/2;
		}

		topLeftX = x1;
		topLeftY = y1;
		topRightX = x2;
		topRightY = y1;
		bottomLeftX = x1;
		bottomLeftY = y2;
		bottomRightX = x2;
		bottomRightY = y2;
	}

	float u1 = ( flipmode & GL2D_FLIP_H ) ? spr->u_off + spr->u_width  : spr->u_off;
	float u2 = ( flipmode & GL2D_FLIP_H ) ? spr->u_off 			       : spr->u_off + spr->u_width;
	float v1 = ( flipmode & GL2D_FLIP_V ) ? spr->v_off + spr->v_height : spr->v_off;
	float v2 = ( flipmode & GL2D_FLIP_V ) ? spr->v_off 			       : spr->v_off + spr->v_height;

	topLeftX*=scaleX;
	topRightX*=scaleX;
	bottomLeftX*=scaleX;
	bottomRightX*=scaleX;

	topLeftY*=scaleY;
	topRightY*=scaleY;
	bottomLeftY*=scaleY;
	bottomRightY*=scaleY;

	//	Convert the colors into bytes
	unsigned char red = r * 255.0f;
	unsigned char green = g  * 255.0f;
	unsigned char blue = b  * 255.0f;
	unsigned char shortAlpha = a  * 255.0f;

	//	pack all of the color data bytes into an unsigned int
	unsigned _color = (shortAlpha << 24) | (blue << 16) | (green << 8) | (red << 0);

		// Triangle #1
	//__android_log_print(ANDROID_LOG_INFO, "adMOB","addVertex 1");
	addVertex(glid, topLeftX, topLeftY, u1, v1, _color);
	//__android_log_print(ANDROID_LOG_INFO, "adMOB","addVertex 2");
	addVertex(glid, topRightX, topRightY, u2, v1, _color);
	//__android_log_print(ANDROID_LOG_INFO, "adMOB","addVertex 3");
	addVertex(glid, bottomLeftX, bottomLeftY, u1, v2, _color);

	// Triangle #2
	//__android_log_print(ANDROID_LOG_INFO, "adMOB","addVertex 4");
	addVertex(glid, topRightX, topRightY, u2, v1, _color);
	//__android_log_print(ANDROID_LOG_INFO, "adMOB","addVertex 5");
	addVertex(glid, bottomLeftX, bottomLeftY, u1, v2, _color);
	//__android_log_print(ANDROID_LOG_INFO, "adMOB","addVertex 6");
	addVertex(glid, bottomRightX, bottomRightY, u2, v2, _color);
}

/******************************************************************************

    Draws a center rotated sprite
	Parameters:
       x 		-> x position of the sprite center
	   y 		-> y position of the sprite center
	   angle    -> is the angle to rotated (in degrees)
	   flipmode -> mode for flipping (see GL_FLIP_MODE enum)
 *spr 	-> pointer to a glImage

 ******************************************************************************/
void glSpriteRotate( int x, int y, const float angle, int flipmode, const glImage *spr ,float r, float g, float b, float a)
{

	glSprite(x,y,flipmode,spr,true,angle,1,1,r,g,b,a);
}

void glSpriteNoScale( int x, int y, int flipmode, const glImage *spr )
{
	glSprite(x,y,GL2D_NO_SCALE,spr);
}

void glSpriteNoScaleCenter( int x, int y, int flipmode, const glImage *spr ,float r, float g, float b, float a)
{
	glSprite(x,y,GL2D_NO_SCALE | GL2D_CENTER,spr,false,0,1,1,r,g,b,a);
}

void glSpriteNoScaleCenter2( int x, int y, int flipmode, const glImage *spr,float r, float g, float b, float a )
{
	glSprite(x,y,GL2D_NO_SCALE | GL2D_CENTER_2,spr,false,0,1,1,r,g,b,a);
}


void glSpriteRotateScale(int x, int y, const float angle, const float scale, const int flipmode, const glImage *spr,float r, float g, float b, float a){
	glSprite(x,y,GL2D_CENTER,spr,false,0,scale,scale,r,g,b,a);
}

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
                      )
{

    int i;
    GLuint textureID;

    //glGenTextures ( 1, &textureID );
    //glBindTexture ( GL_TEXTURE_2D, textureID );

	textureID=JNIUtil::jni_load_texture(filename);
	


    //LoadTexture( filename );

    //glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    //glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtermode );
    //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtermode );
    //glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

    //glBindTexture( GL_TEXTURE_2D, 0 );

    // init sprites texture coords and texture ID
    for ( i = 0; i < numframes; i++)
    {
        int j = i * 4; // texcoords array is u_off, wid, hei
        sprite[i].textureID = textureID;
        sprite[i].width = texcoords[j+2];
        sprite[i].height = texcoords[j+3];
        sprite[i].u_off = texcoords[j] / (float)width;				// set x-coord
        sprite[i].v_off = texcoords[j+1] / (float)height;			// y-coord
        sprite[i].u_width = texcoords[j+2] / (float)width;			// set x-coord
        sprite[i].v_height = texcoords[j+3] / (float)height;		// y-coord


    }

    return textureID;

}


int glLoadTileSet(  const char *filename,
				   glImage              *sprite,
				   int                  tile_wid,
				   int                  tile_hei,
                   int                  bmp_wid,
				   int                  bmp_hei	  
                 )
{

	GLuint textureID;
	textureID=JNIUtil::jni_load_texture(filename);

	int i=0;
	int x, y;
	
	// init sprites texture coords and texture ID
	int calcHeight=bmp_hei/tile_hei;
	int calcWidth=bmp_wid/tile_wid;

	//__android_log_print(ANDROID_LOG_INFO, "*************", filename);
	for (y = 0; y < calcHeight; y++)
	{
		for (x = 0; x < calcWidth; x++) 
		{
			//__android_log_print(ANDROID_LOG_INFO, "*************", "%d %d %d %d %d",x, y, calcWidth, calcHeight, i);
			sprite[i].width 			= tile_wid;
			sprite[i].height 			= tile_hei;
			sprite[i].u_off				= (float)(x*tile_wid)/ (float)bmp_wid;
			sprite[i].v_off				= (float)(y*tile_hei)/ (float)bmp_hei; 
			sprite[i].u_width			= (float)(tile_wid)/ (float)bmp_wid;
			sprite[i].v_height			= (float)(tile_hei)/ (float)bmp_hei; 
			sprite[i].textureID 		= textureID;
			i++;
		}
	}

	return textureID;
}


void spriteBatchBegin(){
	spriteBatch->begin(SpriteBatch::eALPHA);

}
void spriteBatchEnd(){
	spriteBatch->end();
}
void spriteBatchDraw(int x, int y, int flipmode, const glImage *spr,bool rotate,const float angle, const float scale1, const float scale2, float r, float g, float b, float a){
	spriteBatchDraw_(spr->textureID,x,y,spr->u_off,spr->v_off,spr->u_width,spr->v_height,spr->width,spr->height,flipmode,rotate,angle,scale1,scale2,r,g,b,a);
	return;
    SpriteDrawInfo sdi;
	sdi.textureHandle = spr->textureID;
	sdi.setTargetPos( x, gHeight-y);
    sdi.angle = angle;
    sdi.setScale(spr->width*scale1,spr->height*scale2);
	sdi.a = a;
    sdi.setColor(r, g, b, a);
	sdi.setOrigin(0,0);

	if(flipmode & GL2D_FLIP_H)
		sdi.setSourceRect(spr->u_off+spr->u_width,spr->v_off,-spr->u_width,spr->v_height);
	else
		sdi.setSourceRect(spr->u_off,spr->v_off,spr->u_width,spr->v_height);
    spriteBatch->draw( &sdi );
}

void spriteBatchDraw_(int texid, int x, int y, float u_off, float v_off, float u_width, float v_height, int width,int height, int flipmode, bool rotate,const float angle, const float scale1, const float scale2, float r, float g, float b, float a){
	float scaleX;
	float scaleY;
	if(flipmode & GL2D_NO_SCALE){
		scaleX=gWidth/800.0f;
		scaleY=gHeight/480.0f;
	}else{
		scaleX=gWidth/320.0f;
		scaleY=gHeight/240.0f;
	}
    SpriteDrawInfo sdi;
	sdi.textureHandle = texid;
	sdi.setTargetPos( x*scaleX, (gHeight-(y*scaleY)));
    sdi.angle = angle;
	sdi.setScale(width*scale1*scaleX,height*scale2*scaleY);
	sdi.a = a;
    sdi.setColor(r, g, b, a);
	if(flipmode & GL2D_CENTER){
		sdi.setOrigin(0.5f,0.5f);
	}else{
		sdi.setOrigin(0,0);
	}

	if(flipmode & GL2D_FLIP_H)
		sdi.setSourceRect(u_off+u_width,v_off,-u_width,v_height);
	else
		sdi.setSourceRect(u_off,v_off,u_width,v_height);
    spriteBatch->draw( &sdi );
}