#ifndef WCMISC_H
#define WCMISC_H

/*
	Increase player money
*/
extern void wcmisc_add_money(int amt);

/*
	Decrease player money
*/
extern int wcmisc_take_money(int amt);

/*
	Checks the amount of money held
*/
extern int wcmisc_get_money();

/*
	Get save count
*/
extern int wcmisc_get_save_count();

/*
	Increment save count
*/
extern void wcmisc_increment_save_count();

/*
	Sets player speed
*/
extern void wcmisc_set_player_speed(int spd);

/*
	Gets player speed
*/
extern int wcmisc_get_player_speed();

#endif