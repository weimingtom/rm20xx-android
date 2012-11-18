#include "PreCompile.h"
#include "Util.h"
#ifdef _WIN32
//#include <windows.h>
#else
#include <sys/time.h>
#endif

WndStruct* g_hWnd = NULL;

int GetMilliSeconds()
{
#ifdef _WIN32
	return timeGetTime();
#else
	struct timeval tv;
	gettimeofday(&tv, NULL); 
	return tv.tv_sec*1000 + tv.tv_usec/1000;
#endif
}

