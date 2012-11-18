/* Defines */
#define TFRAG_U 4
#define TFRAG_E7 12
#define TFRAG_E8 14
#define TFRAG_E9 16
#define TFRAG_E4 24
#define TFRAG_E5 26
#define TFRAG_E6 28
#define TFRAG_E1 36
#define TFRAG_E2 38
#define TFRAG_E3 40

#define OFRAG_A (0+2*wcmap_ocean_off)
#define OFRAG_I (12+2*wcmap_ocean_off)
#define OFRAG_U (24+2*wcmap_ocean_off)
#define OFRAG_E (36+2*wcmap_ocean_off)
#define OFRAG_N -1

/*
	I apologize for the programmer's madness and nausea you are about to experience...
	1780 PPS in the test
*/

/* Draws a piece */
#define WCMAP_PIECE(w,h,n,xx,yy)    if((n) >= 0) \
									{ \
										ox = (n)%6; \
										oy = (n)/6; \
										tex->draw(x+(xx),y+(yy),(w),(h),sx+ox*mag8+(xx),sy+oy*mag8+(yy)); \
									}

/* Draws a tile fragment */
void map :: draw_tile_fragment(int x,int y,int sx,int sy,int a,int b,int c,int d)
{
	int ox,oy;
	int mag16,mag8;
	mag16 = 16*rmag;
	mag8 = 8*rmag;
	/* Merge together */
	if(a == b && b == c && c == d)
	{
		WCMAP_PIECE(mag16,mag16,a,0,0);
		return;
	}
	/* Two top */
	if(a == b && c == d)
	{
		WCMAP_PIECE(mag16,mag8,a,0,0);
		WCMAP_PIECE(mag16,mag8,c,0,8);
		return;
	}
	/* Two sides */
	if(a == c && b == d)
	{
		WCMAP_PIECE(mag8,mag16,a,0,0);
		WCMAP_PIECE(mag8,mag16,b,8,0);
		return;
	}
	/* Draw fragmented */
	WCMAP_PIECE(mag8,mag8,a,0,0);
	WCMAP_PIECE(mag8,mag8,b,8,0);
	WCMAP_PIECE(mag8,mag8,c,0,8);
	WCMAP_PIECE(mag8,mag8,d,8,8);
}

