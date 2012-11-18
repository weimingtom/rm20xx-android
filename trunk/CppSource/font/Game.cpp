#include "PreCompile.h"
#include "Game.h"
#include "Util.h"
#include "FTBitmapFont.h"
#include "FontAtlas.h"
#include "ResManager.h"
#include "FTBitmapChar.h"


Game::Game() 
{
}

Game::~Game()
{
	g_resManager.Release();
}

void Game::Init(const string& dirResources, const string& dirUser, int screenWidth, int screenHeight)
{
#ifndef APPSTORE_BUILD
	int timeNow = GetMilliSeconds();
#endif	
	try
	{
		g_resManager.Init(dirResources, dirUser, screenWidth, screenHeight);
	}
	catch(RacException ex)
	{
		m_strErr = ex.GetStrErr();
		cout << ex.GetStrErr() << endl;
		return;
	}
	m_timeLast = GetMilliSeconds();
#ifndef APPSTORE_BUILD
	int timeElapsed = abs(m_timeLast-timeNow);
	cout << "startup time:" << timeElapsed << "ms" << endl;
#endif
}	// Init




void Game::Render()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	FTBitmapFont* pArialFont = g_resManager.GetFTFont(0);
	pArialFont->DrawString(5, 0, "AV Arial font 12 point", BLACK);
	FTBitmapFont* pTimesFont = g_resManager.GetFTFont(1);
	pTimesFont->DrawString(5, pArialFont->GetLineHeight(), "AV Times New Roman font 18 point", BLACK);
	FTBitmapFont* pTimesFont2 = g_resManager.GetFTFont(2);
	pTimesFont2->DrawString(5, pArialFont->GetLineHeight()+pTimesFont->GetLineHeight(), 
							"AV Times New Roman font 16 point", BLACK);
	g_resManager.GetFontAtlas()->RenderAtlas(5, 300);
	g_resManager.GetBatcher().RenderCurr();
}




