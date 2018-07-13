#include "Zoomer.h"

Zoomer::Zoomer()
{
}

Zoomer::~Zoomer()
{
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
	_BitMask = PLAYER | PLATFORM | PLAYER_BULLET | BOOM_EXPLOSION;

	/*prevCollisionDirection.x = NOT_COLLIDED;
	prevCollisionDirection.y = -prevVelocity.y * 100;
	curCollisionDirection.x = NOT_COLLIDED;
	curCollisionDirection.y = -prevVelocity.y * 100;*/
	prevCollisionDirection = POINT(NOT_COLLIDED, NOT_COLLIDED);
	curCollisionDirection = POINT(NOT_COLLIDED, NOT_COLLIDED);

	setVelocity(70, -70);
	//setVelocity(170, -170);

	////set up intial velocity/direction
	//if (Direction)
	//{
	//	prevVelocity.x = 0.7f;
	//	prevVelocity.y = -0.7f;

	//	setVelocity(0.7f, -0.7f);
	//	setVelocity(0, 0);
	//}
	//else
	//{
	//	prevVelocity.x = -0.7f;
	//	prevVelocity.y = -0.7f;

	//	setVelocity(-0.7f, -0.7f);
	//}
	initalDirection = Direction;
	cooldownAfterCollisionChange = 3;
	health = 2;
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
//int t = 0;
//bool bx = false;
//bool by = false;
//bool fixedX = false;
//bool fixedY = false;
void Zoomer::UpdateVelocity(GameObject * player)
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

void Zoomer::Update(float dt)
{
	SetRegion(*zoomerAnimation.Next(dt));
}

void Zoomer::Update(float dt, Camera * cam)
{
	//if (body->GetPosition().x > cam->GetPosition().x - screenWidth / 2 && body->GetPosition().x < cam->GetPosition().x + screenWidth / 2
	//	&&
	//	(body->GetPosition().y - body->GetSize().y / 2 - 5) < (cam->GetPosition().y + screenHeight / 2) && (body->GetPosition().y + body->GetSize().y / 2 + 5) > (cam->GetPosition().y - screenHeight / 2)) {
	//	outsideOfCamera = false;
	//	body->SetBodyType(Body::BodyType::Kinematic);
	//	if (isDead) return;

	//	if (health <= 0)
	//	{
	//		isDead = true;
	//		world->DestroyBody(body);
	//		return;
	//	}

	//	if (hitBulletTime == -1) //-1 means not being hit by bullet
	//	{
	//		SetRegion(*zoomerAnimation.Next(dt));
	//	}
	//	else
	//	{
	//		if (hitBulletTime < MAXHITBULLETTIME)
	//		{
	//			hitBulletTime += dt;
	//		}
	//		else
	//		{
	//			hitBulletTime = -1;
	//			body->SetBodyType(Body::BodyType::Kinematic);
	//		}
	//	}

	//	if (hitPlayerTime != -1)
	//	{
	//		if (hitPlayerTime < MAXHITPLAYERHITTIME)
	//		{
	//			hitPlayerTime += dt;
	//		}
	//		else
	//		{
	//			hitPlayerTime = -1;
	//			body->maskBits = PLAYER_BIT | PLATFORM_BIT | BULLET_BIT | EXPLOSION_BIT | BREAKABLEPLATFORM_BIT | DOOR_BIT;
	//		}
	//	}

	//	//set sprite position
	//	if (body != NULL)
	//		this->SetPosition(body->GetPosition().x, body->GetPosition().y);

	//	//body->SetVelocity(0.5f, -0.5f);
	//	if (body->GetBodyType() != Body::BodyType::Static)
	//	{
	//		StickToGround();
	//	}
	//}
	//else {
	//	body->SetBodyType(Body::BodyType::Static);
	//	outsideOfCamera = true;
	//}
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
		if (fixedX || fixedY)
		{
			prevCollisionDirection = curCollisionDirection;
			//fixedX = fixedX == true ? false : false;
			//fixedY = fixedY == true ? false : false;
			fixedX = fixedY = false;
			return;
		}
		if (curCollisionDirection.x != NOT_COLLIDED)
		{
			_Velocity.y *= -1;
		}
		else if(curCollisionDirection.y != NOT_COLLIDED)
		{
			_Velocity.x *= -1;
		}
		SetRotation(GetRotation() - 90);
	}
	prevCollisionDirection = curCollisionDirection;
}
