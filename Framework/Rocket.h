#pragma once
#include "Sprite.h"
#include "SpriteBatch.h"
#include "TexturePacker.h"
#include "Sound.h"
#define BULLETLIVETIME 0.3f
#define BULLETSPEED 5.0f

class Rocket : public Sprite
{
private:
	float stateTime;
	bool isDestroyed;
	//Sound
	CSound * HitEnemySound;
	CSound * HitRipperSound;
	CSound * HitDoor;
public:
	Rocket();
	~Rocket();

	Rocket(Texture* texture);

	bool isActive;
	void Render(SpriteBatch *batch);
	void Update(float dt) override;
	//void Release();

	bool IsDestroyed();

	float GetBulletSpeed();

	void OnHit();

	void OnHitRipper();

	void OnHitDoor();
};
