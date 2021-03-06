﻿#pragma once
#include "Scene.h"
#include "SpriteBatch.h"
#include "Texture.h"
#include "CKeyboard.h"
#include "Animation.h"
#include "TexturePacker.h"
#include "Sprite.h"
#include "CWindow.h"
#include "Sound.h"
class IntroScene : public Scene
{
private:
	Camera * cam;
	Texture introSceneTexture;
	Animation introSceneAnimation;
	SpriteBatch * batch;
	Sprite background;
	
	CKeyboard * KeyBoard;
	CWindow * Window;
	//thiếu sound
	static CSound * TitleTheme;
public:
	IntroScene();
	~IntroScene();

	static CSound* getSound();

	void Init() override;
	//void Update() override;
	eSceneID Render() override;
	void ProcessInput() override;
	void End() override;

	DEFINE_SCENE_UID(INTROSCENE)

};