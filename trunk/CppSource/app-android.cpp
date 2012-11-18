#include "jniUtil.h"
#include <sys/time.h>
#include <time.h>
#include <android/log.h>
#include <stdint.h>

#include "external/zlib/zip.h"

//

#include <stdlib.h>
#include <math.h>
#include <assert.h> 

#include <GLES/gl.h>
#include <GLES/glext.h>
#include "gl2d.h"
#include <algorithm>
#include "Vector2D.h"

#include "gmUtil.h"

using namespace std;

int   gAppAlive   = 1; 

static int  sWindowWidth  = 320;
static int  sWindowHeight = 480;
static int  sDemoStopped  = 0;
static long sTimeOffset   = 0;
static int  sTimeOffsetInit = 0;
static long sTimeStopped  = 0;


zip* APKArchive;

#include "Engine.h"


static long sStartTick = 0; 
static long sTick = 0;

long lastUpdate = 0;
long lastTouch = 0;

JNIEnv* JNIUtil::_my_jnienv=0;

Engine engine;

int previousInput=0;

bool isPressedRot=false;
bool isPressedPos=false;

int rotActionID=-1;
int posActionID=-1;

// The radius of the touchable region.
#define JOYSTICK_RADIUS 64.0f
//#define JOYSTICK_RADIUS 92.0f

// How far from the center should the thumb be allowed to move?
// Used only for visual feedback not for velocity calculations.
#define THUMB_RADIUS 48.0f
//#define THUMB_RADIUS 72.0f

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <string.h>

typedef struct stat Stat;

extern void wcinput_handle(int);

static int do_mkdir(const char *path, mode_t mode)
{
    Stat            st;
    int             status = 0;

    if (stat(path, &st) != 0)
    {
        /* Directory does not exist */
        if (mkdir(path, mode) != 0)
            status = -1;
    }
    else if (!S_ISDIR(st.st_mode))
    {
        status = -1;
    }

    return(status);
}

/**
** mkpath - ensure all directories in path exist
** Algorithm takes the pessimistic view and works top-down to ensure
** each directory in path exists, rather than optimistically creating
** the last element and working backwards.
*/
int mkpath(const char *path, mode_t mode)
{
    char           *pp;
    char           *sp;
    int             status;
	char           *copypath = strdup(path);

    status = 0;
    pp = copypath;
    while (status == 0 && (sp = strchr(pp, '/')) != 0)
    {
        if (sp != pp)
        {
            /* Neither root nor double slash in path */
            *sp = '\0';
            status = do_mkdir(copypath, mode);
            *sp = '/';
        }
        pp = sp + 1;
    }
    if (status == 0)
        status = do_mkdir(path, mode);
    free(copypath);
    return (status);
}

