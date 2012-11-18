/*
	Battle Commands
*/

/* Includes */
#include <malloc.h>
#include "wcarray.h"
#include "wcbattlecommand.h"
#include "wctype.h"
#include "wcrpg.h"

/* Globals */
arraylist battlecommands;
table *battlecommand_table;

/* Create */
battlecommand :: battlecommand(entry *en)
{
	/* Defaults */
	name = 0;
	type = 0;
	/* Load */
	if(en->has_data(0x01))
		name = wcstr_read(en->get_data(0x01),en->get_length(0x01));
	if(en->has_data(0x02))
		type = wcint_read(en->get_data(0x02));
}

/* Delete */
battlecommand :: ~battlecommand()
{
	free(name);
}

/* Get name */
char *battlecommand :: get_name()
{
	return name;
}

/* Get type */
int battlecommand :: get_type()
{
	return type;
}

/* Load all */
void wcbattlecommand_load_all(FILE *f,int len)
{
	unsigned char gb;
	int i,rlen,cod,sublen;
	battlecommand *bcmd;
	/* There's a table outside of a table, wtf enterbrain */
	for(;;)
	{
		/* Read code */
		cod = wcint_read_file(f,&rlen);
		len -= rlen;
		/* Read length */
		sublen = wcint_read_file(f,&rlen);
		len -= rlen;
		/* Is code 0x0A? */
		if(cod == 0x0A)
			break;
		/* Read data */
		for(i = 0;i < sublen;i++)
		{
			fread(&gb,1,1,f);
			len--;
		}
	}
	/* Create */
	battlecommands = wcarray_new();
	battlecommand_table = new table(f);
	len -= battlecommand_table->get_size();
	wclog_write(wcrpg_get_log(),battlecommand_table->get_count());
	wclog_write(wcrpg_get_log()," commands found.");
	wclog_enter(wcrpg_get_log());
	/* Load all in */
	for(i = 0;i < battlecommand_table->get_count();i++)
	{
		bcmd = new battlecommand(battlecommand_table->get_entry(i));
		wclog_write(wcrpg_get_log(),"Command: ");
		wclog_write(wcrpg_get_log(),bcmd->get_name());
		wclog_enter(wcrpg_get_log());
		wcarray_add(battlecommands,bcmd);
	}
	/* Skip over shit */
	for(i = 0;i < len;i++)
		fread(&gb,1,1,f);
}

/* Free all */
void wcbattlecommand_free_all()
{
	int i;
	for(i = 0;i < wcarray_size(battlecommands);i++)
		delete (battlecommand*)wcarray_get(battlecommands,i);
	wcarray_delete(battlecommands);
	delete battlecommand_table;
}

/* Get */
battlecommand *wcbattlecommand_get(int id)
{
	/* Validate */
	if(id <= 0 || id > wcarray_size(battlecommands))
	{
		wcrpg_error("Invalid battle command (%d)",id);
		return 0;
	}
	/* Get */
	id--;
	return (battlecommand*)wcarray_get(battlecommands,id);
}