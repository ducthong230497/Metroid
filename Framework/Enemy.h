#pragma once
#include "HealthItem.h"
class Enemy
{
private:
	Texture healthTextTure;
	void InitHealthItem();
protected:
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