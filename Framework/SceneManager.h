#pragma once
#include "define.h"
#include "Scene.h"
#include "testScene_1.h"
#include "testScene_2.h"
#include "IntroScene.h"
#include "MenuScene.h"
#include "MainScene.h"
#include "GameOverScene.h"
#include "Texture.h"
#include "SpriteBatch.h"
#ifdef _DEBUG
#include "CKeyboard.h"
#endif

class SceneManager
{
private:
	Scene * CurrentScene;
	eSceneID CurrentID, NextID;
	Texture *BlackScreen;
	UINT BlackScreenDelay;
#ifdef _DEBUG
	CKeyboard *Keyboard;
#endif

	SceneManager();
	bool CloseScene();
	Scene* get(eSceneID uid);

	static SceneManager* Instance;
public:
	~SceneManager();

	Scene *getCurrentScene() const;
	eSceneID getCurrentID() const;

	void Init();
	void Render();
	void ProcessInput();

	static SceneManager* getInstance();
};

