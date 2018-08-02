#pragma once

#include "Sprite.h"
#include "SpriteBatch.h"
#include "Animation.h"
#include "TexturePacker.h"
#include "Scene.h"
#include "Effect.h"

class CannonBullet : public Sprite
{
public:
	Sprite * cannon;
	void OnHitGround();
};

class Cannon : public Sprite
{
public:
	enum Type { Left, Right, Top };
private:
	Animation cannonAnimation;
	Animation bulletAnimation;

	Scene* scene;

	float lastShot;

	float stateTime; //
public:
	bool isDestroyed;
	Effect bulletExplosionEffect;
	CannonBullet * cannonBullet;
	Cannon();
	~Cannon();
	void SetScene(Scene * s);

	void Init(Texture *texture, Cannon::Type type, int randomIndex, int x, int y);

	void Render(SpriteBatch *batch);

	void Update(float dt) override;
};
