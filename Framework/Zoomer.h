#pragma once
#include "Sprite.h"
#include "Animation.h"
#include "SpriteBatch.h"
#include "TexturePacker.h"
#include "Enemy.h"
#include "Scene.h"

#define NOT_COLLIDED 10000.0f
#define MAXHITBULLETTIME 0.2
#define MAXHITPLAYERHITTIME 0.3
#define DAMAGE 25;
#define BOMBDAMAGE 100;

class Zoomer : public Sprite, public Enemy
{
private:
	Animation zoomerAnimation;

	bool isDead;
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
	int t = 0;	// this variable ensure the UpdateVelocity run 1 frame later than OnHitGround
	bool bx = false; //this variable ensure the UpdateVelocity run 2 frame later than OnHitGround
	bool by = false; //this variable ensure the UpdateVelocity run 2 frame later than OnHitGround
	bool fixedX = false; //this variable prevent rotation and change velocity in some special cases
	bool fixedY = false; //this variable prevent rotation and change velocity in some special cases

	// Either from OnCollisionEnter or OnColliding
	int prevSource;

	Scene * scene;
public:
	float startVelocityX;

	Zoomer();
	~Zoomer();

	void SetScene(Scene* s);

	void Init(Texture *zoomerTexture, float x, float y, bool Direction);

	void Render(SpriteBatch *batch);

	void UpdateVelocity(GameObject *player) override;

	void Update(float dt) override;

	void Update(float dt, Camera *cam);

	void OnHitGround(POINT collisionDirection);

	void OnHitBullet();

	void OnHitBomb();

	void OnHitPlayer(GameObject* samus, POINT CollisionDirection) override;

	void OnDie() override;

	void SetCurCollisionDirection(POINT collisionDirection, int source);

	int GetHealth();

};