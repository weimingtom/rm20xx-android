#ifndef WCPICTURE_H
#define WCPICTURE_H

/* Defines */
#define WCPICTURE_MAX 256

/* Requirements */
#include "wcdraw.h"

/* Classes */
class picture
{
private:
	texture *tex; /* Image */
	float from_x,from_y,to_x,to_y; /* Position */
	float from_a,to_a; /* The OTHER alpha, as in transparency */
	float from_a2,to_a2; /* The OTHER alpha, as in transparency, and for the bottom half */
	float from_s,to_s; /* Scaling */
	float rot,from_r,to_r; /* Rotation POSITION / Rotation SPEED */
	float alpha; /* Linear interpolant */
	float spd; /* Speed it should take for alpha to reach 1 */
	float phase,from_wav,to_wav; /* Wavering from and too */
	float from_red,from_green,from_blue,from_sat; /* Color vector */
	float to_red,to_green,to_blue,to_sat; /* To vector */
	int pin_x,pin_y,pinned; /* This pins the picture to the map and makes it follow the map */
public:
	/*
		Creates a new picture at x and y
	*/
	picture(char *file,int x,int y,int trans);
	~picture();
	/*
		Draws this picture
	*/
	void draw();
	/*
		Pins this picture in its place immediatly so that it follows the map
	*/
	void pin();
	/*
		Moves this picture
		a - alpha transparency
		s - scaling factor
		r - rotation speed
	*/
	void move(int x,int y,int t,float a,float a2,float s,float r,float ww,float cr,float cg,float cb,float cs);
	/*
		Sets the transparency
	*/
	void set_transparency(float a,float a2);
	/*
		Sets the scale
	*/
	void set_scale(float s);
	/*
		Sets the rotation speed
	*/
	void set_rotation(float r);
	/*
		Sets the waver speed
	*/
	void set_waver(float ww);
	/*
		Sets tone color
	*/
	void tone(float r,float g,float b,float s);
	/*
		Handles this picture
	*/
	void handle();
	/*
		Finishes moving immediatly, preparing the image for the next move
	*/
	void finish();
};

/*
	Initialize pictures
*/
extern void wcpicture_init();

/*
	Clean pictures
*/
extern void wcpicture_exit();

/*
	Show picture
*/
extern void wcpicture_show(char *file,int n,int x,int y,int trans);

/*
	Erase picture
*/
extern void wcpicture_erase(int n);

/*
	Draw all pictures
*/
extern void wcpicture_draw();

/*
	Get picture
*/
extern picture *wcpicture_get(int i);

/*
	Handle picture
*/
extern void wcpicture_handle();

/*
	Wipe pictures
*/
extern void wcpicture_wipe();

#endif