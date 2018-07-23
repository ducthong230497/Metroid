#include "Bullet.h"

Bullet::Bullet()
{
}

Bullet::~Bullet()
{
}

Bullet::Bullet(Texture * texture)
{
	isDestroyed = false;
	stateTime = 0;

	//get bullet 
	TexturePacker p = TexturePacker(texture, "Resources/samusaran_packer.xml");
	SetRegion(p.GetRegion("bullet").front());

	SetSize(6, 7);
	setVelocity(400, 0);
	//body definition
	collisionType = CollisionType::Dynamic;
	setSize(6, 7);

	//create body
	_CategoryMask = PLAYER_BULLET;
	_BitMask = SKREE | ZOOMER | RIO | RIPPER | PLATFORM | BREAKABLE_PLATFORM | KRAID /*| MOTHERBRAIN_BIT | HEALTHPILE_BIT | KRAID_BIT | DOOR_BIT*/;
}

void Bullet::Render(SpriteBatch * batch)
{
	if (!isDestroyed)
	{
		batch->Draw(*this);
	}
}

void Bullet::Update(float dt)
{
	//mainBody.SetPosition(mainBody.GetPosition().x+10, mainBody.GetPosition().y);
	//setPosition(mainBody->GetPosition().x, mainBody->GetPosition().y);
	stateTime += dt;
	if (stateTime > BULLETLIVETIME)
	{
		//world->DestroyBody(mainBody);
		//mainBody = NULL;
		isDestroyed = true;
	}
}

bool Bullet::IsDestroyed()
{
	return isDestroyed;
}

float Bullet::GetBulletSpeed()
{
	return BULLETSPEED;
}

void Bullet::OnHit()
{
	stateTime = 100; //force to be dead
}