static void loadAPK (const char* apkPath) {
  __android_log_print(ANDROID_LOG_INFO, "adMOB","Loading APK %s", apkPath);
  APKArchive = zip_open(apkPath, 0, NULL);
  if (APKArchive == NULL) {
   __android_log_print(ANDROID_LOG_INFO, "adMOB","Error loading APK");
    return;
  }

  //Just for debug, print APK contents
  int numFiles = zip_get_num_files(APKArchive);
  //mkdir("/sdcard/wcr2kxx/",777);
  //mkdir("/sdcard/wcr2kxx/assets",777);
  //mkdir("/sdcard/wcr2kxx/assets/Backdrop",777);
  //mkdir("/sdcard/wcr2kxx/assets/Battle",777);
  //mkdir("/sdcard/wcr2kxx/assets/Charset",777);
  //mkdir("/sdcard/wcr2kxx/assets/Chipset",777);
  //mkdir("/sdcard/wcr2kxx/assets/Faceset",777);
  //mkdir("/sdcard/wcr2kxx/assets/GameOver",777);
  //mkdir("/sdcard/wcr2kxx/assets/System",777);
  //mkdir("/sdcard/wcr2kxx/assets/System2",777);
  //mkdir("/sdcard/wcr2kxx/assets/Title",777);
  for (int i=0; i<numFiles; i++) {
    const char* name = zip_get_name(APKArchive, i, 0);
	__android_log_print(ANDROID_LOG_INFO, "adMOB","%s", name);
	//char buffer[260];
	
	std::string buffer="/sdcard/wcr2kxx/";
	std::string nameStr=name;
	std::string final=buffer+nameStr;
	size_t found=final.find_last_of("/");

	//sprintf(buffer,"/sdcard/wcr2kxx/%s", name);
	__android_log_print(ANDROID_LOG_INFO, "adMOB","%s", final.substr(0,found).c_str());
	mkpath(final.substr(0,found).c_str(),777);
	//mkdir(buffer,777);
	//mkdirs(buffer);

	int fd1 = open(final.c_str(), O_RDWR | O_CREAT ); 

	//FILE* outFile = fopen(buffer, "w");

	size_t read = 0;
	char tmp[8092];

	struct zip_file* file = zip_fopen(APKArchive, name, 0);
	read = zip_fread(file, tmp, sizeof(tmp));
	while (read > 0) 
	{
		//fwrite(tmp, sizeof(char), read, outFile);
		write(fd1,tmp,read); 
		
		read = zip_fread(file, tmp, sizeof(tmp));
	}
	close(fd1);

	zip_fclose(file);
	//fclose(outFile);

    if (name == NULL) {
      __android_log_print(ANDROID_LOG_INFO, "adMOB","Error reading zip file name at index %i : %s", zip_strerror(APKArchive));
      return;
    }
	__android_log_print(ANDROID_LOG_INFO, "adMOB","File %i : %s copied\n", i, name);
  }
}

static void drawFadeQuad() 
{
	static const GLfixed quadVertices[] = {
		-0x10000, -0x10000, 
		0x10000, -0x10000,
		-0x10000,  0x10000,
		0x10000, -0x10000, 
		0x10000,  0x10000,
		-0x10000,  0x10000
	};

	//const int beginFade = sTick - sCurrentCamTrackStartTick;
	//const int endFade = sNextCamTrackStartTick - sTick;
	//const int minFade = beginFade < endFade ? beginFade : endFade;
	const int minFade=10000;

	if (minFade < 1024)
	{
		const GLfixed fadeColor = minFade << 6;
		glColor4x(fadeColor, fadeColor, fadeColor, 0);

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_ZERO, GL_SRC_COLOR);
		glDisable(GL_LIGHTING);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glVertexPointer(2, GL_FIXED, 0, quadVertices);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glEnableClientState(GL_COLOR_ARRAY);

		glMatrixMode(GL_MODELVIEW);

		glEnable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
	}
}

static long _getTime(void)
{
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	return now.tv_sec*1000000 + now.tv_nsec/1000;

}

// Called from the app framework.
void appInit()
{
	/*
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glShadeModel(GL_FLAT);
	*/
	__android_log_print(ANDROID_LOG_INFO, "adMOB", "appInit");
}


// Called from the app framework.
void appDeinit()
{

}

bool isPointInCircle(Vector2D<float> point,Vector2D<float> center, float radius){
	float dx = (point.x - center.x);
	float dy = (point.y - center.y);
	return (radius*radius >= (dx * dx) + (dy * dy));
}


static void prepareFrame(int width, int height, float time)
{
	glClearScreen();
	//engine.render(time);
	engine.render(0);
}


// Called from the app framework.
/* The tick is current time in milliseconds, width and height
* are the image dimensions to be rendered.
*/
void appRender(float tick, int width, int height)
{
	//if (sStartTick == 0)
	//    sStartTick = tick;
	//if (!gAppAlive)
	//    return;

	//// Actual tick value is "blurred" a little bit.
	//sTick = (sTick + tick - sStartTick) >> 1;

	// Prepare OpenGL ES for rendering of the frame.
	prepareFrame(width, height, tick);  
}

