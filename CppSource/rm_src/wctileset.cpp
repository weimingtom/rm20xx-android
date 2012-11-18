/*
	Tileset
*/

/* Includes */
#include <malloc.h>
#include <memory.h>
#include "wcmaterial.h"
#include "wctileset.h"
#include "wctype.h"
#include "wcrpg.h"

#include <string.h>

/* Globals */
tile wctileset_sub[TILESET_MAX];
tile wctileset_back_sub[TILESET_MAX];
int wctileset_shader[TILESET_MAX];

/* Get shader */
int wctileset_get_shader(tile t)
{
	/* Sub first */
	t = wctileset_sub_tile(t);
	/* Oceans */
	if(t < 3000)
	{
		t /= 1000;
		return wctileset_shader[t];
	}
	/* Animated */
	if(t >= 3000 && t < 4000)
	{
		t -= 3000;
		t /= 50;
		t += 3;
		return wctileset_shader[t];
	}
	/* Autotiles */
	if(t >= 4000 && t < 5000)
	{
		t -= 4000;
		t /= 50;
		t += 6;
		return wctileset_shader[t];
	}
	/* Lower tiles */
	if(t >= 5000 && t < 10000)
	{
		t -= 5000;
		t += 18;
		return wctileset_shader[t];
	}
	/* Upper tiles */
	if(t >= 10000)
	{
		t -= 10000;
		t += 18+TILESET_HALF;
		return wctileset_shader[t];
	}
	/* I don't know */
	return 0;
}

/* Set shaders */
void wctileset_set_shader(int lt,int sid)
{
	wctileset_shader[lt] = sid;
}

/* Reset shaders */
void wctileset_reset_shader()
{
	memset(wctileset_shader,0,sizeof(wctileset_shader));
}

/* Replace tile */
void wctileset_set_sub(tile from,tile to)
{
	from = wctileset_back_sub[from]; /* Get back sub. A sub of 40-46 means putting 46 should return 40 */
	wctileset_sub[from] = to; /* Assign the sub */
	wctileset_back_sub[to] = from; /* Backsub, so 46 = 40 */
}

/* Get final tile */
tile wctileset_sub_tile(tile t)
{
	/* Only lower and upper tiles get subbed */
	if(t >= 5000 && t < 10000)
	{
		t -= 5000;
		t = wctileset_sub[t+18];
		t += 5000;
	}
	if(t >= 10000)
	{
		t -= 10000;
		t = wctileset_sub[t+18+TILESET_HALF];
		t += 10000;
	}
	/* Return result */
	return t;
}

/* Reset subs */
void wctileset_reset_sub()
{
	tile i;
	for(i = 0;i < 18;i++)
		wctileset_sub[i] = i;
	for(i = 18;i < 18+TILESET_HALF;i++)
		wctileset_sub[i] = i-18;
	for(i = 18+TILESET_HALF;i < 18+TILESET_HALF+TILESET_HALF;i++)
		wctileset_sub[i] = i-(18+TILESET_HALF);
	for(i = 0;i < TILESET_MAX;i++)
		wctileset_back_sub[i] = wctileset_sub[i];
}

/* Read em all */
arraylist wctileset_load_all(FILE *f,int len)
{
	tileset *ts;
	int count,i,rlen;
	arraylist ls;
	/* Reset subs */
	wctileset_reset_sub();
	/* Reset shaders */
	wctileset_reset_shader();
	/* Find out how many there are */
	count = wcint_read_file(f,&rlen);
	len -= rlen;
	wclog_write(wcrpg_get_log(),"Loading ");
	wclog_write(wcrpg_get_log(),count);
	wclog_write(wcrpg_get_log()," tilesets...");
	wclog_enter(wcrpg_get_log());
	/* Make the list */
	ls = wcarray_new();
	/* Create and add */
	for(i = 0;i < count;i++)
	{
		/* Read */
		ts = new tileset(f);
		len -= ts->get_size();
		/* Report */
		ts->report(wcrpg_get_log());
		/* Add */
		wcarray_add(ls,ts);
	}
	/* List off garbage data */
	if(len != 0)
		wcrpg_error("Garbage data found in tileset specification data");
	/* Return that list */
	return ls;
}

/* Remove em all */
void wctileset_free_all(arraylist ls)
{
	int i;
	for(i = 0;i < wcarray_size(ls);i++)
		delete ((tileset*)wcarray_get(ls,i));
	wcarray_delete(ls);
}

/* Create */
tileset :: tileset(FILE *f)
{
	unsigned char *temp;
	int code,len,rlen;
	/* Init */
	name = 0;
	source = 0;
	terr = 0;
	pass = 0;
	upass = 0;
	size = 0;
	/* ID */
	id = wcint_read_file(f,&rlen);
	size += rlen;
	/* Read chunks */
	for(;;)
	{
		/* Get code */
		code = 0;
		fread(&code,1,1,f);
		size++;
		if(!code)
			return; /* Done */
		/* Pick out data I know */
		if(code == TILESET_NAME)
		{
			/* Name of the tileset */
			name = wcstr_read_file(f,&rlen);
			size += rlen;
		}
		else if(code == TILESET_SOURCE)
		{
			/* Source of the tileset */
			source = wcstr_read_file(f,&rlen);
			size += rlen;
		}
		else if(code == TILESET_TERRAIN)
		{
			/* Terrain of the tileset */
			len = wcint_read_file(f,&rlen);
			terr = (unsigned short*)malloc(len);
			fread(terr,1,len,f);
			size += rlen+len;
		}
		else if(code == TILESET_PASS)
		{
			/* Passabillity of the tileset */
			len = wcint_read_file(f,&rlen);
			pass = (unsigned char*)malloc(len);
			fread(pass,1,len,f);
			size += rlen+len;
		}
		else if(code == TILESET_UPPER_PASS)
		{
			/* .. I think this some extra pass information */
			len = wcint_read_file(f,&rlen);
			upass = (unsigned char*)malloc(len);
			fread(upass,1,len,f);
			size += rlen+len;
		}
		else
		{
			/* Some kind of data I don't know */
			len = wcint_read_file(f,&rlen);
			temp = (unsigned char*)malloc(len);
			fread(temp,1,len,f);
			free(temp);
			size += rlen+len;
		}
	}
}

