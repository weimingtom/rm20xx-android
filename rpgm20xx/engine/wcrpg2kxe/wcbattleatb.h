#ifndef WCBATTLEATB_H
#define WCBATTLEATB_H

/* Includes */
#include "wcbattle.h"

/* Defines */
#define WCBATTLEATB_PLAYERS 4
#define WCBATTLEATB_ENTERING 0
#define WCBATTLEATB_READY 1
#define WCBATTLEATB_COMMAND 2
#define WCBATTLEATB_EXECUTE 3
#define WCBATTLEATB_WIN 4
#define WCBATTLEATB_DOWNED 5

#define WCBATTLEATB_ATB_WIDTH 192

#define WCBATTLEATB_MENU_WAIT 0
#define WCBATTLEATB_MENU_COMMAND 1
#define WCBATTLEATB_MENU_TARGET 2
#define WCBATTLEATB_MENU_VICTORY 3
#define WCBATTLEATB_MENU_DEFEAT 4

/* Nucleus */
extern void wcbattleatb_init();
extern void wcbattleatb_exit();
extern void wcbattleatb_main();
extern void wcbattleatb_draw(int menulayer);
extern void wcbattleatb_handle_hero(heropos *hp);

#endif