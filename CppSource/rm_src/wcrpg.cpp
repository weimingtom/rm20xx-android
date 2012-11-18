/*
	WolfCoder's RPG-20XX Engine

	Designed as a replacement for Enterbrain's RPG_RT.exe,
	this engine allows games written with RPG Maker 2003 to run with latest systems.
	As a side effect, no part of the distributable shall belong to Enterbrain
	since all binaries are no longer needed with this.

	Licenced under the LGPL
	(http://www.gnu.org/licenses/lgpl.html)

	written by Sayuri L. Kitsune (2010)
*/

/* Includes */
//#include <SDL.h>
//#include <SDL_image.h>
//#include <SDL_syswm.h>
//#include "resource.h"
#include "wcrpg.h"
#include "wcini.h"
#include "wcdraw.h"
#include "wcsound.h"
#include "wcmaterial.h"
#include "wcdatabase.h"
#include "wclevel.h"
#include "wcinput.h"
#include "wcnucleus.h"
#include "wcmaptree.h"
#include "wcevent.h"
#include "wcparty.h"
#include "wcsystem.h"
#include "wcrun.h"
#include "wcdialog.h"
#include "wcmanage.h"
#include "wccommand.h"
#include "wcguibuild.h"
#include "wcpicture.h"
#include "wcsave.h"
#include "wcsavemenu.h"
#include "wcitemmenu.h"
#include "wcstatusmenu.h"
#include "wcwindow.h"
#include "wcmenu.h"
#include "wcheroplate.h"
#include "wcmem.h"
#include "wcfx.h"
#include "wcperform.h"
#include "wcmetric.h"
#include "wcplayer.h"
#include "wcbattle.h"
#include "wctrans.h"
#include "wcmessage.h"
#include "wcpopnum.h"
#include "wctimer.h"
#include "wcscreenshot.h"

#include <android\log.h>
#include "../jniUtil.h"


#include <stdlib.h>

/* Globals */
char *wcrpg_test_play_command = "TestPlay"; /* If this begins the command line, then run with rpg2003.exe */
char *wcrpg_root = 0; /* Root folder name, defaults to current */

//SDL_Surface *screen; /* Main screen SDL surface */
int screen; /* Main screen SDL surface */

elog wcrpg_log; /* Main log */
int wcrpg_error_count = 0; /* Number of errors */
char *wcrpg_title = 0; /* Game title */
int wcrpg_mag = WCRPG_DEFAULT_MAG; /* Screen multiplier */

//HINSTANCE wcrpg_inst; /* RPG instance */
int wcrpg_inst; /* RPG instance */

int wcrpg_ms_select = 0; /* Selected font for this game */
font *wcrpg_sysfont; /* System font for debugging in */
font *wcrpg_ms_gothic; /* Default RM2K font */
font *wcrpg_ms_mincho; /* Other Default RM2K font */
float wcrpg_time_step = 0.0f; /* Current system time step */
//LARGE_INTEGER wcrpg_time_measure; /* Measured time */
//LARGE_INTEGER wcrpg_time_final; /* Time ended */
//LARGE_INTEGER wcrpg_time_freq; /* Frequency of cycles */
int wcrpg_time_measure; /* Measured time */
int wcrpg_time_final; /* Time ended */
int wcrpg_time_freq; /* Frequency of cycles */


int wcrpg_fps_cur = 0; /* Current measuring of frames per second */
int wcrpg_fps_buffer[WCRPG_FPS_LENGTH]; /* Samples of FPS */
int wcrpg_fps = 0; /* Frames per second */
int wcrpg_signal_quit = 0; /* If this is 1, the engine quits next frame */
int wcrpg_20xx_mode = 0; /* If this is 1, all the extensions are enabled */

//SDL_Surface *wcrpg_window_icon = 0; /* Window icon */
int *wcrpg_window_icon = 0; /* Window icon */

