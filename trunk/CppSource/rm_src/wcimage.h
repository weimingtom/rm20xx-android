#ifndef WCIMAGE_H
#define WCIMAGE_H

/* Defines */
#define WCIMAGE_EXPAND_MAX 24

/* Typedefs */
typedef struct
{
	unsigned char r,g,b,a;
}color;

/* Classes */
class image
{
private:
	int width,height; /* Dimensions of the image */
	int vwidth,vheight; /* Dimensions of the VISIBLE image, not the power of two excess */
	color *data; /* Actual image data */

	/*
		Initializes an image
	*/
	void init(int w,int h);
	/*
		Expands the image to fit powers of 2 boundaries
	*/
	void expand();
public:

	int texID;
	/*
		Creates a new image from file
	*/
	image(char *filename);
	/*
		Creates a new image that is blank
	*/
	image(int w,int h);
	/*
		Creates a new image that is filled with the color given
	*/
	image(int w,int h,color c);
	~image();
	/*
		Saves the .bmp file that is also used for texture loading
	*/
	void save(char *filename);
	/*
		Generates a .bmp file in memory of this
		the given pointer must be freed by the caller
	*/
	void *generate_bmp();
	/*
		Returns the image's memory size as though it were a .bmp file
	*/
	int size_of_bmp();
	/*
		Returns the size of the image.
	*/
	int get_width();
	int get_height();
	/*
		Returns the visible size of the image.
	*/
	int get_visible_width();
	int get_visible_height();
	/*
		De-keys the color keys on this image
	*/
	void de_color_key();
	/*
		Executes a blit from one image to another
	*/
	void blit(image *src,int sx,int sy,int w,int h,int dx,int dy);
	/*
		Set color 
	*/
	void set(int x,int y,color c);
	/*
		Get color
	*/
	color get(int x,int y);
	/*
		Tile with a blit
	*/
	void blit_tile(image *src,int sx,int sy,int w,int h);
	/*
		Multiplies this image with the source
	*/
	void multiply(image *src);
	/*
		Releases the memory held by the actual image such that only the dimensions remain
		calls to save image to disk or get dimensions will fail
	*/
	void reduce();
	/*
		Flip this image horizontally
	*/
	void flip_horz();
	/*
		Takes this image and creates a flash panel of this image
	*/
	image *create_flash_image();
	/*
		Extends the canvas of an image
	*/
	void extend(int nw,int nh);
};

#endif