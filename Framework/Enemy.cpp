#include "Enemy.h"

void Enemy::InitHealthItem()
{
	itemTexture = Texture("Resources/items.png");
	healthItem = new HealthItem();
	healthItem->Init(&itemTexture, 0, 0);
	rocketItem = new RocketItem();
	rocketItem->Init(&itemTexture, 0, 0);
}

Enemy::Enemy()
{
	health = 100;
	takeDamage = 25;
	takeBombDamage = 100;
	InitHealthItem();
}

Enemy::~Enemy()
{
}

void Enemy::OnHitPlayer()
{
}

void Enemy::OnDie()
{
}
