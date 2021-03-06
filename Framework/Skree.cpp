#include "Skree.h"
#include "MainScene.h"

#define SKREEBULLETSPEED 600
#define SKREESPEEDX 90
#define SKREESPEEDY 300
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
	_BitMask = PLAYER | PLATFORM | PLAYER_BULLET | BOMB_EXPLOSION | PLAYER_ROCKET;
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
				setVelocity(SKREESPEEDX, -SKREESPEEDY);
			}
			else
			{
				setVelocity(-SKREESPEEDX, -SKREESPEEDY);
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
					skreeBullet->setVelocity(SKREEBULLETSPEED * cos(45 * i*PI / 180), SKREEBULLETSPEED * sin(45 * i*Pi / 180));
					skreeBullet->_CategoryMask = SKREE_BULLET;
					skreeBullet->_BitMask = PLAYER;
					//sprite
					skreeBullet->SetRegion(*skreeBulletAnimation.GetKeyAnimation());
					skreeBullet->SetSize(6, 7);
					skreeBullet->SetPosition(GetPosition().x, GetPosition().y);

					SkreeBullets.push_back(skreeBullet);
					int size = scene->GameObjects.size();
					//scene->GameObjects.push_back(skreeBullet);
					((MainScene*)scene)->skreeBullet.push_back(skreeBullet);
					((MainScene*)scene)->RemoveObject(this);
				}

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
			for (int i = 0; i < ((MainScene*)scene)->skreeBullet.size(); ++i)
			{
				 
				std::vector<GameObject*>::iterator it = std::find(((MainScene*)scene)->skreeBullet.begin(), ((MainScene*)scene)->skreeBullet.end(), ((MainScene*)scene)->skreeBullet[i]);
				if (it != ((MainScene*)scene)->skreeBullet.end())
				{
					((MainScene*)scene)->RemoveObject(*it);
					delete *it;
					((MainScene*)scene)->skreeBullet.erase(it);
					i--;
				}
			}
			SkreeBullets.clear();
			isActive = false;
			isDead = true;
		}
	}
	if (health <= 0)
	{
		OnDie();
		isActive = false;
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
	//if (hitBulletTime != -1) return;
	health -= takeDamage;
	//hitBulletTime = 0;
	//stop this body a little bit 
	setVelocity(0, 0);
}

void Skree::OnHitBomb()
{
	health -= takeBombDamage;
}

void Skree::OnDie()
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

void Skree::OnHitPlayer(GameObject* samus, POINT CollisionDirection)
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
	return health;
}
