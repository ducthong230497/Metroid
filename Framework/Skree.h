#pragma once
#include "Sprite.h"
#include "Animation.h"
#include "TexturePacker.h"
#include "SpriteBatch.h"
#include "Scene.h"
#include "Enemy.h"

#define SKREELIVETIMEAFTERGROUND 0.5f
#define SKREEBULLETLIVETIME 1
#define MAXHITBULLETTIME 0.2
#define PI 3.14159265

class Skree : public Sprite, public Enemy
{
private:
	Animation skreeAnimation;
	Animation skreeHitAnimation;
	Animation skreeBulletAnimation;
	std::vector<Sprite*> SkreeBullets;
	float stateTime;
	float hitBulletTime;
	bool isHitGround;
	bool isDead;
	friend class CollisionCallback;
	Scene *scene;
	//CSound *isShot = NULL;
public:
	Skree();
	~Skree();

	void Init(Texture * skreeTexture, float x, float y);
	void SetScene(Scene* scene);
	void UpdateVelocity(GameObject * Player) override;
	void Update(float dt) override;

	void Render(SpriteBatch * batch);
	void OnHitGround();
	void OnHitBullet();
	void OnHitBomb();
	void OnDie();
	void OnHitPlayer() override;
	bool IsHitGround();
	bool IsDead();
	int GetHealth();
};