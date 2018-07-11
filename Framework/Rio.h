#pragma once
#include "Enemy.h"
#include "Sprite.h"
#include "Animation.h"
#include "SpriteBatch.h"
#include "TexturePacker.h"

#define MAXHITBULLETTIME 0.05
#define MAXHITPLAYERHITTIME 0.2

class Rio : public Sprite, public Enemy
{
private:
	Animation rioAnimation;

	bool phase1;

	bool phase2;

	bool right;

	bool still;

	bool isHitRoof;

	bool isHitGround;

	bool isHitRight;

	bool isHitLeft;

	bool jumpCheck;

	float delayTime;
	float delayTimeDuringGame;

	bool isDead;
	int health;
	float hitBulletTime;
	float hitPlayerTime;

	POINT startVelo1;
	POINT startVelo2;

	float deacceleration;

	POINT midVelo1;
	POINT midVelo2;

	float acceleration;

public:
	Rio();
	~Rio();
	void Init(Texture *rioTexture, float x, float y);

	void UpdateVelocity(GameObject *player) override;

	void Render(SpriteBatch *batch);

	void Update(float dt);

	void OnHitGround();

	void OnHitRoof();

	void OnHitLeft();

	void OnHitRight();

	bool IsDead();

	int GetHealth();

	void OnHitPlayer() override;

	void OnHitBomb();

	void OnHitBullet();
};

