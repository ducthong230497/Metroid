#pragma once
#include "HealthItem.h"
#include "RocketItem.h"
class Enemy
{
private:
	void InitHealthItem();
protected:
	Texture itemTexture;
	int health;
	int takeDamage;
	int takeBombDamage;
	HealthItem * healthItem;
	RocketItem * rocketItem;
public:
	Enemy();
	~Enemy();
	virtual void OnHitPlayer(GameObject* samus, POINT CollisionDirection);
	virtual void OnDie();
};