/* Draws an auto tile fragment */
void map :: draw_tile_auto(int x,int y,int sx,int sy,int code)
{
	/* Column 1 */
	if(code == 0x00)
		draw_tile_fragment(x,y,sx,sy,TFRAG_E5,TFRAG_E5,TFRAG_E5,TFRAG_E5);
	if(code == 0x01)
		draw_tile_fragment(x,y,sx,sy,TFRAG_U,TFRAG_E5,TFRAG_E5,TFRAG_E5);
	if(code == 0x02)
		draw_tile_fragment(x,y,sx,sy,TFRAG_E5,TFRAG_U,TFRAG_E5,TFRAG_E5);
	if(code == 0x03)
		draw_tile_fragment(x,y,sx,sy,TFRAG_U,TFRAG_U,TFRAG_E5,TFRAG_E5);
	if(code == 0x04)
		draw_tile_fragment(x,y,sx,sy,TFRAG_E5,TFRAG_E5,TFRAG_E5,TFRAG_U);
	if(code == 0x05)
		draw_tile_fragment(x,y,sx,sy,TFRAG_U,TFRAG_E5,TFRAG_E5,TFRAG_U);
	if(code == 0x06)
		draw_tile_fragment(x,y,sx,sy,TFRAG_E5,TFRAG_U,TFRAG_E5,TFRAG_U);
	if(code == 0x07)
		draw_tile_fragment(x,y,sx,sy,TFRAG_U,TFRAG_U,TFRAG_E5,TFRAG_U);
	if(code == 0x08)
		draw_tile_fragment(x,y,sx,sy,TFRAG_E5,TFRAG_E5,TFRAG_U,TFRAG_E5);
	if(code == 0x09)
		draw_tile_fragment(x,y,sx,sy,TFRAG_U,TFRAG_E5,TFRAG_U,TFRAG_E5);
	if(code == 0x0A)
		draw_tile_fragment(x,y,sx,sy,TFRAG_E5,TFRAG_U,TFRAG_U,TFRAG_E5);
	if(code == 0x0B)
		draw_tile_fragment(x,y,sx,sy,TFRAG_U,TFRAG_U,TFRAG_U,TFRAG_E5);
	if(code == 0x0C)
		draw_tile_fragment(x,y,sx,sy,TFRAG_E5,TFRAG_E5,TFRAG_U,TFRAG_U);
	if(code == 0x0D)
		draw_tile_fragment(x,y,sx,sy,TFRAG_U,TFRAG_E5,TFRAG_U,TFRAG_U);
	if(code == 0x0E)
		draw_tile_fragment(x,y,sx,sy,TFRAG_E5,TFRAG_U,TFRAG_U,TFRAG_U);
	if(code == 0x0F)
		draw_tile_fragment(x,y,sx,sy,TFRAG_U,TFRAG_U,TFRAG_U,TFRAG_U);
	/* Column 2 */
	if(code == 0x10)
		draw_tile_fragment(x,y,sx,sy,TFRAG_E4,TFRAG_E5,TFRAG_E4,TFRAG_E5);
	if(code == 0x11)
		draw_tile_fragment(x,y,sx,sy,TFRAG_E4,TFRAG_U,TFRAG_E4,TFRAG_E5);
	if(code == 0x12)
		draw_tile_fragment(x,y,sx,sy,TFRAG_E4,TFRAG_E5,TFRAG_E4,TFRAG_U);
	if(code == 0x13)
		draw_tile_fragment(x,y,sx,sy,TFRAG_E4,TFRAG_U,TFRAG_E4,TFRAG_U);
	if(code == 0x14)
		draw_tile_fragment(x,y,sx,sy,TFRAG_E8,TFRAG_E8,TFRAG_E8,TFRAG_E8);
	if(code == 0x15)
		draw_tile_fragment(x,y,sx,sy,TFRAG_E8,TFRAG_E8,TFRAG_E5,TFRAG_U);
	if(code == 0x16)
		draw_tile_fragment(x,y,sx,sy,TFRAG_E8,TFRAG_E8,TFRAG_U,TFRAG_E5);
	if(code == 0x17)
		draw_tile_fragment(x,y,sx,sy,TFRAG_E8,TFRAG_E8,TFRAG_U,TFRAG_U);
	if(code == 0x18)
		draw_tile_fragment(x,y,sx,sy,TFRAG_E5,TFRAG_E6,TFRAG_E5,TFRAG_E6);
	if(code == 0x19)
		draw_tile_fragment(x,y,sx,sy,TFRAG_E5,TFRAG_E6,TFRAG_U,TFRAG_E6);
	if(code == 0x1A)
		draw_tile_fragment(x,y,sx,sy,TFRAG_U,TFRAG_E6,TFRAG_E5,TFRAG_E6);
	if(code == 0x1B)
		draw_tile_fragment(x,y,sx,sy,TFRAG_U,TFRAG_E6,TFRAG_U,TFRAG_E6);
	if(code == 0x1C)
		draw_tile_fragment(x,y,sx,sy,TFRAG_E2,TFRAG_E2,TFRAG_E2,TFRAG_E2);
	if(code == 0x1D)
		draw_tile_fragment(x,y,sx,sy,TFRAG_U,TFRAG_E2,TFRAG_E2,TFRAG_E2);
	if(code == 0x1E)
		draw_tile_fragment(x,y,sx,sy,TFRAG_E2,TFRAG_U,TFRAG_E2,TFRAG_E2);
	if(code == 0x1F)
		draw_tile_fragment(x,y,sx,sy,TFRAG_U,TFRAG_U,TFRAG_E2,TFRAG_E2);
	/* Column 3 */
	if(code == 0x20)
		draw_tile_fragment(x,y,sx,sy,TFRAG_E4,TFRAG_E6,TFRAG_E4,TFRAG_E6);
	if(code == 0x21)
		draw_tile_fragment(x,y,sx,sy,TFRAG_E8,TFRAG_E8,TFRAG_E2,TFRAG_E2);
	if(code == 0x22)
		draw_tile_fragment(x,y,sx,sy,TFRAG_E7,TFRAG_E7,TFRAG_E7,TFRAG_E7);
	if(code == 0x23)
		draw_tile_fragment(x,y,sx,sy,TFRAG_E7,TFRAG_E7,TFRAG_E7,TFRAG_U);
	if(code == 0x24)
		draw_tile_fragment(x,y,sx,sy,TFRAG_E9,TFRAG_E9,TFRAG_E9,TFRAG_E9);
	if(code == 0x25)
		draw_tile_fragment(x,y,sx,sy,TFRAG_E9,TFRAG_E9,TFRAG_U,TFRAG_E9);
	if(code == 0x26)
		draw_tile_fragment(x,y,sx,sy,TFRAG_E3,TFRAG_E3,TFRAG_E3,TFRAG_E3);
	if(code == 0x27)
		draw_tile_fragment(x,y,sx,sy,TFRAG_U,TFRAG_E3,TFRAG_E3,TFRAG_E3);
	if(code == 0x28)
		draw_tile_fragment(x,y,sx,sy,TFRAG_E1,TFRAG_E1,TFRAG_E1,TFRAG_E1);
	if(code == 0x29)
		draw_tile_fragment(x,y,sx,sy,TFRAG_E1,TFRAG_U,TFRAG_E1,TFRAG_E1);
	if(code == 0x2A)
		draw_tile_fragment(x,y,sx,sy,TFRAG_E7,TFRAG_E9,TFRAG_E7,TFRAG_E9);
	if(code == 0x2B)
		draw_tile_fragment(x,y,sx,sy,TFRAG_E7,TFRAG_E8,TFRAG_E1,TFRAG_E2);
	if(code == 0x2C)
		draw_tile_fragment(x,y,sx,sy,TFRAG_E1,TFRAG_E3,TFRAG_E1,TFRAG_E3);
	if(code == 0x2D)
		draw_tile_fragment(x,y,sx,sy,TFRAG_E9,TFRAG_E9,TFRAG_E3,TFRAG_E3);
	if(code == 0x2E)
		draw_tile_fragment(x,y,sx,sy,TFRAG_E7,TFRAG_E9,TFRAG_E1,TFRAG_E3);
}

