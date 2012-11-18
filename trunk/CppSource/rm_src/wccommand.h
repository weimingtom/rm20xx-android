#ifndef WCCOMMAND_H
#define WCCOMMAND_H

/* Codes */
#define COMMAND_MESSAGE 0x277E
#define COMMAND_MESSAGE_LINE 0x4E8E
#define COMMAND_SET_FACE 0x2792
#define COMMAND_SWITCH 0x27E2
#define COMMAND_VARIABLE 0x27EC
#define COMMAND_WAIT 0x2C92
#define COMMAND_SOUND 0x2D1E
#define COMMAND_TELEPORT 0x2A3A
#define COMMAND_MESSAGE_OPTIONS 0x2788
#define COMMAND_ENABLE_MAIN_MENU 0x2EB8
#define COMMAND_ERASE_EVENT 0x3020
#define COMMAND_MOVE 0x2C42
#define COMMAND_PROCEED_MOVE 0x2C4C
#define COMMAND_SHOW_PICTURE 0x2B66
#define COMMAND_ERASE_PICTURE 0x2B7A
#define COMMAND_MOVE_PICTURE 0x2B70
#define COMMAND_CHANGE_PARTY 0x285A
#define COMMAND_CHANGE_SPRITE 0x2986
#define COMMAND_CHANGE_FACE 0x2990
#define COMMAND_LABEL 0x2F4E
#define COMMAND_GOTO 0x2F58
#define COMMAND_SHOW_CHOICE 0x279C
#define COMMAND_CHOICE_OPTION 0x4EAC
#define COMMAND_BREAK 0xA
#define COMMAND_BRANCH 0x2EEA
#define COMMAND_ELSE 0x55FA
#define COMMAND_WHILE 0x2FB2
#define COMMAND_END_WHILE 0x56C2
#define COMMAND_BREAK_WHILE 0x2FBC
#define COMMAND_CALL_EVENT 0x302A
#define COMMAND_ENABLE_SAVE 0x2E9A
#define COMMAND_OPEN_SAVE 0x2E86
#define COMMAND_OPEN_MAIN_MENU 0x2EAE
#define COMMAND_ITEM 0x2850
#define COMMAND_MONEY 0x2846
#define COMMAND_TINT_SCREEN 0x2B16
#define COMMAND_LEVEL_UP 0x28B4
#define COMMAND_CHANGE_STAT 0x28BE
#define COMMAND_CHANGE_EXP 0x28AA
#define COMMAND_CHANGE_HEALTH 0x28DC
#define COMMAND_CHANGE_MANA 0x28E6
#define COMMAND_KEY_INPUT 0x2D5A
#define COMMAND_BATTLE_ANIMATION 0x2BCA
#define COMMAND_FLASH_EVENT 0x2C38
#define COMMAND_FLASH_SCREEN 0x2B20
#define COMMAND_PLAY_MUSIC 0x2CF6
#define COMMAND_FADE_MUSIC 0x2D00
#define COMMAND_MEMORIZE_MUSIC 0x2D0A
#define COMMAND_RECALL_MUSIC 0x2D14
#define COMMAND_CHANGE_EQUIPMENT 0x28D2
#define COMMAND_PAN_SCREEN 0x2B34
#define COMMAND_RETURN 0x3016
#define COMMAND_CHANGE_SKILLS 0x28C8
#define COMMAND_CHANGE_HERO_NAME 0x2972
#define COMMAND_CHANGE_HERO_TITLE 0x297C
#define COMMAND_START_COMBAT 0x29D6
#define COMMAND_ERASE_SCREEN 0x2B02
#define COMMAND_SHOW_SCREEN 0x2B0C
#define COMMAND_GET_TERRAIN 0x2A9E
#define COMMAND_GET_EVENT 0x2AA8
#define COMMAND_SWAP_TILE 0x2DE6
#define COMMAND_PLACE_EVENT 0x2A6C
#define COMMAND_TIMER 0x27F6
#define COMMAND_GAME_OVER 0x3084
#define COMMAND_CHANGE_BG 0x2DC8
#define COMMAND_CHANGE_SYSTEM_GRAPHIC 0x29B8
#define COMMAND_SWAP_EVENT_LOCATIONS 0x2A76

#define COMMAND_HALT_ON_PAN 1

/* Includes */
#include <stdio.h>
#include "wcmove.h"

/* Classes */
class command
{
private:
	int id; /* Command ID */
	int deep; /* Depth */
	int len; /* String length */
	char *str; /* The string itself */
	int args; /* Number of args */
	int *arg; /* Args */
	int halt; /* Halt on.. code */
	movelist *mls; /* Movelist */
public:
	/*
		Loads a command from a file
	*/
	command(FILE *f);
	~command();
	/*
		Gets command #
	*/
	int get_id();
	/*
		Gets argument
	*/
	int get_argument(int ix);
	/*
		Prints to file
	*/
	void print(FILE *f);
	/*
		Returns true if this is a stop code
	*/
	int is_stop();
	/*
		Runs this command with a pointer to the THIS_EVENT event given
	*/
	int run(void *me,void *st);
	/*
		Is this an end code?
	*/
	int is_end();
	/*
		Returns code level
	*/
	int get_level();
};

/*
	Initialize commands
*/
extern void wccommand_init();

#endif