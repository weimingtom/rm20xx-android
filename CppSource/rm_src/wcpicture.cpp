/*
	Pictures used with the show picture command
*/

/* Includes */
#include <memory.h>
#include <math.h>
#include "wcmaterial.h"
#include "wcpicture.h"
#include "wclevel.h"
#include "wcrpg.h"

#include <string>

/* Globals */
picture *pics[WCPICTURE_MAX];

/* Create a new picture */
picture :: picture(char *file,int x,int y,int trans)
{
	/* Load texture */
	tex = new texture(wcmaterial_request(MATERIAL_PICTURE,file),trans);
	from_a = 0.0f;
	to_a = 0.0f;
	from_a2 = 0.0f;
	to_a2 = 0.0f;
	from_x = (float)x;
	from_y = (float)y;
	to_x = from_x;
	to_y = from_y;
	alpha = 0;
	from_r = 0.0f;
	to_r = 0.0f;
	rot = 0.0f;
	spd = 0;
	pinned = 0;
	pin_x = 0;
	pin_y = 0;
	from_s = 1.0f;
	to_s = 1.0f;
	phase = 0.0f;
	from_wav = 0.0f;
	to_wav = 0.0f;
	from_red = 1.0f;
	from_green = 1.0f;
	from_blue = 1.0f;
	from_sat = 1.0f;
	to_red = 1.0f;
	to_green = 1.0f;
	to_blue = 1.0f;
	to_sat = 1.0f;
}

/* Delete picture */
picture :: ~picture()
{
	delete tex;
}

/* Pin the picture to the map */
void picture :: pin()
{
	pinned = 1;
	pin_x = wclevel_get_camera_x();
	pin_y = wclevel_get_camera_y();
}

/* Draw picture */
void picture :: draw()
{
	float dx,dy;
	float da,da2;
	float ds;
	float drawa,drawa2;
	float draws;
	float drawww;
	float dw,drawws;
	float dcr,dcg,dcb,dcs;
	float drawcr,drawcg,drawcb,drawcs;
	int cdx,cdy;
	int drawx,drawy;
	int i;
	/* Get distance */
	dx = to_x-from_x;
	dy = to_y-from_y;
	da = to_a-from_a;
	da2 = to_a2-from_a2;
	ds = to_s-from_s;
	dw = to_wav-from_wav;
	dcr = to_red-from_red;
	dcg = to_green-from_green;
	dcb = to_blue-from_blue;
	dcs = to_sat-from_sat;
	/* Calculate moving */
	dx *= alpha;
	dy *= alpha;
	da *= alpha;
	da2 *= alpha;
	ds *= alpha;
	dw *= alpha;
	dcr *= alpha;
	dcg *= alpha;
	dcb *= alpha;
	dcs *= alpha;
	drawx = (int)(from_x+dx);
	drawy = (int)(from_y+dy);
	drawa = from_a+da;
	drawa2 = from_a2+da2;
	draws = from_s+ds;
	drawws = from_wav+dw;
	drawcr = from_red+dcr;
	drawcg = from_green+dcg;
	drawcb = from_blue+dcb;
	drawcs = from_sat+dcs;
	/* Center */
	drawx -= (int)(tex->get_visible_width()/2*draws);
	drawy -= (int)(tex->get_visible_height()/2*draws);
	/* Pinned? */
	if(pinned)
	{
		cdx = wclevel_get_camera_x()-pin_x;
		cdy = wclevel_get_camera_y()-pin_y;
		drawx -= cdx;
		drawy -= cdy;
	}
	/* Draw it */
	tex->tone(drawcr,drawcg,drawcb,drawcs);
	if(alpha >= 1.0f && to_r == 0.0f)
		tex->set_rotation(0);
	else
		tex->set_rotation(rot);
	tex->set_transparency(drawa,drawa2);
	if(phase != 0.0f)
	{
		drawww = phase;
		for(i = 0;i < tex->get_visible_height();i++)
		{
			tex->draw(drawx+(int)(sin(drawww)*drawws*4.0f),drawy+i,tex->get_visible_width(),1,0,i,(int)(tex->get_visible_width()*draws),1);
			drawww += 0.2f;
		}
	}
	else
	{
		tex->draw(drawx,drawy,tex->get_visible_width(),tex->get_visible_height(),0,0,(int)(tex->get_visible_width()*draws),(int)(tex->get_visible_height()*draws));
	}
}

/* Move picture */
void picture :: move(int x,int y,int t,float a,float a2,float s,float r,float ww,float cr,float cg,float cb,float cs)
{
	/* Set destination */
	to_x = (float)x;
	to_y = (float)y;
	to_a = a;
	to_a2 = a2;
	to_s = s;
	to_r = r;
	to_wav = ww;
	to_red = cr;
	to_green = cg;
	to_blue = cb;
	to_sat = cs;
	alpha = 0.0f;
	spd = 0.6f/((float)(t)); /* Tens of seconds */
}

