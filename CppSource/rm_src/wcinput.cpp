/*
	Game Input
*/

/* Includes */
//#include <SDL.h>
#include "wcinput.h"

///* Globals */
//Uint8 *wcinput_keys;
//int wcinput_lock_enable = 0;

#include <android\log.h>

int key=0;

/* Gathers input for a frame */
void wcinput_handle(int key_)
{
	//__android_log_print(ANDROID_LOG_INFO, "adMOB", "%d",key_);
	key=key_;
	/* Get the state */
	//wcinput_keys = SDL_GetKeyState(0);
}

/* Returns the input code if it is active */
int wcinput_key(int code)
{
	switch(code)
	{
	//__android_log_print(ANDROID_LOG_INFO, "adMOB", "%d",code);
	/* Moving keys */
	case WCINPUT_DOWN:
		if(key & 1<<1)
			return 1;
		return 0;
	case WCINPUT_LEFT:
		if(key & 1<<0)
			return 1;
		return 0;
	case WCINPUT_RIGHT:
		if(key & 1<<2)
			return 1;
		return 0;
	case WCINPUT_UP:
		if(key & 1<<3)
			return 1;
		return 0;
	/* Main keys */
	case WCINPUT_OK:
		if(key & 1<<4)
			return 1;
		return 0;
	case WCINPUT_CANCEL:
		return 0;
	case WCINPUT_SHIFT:
		//if(wcinput_keys[SDLK_RSHIFT])
		//	return 1;
		//if(wcinput_keys[SDLK_LSHIFT])
		//	return 1;
		return 0;
	/* Numbers */
	case WCINPUT_0:
		/*if(wcinput_keys[SDLK_0])
			return 1;*/
		return 0;
	case WCINPUT_1:
		/*if(wcinput_keys[SDLK_1])
			return 1;*/
		return 0;
	case WCINPUT_2:
		/*if(wcinput_keys[SDLK_2])
			return 1;*/
		return 0;
	case WCINPUT_3:
		//if(wcinput_keys[SDLK_3])
		//	return 1;
		return 0;
	case WCINPUT_4:
		//if(wcinput_keys[SDLK_4])
		//	return 1;
		return 0;
	case WCINPUT_5:
		//if(wcinput_keys[SDLK_5])
		//	return 1;
		return 0;
	case WCINPUT_6:
		//if(wcinput_keys[SDLK_6])
		//	return 1;
		return 0;
	case WCINPUT_7:
		//if(wcinput_keys[SDLK_7])
		//	return 1;
		return 0;
	case WCINPUT_8:
		//if(wcinput_keys[SDLK_8])
		//	return 1;
		return 0;
	case WCINPUT_9:
		//if(wcinput_keys[SDLK_9])
		//	return 1;
		return 0;
	/* Ops */
	case WCINPUT_PLUS:
		//if(wcinput_keys[SDLK_PLUS])
		//	return 1;
		return 0;
	case WCINPUT_MINUS:
		//if(wcinput_keys[SDLK_MINUS])
		//	return 1;
		return 0;
	case WCINPUT_MULTIPLY:
		//if(wcinput_keys[SDLK_ASTERISK])
		//	return 1;
		return 0;
	case WCINPUT_DIVIDE:
		//if(wcinput_keys[SDLK_SLASH])
		//	return 1;
		return 0;
	case WCINPUT_PERIOD:
		//if(wcinput_keys[SDLK_PERIOD])
		//	return 1;
		return 0;
	/* Scrollin keys */
	case WCINPUT_PAGE_DOWN:
		//if(wcinput_keys[SDLK_PAGEDOWN])
		//	return 1;
		return 0;
	case WCINPUT_PAGE_UP:
		//if(wcinput_keys[SDLK_PAGEUP])
		//	return 1;
		return 0;
	/* Debug keys */
	case WCINPUT_DEBUG_SAVE:
		//if(wcinput_keys[SDLK_F5])
		//	return 1;
		return 0;
	case WCINPUT_DEBUG_SCREENSHOT:
		//if(wcinput_keys[SDLK_F6])
		//	return 1;
		return 0;
	case WCINPUT_DEBUG_FOLDER:
		//if(wcinput_keys[SDLK_F7])
		//	return 1;
		return 0;
	case WCINPUT_DEBUG_PLAY:
		//if(wcinput_keys[SDLK_F9])
		//	return 1;
		return 0;
	case WCINPUT_DEBUG_SLOW:
		//if(wcinput_keys[SDLK_F10])
		//	return 1;
		return 0;
	case WCINPUT_DEBUG_FAST:
		//if(wcinput_keys[SDLK_F11])
		//	return 1;
		return 0;
	case WCINPUT_DEBUG_INFO:
		//if(wcinput_keys[SDLK_F12])
		//	return 1;
		return 0;
	}
	/* Nothing pressed */
	return 0;
}

/* Returns the X direction of movement */
int wcinput_x()
{
	if(wcinput_key(WCINPUT_RIGHT))
		return 1;
	if(wcinput_key(WCINPUT_LEFT))
		return -1;
	return 0;
}

/* Returns the Y direction of movement */
int wcinput_y()
{
	if(wcinput_key(WCINPUT_UP))
		return -1;
	if(wcinput_key(WCINPUT_DOWN))
		return 1;
	return 0;
}

/* Returns OK */
int wcinput_ok()
{
	if(wcinput_key(WCINPUT_OK))
		return 1;
	return 0;
}

/* Returns Cancel */
int wcinput_cancel()
{
	if(wcinput_key(WCINPUT_CANCEL))
		return 1;
	return 0;
}

/* Returns menu */
int wcinput_menu()
{
	if(wcinput_key(WCINPUT_CANCEL))
		return 1;
	return 0;
}

/* Returns the key made for map testing when on title screen */
int wcinput_map_test()
{
	//if(wcinput_key(WCINPUT_1))
	//	return 1;
	return 0;
}

/* Returns the key made for sound testing when on title screen */
int wcinput_sound_test()
{
	//if(wcinput_key(WCINPUT_2))
	//	return 1;
	return 0;
}

/* Returns fast scrolling direction */
int wcinput_scroll_y()
{
	//if(wcinput_key(WCINPUT_PAGE_DOWN))
	//	return 1;
	//if(wcinput_key(WCINPUT_PAGE_UP))
	//	return -1;
	return 0;
}

/* Gets the input vector */
void wcinput_vector(int *iv)
{
	int i;
	for(i = 0;i < WCINPUT_NUM_CODES;i++)
	{
		if(wcinput_key(i))
			iv[i] = 1;
		else
			iv[i] = 0;
	}
}