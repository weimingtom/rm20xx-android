/*
	Logging Tool
*/

/* Includes */
//#include <windows.h>
#include <stdio.h>
#include "wclog.h"

#include <android\log.h>

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifndef NDEBUG


char* name_="";

/* Create new */
elog wclog_open(char *name)
{
	return 0;
	
	char buffer[260];
	//FILE *logf;
	sprintf(buffer,"/data/data/br.gjteam.jni/files/%s.txt",name);
	//logf=fopen(buffer,"w");
	//if(logf==NULL){
	//	__android_log_print(ANDROID_LOG_INFO, "adMOB", "fuuu");
	//}

	name_=buffer;
	return (elog)0;
}

/* Close */
void wclog_close(elog l)
{
	return;
	//fclose((FILE*)l);
	//close((int)l); 
}

/* Writes a string to a log */
void wclog_write(elog l,char *str)
{
	//__android_log_print(ANDROID_LOG_INFO, "adMOB", "wclog_write %s",str);
	return;

	int fd1 = open(name_, O_RDWR | O_CREAT ); 
	if(fd1>=0) 
	{ 
		lseek(fd1,0,SEEK_END); 
		write(fd1,str,strlen(str)); 
		close(fd1); 
		
	}

	//if(str)
	//	fprintf((FILE*)l,str);
	//else
	//	fprintf((FILE*)l,"<NULL>");

	__android_log_print(ANDROID_LOG_INFO, "adMOB", "wclog_write[2]");
	//fflush((FILE*)l);
}

/* Returns inside a log */
void wclog_enter(elog l)
{
	return;
	//fprintf((FILE*)l,"\n");
	//fflush((FILE*)l);

	int fd1 = open(name_, O_RDWR | O_CREAT ); 
	if(fd1>=0) 
	{ 
		lseek(fd1,0,SEEK_END); 
		write(fd1,"\n",strlen("\n")); 
		close(fd1);
	}
}

/* Writes a number to a log */
void wclog_write(elog l,int n)
{
	return;
	//fprintf((FILE*)l,"%d",n);
	//fflush((FILE*)l);
}

#endif
#ifdef NDEBUG

/* Create new */
elog wclog_open(char *name)
{
	return 0;
}

/* Close */
void wclog_close(elog l)
{
}

/* Writes a string to a log */
void wclog_write(elog l,char *str)
{
}

/* Returns inside a log */
void wclog_enter(elog l)
{
}

/* Writes a number to a log */
void wclog_write(elog l,int n)
{
}

#endif