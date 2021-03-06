#include "Zoomer.h"
#include "MainScene.h"
#define ZOOMERSPEED 70
Zoomer::Zoomer()
{
}

Zoomer::~Zoomer()
{
}

void Zoomer::SetScene(Scene * s)
{
	scene = s;
}

void Zoomer::Init(Texture * zoomerTexture, float x, float y, bool Direction)
{
	TexturePacker p = TexturePacker(zoomerTexture, "Resources/enemies_packer.xml");

	zoomerAnimation.AddRegion(p.GetRegion("zoomer"));
	zoomerAnimation.SetFrameInterval(0.04);

	SetRegion(*zoomerAnimation.GetKeyAnimation());
	SetSize(25, 25);
	SetPosition(x, y);

	_CategoryMask = ZOOMER;
	_BitMask = PLAYER | PLATFORM | PLAYER_BULLET | BOMB_EXPLOSION | OUTER_DOOR | DOOR | PLAYER_ROCKET;

	prevCollisionDirection = POINT(NOT_COLLIDED, NOT_COLLIDED);
	curCollisionDirection = POINT(NOT_COLLIDED, NOT_COLLIDED);

	setVelocity(ZOOMERSPEED, -ZOOMERSPEED);
	startVelocityX = _Velocity.x;
	initalDirection = Direction;
	cooldownAfterCollisionChange = 3;
	hitBulletTime = -1;


	t = 0;
	bx = false;
	by = false;
	fixedX = false;
	fixedY = true;
}

void Zoomer::Render(SpriteBatch * batch)
{
	if (isDead)
		return;

	batch->Draw(*this);
}

void Zoomer::UpdateVelocity(GameObject * player)
{
	if (startVelocityX > 0)
	{
		if (prevCollisionDirection.x != NOT_COLLIDED)
		{
			if (bx)
			{
				if (_Velocity.x > 0)
				{
					if (_Position.x > prevCollisionPosition.x)
					{
						_Velocity.y *= -1;
						bx = false;
						fixedX = true;
						SetRotation(GetRotation() + 90);
					}
				}
				else if (_Velocity.x < 0)
				{
					if (_Position.x < prevCollisionPosition.x)
					{
						_Velocity.y *= -1;
						bx = false;
						fixedX = true;
						SetRotation(GetRotation() + 90);
					}
				}
			}
		}
		if (prevCollisionDirection.y != NOT_COLLIDED)
		{
			if (by)
			{
				if (_Velocity.y < 0)
				{
					if (_Position.y < prevCollisionPosition.y)
					{
						_Velocity.x *= -1;
						by = false;
						fixedY = true;
						SetRotation(GetRotation() + 90);
					}
				}
				else if (_Velocity.y > 0)
				{
					if (_Position.y > prevCollisionPosition.y)
					{
						_Velocity.x *= -1;
						by = false;
						fixedY = true;
						SetRotation(GetRotation() + 90);
					}
				}
			}
		}
	}
	else
	{
		if (prevCollisionDirection.x != NOT_COLLIDED)
		{
			if (bx)
			{
				if (_Velocity.x > 0)
				{
					if (_Position.x > prevCollisionPosition.x)
					{
						_Velocity.y *= -1;
						bx = false;
						fixedX = true;
						SetRotation(GetRotation() - 90);
					}
				}
				else if (_Velocity.x < 0)
				{
					if (_Position.x < prevCollisionPosition.x)
					{
						_Velocity.y *= -1;
						bx = false;
						fixedX = true;
						SetRotation(GetRotation() - 90);
					}
				}
			}
		}
		if (prevCollisionDirection.y != NOT_COLLIDED)
		{
			if (by)
			{
				if (_Velocity.y < 0)
				{
					if (_Position.y < prevCollisionPosition.y)
					{
						_Velocity.x *= -1;
						by = false;
						fixedY = true;
						SetRotation(GetRotation() - 90);
					}
				}
				else if (_Velocity.y > 0)
				{
					if (_Position.y > prevCollisionPosition.y)
					{
						_Velocity.x *= -1;
						by = false;
						fixedY = true;
						SetRotation(GetRotation() - 90);
					}
				}
			}
		}
	}
}

void Zoomer::Update(float dt)
{
	if (health <= 0)
	{
		isActive = false;
		OnDie();
	}
	SetRegion(*zoomerAnimation.Next(dt));
}

void Zoomer::Update(float dt, Camera * cam)
{
	
}

void Zoomer::OnHitGround(POINT CollisionDirection)
{
	curCollisionDirection = CollisionDirection;
	POINT a = _Velocity;

	if (t == 1)
	{
		prevCollisionPosition = _Position;
		t = 0;
		bx = curCollisionDirection.x != NOT_COLLIDED ? true : false;
		by = curCollisionDirection.y != NOT_COLLIDED ? true : false;
	}
	if (curCollisionDirection != prevCollisionDirection)
	{
		t++;
		if (id == 27)
		{
			int a = 2;
		}
		if (fixedX || fixedY)
		{
			prevCollisionDirection = curCollisionDirection;
			fixedX = fixedY = false;
			return;
		}
		if (curCollisionDirection.x != NOT_COLLIDED)
		{
			_Velocity.y *= -1;
		}
		else if (curCollisionDirection.y != NOT_COLLIDED)
		{
			_Velocity.x *= -1;
		}
		if (startVelocityX > 0)
		{
			SetRotation(GetRotation() - 90);
		}
		else
		{
			SetRotation(GetRotation() + 90);
		}
	}
	prevCollisionDirection = curCollisionDirection;
}

void Zoomer::OnHitBullet()
{
	health -= takeDamage;
}

void Zoomer::OnHitBomb()
{
	health -= takeBombDamage;
}

void Zoomer::OnHitPlayer(GameObject* samus, POINT CollisionDirection)
{
	((Samus*)samus)->OnHitEnemy(this, CollisionDirection);
	Trace::Log("asdasdasdasd");
}

void Zoomer::OnDie()
{
	((MainScene*)scene)->explosionEffect.setPosition(_Position);
	((MainScene*)scene)->explosionEffect.Play();
	int a = rand() % 2;
	if (a == 0)
	{
		healthItem->setPosition(_Position);
		healthItem->SetScene(scene);
		((MainScene*)scene)->healthItems.push_back(healthItem);
	}
	else
	{
		rocketItem->setPosition(_Position);
		rocketItem->SetScene(scene);
		((MainScene*)scene)->rocketItems.push_back(rocketItem);
	}
}