int wcrpg_2000_mode = 0; /* If this is 1, this effects how data is loaded from database */
int wcrpg_test_play_mode = 0; /* If this is 1, debugging tools are enabled */
float wcrpg_speed_factor = 1.0f; /* This speeds up or slows down the entire game engine */

/* Sets game speed */
void wcrpg_set_game_speed(float spd)
{
	wcrpg_speed_factor = spd;
}

/* Returns true if user is playing 2k game */
int wcrpg_old_mode()
{
	return wcrpg_2000_mode;
}

/* Sets the engine into old mode */
void wcrpg_set_old_mode()
{
	if(!wcrpg_2000_mode)
	{
		wclog_write(wcrpg_log,"Game was made with RPG Maker 2000, adjusting database");
		wclog_enter(wcrpg_log);
	}
	wcrpg_2000_mode = 1;
}

/* Returns true if in 20XX mode */
int wcrpg_neo_mode()
{
	return wcrpg_20xx_mode;
}

/* Get built-in font */
font *wcrpg_get_font()
{
	if(wcsystem_get_font())
		return wcrpg_ms_mincho;
	return wcrpg_ms_gothic;
}

/* Measure the current time */
void wcrpg_time_start()
{
	/* Sample the time */
	//QueryPerformanceCounter(&wcrpg_time_measure);
}

/* Measure the end time and create time step */
void wcrpg_time_end()
{
	//float ctime;
	//int accu,i;
	///* Find delta */
	//QueryPerformanceCounter(&wcrpg_time_final);
	//wcrpg_time_final.QuadPart -= wcrpg_time_measure.QuadPart;
	//QueryPerformanceFrequency(&wcrpg_time_freq);
	//wcrpg_time_step = (float)(wcrpg_time_final.QuadPart)/(float)(wcrpg_time_freq.QuadPart);
	///* Now we know the duration in seconds of the last frame. */
	//ctime = 1.0f/16.0f; /* 16 frames per second */
	//wcrpg_fps_buffer[wcrpg_fps_cur] = (int)(1.0f/wcrpg_time_step);
	//wcrpg_fps_cur = (wcrpg_fps_cur+1)%WCRPG_FPS_LENGTH;
	//accu = 0;
	//for(i = 0;i < WCRPG_FPS_LENGTH;i++)
	//	accu += wcrpg_fps_buffer[i];
	//wcrpg_fps = accu/WCRPG_FPS_LENGTH;
	///* At 16FPS the step should be 1 and decrease as speed increases */
	//wcrpg_time_step = wcrpg_time_step/ctime;
	wcrpg_time_step = 0.27;
}

/* Returns the time step */
float wcrpg_get_time_step()
{
	if(wcrpg_speed_factor <= 1.0f)
		return wcrpg_time_step*wcrpg_speed_factor;
	return wcrpg_time_step;
}

/* Gets system font */
font *wcrpg_get_sysfont()
{
	return wcrpg_sysfont;
}

/* Gets program instance */
//HINSTANCE wcrpg_get_instance()
int wcrpg_get_instance()
{
	return wcrpg_inst;
}

/* Gets screen mag value */
int wcrpg_get_mag()
{
	return wcrpg_mag;
}

/* Clones a string */
char *wcrpg_string_clone(char *c)
{
	char *cc;
	int len;
	/* If the string given to me is empty, return empty */
	if(!c)
		return 0;
	/* Clone it */
	len = (int)strlen(c)+1;
	cc = (char*)malloc(len);
	memcpy(cc,c,len);
	return cc;
}

