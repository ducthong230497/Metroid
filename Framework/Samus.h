﻿#ifndef _SAMUS_H
#define _SAMUS_H
#include "SpriteBatch.h"
#include "Sprite.h"
#include "TexturePacker.h"
#include "Animator.h"
#include "Bullet.h"
#include "Scene.h"
#include "Sound.h"
#include "Rocket.h"

#define FIRERATE 0.25f
#define FIRERATEROCKET 0.25f
#define JUMP_1 90;
#define JUMP_2 200;
#define IME 0.4f;

class CKeyboard;
class Samus : public Sprite
{
private:

	int _SPEED, _JUMPFORCE, _ACCELERATION, _PULLINGFORCE, _PUSHFORCE;

	Scene * scene;
	
	Texture samusTexture;
	std::vector<Bullet*> bullets;
	std::vector<Rocket*> rockets;
	float lastShootTime;
	float maxJumpHeight = 0;
	int count = 0, countRoll = 0;
	bool isAppear;
	void InitSamusAnimation(Texture * samusTexture);
	void HandleAnimation();

	void CreateParameter();
	void CreateAnimation_();
	void CreateTransition();
	void LoadImageAnimation(Texture * samusTexture);
	void LoadAnimator();

	//deadEffect
	std::vector<Sprite> deadEffect;

	bool facingRight;
	bool shootRocket;
	bool isDead;
	bool playDeadSound;

	int health;
	int rocket;
	float lastBombTime;
	float deadTime;
	float invincibleTime;
	unsigned long defaultBitMask;
	//sound
	CSound * ShootSound;
	CSound * JumpSound;
	CSound * moveSound;
	CSound * deathSound;

public:
	GameObject * head;
	float dt;
	Animator  animator;
	Animation_
		Appear,
		Stand, StandShoot, StandUp,
		Move, MoveShoot, MoveUp,
		Jump, JumpShoot, JumpUp, JumpRoll,
		Roll,
		Hit;

	bool canRoll, onGround, move, roll, lookUp, canStand, start,shoot,down, canControl, moveThroughDoor, hitEnemy, activeJump,fall, hitBullet,triggerRoll;
	float jumpTime = 0.f;
	Samus();
	void SetScene(Scene *s);
	void Init(Texture * samusTexture, float x, float y);
	void Render(SpriteBatch *batch);
	void Update(float dt) override;
	void ProcessInput(CKeyboard * KeyBoard);
	void OnHitGround(POINT direction);
	void OnExitGround(POINT direction);
	void OnShoot();
	void OnJump();
	void OnHitItem();
	void OnHitHealthItem();
	void OnHitRocketItem();
	void OnHitEnemy(GameObject *enemy, POINT CollisionDirection);
	void OnHitEnemyBullet();
	void OnHitBomb();
	void SetCanRoll();
	int getHealth();
	int getNumberRocket();
	bool IsDead();
	bool PlayDeadSound();
	void UpdateVelocity(GameObject* obj);
	void OnTriggerEnter();
};

#endif