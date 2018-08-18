#pragma once

#include "Game.h"
#include "SceneManager.h"

class Metroid : public Game
{
protected:
	SceneManager * sceneManager;
	void OnKillFocus();
	void OnSetFocus();


	virtual void RenderFrame();
	virtual void ProcessInput();
public:
	Metroid();
	~Metroid();
	void Init(HINSTANCE hInstance, LPCWSTR name, UINT width, UINT height, bool isfullscreen, UINT framerate);
};