/* Handle picture */
void picture :: handle()
{
	float ts,dr,dw;
	/* Increment alpha */
	ts = wcrpg_get_time_step();
	alpha += ts*spd;
	if(alpha > 1.0f)
		alpha = 1.0f;
	/* Handle rotation */
	dr = to_r-from_r;
	dr *= alpha;
	rot += (from_r+dr)*ts*0.1f;
	/* Handle waver */
	dw = to_wav-from_wav;
	dw *= alpha;
	if(to_wav != 0.0f || from_wav != 0.0f)
		phase += ts*0.6f;
}

/* Set transparency */
void picture :: set_transparency(float a,float a2)
{
	from_a = a;
	to_a = a;
	from_a2 = a2;
	to_a2 = a2;
}

/* Set scale */
void picture :: set_scale(float s)
{
	from_s = s;
	to_s = s;
}

/* Set rotation */
void picture :: set_rotation(float r)
{
	from_r = r;
	to_r = r;
	if(r == 0.0f)
		rot = 0.0f;
}

/* Set waver */
void picture :: set_waver(float ww)
{
	from_wav = ww;
	to_wav = ww;
	if(ww == 0.0f)
		phase = 0.0f;
}

/* Set tone */
void picture :: tone(float r,float g,float b,float s)
{
	from_red = r;
	from_green = g;
	from_blue = b;
	from_sat = s;
	to_red = r;
	to_green = g;
	to_blue = b;
	to_sat = s;
}

/* Finishes the moving */
void picture :: finish()
{
	float dx,dy;
	float da,da2;
	float ds;
	float dr;
	float dw;
	float dcr,dcg,dcb,dcs;
	/* Get distance */
	dx = to_x-from_x;
	dy = to_y-from_y;
	da = to_a-from_a;
	da2 = to_a2-from_a2;
	ds = to_s-from_s;
	dr = to_r-from_r;
	dw = to_wav-from_wav;
	dcr = to_red-from_red;
	dcg = to_green-from_green;
	dcb = to_blue-from_blue;
	dcs = to_sat-from_sat;
	/* Calculate moving */
	dx *= alpha;
	dy *= alpha;
	da *= alpha;
	da2 *= alpha;
	ds *= alpha;
	dr *= alpha;
	dw *= alpha;
	dcr *= alpha;
	dcg *= alpha;
	dcb *= alpha;
	dcs *= alpha;
	/* Finish moving */
	from_x = from_x+dx;
	from_y = from_y+dy;
	from_a = from_a+da;
	from_a2 = from_a2+da2;
	from_s = from_s+ds;
	from_r = from_r+dr;
	from_wav = from_wav+dw;
	from_red = from_red+dcr;
	from_green = from_green+dcg;
	from_blue = from_blue+dcb;
	from_sat = from_sat+dcs;
	to_x = from_x;
	to_y = from_y;
	to_a = from_a;
	to_a2 = from_a2;
	to_s = from_s;
	to_r = from_r;
	to_wav = from_wav;
	to_red = from_red;
	to_green = from_green;
	to_blue = from_blue;
	to_sat = from_sat;
	alpha = 0.0f;
}

/* Initializer */
void wcpicture_init()
{
	memset(pics,0,sizeof(pics));
}

/* Eraser */
void wcpicture_wipe()
{
	int i;
	/* Erase all */
	for(i = 0;i < WCPICTURE_MAX;i++)
		wcpicture_erase(i);
}

/* Releaser */
void wcpicture_exit()
{
	wcpicture_wipe();
}

/* Creator */
void wcpicture_show(char *file,int n,int x,int y,int trans)
{
	/* Validate */
	if(n < 0 || n >= WCPICTURE_MAX)
	{
		wcrpg_error("Invalid picture number to show (%d)",n);
		return;
	}
	/* Create new picture */
	wcpicture_erase(n);
	pics[n] = new picture(file,x,y,trans);
}

/* Eraser */
void wcpicture_erase(int n)
{
	/* Validate */
	if(n < 0 || n >= WCPICTURE_MAX)
	{
		wcrpg_error("Invalid picture number to erase (%d)",n);
		return;
	}
	/* Erase picture */
	if(pics[n])
		delete pics[n];
	pics[n] = 0;
}

/* Drawing */
void wcpicture_draw()
{
	int i;
	for(i = 0;i < WCPICTURE_MAX;i++)
	{
		if(pics[i])
			pics[i]->draw();
	}
}

/* Get picture */
picture *wcpicture_get(int i)
{
	if(i < 0 || i >= WCPICTURE_MAX)
	{
		wcrpg_error("Invalid picture number (%d)",i);
		return 0;
	}
	return pics[i];
}

/* Handle pictures */
void wcpicture_handle()
{
	int i;
	for(i = 0;i < WCPICTURE_MAX;i++)
	{
		if(pics[i])
			pics[i]->handle();
	}
}