/* Delete */
tileset :: ~tileset()
{
	free(pass);
	free(upass);
	free(terr);
	free(source);
	free(name);
}

/* Report */
void tileset :: report(elog l)
{
	wclog_write(l,"#");
	wclog_write(l,id);
	wclog_write(l," ");
	if(name)
		wclog_write(l,name);
	else
		wclog_write(l,"(null)");
	wclog_write(l," : <- ");
	if(source)
		wclog_write(l,source);
	else
		wclog_write(l,"(null)");
	wclog_enter(l);
}

/* Load texture */
texture *tileset :: get_texture()
{
	texture *ntex;
	ntex = new texture(wcmaterial_request(MATERIAL_CHIPSET,source));
	ntex->enable_flash();
	ntex->reduce();
	return ntex;
}

/* Get read byte size */
int tileset :: get_size()
{
	return size;
}

/* Determine above */
int tileset :: is_above(tile t)
{
	/* All below */
	if(!upass)
		return 0;
	/* Upper layer stars */
	if(t >= 10000)
	{
		t -= 10000;
		/* Is above */
		if(upass[t]&TILESET_STAR)
			return 1;
	}
	/* Not above */
	return 0;
}

/* Determine if auto tile is square */
int tileset :: is_square(int t)
{
	if(!pass)
		return 0;
	if(pass[t]&TILESET_SQUARE)
		return 1;
	return 0;
}

/* Get code */
int tileset :: get_code(int i,int low)
{
	if(low)
	{
		if(i < 0 || i >= 162)
		{
			wcrpg_error("Tile code request out of range (%d)",i);
			return 0;
		}
		if(!pass)
			return 0x0F;
		return pass[i];
	}
	if(i < 0 || i >= 144)
	{
		wcrpg_error("Tile code request out of range (%d)",i);
		return 0;
	}
	if(!upass)
		return 0x0F;
	return upass[i];
}

/* Get terrain code */
int tileset :: get_terrain_code(tile lo)
{
	int lg;
	/* Transform tile into logical */
	if(lo < 3000)
	{
		lo /= 1000;
	}
	else if(lo >= 3000 && lo < 4000)
	{
		lo -= 3000;
		lo /= 50;
		lo += 3;
	}
	else if(lo >= 4000 && lo < 5000)
	{
		lo -= 4000;
		lo /= 50;
		lo += 6;
	}
	else if(lo >= 5000)
	{
		lo -= 5000;
		lo += 18;
	}
	/* Convert */
	lg = lo;
	/* Then transform logical tile to terrain */
	if(lg < 0 || lg >= 162)
	{
		wcrpg_error("Tile terrain request out of range (%d)",lg);
		return 0;
	}
	if(!terr)
		return 1; /* Always 1 if no data */
	return terr[lg];
}

/* Get code */
int tileset :: get_collision_code(tile lo,tile hi)
{
	int loc,hic,loa,sic,sop;
	/* Replace both lo and hi */
	lo = wctileset_sub_tile(lo);
	hi = wctileset_sub_tile(hi);
	/* Translate into logical tiles */
	sop = 0;
	hi -= 10000;
	if(lo < 3000)
	{
		lo /= 1000;
	}
	else if(lo >= 3000 && lo < 4000)
	{
		lo -= 3000;
		lo /= 50;
		lo += 3;
	}
	else if(lo >= 4000 && lo < 5000)
	{
		/* Make a copy of logical tile because... */
		loa = lo;
		loa -= 4000;
		loa /= 50;
		loa += 6;
		/* Check for square mode */
		sic = get_code((int)loa,1);
		if(sic&TILESET_SQUARE)
		{
			/* Figure out if to put a passable chunk on top */
			loa = (lo-4000)/50;
			loa = (lo-4000)-loa*50;
			/* Override? */
			if(loa >= 0x14 && loa < 0x17)
				sop = 1;
			if(loa >= 0x21 && loa < 0x25)
				sop = 1;
			if(loa >= 0x2A && loa <= 0x2B)
				sop = 1;
			if(loa >= 0x2D && loa < 0x2E)
				sop = 1;
			/* Set */
			lo -= 4000;
			lo /= 50;
			lo += 6;
		}
		else
		{
			/* Normal op */
			lo = loa;
		}
	}
	else if(lo >= 5000)
	{
		lo -= 5000;
		lo += 18;
	}
	/* Get codes */
	loc = get_code((int)lo,1);
	hic = get_code((int)hi,0);
	/* Square override */
	if(sop)
		loc = 0x0F;
	/* Defer to priority code */
	if(hi)
	{
		if(!(hic&TILESET_STAR))
			return (hic&0x0F);
	}
	/* Use lower */
	return (loc&0x0F);
}