#include "Skree.h"

Skree::Skree()
{
}

Skree::~Skree()
{
}

void Skree::Init(Texture * skreeTexture, float x, float y)
{
	stateTime = 0;
	//hitBulletTime = -1;
	Health = 2;

	TexturePacker p = TexturePacker(skreeTexture, "Resources/enemies_packer.xml");

	skreeAnimation.AddRegion(p.GetRegion("skree"));
	skreeAnimation.SetFrameInterval(0.04);

	skreeHitAnimation.AddRegion(p.GetRegion("skreehit"));
	skreeHitAnimation.SetFrameInterval(0.04);

	skreeBulletAnimation.AddRegion(p.GetRegion("skreebullet"));


	SetRegion(*skreeAnimation.GetKeyAnimation());
	SetSize(16, 33);
	SetPosition(x, y);


	//setup body
	_CategoryMask = SKREE;
	_BitMask = PLAYER | PLATFORM | PLAYER_BULLET | BOOM_EXPLOSION;
	collisionType = CollisionType::Static;

	//for (int i = 0; i < 5; i++)
	//{
	//	//5 skree bullets fired when skree hits ground
	//	Sprite skreeBullet;

	//	//create body
	//	skreeBullet.setCollisionType(CollisionType::Dynamic);
	//	skreeBullet.setSize(6, 7);
	//	skreeBullet.setVelocity(200 * cos(45 * i*PI / 180), 200 * sin(45 * i*Pi / 180));
	//	skreeBullet._CategoryMask = SKREE;
	//	skreeBullet._BitMask = PLAYER;
	//	//sprite
	//	skreeBullet.SetRegion(*skreeBulletAnimation.GetKeyAnimation());
	//	skreeBullet.SetSize(6, 7);
	//	skreeBullet.SetPosition(GetPosition().x, GetPosition().y);

	//	SkreeBullets.push_back(skreeBullet);

	//}
}

void Skree::SetScene(Scene * s)
{
	scene = s;
}

void Skree::UpdateVelocity(GameObject * player)
{
	if (stateTime > 0 || isDead || hitBulletTime != -1) return;

	if (this->GetPosition().y - player->getPosition().y < 500 && this->GetPosition().y > player->getPosition().y) //if player and skree is too far apart each other, don't do anything 
	{
		if (abs(player->getPosition().x - this->GetPosition().x) < 100)
		{
			collisionType = CollisionType::Dynamic;
			if (player->getPosition().x - getPosition().x > 0)
			{
				setVelocity(90, -150);
			}
			else
			{
				setVelocity(-90, -150);
			}
		}
	}
}

void Skree::Update(float dt)
{
	if (isDead) return;

	//this->SetPosition(GetPosition().x, GetPosition().y);

	if (hitBulletTime == -1) //-1 means not being hit by bullet
	{
		SetRegion(*skreeAnimation.Next(dt));
	}
	else
	{
		if (hitBulletTime < MAXHITBULLETTIME)
		{
			SetRegion(*skreeHitAnimation.Next(dt));
			hitBulletTime += dt;
		}
		else
		{
			hitBulletTime = -1;
			collisionType = CollisionType::Static;
		}
	}

	if (!isDead)
	{
		if (isHitGround) //hit ground -> fire some bullets
		{
			stateTime += dt;
			collisionType = CollisionType::Static;

			if (stateTime > SKREELIVETIMEAFTERGROUND)
			{
				//instantiate bullets
				for (int i = 0; i < 5; i++)
				{
					//5 skree bullets fired when skree hits ground
					Sprite *skreeBullet = new Sprite();

					//create body
					skreeBullet->setCollisionType(CollisionType::Dynamic);
					skreeBullet->setSize(6, 7);
					skreeBullet->setVelocity(200 * cos(45 * i*PI / 180), 200 * sin(45 * i*Pi / 180));
					skreeBullet->_CategoryMask = SKREE;
					skreeBullet->_BitMask = PLAYER;
					//sprite
					skreeBullet->SetRegion(*skreeBulletAnimation.GetKeyAnimation());
					skreeBullet->SetSize(6, 7);
					skreeBullet->SetPosition(GetPosition().x, GetPosition().y);

					SkreeBullets.push_back(skreeBullet);
					int size = scene->GameObjects.size();
					scene->GameObjects.push_back(skreeBullet);
					int a = 2;
				}

				//world->DestroyBody(body);
				//body = NULL;
				isHitGround = false; //set this to not instantiate more bullets
			}
		}
	}

	//update skree bullets
	if (stateTime > SKREELIVETIMEAFTERGROUND)
	{
		stateTime += dt;
		if (stateTime > SKREEBULLETLIVETIME)
		{
			for (std::vector<Sprite*>::iterator bullet = SkreeBullets.begin(); bullet != SkreeBullets.end(); ++bullet)
			{
				//world->DestroyBody(bullet->body);
				std::vector<GameObject*>::iterator it = std::find(scene->GameObjects.begin(), scene->GameObjects.end(), (*bullet));
				if (it != scene->GameObjects.end())
				{
					delete *it;
					scene->GameObjects.erase(it);
					*bullet = nullptr;
				}
			}
			SkreeBullets.clear();

			isDead = true;
		}
	}
}

void Skree::Render(SpriteBatch * batch)
{
	if (isDead) return;

	if (stateTime > SKREEBULLETLIVETIME) return;

	if (stateTime < SKREELIVETIMEAFTERGROUND)
	{
		batch->Draw(*this);
	}

	for (std::vector<Sprite*>::iterator bullet = SkreeBullets.begin(); bullet != SkreeBullets.end(); ++bullet)
	{
		(*bullet)->setPosition((*bullet)->GetPosition().x, (*bullet)->GetPosition().y);
		batch->Draw((*bullet));
	}
}

void Skree::OnHitGround()
{
	isHitGround = true;
}

void Skree::OnHitBullet()
{
	if (hitBulletTime != -1) return;
	Health--;
	hitBulletTime = 0;
	//stop this body a little bit 
	setVelocity(0, 0);
}

void Skree::OnDie()
{
}

bool Skree::IsHitGround()
{
	return isHitGround;
}

bool Skree::IsDead()
{
	return isDead;
}

int Skree::GetHealth()
{
	return Health;
}
