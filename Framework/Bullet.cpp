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
	_BitMask = SKREE | ZOOMER | RIO | RIPPER | PLATFORM | BREAKABLE_PLATFORM | KRAID | DOOR | OUTER_DOOR | ZEEBETITE;

	HitEnemySound = Sound::LoadSound("Resources/Audio/ShootEnemies.wav");
	HitRipperSound = Sound::LoadSound("Resources/Audio/ShootRipper.wav");
	HitDoor = Sound::LoadSound("Resources/Audio/DoorSound.wav");
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
	Sound::Play(HitEnemySound);
}

void Bullet::OnHitRipper()
{
	stateTime = 100; //force to be dead
	Sound::Play(HitRipperSound);
}

void Bullet::OnHitDoor()
{
	stateTime = 100; //force to be dead
	Sound::Play(HitDoor);
}
