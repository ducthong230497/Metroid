#pragma once
#include "Sprite.h"
#include "SpriteBatch.h"
#include "Animation.h"
#include "TexturePacker.h"
#include "Enemy.h"
#include "BombItem.h"
#include "Scene.h"

#define MAXHITBULLETTIME 0.2f
#define TURNINGDELAYTIME 1.5f
#define KRAIDBULLETLIVETIME 1.5f
#define KRAIDBOOMERANGLIVETIME 1.0f
#define GRAVITY -1000.0f
class KraidBoomerang : public Sprite
{
public:
	void UpdateVelocity(GameObject* object) override
	{
		_Velocity.y += GRAVITY * 2 * 0.016f;
	}
};

class Kraid : public Sprite, public Enemy
{
private:
	Animation animation;
	Animation behingHitAnimation;
	GameObject * player;
	Scene * scene;

	//Player* player;

	float stateTime; //used for flipping skree 

	float bulletStateTime;

	float boomerangStateTime;

	float lastShot;

	Animation boomerangAnimation;


	float hitBulletTime; //used when being hit by bullet
public:
	std::vector<GameObject*> bullets;
	std::vector<GameObject*> boomerangs;
	Kraid();
	~Kraid();

	void SetScene(Scene *s);

	void SetPlayer(GameObject* p);

	void Init(Texture *texture, int x, int y);

	void UpdateVelocity(GameObject* player) override;

	void Update(float dt);

	void Render(SpriteBatch *batch);

	void OnHitBullet();

	void OnHitBomb();

	void OnDie();

	bool IsDead();
};