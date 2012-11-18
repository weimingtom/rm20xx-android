/*
	Invokes other programs
*/

/* Includes */
//#include <windows.h>
#include <memory.h>
#include <stdlib.h>
#include "wcinvoke.h"
#include "wcrpg.h"
#include "wcsave.h"

/* Invokes the program or command with command line params */
void wcinvoke(char *prog,char *cmd)
{
	//STARTUPINFOA si;
	//PROCESS_INFORMATION pi;
	///* Zero set */
	//memset(&si,0,sizeof(si));
	//memset(&pi,0,sizeof(pi));
	///* Sizeof */
	//si.cb = sizeof(si);
	///* Invoke */
	//if(!CreateProcessA(prog,cmd,NULL,NULL,0,0,NULL,NULL,&si,&pi))
	//	wcrpg_error("Could not invoke %s with %s",prog,cmd);
}

/* Invokes the game save folder which pops open a file browser window set in the game saves */
void wcinvoke_game_save_folder()
{
	//int i;
	//char buffer[260];
	//char windir[260];
	//char exdir[260];
	//char commd[260];
	///* Get windows dir */
	//memset(windir,0,sizeof(windir));
	//GetEnvironmentVariableA("WINDIR",windir,sizeof(windir));
	///* Get explorer dir */
	//memset(exdir,0,sizeof(exdir));
	//sprintf_s(exdir,sizeof(exdir),"%s\\explorer.exe",windir);
	///* Open explorer with root set to game saves */
	//memset(buffer,0,sizeof(buffer));
	//sprintf_s(buffer,sizeof(buffer),"/root,%s",wcsave_get_game_base());
	///* Ugh, it only likes \ */
	//for(i = 0;i < 260;i++)
	//{
	//	if(buffer[i] == '/')
	//		buffer[i] = '\\';
	//}
	//buffer[0] = '/';
	//buffer[(int)strlen(buffer)-1] = 0;
	///* Log */
	//wclog_write(wcrpg_get_log(),"Opened game save directory: ");
	//wclog_write(wcrpg_get_log(),exdir);
	//wclog_write(wcrpg_get_log()," ");
	//wclog_write(wcrpg_get_log(),buffer);
	//wclog_enter(wcrpg_get_log());
	///* Open */
	//memset(commd,0,sizeof(commd));
	//sprintf_s(commd,sizeof(commd),"%s %s",exdir,buffer);
	//wcinvoke(NULL,commd);
}