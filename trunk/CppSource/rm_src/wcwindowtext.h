/* Create text */
textstring :: textstring(char *src)
{
	/* Init */
	source = wcrpg_string_clone(src);
	pos_x = 0;
	pos_y = 0;
	col = 0;
	/* Alloc */
	current = (char*)malloc(strlen(source)+1);
	memset(current,0,strlen(source)+1);
	/* Then compile */
	advance(TEXT_ALL);
}

/* Destroy text */
textstring :: ~textstring()
{
	/* Release */
	free(source);
	free(current);
}

/* Move text */
void textstring :: move(int x,int y)
{
	/* Move */
	pos_x = x;
	pos_y = y;
}

/* Advance text */
void textstring :: advance(int p)
{
	/* Copy it all */
	if(p == TEXT_ALL || p >= (int)strlen(source))
	{
		memcpy(current,source,strlen(source));
		return;
	}
	/* Copy some */
	if(p > 0 && p < (int)strlen(source))
	{
		memcpy(current,source,p);
		current[p] = 0;
		return;
	}
	/* Cap off so it's blank after p */
	if(p >= (int)strlen(source))
		p = (int)strlen(source);
	current[p] = 0;
}

/* Set color */
void textstring :: set_color(int c)
{
	/* Set color */
	col = c;
}

/* Draw text */
void textstring :: draw()
{
	/* Draw */
	wcwindow_draw_text(pos_x,pos_y,col,current);
}

/* Returns the width of this text */
int textstring :: width()
{
	return wcwindow_font_character_width()*(int)strlen(source);
}

/* Returns the height of this text */
int textstring :: height()
{
	return wcwindow_font_character_height();
}

/* Returns x location */
int textstring :: get_x()
{
	return pos_x;
}

/* Returns y location */
int textstring :: get_y()
{
	return pos_y;
}

/* Get a length */
int textstring :: length()
{
	return (int)strlen(source);
}

/* Copies a substring of a text string */
char *wcwindow_copy_clip(char *src,int f,int t)
{
	char *cs;
	/* Allocate */
	cs = (char*)malloc(t-f+1);
	memset(cs,0,t-f+1);
	/* Copy */
	memcpy(cs,&src[f],t-f);
	/* Cap off */
	cs[t-f] = 0;
	return cs;
}

/* Returns the length of the control sequence or zero if there was none */
int wcwindow_is_control_sequence(char *src)
{
	int len,i,level;
	/* It's not possible */
	if(*src != '$' && *src != '\\' && *src != '\n')
		return 0;
	/* Dollar sign? */
	if(*src == '$')
		return 2; /* $ followed by some character */
	/* Return */
	if(*src == '\n')
		return 1;
	/* Slash */
	len = 2; /* Must at least have a \ followed by some character */
	/* Is there an extension? */
	if(src[2] == '[')
	{
		/* Set level */
		level = 1;
		/* Go until level is zeroed */
		i = 3;
		/* Add one to length */
		len++;
		while(level > 0)
		{
			/* Increase level */
			if(src[i] == '[')
				level++;
			/* Decrease level */
			if(src[i] == ']')
				level--;
			/* Add one to length */
			i++;
			len++;
		}
	}
	/* Return length */
	return len;
}

/* Divides this text into many texts seperated by codes */
arraylist wcwindow_divide_text(char *src)
{
	arraylist sls;
	int tail,head,end;
	/* Allocate */
	sls = wcarray_new();
	wcarray_set_clean(sls,free);
	/* Position */
	tail = 0;
	head = 0;
	end = (int)strlen(src);
	/* Run until the tail reaches the end */
	while(tail < end)
	{
		/* Find the control character */
		while(!wcwindow_is_control_sequence(&src[head]) && src[head])
			head++;
		/* Add a copy of this substring to the list */
		wcarray_add(sls,wcwindow_copy_clip(src,tail,head));
		/* Bring the tail up */
		tail = head;
		/* Advance the head by the size of control character */
		head += wcwindow_is_control_sequence(&src[tail]);
		/* Now copy this to the list */
		wcarray_add(sls,wcwindow_copy_clip(src,tail,head));
		/* Bring the tail up again */
		tail = head;
	}
	/* Then, return the result */
	return sls;
}

