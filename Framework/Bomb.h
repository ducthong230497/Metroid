#pragma once

#include "Scene.h"
#include "Sprite.h"
#include "SpriteBatch.h"
#include "Animation.h"
#include "TexturePacker.h"
#include "Explosion.h"

#define BOMBLIVETIME 0.6
#define EXPLOSIONLIVETIME 0.9

class Bomb : public Sprite
{
private:
	float stateTime;
	bool isDestroyed;

	Explosion explosionEffect;

	Animation animation;

	Scene *scene;
public:
	Bomb();
	~Bomb();

	Bomb(Texture* texture);
	void SetScene(Scene * scene);

	void Render(SpriteBatch *batch);
	void Update(float dt) override;

	bool IsDestroyed();

	void OnHitEnemy();

	void Play();

};