/* Display error */
void wcrpg_error(char *msg)
{
	/* Echo to log */
	wclog_write(wcrpg_log,"**ERROR** ");
	wclog_write(wcrpg_log,msg);
	wclog_enter(wcrpg_log);
	/* Too many errors */
	if(wcrpg_error_count == 5)
	{
		/* Etc. */
#ifndef NDEBUG
		//MessageBoxA(NULL,"... And many other errors, look inside wcrpg.txt for details.","Error",MB_ICONERROR);
#endif
#ifdef NDEBUG
		MessageBoxA(NULL,"... And additional errors. Please check engine and RTP installations.","Error",MB_ICONERROR);
#endif
	}
	/* Count */
	wcrpg_error_count++;
	/* Become silent */
	if(wcrpg_error_count > 5)
		return;
	/* Report */
	//MessageBoxA(NULL,msg,"Error",MB_ICONERROR);
}

/* Display a formatted error */
void wcrpg_error(char *msg,char *str)
{
	char buffer[260];
	sprintf(buffer,msg,str);
	wcrpg_error(buffer);
}

/* Display a formatted error with two strings */
void wcrpg_error(char *msg,char *str1,char *str2)
{
	char buffer[260];
	sprintf(buffer,msg,str1,str2);
	wcrpg_error(buffer);
}

/* Display a number error */
void wcrpg_error(char *msg,int n)
{
	char buffer[260];
	sprintf(buffer,msg,n);
	wcrpg_error(buffer);
}

/* Returns working folder */
char *wcrpg_get_root()
{
	return wcrpg_root;
}

/* Returns the name of the game */
char *wcrpg_get_title()
{
	if(!wcrpg_title)
		return "Unknown Game";
	return wcrpg_title;
}

/* Returns main log */
elog wcrpg_get_log()
{
	return wcrpg_log;
}

/* Starts the system */
int wcrpg_start()
{
	/*HWND hwnd;
	SDL_SysWMinfo info;
	RECT rect;*/
	int scw,sch;
	/* Announce */
	wclog_write(wcrpg_log,"Starting systems...");
	wclog_enter(wcrpg_log);
	/* Blank out fps buffer */
	memset(wcrpg_fps_buffer,0,sizeof(wcrpg_fps_buffer));
	/* Initialize sdl */
	/*if(SDL_Init(SDL_INIT_VIDEO) == -1)
	{
		wcrpg_error("Could not initialize SDL (%s)",SDL_GetError());
		return 0;
	}*/
	/* Open window */
	//screen = SDL_SetVideoMode(WCRPG_WIDTH*wcrpg_mag,WCRPG_HEIGHT*wcrpg_mag,0,SDL_HWSURFACE|SDL_DOUBLEBUF);
	//if(!screen)
	//{
	//	wcrpg_error("Could not set video mode (%s)",SDL_GetError());
	//	return 0;
	//}
	///* Center it on screen */
	//SDL_VERSION(&info.version);
	//SDL_GetWMInfo(&info);
	//hwnd = info.window;
	//GetWindowRect(hwnd,&rect);
	//scw = GetSystemMetrics(SM_CXSCREEN);
	//sch = GetSystemMetrics(SM_CYSCREEN);
	wclog_write(wcrpg_log,"Window centered at (");
	//wclog_write(wcrpg_log,scw/2-(rect.right-rect.left)/2);
	wclog_write(wcrpg_log,",");
	//wclog_write(wcrpg_log,sch/2-(rect.bottom-rect.top)/2);
	wclog_write(wcrpg_log,")");
	wclog_enter(wcrpg_log);
	//MoveWindow(hwnd,scw/2-(rect.right-rect.left)/2,sch/2-(rect.bottom-rect.top)/2,(rect.right-rect.left),(rect.bottom-rect.top),1);
	/* Title the window */
	//SDL_WM_SetCaption("WolfCoder RPG2KX Engine","wcrpg");
	//if(wcrpg_title)
	//	SDL_WM_SetCaption(wcrpg_title,"wcrpg");
	/* Start drawing system */
	if(!wcdraw_start())
		return 0;
	/* Start sound system */
	if(!wcsound_start())
		return 0;
	/* Load fonts */
	//wcrpg_sysfont = new font(bmp_sysfont,256,32);
	wcrpg_ms_gothic = new font("msgothic.png");
	wcrpg_ms_gothic->set_compression(2);
	wcrpg_ms_mincho = new font("msmincho.png");
	wcrpg_ms_mincho->set_compression(2);
	/* Load window icon */
	//wcrpg_window_icon = IMG_Load("wicon.bmp");
	//SDL_WM_SetIcon(wcrpg_window_icon,0);
	return 1;
}