extern "C" {
	jboolean Java_br_gjteam_jni_Main_init(JNIEnv* env, jobject thiz, jstring arg);
	jboolean Java_br_gjteam_jni_Main_render(JNIEnv* env, jobject thiz, jint drawWidth, jint drawHeight, jboolean forceRedraw);
	jboolean Java_br_gjteam_jni_Main_sensorEvent(JNIEnv* env, jobject thiz, jint sensorType, jfloat values0, jfloat values1, jfloat values2);
	void Java_br_gjteam_jni_Main_cleanup(JNIEnv* env);
	void Java_br_gjteam_jni_Main_shownScore(JNIEnv* env);
	jboolean Java_br_gjteam_jni_Main_inputEvent(JNIEnv* env, jobject thiz, jint action, jint id, jfloat mx, jfloat my);
	jboolean Java_br_gjteam_jni_Main_multiInputEvent(JNIEnv* env, jobject thiz, jint action, jint id, jfloat mx, jfloat my);
	jboolean Java_br_gjteam_jni_Main_keyEvent(JNIEnv* env, jobject thiz, jint action, jint unicodeChar, jint keyCode, jobject keyEvent);
	void Java_br_gjteam_jni_Main_setOffsets(JNIEnv* env, jobject thiz, jint x, jint y);
}


jboolean Java_br_gjteam_jni_Main_sensorEvent(JNIEnv* env, jobject thiz, jint sensorType, jfloat values0, jfloat values1, jfloat values2){
}

void handleAccel(int type, int x, int y){
}

jboolean Java_br_gjteam_jni_Main_inputEvent(JNIEnv* env, jobject thiz, jint type,jint id, jfloat x, jfloat y){
}

jboolean Java_br_gjteam_jni_Main_multiInputEvent(JNIEnv* env, jobject thiz, jint type,jint id, jfloat x, jfloat y){

	if(type==1 || type==6){
		if(posActionID==id){
			isPressedPos=false;
			engine.controlKnobPos=engine.controlBasePos;
			wcinput_handle(0);
		}

		//if(rotActionID==id){
		//	isPressedRot=false;
		//	//engine.controlKnobRot=engine.controlBaseRot;
		//}
	}
	//engine.isShooting=isPressedRot;

	if(type==0 || type==5){
		//if(type==0){
		//	long curTime = _getTime();
		//	float elapsedTime=curTime-lastTouch;
		//	elapsedTime/=1000000.0f;
		//	lastTouch=curTime;

		//	if(elapsedTime<=0.4f){
		//		engine.doubleTouch=true; 
		//	}
		//}

		

		if(isPointInCircle(Vector2D<float>(x,y),Vector2D<float>(engine.controlBasePos),JOYSTICK_RADIUS)){
			isPressedPos=true;
			posActionID=id;
		}else{
			wcinput_handle(1<<4);
			engine.touch=true;
		}
		//if(isPointInCircle(Vector2D<float>(x,y),Vector2D<float>(engine.controlBaseRot),JOYSTICK_RADIUS)){
		//	isPressedRot=true;
		//	rotActionID=id;
		//}	

	}

	if(isPressedPos){
		if(posActionID==id){
			//float dx = x - engine.controlBasePos.x;
			//float dy = y - engine.controlBasePos.y;
			//float distance = sqrt(dx * dx + dy * dy);
			//float angle = atan2(dy, dx); // in radians

			//if (distance > JOYSTICK_RADIUS) {
			//	dx = cos(angle) * JOYSTICK_RADIUS;
			//	dy = sin(angle) * JOYSTICK_RADIUS;
			//}

			//Vector2D<float> velocity = Vector2D<float>(dx/JOYSTICK_RADIUS, dy/JOYSTICK_RADIUS);

			//Vector2D<float> point=Vector2D<float>(x,y);
			//if (distance > THUMB_RADIUS) {
			//	point.x = engine.controlBasePos.x + cos(angle) * THUMB_RADIUS;
			//	point.y = engine.controlBasePos.y + sin(angle) * THUMB_RADIUS;
			//}

			//engine.controlKnobPos=point;
			////engine.player->dir=velocity;


			//DPAD

			float dx = x - engine.controlBasePos.x;
			float dy = y - engine.controlBasePos.y;
			float angle = atan2(dy, dx); // in radians
			
			float anglePerSector = 360.0f / 4 * (M_PI/180.0f);
			angle = roundf(angle/anglePerSector) * anglePerSector;

			Vector2D<float> velocity = Vector2D<float>(cos(angle), sin(angle));

			engine.controlKnobPos.x = engine.controlBasePos.x + cos(angle) * THUMB_RADIUS;
			engine.controlKnobPos.y = engine.controlBasePos.y + sin(angle) * THUMB_RADIUS;

			//__android_log_print(ANDROID_LOG_INFO, "adMOB", "%f %f", velocity.x, velocity.y);

			if(((int) velocity.x)==-1){
				wcinput_handle(1<<0);
			}
			else if(((int) velocity.y)==1){
				wcinput_handle(1<<1);
			}
			else if(((int) velocity.x)==1){
				wcinput_handle(1<<2);
			}
			else if(((int) velocity.y)==-1){
				wcinput_handle(1<<3);
			}

		}
	}

	//if(isPressedRot){
	//	if(rotActionID==id){
	//		////__android_log_print(ANDROID_LOG_INFO, "input", "isPressedRot: %d %d %f %f", type, id, x, y);
	//		float dx = x - engine.controlBaseRot.x;
	//		float dy = y - engine.controlBaseRot.y;
	//		float distance = sqrt(dx * dx + dy * dy);
	//		float angle = atan2(dy, dx); // in radians

	//		//if (distance > JOYSTICK_RADIUS) {
	//		//	dx = cos(angle) * JOYSTICK_RADIUS;
	//		//	dy = sin(angle) * JOYSTICK_RADIUS;
	//		//}

	//		//Vector2D<float> velocity = Vector2D<float>(dx/JOYSTICK_RADIUS, dy/JOYSTICK_RADIUS);

	//		Vector2D<float> point=Vector2D<float>(x,y);
	//		if (distance > THUMB_RADIUS) {
	//			point.x = engine.controlBaseRot.x + cos(angle) * THUMB_RADIUS;
	//			point.y = engine.controlBaseRot.y + sin(angle) * THUMB_RADIUS;
	//		}

	//		engine.controlKnobRot=point;
	//		//engine.player->angle=angle;

	//	}
	//} 
}

