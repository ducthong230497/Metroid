#include "Kraid.h"
#include "MainScene.h"
#define BULLETSPEED 300
Kraid::Kraid()
{
}

Kraid::~Kraid()
{
}

void Kraid::SetScene(Scene * s)
{
	scene = s;
}

void Kraid::SetPlayer(GameObject * p)
{
	player = p;
}

void Kraid::Init(Texture * texture, int x, int y)
{
	stateTime = -1;
	bulletStateTime = -1;
	boomerangStateTime = -0.5;
	hitBulletTime = -1;

	TexturePacker p = TexturePacker(texture, "Resources/bosses_packer.xml");

	animation.AddRegion(p.GetRegion("kraid"));
	animation.SetFrameInterval(0.2);

	behingHitAnimation.AddRegion(p.GetRegion("beinghitkraid"));
	behingHitAnimation.SetFrameInterval(0.2);

	SetRegion(*animation.GetKeyAnimation());
	SetSize(48, 64);
	SetPosition(x, y);

	//setup body
	collisionType = Dynamic;
	
	_Velocity.y = -50;
	
	_CategoryMask = KRAID;
	_BitMask = PLAYER | PLAYER_BULLET | PLATFORM | BOMB_EXPLOSION;

	//setup bullets
	TextureRegion bulletRegion = p.GetRegion("kraidbullet").front();
	for (int i = 0; i < 3; i++)
	{
		Sprite * bullet = new Sprite();
		bullet->SetRegion(bulletRegion);
		bullet->SetSize(16, 6);
		//setup body
		bullet->collisionType = Dynamic;
		bullet->setPosition(x, y);
		bullet->setSize(16, 6);
		//bulletDef.isSensor = true;
		bullet->_CategoryMask = KRAID_BULLET;
		bullet->_BitMask = PLAYER;
		//bullet->SetID("bullet");
		bullets.push_back(bullet);
		//((MainScene*)scene)->kraidBullets.push_back(bullet);
	}


	//setup boomerang
	boomerangAnimation.AddRegion(p.GetRegion("kraidboomerang"));
	boomerangAnimation.SetFrameInterval(0.02);
	for (int i = 0; i < 2; i++)
	{
		KraidBoomerang * boomerang = new KraidBoomerang();
		boomerang->SetRegion(*boomerangAnimation.GetKeyAnimation());
		boomerang->SetSize(16, 14);
		//setup body
		boomerang->collisionType = Dynamic;
		boomerang->setPosition(x, y);
		boomerang->setSize(16, 14);
		boomerang->_CategoryMask = KRAID_BULLET;
		boomerang->_BitMask = PLAYER;
		boomerangs.push_back(boomerang);
		//((MainScene*)scene)->kraidBullets.push_back(boomerang);
	}
}

void Kraid::UpdateVelocity(GameObject * player)
{
	if (player->getPosition().x > this->GetPosition().x)
	{
		if (this->IsFlipX())
		{
			float randomX = (rand() % 200) / 100.0f - 0.3f;
			setVelocity(randomX*100, 0);
		}
	}
	else
	{
		if (player->getPosition().x < this->GetPosition().x)
		{
			if (!this->IsFlipX())
			{
				float randomX = -(rand() % 200) / 100.0f + 0.3f;
				setVelocity(randomX*100, 0);
			}
		}
	}
}

