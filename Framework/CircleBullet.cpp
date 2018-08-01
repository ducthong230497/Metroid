#include "CircleBullet.h"

#define FIRERATE 2
#define CIRCLEBULLETLIVETIME 3
#define SPEED 100 

Rinka::Rinka()
{
}

Rinka::~Rinka()
{
}

void Rinka::SetScene(Scene * s)
{
	scene = s;
}

void Rinka::SetPlayer(GameObject * p)
{
	player = p;
}

void Rinka::Init(Texture * enemiesTexture, int x, int y)
{
	TexturePacker p = TexturePacker(enemiesTexture, "Resources/enemies_packer.xml");

	SetRegion(p.GetRegion("circlecannon").at(2));

	basePosition.x = x;
	basePosition.y = y;
	SetPosition(x, y);
	SetSize(16, 16);

	collisionType = Dynamic;

	_CategoryMask = CIRCLEBULLET;
	_BitMask = PLAYER;

	isDestroyed = true;
}

void Rinka::Update(float dt)
{
	float currentTime = GetTickCount();
	if (currentTime > lastShootTime + FIRERATE)
	{
		if (isDestroyed)
		{
			stateTime = 0;
			isDestroyed = false;
			lastShootTime = currentTime;

			POINT offset = player->getPosition() - _Position;

			float angle = atan(offset.y / offset.x) * 180 / Pi;

			if (offset.x < 0) angle += 180;

			_Velocity = POINT(cos(angle * Pi / 180) * SPEED, sin(angle * Pi / 180) * SPEED);

			int a = 2;

		}
	}
	else
	{
		if (stateTime >= 0)
		{
			stateTime += dt;
			if (stateTime > CIRCLEBULLETLIVETIME)
			{
				isDestroyed = true;
				stateTime = -1;
			}
		}
	}
}

void Rinka::Render(SpriteBatch * batch)
{
	batch->Draw(*this);
}