jboolean Java_br_gjteam_jni_Main_init(JNIEnv* env, jobject thiz, jstring apkPath)
{	
	JNIUtil::_my_jnienv=env;

	const char* str;
	jboolean isCopy;
	str = env->GetStringUTFChars(apkPath, &isCopy);
	loadAPK(str);

	env->ReleaseStringUTFChars(apkPath, str);

	//appInit(); 
	////__android_log_print(ANDROID_LOG_INFO, "*************", "[INICIOU!!]");
	engine.init(sWindowWidth, sWindowHeight);
	glScreenInit( sWindowWidth, sWindowHeight );

	//	////__android_log_print(ANDROID_LOG_INFO, "*************", "[texID:] %d", texID);
	gAppAlive    = 1;
	sDemoStopped = 0;
	sTimeOffsetInit = 0;  

}

jboolean Java_br_gjteam_jni_Main_render(JNIEnv* env, jobject thiz, jint drawWidth, jint drawHeight, jboolean forceRedraw)
{
	long   curTime;

	////if(!sDemoStopped){
	//curTime = _getTime();

	//long elapsedTime=curTime-lastUpdate;
	////float finalElapsed=elapsedTime/1000000.0f;

	//if(elapsedTime>(1000000.0f/60.0f)){//60FPS
		lastUpdate=curTime;
		//engine.update(finalElapsed);
		engine.update(0);
		appRender(0, sWindowWidth, sWindowHeight);
	//}


	////}
}

void Java_br_gjteam_jni_Main_cleanup(JNIEnv* env)
{
	engine.deinit();
}

void Java_br_gjteam_jni_Main_shownScore(JNIEnv* env)
{
	engine.shownScore=true;
}

jboolean Java_br_gjteam_jni_Main_keyEvent(JNIEnv* env, jobject thiz, jint action, jint unicodeChar, jint keyCode, jobject keyEvent)
{
}

void Java_br_gjteam_jni_Main_setOffsets(JNIEnv* env, jobject thiz, jint x, jint y)
{
	sWindowWidth  = x;
	sWindowHeight = y;
	////__android_log_print(ANDROID_LOG_INFO, "resize", "resize w=%d h=%d", x, y);
	//lastUpdate=_getTime();  

}


