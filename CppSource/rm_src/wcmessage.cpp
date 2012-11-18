/*
	Messages that pop on the top of the screen
*/

/* Includes */
#include "wcrpg.h"
#include "wcarray.h"
#include "wcmessage.h"
#include "wcwindow.h"

/* Globals */
arraylist wcmessage_queue;
float wcmessage_slide = 0.0f;
float wcmessage_time = 0.0f;

/* Initialize */
void wcmessage_init()
{
	wcmessage_queue = wcarray_new();
}

/* Show a message */
void wcmessage_show(char *msg)
{
	/* Check if there is anything to display at all */
	if(!msg)
		return;
	if(!*msg)
		return;
	wcarray_add(wcmessage_queue,new text(msg));
}

/* Handle messenger */
void wcmessage_handle()
{
	float ts;
	ts = wcrpg_get_time_step();
	/* Slide it open if there are messages */
	if(wcarray_size(wcmessage_queue) > 0)
	{
		/* Move open */
		wcmessage_slide += (1.0f-wcmessage_slide)/2.0f*ts;
		/* Wait for a bit */
		wcmessage_time += ts;
		if(wcmessage_time >= 24.0f)
		{
			/* Next message */
			delete (text*)wcarray_get(wcmessage_queue,0);
			wcarray_remove(wcmessage_queue,0);
			wcmessage_time = 0.0f;
			/* Flip back if there is more */
			if(wcarray_size(wcmessage_queue) > 0)
				wcmessage_slide = 0.0f;
		}
		return;
	}
	else
	{
		/* Move closed */
		wcmessage_slide += (0.0f-wcmessage_slide)/2.0f*ts;
	}
}

/* Draw messenger */
void wcmessage_draw()
{
	text *tx;
	wcwindow_draw(0,-32+(int)(32*wcmessage_slide),WCRPG_WIDTH,32);
	if(wcarray_size(wcmessage_queue) <= 0)
		return;
	tx = (text*)wcarray_get(wcmessage_queue,0);
	tx->move(8,6-32+(int)(32*wcmessage_slide));
	tx->draw();
}

/* Exit */
void wcmessage_exit()
{
	int i;
	for(i = 0;i < wcarray_size(wcmessage_queue);i++)
		delete (text*)wcarray_get(wcmessage_queue,i);
	wcarray_delete(wcmessage_queue);
}