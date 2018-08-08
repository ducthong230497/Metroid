#pragma once
#include "Enemy.h"
#include "Sprite.h"
#include "Animation.h"
#include "SpriteBatch.h"
#include "TexturePacker.h"
#include "Scene.h"

#define MAXHITBULLETTIME 0.05
#define MAXHITPLAYERHITTIME 0.2

class Rio : public Sprite, public Enemy
{
private:
	Scene * scene;

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
	float hitBulletTime;
	float hitPlayerTime;

	POINT previousVelocity;

	POINT startVelo1;
	POINT startVelo2;

	float deacceleration;

	POINT midVelo1;
	POINT midVelo2;

	float acceleration;

public:
	Rio();
	~Rio();

	void SetScene(Scene * s);

	void Init(Texture *rioTexture, float x, float y);

	void UpdateVelocity(GameObject *player) override;

	void Render(SpriteBatch *batch);

	void Update(float dt);

	void OnHitGround();

	void OnHitRoof();

	void OnHitLeft();

	void OnHitRight();

	bool IsDead();

	void OnHitPlayer(GameObject* samus, POINT CollisionDirection) override;

	void OnHitBomb();

	void OnHitBullet();

	void OnDie();
};