/* Ends the system */
void wcrpg_end()
{
	/* Release font */
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcrpg_end[1]");
	delete wcrpg_sysfont;
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcrpg_end[2]");
	delete wcrpg_ms_gothic;
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcrpg_end[3]");
	delete wcrpg_ms_mincho;
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcrpg_end[4]");
	/* Report */
	wclog_write(wcrpg_log,"Peak VRAM: ");
	wclog_write(wcrpg_log,wcdraw_get_peak_vram());
	wclog_enter(wcrpg_log);
	/* Announce */
	wclog_write(wcrpg_log,"Shutting down...");
	wclog_enter(wcrpg_log);
	/* Stop sound system */
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcrpg_end[5]");
	//wcsound_end();
	/* Stop drawing system */
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcrpg_end[6]");
	wcdraw_end();
	/* Stop SDL */
	//SDL_Quit();
	/* Free game title */
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcrpg_end[7]");
	free(wcrpg_title);
}

/* Handles one frame of the system */
int wcrpg_handle()
{
	//SDL_Event ev;
	///* Start time */
	//wcrpg_time_start();
	//wcdraw_triangle_count_reset();
	///* Handle events */
	//while(SDL_PollEvent(&ev))
	//{
	//	/* User wants to quit */
	//	if(ev.type == SDL_QUIT)
	//		return 0;
	//}
	///* Quit? */
	//if(wcrpg_signal_quit)
	//	return 0;
	return 1;
}

/* Signals to close */
void wcrpg_exit()
{
	wcrpg_signal_quit = 1;
}

/* Gets test play mode */
int wcrpg_test_play()
{
	return wcrpg_test_play_mode;
}

/* Identifys the game */
int wcrpg_identify()
{
	char buffer[260];
	char *video_str;
	char *neo_str;
	char *reg_str;
	int video_size;
	FILE *f;
	ini *ii;
	/* Attempt to open RPG_RT.ini */
	sprintf(buffer,"/sdcard/wcr2kxx/assets/RPG_RT.ini");

	f=fopen(buffer,"r");
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcrpg_identify[%s]",buffer);
	if(!f)
	{
		__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcrpg_identify[%s] not found",buffer);
		/* Look for wcrpg.ini */
		wclog_write(wcrpg_log,buffer);
		wclog_write(wcrpg_log," not found, trying wcrpg.ini...");
		wclog_enter(wcrpg_log);
		sprintf(buffer,"%swcrpg.ini","/sdcard/wcr2kxx/assets/");
		f=fopen(buffer,"r");
		/* Error if this failed */
		if(!f)
		{
			wclog_write(wcrpg_log,buffer);
			wclog_write(wcrpg_log," not found, can't identify game.");
			wclog_enter(wcrpg_log);
			return 0;
		}
	}
	/* All OK, identify */
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcrpg_identify[1]");
	ii = new ini(buffer);
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcrpg_identify[2]");
	wclog_write(wcrpg_log,"Loaded .ini file.");
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "wcrpg_identify[3]");
	wclog_enter(wcrpg_log);
	ii->report(wcrpg_log);
	/* Get year */
	neo_str = ii->get_value("Year");
	if(neo_str)
	{
		if(!strcmp(neo_str,"20XX"))
		{
			wcrpg_20xx_mode = 1;
			wclog_write(wcrpg_log,"In the year 20XX...");
			wclog_enter(wcrpg_log);
		}
	}
	/* Get base */
	reg_str = ii->get_value("RegBase");
	if(reg_str)
	{
		wcmem_set_register_base(atoi(reg_str));
		wclog_write(wcrpg_log,"Register base set to ");
		wclog_write(wcrpg_log,wcmem_get_register_base());
		wclog_enter(wcrpg_log);
	}
	/* Get resolution scale */
	video_str = ii->get_value("VideoSize");
	if(video_str)
	{
		/* Get size */
		video_size = atoi(video_str);
		/* Enforce size */
		if(video_size > wcrpg_mag)
			wcrpg_mag = video_size;
		/* Announce */
		wclog_write(wcrpg_log,"Minimum acceptable resolution set at x");
		wclog_write(wcrpg_log,wcrpg_mag);
		wclog_enter(wcrpg_log);
	}
	/* Get game title */
	wcrpg_title = ii->get_value("GameTitle");
	if(!wcrpg_title)
	{
		wclog_write(wcrpg_log,"GameTitle field not found, can't identify.");
		wclog_enter(wcrpg_log);
		return 0;
	}	
	wclog_write(wcrpg_log,"Game identified as ");
	wclog_write(wcrpg_log,wcrpg_title);
	wclog_enter(wcrpg_log);
	wcrpg_title = wcrpg_string_clone(wcrpg_title);
	/* Done */
	delete ii;
	return 1;
}

