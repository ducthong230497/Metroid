#include "Rio.h"

Rio::Rio()
{
}

Rio::~Rio()
{
}

void Rio::Init(Texture * rioTexture, float x, float y)
{
	isHitRoof = true;
	isHitGround = false;
	isHitLeft = false;
	isHitRight = false;
	jumpCheck = true;
	health = 4;
	phase1 = true;
	phase2 = true;
	right = true;
	still = false;
	delayTime = 0.5;
	delayTimeDuringGame = 0;

	hitBulletTime = -1;

	startVelo1 = POINT(0.2, -0.3);
	startVelo2 = POINT(-0.2, -0.3);
	deacceleration = (float)0.05;

	midVelo1 = POINT(0.2, (float)0.05);
	midVelo2 = POINT(-0.2, (float)0.05);
	acceleration = (float)0.5;

	TexturePacker p = TexturePacker(rioTexture, "Resources/enemies_packer.xml");

	rioAnimation.AddRegion(p.GetRegion("rio"));
	//zoomerAnimation.SetFrameInterval(0.04);

	SetRegion(*rioAnimation.GetKeyAnimation());
	SetSize(45, 40);
	SetPosition(x, y);

	//setup body
	_CategoryMask = RIO;
	_BitMask = PLAYER | PLATFORM | PLAYER_BULLET;
	collisionType = CollisionType::Dynamic;

	_Velocity.x = -0;
	_Velocity.y = 0;

	//sound when Ripper is shot
	//isShot = Sound::LoadSound("Resources/SoundEffect/ShootRipper.wav");
}

void Rio::UpdateVelocity(GameObject * player)
{
}

void Rio::Render(SpriteBatch * batch)
{
	if (isDead)
		return;
	batch->Draw(*this);
}

void Rio::Update(float dt)
{
	if (isDead) return;

	if (health <= 0)
	{
		isDead = true;
		//world->DestroyBody(body);
		return;
	}

	if (hitBulletTime == -1) //-1 means not being hit by bullet
	{
		delayTimeDuringGame += dt;
		SetRegion(*rioAnimation.Next(dt));
	}
	else
	{
		if (hitBulletTime < MAXHITBULLETTIME)
		{

			hitBulletTime += dt;
		}
		else
		{
			hitBulletTime = -1;
			//collisionType = CollisionType::Static;
		}
	}

	if (hitPlayerTime != -1)
	{
		if (hitPlayerTime < MAXHITPLAYERHITTIME)
		{
			hitPlayerTime += dt;
		}
		else
		{
			hitPlayerTime = -1;
			//body->maskBits = PLAYER_BIT | PLATFORM_BIT | BULLET_BIT | EXPLOSION_BIT | BREAKABLEPLATFORM_BIT;
		}
	}

	//if (body != NULL)
		this->SetPosition(GetPosition().x, GetPosition().y);
}
