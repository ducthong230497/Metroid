#pragma once

#include "Sprite.h"
#include "SpriteBatch.h"
#include "Animation.h"
#include "TexturePacker.h"
#include "Scene.h"

class Effect :public Sprite //this is not a really good design but I'm just so lazy to change this
{
private:
	Animation _Animation;

	float _StateTime;

	float _LiveTime;

public:
	Effect();
	~Effect();

	Effect(const Animation &ani);

	void Update(float dt);

	void Render(SpriteBatch *batch);

	void Play();

	void SetLiveTime(float liveTime);

};