/* Prints the list of strings and control marks to the given stream */
void wcwindow_debug_text(FILE *f,char *src)
{
	int i;
	arraylist sls;
	/* Make a divide */
	sls = wcwindow_divide_text(src);
	/* Print them */
	for(i = 0;i < wcarray_size(sls);i += 2)
		fprintf(f,"%s\n> (%s)\n",(char*)wcarray_get(sls,i),(char*)wcarray_get(sls,i+1));
	/* Clean */
	wcarray_delete(sls);
}

/* Gets the color meaning of this control sequence */
int wcwindow_get_color_control(char *str)
{
	/* Is this even a color change command? */
	if(str[1] != 'c')
		return -1; /* No change */
	/* Find the color to change */
	return atoi(&str[3]);
}

/* Gets the return meaning of this control sequence */
int wcwindow_get_return_control(char *str)
{
	/* Is this r? */
	if(str[1] != 'r')
		return -1; /* No change */
	/* Yes, change */
	return atoi(&str[3]);
}

/* Gets the speed meaning of this control sequence */
int wcwindow_get_speed_control(char *str)
{
	/* Is this even a speed change command? */
	if(str[1] != 's')
		return -1; /* No change */
	/* Find the speed to change */
	return atoi(&str[3]);
}

/* Gets the ID meaning of this control sequence for displaying hero name */
int wcwindow_get_hero_control(char *str)
{
	/* Is this even a hero name command? */
	if(str[1] != 'n')
		return -1; /* No change */
	/* Find the speed to change */
	return atoi(&str[3]);
}

/* Returns true if this string begins with a valid variable command */
int wcwindow_contains_variable(char *str)
{
	int i;
	/* Those first three characters */
	if(str[0] != '\\')
		return 0;
	if(str[1] != 'v')
		return 0;
	if(str[2] != '[')
		return 0;
	/* Must have number inside */
	if(!(str[3] >= '0' && str[3] <= '9'))
		return 0;
	/* From str[4] and on, find ] */
	for(i = 4;i < (int)strlen(str);i++)
	{
		if(str[i] == ']')
			return 1;
	}
	/* Not a variable command */
	return 0;
}

/* Counts the number of variable commands left in this string */
int wcwindow_count_variable(char *str)
{
	int i,c;
	c = 0;
	/* Count them */
	for(i = 0;i < (int)strlen(str);i++)
	{
		/* Has command here? */
		if(wcwindow_contains_variable(&str[i]))
			c++;
	}
	return c;
}

/* Returns the pointer of the first variable command, or null if none are left */
char *wcwindow_advance_variable(char *str)
{
	int i;
	/* None left */
	if(!wcwindow_count_variable(str))
		return 0;
	/* Seek */
	for(i = 0;i < (int)strlen(str);i++)
	{
		/* Has command there? */
		if(wcwindow_contains_variable(&str[i]))
			return &str[i]; /* Then return this */
	}
	/* Not found? */
	return 0;
}

/* Returns the number inside a command, not the command parameter, the NUMBER from variables */
int wcwindow_resolve_variable(char *str)
{
	int ref;
	/* Get ref */
	ref = atoi(&str[3]);
	/* Get var */
	return wcmem_get_variable(ref);
}

/* Returns the distance to the next variable command */
int wcwindow_pos_variable(char *str)
{
	int i;
	/* Seek */
	for(i = 0;i < (int)strlen(str);i++)
	{
		if(wcwindow_contains_variable(&str[i]))
			return i;
	}
	/* Not found */
	return 0;
}

