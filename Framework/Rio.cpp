#include "Rio.h"
#include "Samus.h"
#include "MainScene.h"
Rio::Rio()
{
}

Rio::~Rio()
{
}

void Rio::SetScene(Scene * s)
{
	scene = s;
}

void Rio::Init(Texture * rioTexture, float x, float y)
{
	isHitRoof = false;
	isHitGround = false;
	isHitLeft = false;
	isHitRight = false;
	jumpCheck = true;
	//health = 4;
	phase1 = true;
	phase2 = true;
	right = true;
	still = false;
	delayTime = 0.5;
	delayTimeDuringGame = 0;

	hitBulletTime = -1;

	startVelo1 = POINT(70, -150);
	startVelo2 = POINT(-70, -150);
	deacceleration = (float)0.05;

	midVelo1 = POINT(70, (float)25);
	midVelo2 = POINT(-70, (float)25);
	acceleration = (float)30;

	TexturePacker p = TexturePacker(rioTexture, "Resources/enemies_packer.xml");

	rioAnimation.AddRegion(p.GetRegion("rio"));
	rioAnimation.SetFrameInterval(0.04);

	SetRegion(*rioAnimation.GetKeyAnimation());
	SetSize(45, 40);
	SetPosition(x, y);

	//setup body
	_CategoryMask = RIO;
	_BitMask = PLAYER | PLATFORM | PLAYER_BULLET | PLAYER_ROCKET | BOMB_EXPLOSION;
	collisionType = CollisionType::Dynamic;

	_Velocity.x = 150;
	_Velocity.y = -350;

	//sound when Ripper is shot
	//isShot = Sound::LoadSound("Resources/SoundEffect/ShootRipper.wav");
}

void Rio::UpdateVelocity(GameObject * player)
{
	float distance = _Position.x - player->getPosition().x;
	if (isHitRoof)
	{
		if (delayTimeDuringGame > delayTime)
		{
			isHitRoof = false;
			delayTimeDuringGame = 0;
			_Velocity = previousVelocity * -1;
		}
	}
	else if(isHitGround)
	{
		if (abs(distance) <= 200 && ((Samus*)player)->onGround)
		{
			setVelocity(previousVelocity.x, 0);
		}
		else if (abs(distance) <= 200 && !((Samus*)player)->onGround)
		{
			setVelocity(previousVelocity.x, -previousVelocity.y);
			isHitGround = false;
		}
		else
		{
			setVelocity(_Velocity.x, -previousVelocity.y);
			isHitGround = false;
		}
	}

#pragma region MyRegion
	//if (player->getisGrounded() == true)
/*if (player->getPosition().y > _Position.y + 50)
{
	jumpCheck = true;
}
if (still)
{
	if (delayTimeDuringGame >= delayTime)
	{
		delayTimeDuringGame = 0;
		still = false;
	}
	else
	{

	}
}
else
{
	if (right == true)
	{
		if (phase1 == true)
		{
			midVelo1= POINT(2, 0.05);
			if (isHitRoof)
			{
				isHitRoof = false;
			}
			if (isHitRoof == false)
			{
				setVelocity(startVelo1.x, startVelo1.y);
				if (isHitGround == false) startVelo1 = POINT(startVelo1.x, startVelo1.y + deacceleration);
				if (isHitGround == true && player->getPosition().x - getPosition().x < 0)
				{
					right = true;
					phase1 = false;
				}
				if (player->getPosition().y >= 65 && jumpCheck == true && abs(player->getPosition().x - getPosition().x) < 50)
				{
					jumpCheck = false;
					right = true;
					phase1 = false;
				}
				if (isHitRight)
				{
					isHitRight = false;
					isHitLeft = false;
					right = false;
					phase2 = true;
					phase1 = false;
					startVelo2 = POINT(-2, -6);
				}
			}
		}
		else if (phase1 == false)
		{
			startVelo1 = POINT(2, -6);
			if (isHitGround == true)
			{
				isHitGround = false;
			}
			setVelocity(midVelo1.x, midVelo1.y);
			midVelo1 = POINT(midVelo1.x, midVelo1.y + acceleration);
			if (isHitRoof == true)
			{
				still = true;
				delayTimeDuringGame = 0;
				setVelocity(0, 0);
				right = false;
				phase1 = true;
			}
		}
	}
	else if (right == false)
	{
		if (phase2 == true)
		{
			midVelo2 = POINT(-2, (float)0.05);
			if (isHitRoof)
			{
				isHitRoof = false;
			}
			if (isHitRoof == false)
			{
				setVelocity(startVelo2.x, startVelo2.y);
				if (isHitGround == false) startVelo2 = POINT(startVelo2.x, startVelo2.y + deacceleration);
				if (isHitGround == true && player->getPosition().x - getPosition().x > 0)
				{
					right = false;
					phase2 = false;
				}
				if (player->getPosition().y >= 64 && jumpCheck == true && abs(player->getPosition().x - getPosition().x) < 50)
				{
					jumpCheck = false;
					right = false;
					phase2 = false;
				}
				if (isHitLeft)
				{
					isHitLeft = false;
					isHitRight = false;
					right = true;
					phase1 = true;
					phase2 = false;
					startVelo1 = POINT(2, -6);
				}
			}
		}
		else if (phase2 == false)
		{
			startVelo2 = POINT(-2, -6);
			if (isHitGround == true)
			{
				isHitGround = false;
			}
			setVelocity(midVelo2.x, midVelo2.y);
			midVelo2 = POINT(midVelo2.x, midVelo2.y + acceleration);
			if (isHitRoof == true)
			{
				still = true;
				delayTimeDuringGame = 0;
				setVelocity(0, 0);
				phase2 = true;
				right = true;
			}
		}
	}
}*/
#pragma endregion

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
		isActive = false;
		OnDie();
		//world->DestroyBody(body);
		return;
	}
	if (isHitRoof)
	{
		delayTimeDuringGame += dt;
	}
	if (hitBulletTime == -1) //-1 means not being hit by bullet
	{
		//delayTimeDuringGame += dt;
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

void Rio::OnHitGround()
{
	isHitGround = true;
	previousVelocity = _Velocity;
}

void Rio::OnHitRoof()
{
	isHitRoof = true;
	previousVelocity = _Velocity;
	setVelocity(0, 0);
}

void Rio::OnHitLeft()
{
	isHitLeft = true;
}

void Rio::OnHitRight()
{
	isHitRight = true;
}

void Rio::OnHitPlayer()
{
	Trace::Log("Rio hit player");
}

void Rio::OnHitBomb()
{
	health -= takeBombDamage;
}

void Rio::OnHitBullet()
{
	health -= takeDamage;
}

void Rio::OnDie()
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
