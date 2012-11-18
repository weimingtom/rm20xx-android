#include "Engine.h"
#include "gl2d.h"
#include "gmUtil.h"

#include <time.h>

#include <android/log.h>

//#include "font/FTBitmapChar.h"
//#include "font/ResManager.h"
#include "jniUtil.h"

#include "rm_src/wcrpg.h"


const char* szLetters = " !\"#&'()*,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ\\_abcdefghijklmnopqrstuvwxyzÁáÉéÍíÑñÓóÚú";


void Engine::init(int width_, int height_){
	 
	width=width_;
	height=height_;
	//srand(time(NULL));

	//deinit();

	//m_pFontAtlas = new FontAtlas();
	//m_pFontAtlas->AddFont(joystix, joystix_size, 36, szLetters);
	//m_pFontAtlas->AddFont(joystix, joystix_size, 60, szLetters);
	//m_pFontAtlas->AddFont(joystix, joystix_size, 12, szLetters);
	//m_pFontAtlas->CreateAtlas();
	
	//JNIUtil::jni_music_play("lazy_space.ogg");
	reset();

	controlBase=new glImage();
	controlKnob=new glImage();

	glLoadTileSet("onscreen_control_base.png",controlBase,128,128,128,128);  
	glLoadTileSet("onscreen_control_knob.png",controlKnob,64,64,64,64);  

	controlBasePos.x=100;
	controlBasePos.y=380;
	controlKnobPos=controlBasePos;

	init_();

	//gameState=LOGO1;
	gameState=PLAYING;

	//gameState=TITLE;
}

Engine::~Engine(){
	//deinit();

}

void Engine::reset(){
	//__android_log_print(ANDROID_LOG_INFO, "adMOB", "Engine::reset()");
	frames=0; 

	gameState=PLAYING; 
	shownScore=false;

	touch=false;
	isShooting=false;
}

void Engine::deinit(){
	delete controlBase;
	delete controlKnob;

	destroy_();	
}
	

void Engine::update(float time){
	if(gameState==LOGO1){
		if(frames>180){
			frames=0;

			gameState=LOGO2;
			////__android_log_print(ANDROID_LOG_INFO, "adMOB", "render [1] %d %d", gameState ,frames);
		}
	}

	else if(gameState==LOGO2){
		if(frames>180){
			frames=0;

			gameState=TITLE;
			////__android_log_print(ANDROID_LOG_INFO, "adMOB", "render [1] %d %d", gameState ,frames);
		}
	}
	else{
		if(gameState==PLAYING || gameState==GAMEOVER)
			playing(time);
	}
	frames++;
}

void Engine::render(float time){
	 
	if(gameState==LOGO1){ 
		//glSpriteNoScale(0,0,0,lazyBrain);
		////__android_log_print(ANDROID_LOG_INFO, "adMOB", "render %d", glGetError());
	}
	else if(gameState==LOGO2){
		//glSpriteNoScale(0,0,0,greenJuiceTeam);
		////__android_log_print(ANDROID_LOG_INFO, "adMOB", "render %d", glGetError());
	}else{
		spriteBatchBegin();
		render_();

		glSpriteNoScaleCenter(controlBasePos.x,controlBasePos.y, 0,controlBase,1,1,1,0.5);
		glSpriteNoScaleCenter(controlKnobPos.x,controlKnobPos.y, 0,controlKnob,1,1,1,0.5);

		spriteBatchEnd();

		


	}
	__android_log_print(ANDROID_LOG_INFO, "glError", "glError: %d", glGetError());
}




void Engine::playing(float time){ 
} 