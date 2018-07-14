#pragma once
#include "Sprite.h"
#include "Animation.h"
#include "SpriteBatch.h"
#include "TexturePacker.h"

#define EFFECTLIVETIME 0.2
class Explosion :public Sprite
{
private:
	Animation explosionAnimation;

	float stateTime;

public:
	Explosion();
	~Explosion();

	void Init(Texture* explosionTexture);

	void Update(float dt) override;

	void Render(SpriteBatch *batch);

	void Play();

};

