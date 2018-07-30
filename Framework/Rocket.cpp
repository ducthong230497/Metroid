#include "Rocket.h"

Rocket::Rocket()
{
}

Rocket::~Rocket()
{
}

Rocket::Rocket(Texture * texture)
{
	isDestroyed = false;
	stateTime = 0;

	//get bullet 
	TexturePacker p = TexturePacker(texture, "Resources/samusaran_packer.xml");
	SetRegion(p.GetRegion("rocket").front());

	SetSize(24, 16);
	setVelocity(400, 0);
	//body definition
	collisionType = CollisionType::Dynamic;

	//create body
	_CategoryMask = PLAYER_ROCKET;
	_BitMask = SKREE | ZOOMER | RIO | RIPPER | PLATFORM | BREAKABLE_PLATFORM | KRAID | DOOR | OUTER_DOOR | ZEEBETITE;

	HitEnemySound = Sound::LoadSound("Resources/Audio/ShootEnemies.wav");
	HitRipperSound = Sound::LoadSound("Resources/Audio/ShootRipper.wav");
	HitDoor = Sound::LoadSound("Resources/Audio/DoorSound.wav");
}

void Rocket::Render(SpriteBatch * batch)
{
	if (!isDestroyed)
	{
		batch->Draw(*this);
	}
}

void Rocket::Update(float dt)
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

bool Rocket::IsDestroyed()
{
	return isDestroyed;
}

float Rocket::GetBulletSpeed()
{
	return BULLETSPEED;
}

void Rocket::OnHit()
{
	stateTime = 100; //force to be dead
	Sound::Play(HitEnemySound);
}

void Rocket::OnHitRipper()
{
	stateTime = 100; //force to be dead
	Sound::Play(HitRipperSound);
}

void Rocket::OnHitDoor()
{
	stateTime = 100; //force to be dead
	Sound::Play(HitDoor);
}