/* Draws debugging info */
void wcrpg_draw_debug()
{
	/* Don't draw if not testing play */
	if(!wcrpg_test_play())
		return;
	/* Draw stuff */
#ifndef NDEBUG
	rpgevent *ev;
#endif
	if(wcnucleus_get_debug_info_mode())
		wcrpg_sysfont->draw_text(0,0,"FPS %d",wcrpg_fps);
	else
		return;
	if(wcnucleus_get_debug_speed_mode() == -1)
		wcrpg_sysfont->draw_text(0,WCRPG_HEIGHT-8,"SLOW");
	if(wcnucleus_get_debug_speed_mode() == 1)
		wcrpg_sysfont->draw_text(0,WCRPG_HEIGHT-8,"FF");
#ifndef NDEBUG
	wcrpg_sysfont->draw_text(64,0,"M %d",wcperform_get_measure_count());
	wcrpg_sysfont->draw_text(128,0,"PPF %d",wcdraw_triangle_count());
	wcrpg_sysfont->draw_text(256-16,0,"SAV %d",wcmem_get_svar_count());
	if(wcplayer_get())
	{
		ev = wcplayer_get();
		wcrpg_sysfont->draw_text(0,8,"TER %d",ev->get_terrain());
	}
	wcrpg_sysfont->draw_text(128,8,"MOV %d",wcmove_get_active());
	wcrpg_sysfont->draw_text(256-16,8,"M %d",wcnucleus_get_mode());
#endif
}

/* Handle logic, includes frame redundancy under some settings */
void wcrpg_logic_handle()
{
	int i,m;
	/* Find maximum frame count to run */
	if(wcrpg_speed_factor > 1.0f)
		m = (int)wcrpg_speed_factor;
	else
		m = 1;
	/* Run frames */
	for(i = 0;i < m;i++)
		wcnucleus_handle();
}

/* Program entry */
//int WINAPI WinMain(HINSTANCE h,HINSTANCE hp,PSTR c,int s)
int init_()
{ 
	/* Instance */
	//wcrpg_inst = h;
	/* Open log */
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "init_[1]");
	wcrpg_log = wclog_open("wcrpg");
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "init_[2]");
	wclog_write(wcrpg_log,"WolfCoder's RPG2KX Engine V1.0 (2010)");
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "init_[3]");
	JNIUtil::jni_readLog();
	//wclog_enter(wcrpg_log);
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "init_[4]");
	/* Working dir */
	//wcrpg_root = (char*)c;
	/* Enforce test play during debugging */
#ifndef NDEBUG
	wcrpg_test_play_mode = 1;