/* Draws a beach tile */
void map :: draw_tile_beach(int x,int y,int sx,int sy,int code)
{
	/* Column 0 */
	if(code/0x10 == 0)
	{
		if(code%2)
			draw_tile_fragment(x,y,sx,sy,OFRAG_E,OFRAG_N,OFRAG_N,OFRAG_N);
		if((code/2)%2)
			draw_tile_fragment(x,y,sx,sy,OFRAG_N,OFRAG_E,OFRAG_N,OFRAG_N);
		if(code >= 0x08)
			draw_tile_fragment(x,y,sx,sy,OFRAG_N,OFRAG_N,OFRAG_E,OFRAG_N);
		if((code/4)%2)
			draw_tile_fragment(x,y,sx,sy,OFRAG_N,OFRAG_N,OFRAG_N,OFRAG_E);
	}
	/* Column 1 */
	if(code/0x10 == 1)
	{
		/* L/R unders */
		if(code == 0x11 || code == 0x1A)
			draw_tile_fragment(x,y,sx,sy,OFRAG_E,OFRAG_E,OFRAG_N,OFRAG_N);
		if(code == 0x12 || code == 0x19)
			draw_tile_fragment(x,y,sx,sy,OFRAG_N,OFRAG_N,OFRAG_E,OFRAG_E);
		if(code == 0x16 || code == 0x1D)
			draw_tile_fragment(x,y,sx,sy,OFRAG_E,OFRAG_N,OFRAG_E,OFRAG_N);
		if(code == 0x15 || code == 0x1E)
			draw_tile_fragment(x,y,sx,sy,OFRAG_N,OFRAG_E,OFRAG_N,OFRAG_E);
		if(code == 0x13 || code == 0x17 || code == 0x1B || code == 0x1F)
			draw_tile_fragment(x,y,sx,sy,OFRAG_E,OFRAG_E,OFRAG_E,OFRAG_E);
		/* T/D sides */
		if(code >= 0x10 && code < 0x14)
			draw_tile_fragment(x,y,sx,sy,OFRAG_I,OFRAG_N,OFRAG_I,OFRAG_N);
		if(code >= 0x14 && code < 0x18)
			draw_tile_fragment(x,y,sx,sy,OFRAG_U,OFRAG_U,OFRAG_N,OFRAG_N);
		if(code >= 0x18 && code < 0x1C)
			draw_tile_fragment(x,y,sx,sy,OFRAG_N,OFRAG_I,OFRAG_N,OFRAG_I);
		if(code >= 0x1C && code < 0x20)
			draw_tile_fragment(x,y,sx,sy,OFRAG_N,OFRAG_N,OFRAG_U,OFRAG_U);
	}
	/* Column 2 */
	if(code == 0x20)
		draw_tile_fragment(x,y,sx,sy,OFRAG_I,OFRAG_I,OFRAG_I,OFRAG_I);
	if(code == 0x21)
		draw_tile_fragment(x,y,sx,sy,OFRAG_U,OFRAG_U,OFRAG_U,OFRAG_U);
	if(code == 0x22)
		draw_tile_fragment(x,y,sx,sy,OFRAG_A,OFRAG_U,OFRAG_I,OFRAG_N);
	if(code == 0x23)
		draw_tile_fragment(x,y,sx,sy,OFRAG_A,OFRAG_U,OFRAG_I,OFRAG_E);
	if(code == 0x24)
		draw_tile_fragment(x,y,sx,sy,OFRAG_U,OFRAG_A,OFRAG_N,OFRAG_I);
	if(code == 0x25)
		draw_tile_fragment(x,y,sx,sy,OFRAG_U,OFRAG_A,OFRAG_E,OFRAG_I);
	if(code == 0x26)
		draw_tile_fragment(x,y,sx,sy,OFRAG_N,OFRAG_I,OFRAG_U,OFRAG_A);
	if(code == 0x27)
		draw_tile_fragment(x,y,sx,sy,OFRAG_E,OFRAG_I,OFRAG_U,OFRAG_A);
	if(code == 0x28)
		draw_tile_fragment(x,y,sx,sy,OFRAG_I,OFRAG_N,OFRAG_A,OFRAG_U);
	if(code == 0x29)
		draw_tile_fragment(x,y,sx,sy,OFRAG_I,OFRAG_E,OFRAG_A,OFRAG_U);
	if(code == 0x2A)
		draw_tile_fragment(x,y,sx,sy,OFRAG_A,OFRAG_A,OFRAG_I,OFRAG_I);
	if(code == 0x2B)
		draw_tile_fragment(x,y,sx,sy,OFRAG_A,OFRAG_U,OFRAG_A,OFRAG_U);
	if(code == 0x2C)
		draw_tile_fragment(x,y,sx,sy,OFRAG_I,OFRAG_I,OFRAG_A,OFRAG_A);
	if(code == 0x2D)
		draw_tile_fragment(x,y,sx,sy,OFRAG_U,OFRAG_A,OFRAG_U,OFRAG_A);
	if(code == 0x2E)
		draw_tile_fragment(x,y,sx,sy,OFRAG_A,OFRAG_A,OFRAG_A,OFRAG_A);
}

