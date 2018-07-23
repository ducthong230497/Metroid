#pragma once
#include "HealthItem.h"
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
public:
	Enemy();
	~Enemy();
	virtual void OnHitPlayer();
	virtual void OnDie();
};