#pragma once
#include "Sprite.h"
#include "Animation.h"
#include "TexturePacker.h"
#include "SpriteBatch.h"

#define SKREELIVETIMEAFTERGROUND 0.5f
#define SKREEBULLETLIVETIME 1
#define MAXHITBULLETTIME 0.2
#define PI 3.14159265

class Skree : public Sprite
{
private:
	Animation skreeAnimation;
	Animation skreeHitAnimation;
	Animation skreeBulletAnimation;
	std::vector<Sprite> SkreeBullets;
	float stateTime;
	float hitBulletTime;
	int Health;
	bool isHitGround;
	bool isDead;
	friend class CollisionCallback;
	//CSound *isShot = NULL;
public:
	Skree();
	~Skree();

	void Init(Texture * skreeTexture, float x, float y);
	void UpdateVelocity(GameObject * Player) override;
	void Update(float dt) override;

	void Render(SpriteBatch * batch);
	void OnHitGround();
	void OnHitBullet();
	void OnDie();
	bool IsHitGround();
	bool IsDead();
	int GetHealth();
};