/* Draws an oceanic level tile */
void map :: draw_tile_ocean(int x,int y,int sx,int sy,int code,int deep)
{
	/* Not deep */
	if(!deep)
	{
		draw_tile_fragment(x,y,sx,sy,OFRAG_A,OFRAG_A,OFRAG_A,OFRAG_A);
		if(code%2)
			draw_tile_fragment(x,y,sx,sy,OFRAG_I,OFRAG_N,OFRAG_N,OFRAG_N);
		if((code/2)%2)
			draw_tile_fragment(x,y,sx,sy,OFRAG_N,OFRAG_I,OFRAG_N,OFRAG_N);
		if((code/4)%2)
			draw_tile_fragment(x,y,sx,sy,OFRAG_N,OFRAG_N,OFRAG_I,OFRAG_N);
		if(code >= 0x08)
			draw_tile_fragment(x,y,sx,sy,OFRAG_N,OFRAG_N,OFRAG_N,OFRAG_I);
	}
	else
	{
		draw_tile_fragment(x,y,sx,sy,OFRAG_E,OFRAG_E,OFRAG_E,OFRAG_E);
		if(code%2)
			draw_tile_fragment(x,y,sx,sy,OFRAG_U,OFRAG_N,OFRAG_N,OFRAG_N);
		if((code/2)%2)
			draw_tile_fragment(x,y,sx,sy,OFRAG_N,OFRAG_U,OFRAG_N,OFRAG_N);
		if((code/4)%2)
			draw_tile_fragment(x,y,sx,sy,OFRAG_N,OFRAG_N,OFRAG_U,OFRAG_N);
		if(code >= 0x08)
			draw_tile_fragment(x,y,sx,sy,OFRAG_N,OFRAG_N,OFRAG_N,OFRAG_U);
	}
}

