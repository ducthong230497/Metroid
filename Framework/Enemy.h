#pragma once

class Enemy
{
protected:
	int health;
public:
	Enemy();
	~Enemy();
	virtual void OnHitPlayer();
};