void Kraid::Update(float dt)
{
	//if (body == NULL) return;


	if (health <= 0)
	{
		//world->DestroyBody(body);
		//body = NULL;

		//destroy bullets
		for (std::vector<GameObject*>::iterator it = bullets.begin(); it != bullets.end(); it++)
		{
			//world->DestroyBody(it->body);

		}

		//destroy boomerangs
		for (std::vector<GameObject*>::iterator it = boomerangs.begin(); it != boomerangs.end(); it++)
		{
			//world->DestroyBody(it->body);
		}
		health = -100;
		isActive = false;
		return;
	}

	if (hitBulletTime == -1) //not being hit by any bullet
	{
		SetRegion(*animation.Next(dt));
	}
	else
	{
		SetRegion(*behingHitAnimation.Next(dt));

		hitBulletTime += 0.02;
		if (hitBulletTime > MAXHITBULLETTIME)
		{
			hitBulletTime = -1;
		}
	}


	if (player->getPosition().x > this->GetPosition().x && !this->IsFlipX() && stateTime == -1)
	{
		stateTime = 0; //start flipping
	}

	if (player->getPosition().x < this->GetPosition().x && this->IsFlipX() && stateTime == -1)
	{
		stateTime = 0; //start flipping
	}

	if (stateTime >= 0)
	{
		stateTime += dt;
	}

	if (stateTime > TURNINGDELAYTIME) //flip after delaytime
	{
		if (player->getPosition().x > this->GetPosition().x)
		{
			Flip(true, false);
		}
		else
		{
			Flip(false, false);
		}

		stateTime = -1;
	}

	//SetPosition(body->GetPosition().x, body->GetPosition().y);


	if (bulletStateTime >= 0)
	{
		bulletStateTime += dt;

	}

	if (boomerangStateTime >= 0)
	{
		boomerangStateTime += dt;
		boomerangAnimation.Next(dt);
	}

	if (bulletStateTime > KRAIDBULLETLIVETIME)
	{
		bulletStateTime = -1;
	}

	if (boomerangStateTime > KRAIDBOOMERANGLIVETIME)
	{
		boomerangStateTime = -0.5;
	}


	if (bulletStateTime < 0)
	{
		//update bullet
		int i = -1;
		for (std::vector<GameObject*>::iterator it = bullets.begin(); it != bullets.end(); it++)
		{
			if (player->getPosition().x > this->GetPosition().x)
			{
				(*it)->setPosition(this->GetPosition().x + this->GetSize().x / 2 + (*it)->getSize().x / 2 - i * 5, this->GetPosition().y + i * 20);
				((Sprite*)(*it))->Flip(true, false);
				(*it)->setVelocity(BULLETSPEED, 0);
			}
			else
			{
				(*it)->setPosition(this->GetPosition().x - this->GetSize().x / 2 - (*it)->getSize().x / 2 + i * 5, this->GetPosition().y + i * 20);
				((Sprite*)(*it))->Flip(false, false);
				(*it)->setVelocity(-BULLETSPEED, 0);
			}
			i++;
		}


		bulletStateTime += dt;
	}

	if (boomerangStateTime < 0)
	{
		int i = 0;
		//update boomerang
		for (std::vector<GameObject*>::iterator it = boomerangs.begin(); it != boomerangs.end(); it++)
		{
			if (player->getPosition().x > this->GetPosition().x)
			{
				(*it)->setVelocity((rand() % 5 + 4)*50, (rand() % 6 + 4)* 50);
			}
			else
			{
				(*it)->setVelocity(-((rand() % 5 + 4)* 50), (rand() % 6 + 4)* 50);
			}

			if (!this->IsFlipX())
			{
				(*it)->setPosition(this->GetPosition().x + this->GetSize().x / 2, this->GetPosition().y + this->GetSize().y / 2 - i * 10);
			}
			else
			{
				(*it)->setPosition(this->GetPosition().x - this->GetSize().x / 2, this->GetPosition().y + this->GetSize().y / 2 - i * 10);
			}

			i++;
		}

		boomerangStateTime += dt;
	}
}

void Kraid::Render(SpriteBatch * batch)
{

	//render bullets
	for (std::vector<GameObject*>::iterator it = bullets.begin(); it != bullets.end(); it++)
	{

		//it->setPosition(it->body->GetPosition().x, it->body->GetPosition().y);
		batch->Draw((Sprite*)(*it));
	}

	batch->Draw(*this);

	//render boomerangs
	for (std::vector<GameObject*>::iterator it = boomerangs.begin(); it != boomerangs.end(); it++)
	{
		((Sprite*)(*it))->SetRegion(*boomerangAnimation.GetKeyAnimation());
		//(Sprite*)(*it)->SetPosition(it->body->GetPosition().x, it->body->GetPosition().y);
		batch->Draw((Sprite*)(*it));
	}
}

void Kraid::OnHitBullet()
{
	health -= 5;
	hitBulletTime = 0;
	if (health <= 0)
	{
		OnDie();
	}
}

void Kraid::OnHitBomb()
{
}

void Kraid::OnDie()
{
	isActive = false;
	std::vector<GameObject*>::iterator it = std::find(scene->GameObjects.begin(), scene->GameObjects.end(), this);
	if (it != scene->GameObjects.end())
	{
		scene->GameObjects.erase(it);
	}
	BombItem * bombitem = new BombItem();
	bombitem->Init(&itemTexture, _Position.x, _Position.y);
	bombitem->SetScene(scene);
	((MainScene*)scene)->bombItem = bombitem;
}

bool Kraid::IsDead()
{
	return health <= 0;
}
