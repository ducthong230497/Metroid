#pragma once

#include "Sprite.h"
#include "SpriteBatch.h"
#include "Animation.h"
#include "TexturePacker.h"
//#include "ExplosionEffect.h"

#define BOMBLIVETIME 0.6
#define EXPLOSIONLIVETIME 0.9

class Bomb : public Sprite
{
private:
	float stateTime;
	bool isDestroyed;

	//ExplosionEffect explosionEffect;

	Animation animation;

public:
	Bomb();
	~Bomb();

	Bomb(Texture* texture);

	void Render(SpriteBatch *batch);
	void Update(float dt) override;
	//void Release();

	bool IsDestroyed();

	void OnHitEnemy();

	void Play();

};

