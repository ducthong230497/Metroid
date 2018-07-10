#pragma once
#include "Sprite.h"
#include "SpriteBatch.h"
#include "TexturePacker.h"
#define BULLETLIVETIME 0.3f
#define BULLETSPEED 5.0f

class Bullet : public Sprite
{
private:
	float stateTime;
	bool isDestroyed;
public:
	Bullet();
	~Bullet();

	Bullet(Texture* texture); 

	bool isActive;
	void Render(SpriteBatch *batch);
	void Update(float dt) override;
	//void Release();

	bool IsDestroyed();

	float GetBulletSpeed();

	void OnHitEnemy();
};