/* Processes the first variable command */
char *wcwindow_process_variable(char *str)
{
	char *cstr,*nstr;
	int slen,mlen,elen,nlen;
	/* Find the starting length */
	slen = wcwindow_pos_variable(str);
	/* Find the command length */
	mlen = wcwindow_is_control_sequence(&str[slen]);
	/* The final length is the rest */
	elen = (int)strlen(str)-mlen-slen;
	/* Resolve the number */
	nstr = (char*)malloc(64);
	sprintf(nstr,"%d",wcwindow_resolve_variable(&str[slen]));
	/* Find out how long IT is */
	nlen = (int)strlen(nstr);
	/* Now allocate final string */
	cstr = (char*)malloc(slen+nlen+elen+1);
	/* Copy the parts */
	memcpy(cstr,str,slen);
	memcpy(&cstr[slen],nstr,nlen);
	memcpy(&cstr[slen+nlen],&str[slen+mlen],elen);
	cstr[slen+nlen+elen] = 0;
	/* Erase number string */
	free(nstr);
	/* Return result */
	return cstr;
}

/* Processes all variables */
char *wcwindow_process_all_variables(char *str)
{
	char *cstr,*sstr;
	/* Clone */
	sstr = wcrpg_string_clone(str);
	/* Keep going */
	cstr = str;
	for(;;)
	{
		/* Complete */
		if(wcwindow_count_variable(sstr) <= 0)
			return sstr;
		/* Process */
		cstr = wcwindow_process_variable(sstr);
		/* Erase old and make anew */
		free(sstr);
		sstr = cstr;
	}
	/* Error? */
	return 0;
}

/* Creates a new text object */
text :: text(char *src)
{
	hero *hr;
	textstring *tx;
	icon *ic;
	arraylist sls;
	int cx,cy,i,ix,p;
	int color;
	char *ctrlstr;
	/* Hijack and fix null strings */
	if(!src)
		src = "";
	/* Reset auto close */
	auto_close_pos = -1;
	/* First, resolve variables */
	src = wcwindow_process_all_variables(src);
	/* Make lists */
	txs = wcarray_new();
	ics = wcarray_new();
	ccs = wcarray_new();
	wcarray_set_clean(ccs,free);
	/* Analyze the text */
	sls = wcwindow_divide_text(src);
	/* Position the cursor */
	p = 0;
	cx = 0;
	cy = 0;
	/* Initialize state */
	color = 0;
	/* Begin constructing the text back as it should be in the final result */
	for(i = 0;i < wcarray_size(sls);i += 2)
	{
		/* Create the textstring */
		tx = new textstring((char*)wcarray_get(sls,i));
		/* Get control string */
		ctrlstr = wcrpg_string_clone((char*)wcarray_get(sls,i+1));
		wcarray_add(ccs,ctrlstr);
		/* Move it */
		tx->move(cx,cy);
		/* Color it */
		tx->set_color(color);
		/* Change color? */
		if(wcwindow_get_color_control(ctrlstr) != -1)
			color = wcwindow_get_color_control(ctrlstr);
		/* Add it */
		wcarray_add(txs,tx);
		p += tx->length();
		/* Advance */
		if(*ctrlstr == '\n')
		{
			cx = 0;
			cy += tx->height();
		}
		else
		{
			cx += tx->width();
		}
		/* Add an icon? */
		if(*ctrlstr == '$')
		{
			/* Get index from letter */
			ix = 0;
			if(ctrlstr[1] >= 'A' && ctrlstr[1] <= 'Z')
				ix = ctrlstr[1]-'A';
			if(ctrlstr[1] >= 'a' && ctrlstr[1] <= 'z')
				ix = ctrlstr[1]-'a'+26;
			/* Make icon */
			ic = new icon(ix);
			ic->move(cx,cy+2);
			/* Add it */
			wcarray_add(ics,ic);
			/* Advance */
			cx += 16;
			p++;
		}
		else
		{
			/* Add null spacer */
			wcarray_add(ics,0);
		}
		/* Just a slash */
		if(ctrlstr[1] == '\\')
		{
			tx = new textstring("\\");
			tx->move(cx,cy);
			tx->set_color(color);
			cx += tx->width();
			wcarray_add(txs,tx);
			wcarray_add(ics,0);
		}
		/* Return */
		if(wcwindow_get_return_control(ctrlstr) != -1)
		{
			cx = 0;
			cy += tx->height()*wcwindow_get_return_control(ctrlstr);
		}
		/* Hero command */
		if(wcwindow_get_hero_control(ctrlstr) != -1)
		{
			if(wcwindow_get_hero_control(ctrlstr) == 0)
				hr = wcparty_leader();
			else
				hr = wchero_get(wcwindow_get_hero_control(ctrlstr));
			tx = new textstring(hr->get_name());
			tx->move(cx,cy);
			tx->set_color(color);
			cx += tx->width();
			wcarray_add(txs,tx);
			wcarray_add(ics,0);
			wcarray_add(ccs,0);
			p += tx->length();
		}
		/* Half a space */
		if(ctrlstr[1] == '_')
		{
			/* Nudge it out a lil */
			cx += 2;
			p++;
		}
		/* Auto close */
		if(ctrlstr[1] == '^')
		{
			auto_close_pos = p;
		}
	}
	/* Clean out the list */
	wcarray_delete(sls);
	free(src);
	/* Make visible */
	advance(TEXT_ALL,0);
}

