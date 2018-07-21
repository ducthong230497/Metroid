#pragma once
#include "Sprite.h"
#include "SpriteBatch.h"
#include "Animation.h"
#include "TexturePacker.h"
#include "Enemy.h"
#include "Scene.h"

class Kraid : public Sprite, public Enemy
{
private:
	Animation animation;
	Animation behingHitAnimation;

	Scene * scene;

	int health;

	//Player* player;

	float stateTime; //used for flipping skree 

	float bulletStateTime;

	float boomerangStateTime;

	float lastShot;

	std::vector<GameObject*> bullets;
	std::vector<GameObject*> boomerangs;
	Animation boomerangAnimation;


	float hitBulletTime; //used when being hit by bullet
public:
	Kraid();
	~Kraid();

	void Init(Texture *texture, int x, int y);

	void UpdateVelocity(GameObject* player) override;

	void Update(float dt);

	void Render(SpriteBatch *batch);

	void OnHitBullet();

	void OnHitBomb();

	bool IsDead();
};