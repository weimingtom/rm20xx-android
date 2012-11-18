#ifndef ENGINE_H
#define ENGINE_H

#include <stdio.h>
#include <vector>
#include <map>

//#include "font/PreCompile.h"
//#include "font/FontAtlas.h"
//#include "font/FTBitmapFont.h"

#include "Vector2D.h"
#include <sstream>
#include <string>
#include "gl2d.h"

class Engine{
public:
	~Engine();
	void init(int width, int height);
	void deinit();
	void update(float time);
	void render(float time);
	void playing(float time);

	void reset();

	enum STATE{
		LOGO1,
		LOGO2,
		TITLE,
		INSTRUCT,
		STARTER,
		PLAYING,
		GAMEOVER
	};
	
	bool touch;
	bool doubleTouch, shownScore;
	STATE gameState;

	bool isShooting;

	Vector2D<float> controlKnobPos;
	Vector2D<float> controlBasePos;

private:
	
	int score, lastScore;
	int starter;
	std::string starterStr;

	//FontAtlas* m_pFontAtlas;

	int frames;
	int width, height;
	std::string scoreStr;

	glImage* controlBase;
	glImage* controlKnob;

};

#endif