/* Deletes a text object */
text :: ~text()
{
	int i;
	/* Delete text strings */
	for(i = 0;i < wcarray_size(txs);i++)
		delete ((textstring*)wcarray_get(txs,i));
	for(i = 0;i < wcarray_size(ics);i++)
	{
		if(wcarray_get(ics,i))
			delete ((icon*)wcarray_get(ics,i));
	}
	/* Delete */
	wcarray_delete(ics);
	wcarray_delete(txs);
	wcarray_delete(ccs);
}

/* Moves a text object */
void text :: move(int x,int y)
{
	textstring *tx;
	icon *ic;
	int dx,dy,i;
	/* No anchor? */
	if(wcarray_size(txs) < 1)
		return;
	/* Get anchor */
	tx = (textstring*)wcarray_get(txs,0);
	/* Find dist */
	dx = x-tx->get_x();
	dy = y-tx->get_y();
	/* Addition it all */
	for(i = 0;i < wcarray_size(txs);i++)
	{
		tx = (textstring*)wcarray_get(txs,i);
		tx->move(tx->get_x()+dx,tx->get_y()+dy);
	}
	for(i = 0;i < wcarray_size(ics);i++)
	{
		if(wcarray_get(ics,i))
		{
			ic = (icon*)wcarray_get(ics,i);
			ic->move(ic->get_x()+dx,ic->get_y()+dy);
		}
	}
}

/* Draws a text object */
void text :: draw()
{
	int i;
	for(i = 0;i < wcarray_size(txs);i++){
		textstring* lol0=((textstring*)wcarray_get(txs,i));
		//__android_log_print(ANDROID_LOG_INFO, "adMOB", "text :: draw() %s", lol0->source);
		lol0->draw();
	}
	for(i = 0;i < wcarray_size(ics);i++)
	{
		if(wcarray_get(ics,i))
			((icon*)wcarray_get(ics,i))->draw();
	}
}

/* Advances text in a text object */
void text :: advance(int p,int fill)
{
	int i;
	textstring *ts;
	icon *ic;
	/* Advance all? */
	if(p == TEXT_ALL)
		p = length();
	/* Now for the hard part, show some */
	for(i = 0;i < wcarray_size(txs);i++)
	{
		/* Get the parts */
		ts = (textstring*)wcarray_get(txs,i);
		ic = (icon*)wcarray_get(ics,i);
		/* Advance text */
		if(p <= 0)
		{
			ts->advance(0);
		}
		else
		{
			if(p >= ts->length())
			{
				p -= ts->length();
				ts->advance(TEXT_ALL);
			}
			else
			{
				if(!fill)
					ts->advance(p);
				else
					ts->advance(TEXT_ALL);
				p = 0;
			}
		}
		/* Icon show */
		if(ic)
		{
			if(p <= 0)
			{
				ic->set_visible(0);
			}
			else
			{
				ic->set_visible(1);
				p--;
			}
		}
	}
}

