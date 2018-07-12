#pragma once
#include "Sprite.h"
#include "Animation.h"
#include "SpriteBatch.h"
#include "TexturePacker.h"

#define NOT_COLLIDED 10000.0f
#define MAXHITBULLETTIME 0.2
#define MAXHITPLAYERHITTIME 0.3

class Zoomer : public Sprite
{
private:
	Animation zoomerAnimation;

	bool isDead;
	int health;
	float hitBulletTime;
	float hitPlayerTime;
	bool initalDirection;
	bool outsideOfCamera;
	
	POINT prevCollisionPosition;
	POINT prevCollisionDirection;
	POINT curCollisionDirection;
	POINT prevVelocity;

	// Zoomer will continue to move in the same direction if on cooldown
	int cooldownAfterCollisionChange;

	// Keeps track of where the curCollisionDirection was being set from
	// Either from OnCollisionEnter or OnColliding
	int prevSource;

public:

	Zoomer();
	~Zoomer();

	void Init(Texture *zoomerTexture, float x, float y, bool Direction);

	/*void HandlePhysics();*/

	void Render(SpriteBatch *batch);

	void UpdateVelocity(GameObject *player) override;

	void Update(float dt) override;

	void Update(float dt, Camera *cam);

	void OnHitGround(POINT collisionDirection);

	void OnHitBullet();

	void OnHitBomb();

	void OnHitPlayer();

	void StickToGround();

	void SetCurCollisionDirection(POINT collisionDirection, int source);

	int GetHealth();

};