#endif
	/* Check for test play mode */
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "init_[5]");
	//if(!strncmp(wcrpg_root,wcrpg_test_play_command,8))
	//{
	//	wcrpg_test_play_mode = 1;
	//	wclog_write(wcrpg_log,"Running directly from IDE (Editor)");
	//	wclog_enter(wcrpg_log);
	//	wcrpg_root = "";
	//}
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "init_[6]");
	/* Identify game */
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "init_[7]");
	if(!wcrpg_identify())
	{
		if(!*wcrpg_root)
			wcrpg_error("No such game found in current directory.");
		else
			wcrpg_error("No such game found in %s",wcrpg_root);
		return 1;
	}
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "init_[8]");
	/* Start */
	if(!wcrpg_start())
		return 1;
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "init_[9]");
	/*if(*wcrpg_root)
	{
		wclog_write(wcrpg_log,"Working directory set to: ");
		wclog_write(wcrpg_log,wcrpg_root);
		wclog_enter(wcrpg_log);
	}*/
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "init_[10]");
	/* Index resources */
	wcmaterial_prepare();
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "init_[11]");
	wcdatabase_load();
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "init_[12]");
	wcmaptree_load();
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "init_[13]");
	wcparty_init();
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "init_[14]");
	wcevent_start();
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "init_[15]");
	wcmanage_init();
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "init_[16]");
	wccommand_init();
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "init_[17]");
	wcguibuild_init();
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "init_[18]");
	wcpicture_init();
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "init_[19]");
	wcmove_init();
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "init_[20]");
	wcsave_init();
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "init_[21]");
	wcsavemenu_init();
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "init_[22]");
	wcheroplate_init();
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "init_[23]");
	wcstatusmenu_init();
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "init_[24]");
	wcfx_init();
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "init_[25]");
	wcperform_init();
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "init_[26]");
	wchero_validate_all_equipment();
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "init_[27]");
	wcmetric_init();
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "init_[28]");
	wcbattle_set_system(WCBATTLE_ATB_REMIX);
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "init_[29]");
	wctrans_init();
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "init_[30]");
	wcmessage_init();
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "init_[31]");
	wcpopnum_init();
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "init_[32]");
	wctimer_init();
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "init_[33]");
	//wcscreenshot_init();
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "init_[34]");
	/* Run */
	return 0;
}

void render_(){
		/* Get input */
		//wcinput_handle();
		/* Run game */
		wcrpg_logic_handle();
		/* Handle current music */
		//wcsound_music_handle();
		/* Draw current scene */
		wcdraw_frame();
		/* End time measure */
		wcrpg_time_end();
		/* Measure performance */
		//wcperform_handle();
}

void destroy_(){
	/* End */
	wcpopnum_exit();
	wcmessage_exit();
	wctrans_exit();
	//wcmetric_exit();
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "destroy_[1]");
	wcperform_exit();
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "destroy_[2]");
	wcfx_exit();
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "destroy_[3]");
	wcstatusmenu_exit();
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "destroy_[4]");
	wcheroplate_exit();
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "destroy_[5]");
	wcsave_exit();
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "destroy_[6]");
	wcmove_exit();
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "destroy_[7]");
	wcpicture_exit();
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "destroy_[8]");
	wcguibuild_exit();
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "destroy_[9]");
	wcmanage_exit();
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "destroy_[10]");
	wcparty_exit();
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "destroy_[11]");
	wcrun_end();
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "destroy_[12]");
	wcwindow_clear_faceset();
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "destroy_[13]");
	wcdialog_erase_face();
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "destroy_[14]");
	wcevent_end();
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "destroy_[15]");
	wcmaptree_free();
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "destroy_[16]");
	wclevel_free();
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "destroy_[17]");
	wcmaterial_close();
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "destroy_[18]");
	wcrpg_end();
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "destroy_[19]");
	/* Close log */
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "destroy_[20]");
	wclog_close(wcrpg_log);
	//fazer o q =/
	wcheroplate_exit();

}