/* Draws a tile */
void map :: draw_tile(tile t,int x,int y,int l)
{
	int tx,ty;
	int sx,sy,sz;
	int mag16;
	int shader_id;
	mag16 = 16*rmag;
	/* Get shader ID */
	shader_id = wctileset_get_shader(t);
	/* Lower tile */
	if(t >= 5000 && t < 10000)
	{
		if(l != WCMAP_BELOW)
			return;
		/* Hijack the tile */
		t = wctileset_sub_tile(t);
		t -= 5000;
		tx = t%6+(t/96)*6;
		ty = (t/6)%16;
		/* Shade */
		wcdraw_set_shader(shader_id);
		tex->draw(x,y,mag16,mag16,192*rmag+tx*mag16,ty*mag16);
		wcdraw_set_shader(0);
		return;
	}
	/* Upper tile */
	if(t >= 10000)
	{
		/* Must match above-ness */
		if(tset->is_above(t) != l)
			return;
		/* Hijack the tile */
		t = wctileset_sub_tile(t);
		t -= 10000;
		t += 48; /* Brings the number to the start of upper chips on image */
		tx = t%6+(t/96)*6;
		ty = (t/6)%16;
		/* Shade */
		wcdraw_set_shader(shader_id);
		tex->draw(x,y,mag16,mag16,288*rmag+tx*mag16,ty*mag16);
		wcdraw_set_shader(0);
		return;
	}
	/* Regular autotile */
	if(t >= 4000 && t < 5000)
	{
		/* Draw it */
		t -= 4000;
		sx = t/50;
		t -= sx*50;
		sy = t;
		sx += 4; /* Skip over waters */
		tx = sx%2+sx/8*2;
		ty = (sx/2)%4;
		tx *= 3;
		ty *= 4;
		/* Not in above */
		if(!tset->is_square(sx-4+6) && l == WCMAP_ABOVE)
			return;
		/* Not in below */
		if(tset->is_square(sx-4+6) && l == WCMAP_BELOW)
			return;
		/* Shade */
		wcdraw_set_shader(shader_id);
		draw_tile_auto(x,y,tx*mag16,ty*mag16,sy);
		wcdraw_set_shader(0);
		return;
	}
	/* Animated tile */
	if(t >= 3000 && t < 4000)
	{
		if(l != WCMAP_BELOW)
			return;
		t -= 3000;
		t /= 50;
		/* Shade */
		wcdraw_set_shader(shader_id);
		tex->draw(x,y,mag16,mag16,48*rmag+mag16*t,64*rmag+wcmap_anim_off*mag16);
		wcdraw_set_shader(0);
		return;
	}
	/* Oceanic autotile */
	if(t < 3000)
	{
		if(l != WCMAP_BELOW)
			return;
		sx = t/1000;
		t -= sx*1000;
		sy = t/50;
		t -= sy*50;
		sz = t;
		tx = (sx%2)*3;
		ty = (sx/2)*4;
		/* Shade */
		wcdraw_set_shader(shader_id);
		draw_tile_ocean(x,y,0,64*rmag,sy,(sx == 2));
		draw_tile_beach(x,y,tx*mag16,0,sz);
		wcdraw_set_shader(0);
		return;
	}
}