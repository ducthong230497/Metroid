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

	prevCollisionDirection.x = NOT_COLLIDED;
	prevCollisionDirection.y = -prevVelocity.y * 100;
	curCollisionDirection.x = NOT_COLLIDED;
	curCollisionDirection.y = -prevVelocity.y * 100;

	//set up intial velocity/direction
	if (Direction)
	{
		prevVelocity.x = 0.7f;
		prevVelocity.y = -0.7f;

		setVelocity(0.7f, -0.7f);
		setVelocity(0, 0);
	}
	else
	{
		prevVelocity.x = -0.7f;
		prevVelocity.y = -0.7f;

		setVelocity(-0.7f, -0.7f);
	}
	initalDirection = Direction;
	cooldownAfterCollisionChange = 3;
	health = 2;
	hitBulletTime = -1;
}

void Zoomer::Render(SpriteBatch * batch)
{
	if (isDead)
		return;

	batch->Draw(*this);
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
