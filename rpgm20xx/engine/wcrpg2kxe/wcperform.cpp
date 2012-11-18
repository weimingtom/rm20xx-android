/*
	Performance Measuring Tool
*/

/* Includes */
#include <memory.h>
#include "wcperform.h"
#include "wcrpg.h"
#include "wcimage.h"

#include <string>

#ifndef NDEBUG

/* Globals */
float wcperform_sample[WCPERFORM_SIZE];
int wcperform_frames = 0;

/* Initializer */
void wcperform_init()
{
	/* Wipe out data */
	memset(wcperform_sample,0,sizeof(wcperform_sample));
}

/* Handler */
void wcperform_handle()
{
	float fps;
	int i;
	/* Measure FPS */
	fps = wcrpg_get_time_step();
	fps = 16.0f/fps;
	/* Move data */
	for(i = 0;i < WCPERFORM_SIZE-1;i++)
		wcperform_sample[i] = wcperform_sample[i+1];
	/* Insert */
	wcperform_sample[WCPERFORM_SIZE-1] = fps;
	wcperform_frames++;
}

/* Get number of full graphs */
int wcperform_get_measure_count()
{
	return wcperform_frames/WCPERFORM_SIZE;
}

/* Finisher */
void wcperform_exit()
{
	image *img;
	int i,h;
	img = new image(WCPERFORM_SIZE,80);
	color cfill,cem,cmark;
	/* Fill color */
	cfill.a = 255;
	cfill.r = 0;
	cfill.g = 255;
	cfill.b = 255;
	/* Empty color */
	cem.a = 255;
	cem.r = 192;
	cem.g = 192;
	cem.b = 192;
	/* Mark color */
	cmark.a = 255;
	cmark.r = 64;
	cmark.g = 64;
	cmark.b = 64;
	/* Fill the bars */
	for(i = 0;i < WCPERFORM_SIZE;i++)
	{
		for(h = 0;h < WCPERFORM_HEIGHT;h++)
		{
			if((80-h) <= (int)wcperform_sample[i])
			{
				img->set(i,h,cfill); /* Full */
			}
			else
			{
				/* Empty background */
				if((80-h) == 60)
					img->set(i,h,cmark); /* 60FPS mark */
				else
					img->set(i,h,cem); /* Empty */
			}
		}
	}
	/* Save */
	img->save("perform.bmp");
	delete img;
}

#endif

/* Does nothing if no debug */

#ifdef NDEBUG

/* Stub */
void wcperform_init(){}
void wcperform_handle(){}
void wcperform_exit(){}
int wcperform_get_measure_count(){return 0;}

#endif