/* Gets length */
int text :: length()
{
	textstring *ts;
	icon *ic;
	int i,c;
	/* Count */
	c = 0;
	for(i = 0;i < wcarray_size(txs);i++)
	{
		ts = (textstring*)wcarray_get(txs,i);
		ic = (icon*)wcarray_get(ics,i);
		/* Add string length */
		c += ts->length();
		/* Has icon? */
		if(ic)
			c++;
	}
	/* That's it */
	return c;
}

/* Sets color */
void text :: set_color(int cl)
{
	int i;
	textstring *ts;
	/* Straightforwardly set all the colors for all the strings */
	for(i = 0;i < wcarray_size(txs);i++)
	{
		ts = (textstring*)wcarray_get(txs,i);
		ts->set_color(cl);
	}
}

/* Gets total tempo time */
float text :: tempo_time(int p)
{
	int i,len;
	textstring *ts;
	icon *ic;
	char *ctr;
	float ttempo;
	/* Starts at 0 */
	ttempo = 0.0f;
	/* Begin adding it up */
	for(i = 0;i < wcarray_size(txs);i++)
	{
		/* Get all data */
		ts = (textstring*)wcarray_get(txs,i);
		ic = (icon*)wcarray_get(ics,i);
		ctr = (char*)wcarray_get(ccs,i);
		/* Does this exhaust data */
		len = ts->length();
		if(ic)
			len++;
		if(len > p)
			return ttempo;
		/* No, see if theres a tempo */
		if(ctr)
		{
			if(*ctr)
			{
				if(ctr[1] == '.')
					ttempo += 32.0f;
				if(ctr[1] == '|')
					ttempo += 96.0f;
			}
		}
		/* Consume */
		p -= len;
	}
	/* Return it */
	return ttempo;
}

/* Gets total speed */
float text :: current_speed(int p)
{
	int i,len;
	textstring *ts;
	icon *ic;
	char *ctr;
	float speed;
	float rspeed;
	int warpmode;
	/* Starts at 2 */
	speed = 2.0f;
	rspeed = speed;
	warpmode = 0;
	/* Begin adding it up */
	for(i = 0;i < wcarray_size(txs);i++)
	{
		/* Get all data */
		ts = (textstring*)wcarray_get(txs,i);
		ic = (icon*)wcarray_get(ics,i);
		ctr = (char*)wcarray_get(ccs,i);
		/* Does this exhaust data */
		len = ts->length();
		if(ic)
			len++;
		if(len > p)
			return speed;
		/* No, see if theres a tempo */
		if(ctr)
		{
			if(*ctr)
			{
				/* Speed change */
				if(ctr[1] == 's')
					if(ctr[2] == '[')
						speed = wcwindow_calculate_text_speed(atoi(&ctr[3]));
				/* Begin warp speed */
				if(ctr[1] == '>')
				{
					rspeed = speed;
					speed = 25.0f;
					warpmode = 1;
				}
				/* End warp speed */
				if(ctr[1] == '<')
				{
					speed = rspeed;
					warpmode = 0;
				}
			}
		}
		/* Consume */
		p -= len;
	}
	/* Return it */
	return speed;
}

/* Auto closing? */
int text :: auto_close(int p)
{
	if(auto_close_pos == -1)
		return 0;
	if(p >= auto_close_pos)
		return 1;
	return 0;
}

/* Creates a new icon object */
icon :: icon(int ix)
{
	subimage = ix;
	pos_x = 0;
	pos_y = 0;
	vis = 1;
}

/* Moves the icon object */
void icon :: move(int x,int y)
{
	pos_x = x;
	pos_y = y;
}

/* Draws the icon object */
void icon :: draw()
{
	if(vis)
		wcwindow_draw_icon(pos_x,pos_y,subimage);
}

/* Returns x position */
int icon :: get_x()
{
	return pos_x;
}

/* Returns y position */
int icon :: get_y()
{
	return pos_y;
}

/* Visible/invisible set */
void icon :: set_visible(int en)
{
	vis = en;
}