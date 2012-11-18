/*
	Misc Game Data
*/

/* Includes */
#include "wcmisc.h"

/* Globals */
int wcmisc_money = 0; /* Current money */
int wcmisc_timer_1 = 0; /* Timer one */
int wcmisc_timer_2 = 0; /* Timer 2 */
int wcmisc_save_count = 0; /* Times saved */
int wcmisc_player_add_speed = 0; /* Player's speed */

/* Add money */
void wcmisc_add_money(int amt)
{
	wcmisc_money += amt;
}

/* Take money */
int wcmisc_take_money(int amt)
{
	int ret;
	/* Had enough */
	if(amt <= wcmisc_money)
	{
		wcmisc_money -= amt;
		return amt;
	}
	/* Wasn't enough */
	ret = wcmisc_money;
	wcmisc_money = 0;
	return ret;
}

/* Get current money */
int wcmisc_get_money()
{
	return wcmisc_money;
}

/* Get save count */
int wcmisc_get_save_count()
{
	return wcmisc_save_count;
}

/* Increment save count */
void wcmisc_increment_save_count()
{
	wcmisc_save_count++;
}

/* Set player speed */
void wcmisc_set_player_speed(int spd)
{
	wcmisc_player_add_speed = spd;
}

/* Get player speed */
int wcmisc_get_player_speed()
{
	return wcmisc_player_add_speed;
}