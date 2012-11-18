/*
	Command
*/

/* Includes */
#include <malloc.h>
#include "wccommand.h"
#include "wctype.h"
#include "wcmem.h"
#include "wcrpg.h"

/* Name finder */
#include "wccommandname.h"

/* Globals */
music *wccommand_memo_music = 0;
music *wccommand_play_music = 0;

/* Read */
command :: command(FILE *f)
{
	int rlen,i;
	/* Init */
	id = 0;
	str = 0;
	deep = 0;
	len = 0;
	args = 0;
	arg = 0;
	mls = 0;
	halt = 0;
	/* Get command ID */
	id = wcint_read_file(f,&rlen);
	/* Get depth */
	deep = wcint_read_file(f,&rlen);
	/* Get string */
	len = wcint_read_file(f,&rlen);
	if(len)
		str = wcstr_read_file(f,len);
	/* Get arg count */
	args = wcint_read_file(f,&rlen);
	/* Alloc */
	if(args)
	{
		arg = (int*)malloc(sizeof(int)*args);
		for(i = 0;i < args;i++)
			arg[i] = wcint_read_file(f,&rlen);
	}
	/* Move command */
	if(id == COMMAND_MOVE)
		mls = new movelist(args,arg);
}

/* Free */
command :: ~command()
{
	if(mls)
		delete mls;
	free(arg);
	free(str);
}

/* Get ID */
int command :: get_id()
{
	return id;
}

/* Get arg */
int command :: get_argument(int ix)
{
	return arg[ix];
}

/* Stop code */
int command :: is_stop()
{
	if(!id && !deep && !len && !args)
		return 1;
	return 0;
}

/* End code */
int command :: is_end()
{
	if(wccommand_is_end(id))
		return 1;
	return 0;
}

/* Code level */
int command :: get_level()
{
	return deep;
}

/* Print */
void command :: print(FILE *f)
{
	int i;
	for(i = 0;i < deep;i++)
		fprintf(f," ");
	/* Is comment? */
	if(!wccommand_is_comment(id))
	{
		/* Print code */
		if(!wccommand_get_name(id))
			fprintf(f,"(%X) ",id);
		else
			fprintf(f,"%s ",wccommand_get_name(id));
		if(str)
		{
			if(!wccommand_has_resource(id))
				fprintf(f,"\"%s\" ",str);
			else
				fprintf(f,"<%s> ",str);
		}
		if(args)
		{
			for(i = 0;i < args;i++)
				fprintf(f,"%d ",arg[i]);
		}
	}
	else
	{
		/* String */
		fprintf(f,"// %s",str);
	}
}

/* Command executive */
#include "wccommandrun.h"

/* 20XX Command executive */
#include "wccommandneo.h"

/* Initialize commands */
void wccommand_init()
{
	/* Set */
	wcmem_set_register_callback(wccommand_neo_run);
}