#include "Kraid.h"

Kraid::Kraid()
{
}

Kraid::~Kraid()
{
}

void Kraid::Init(Texture * texture, int x, int y)
{
	health = 20;
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
		bullet->_CategoryMask = KRAID;
		bullet->_BitMask = PLAYER;
		//bullet->SetID("bullet");
		bullets.push_back(bullet);
	}


	//setup boomerang
	boomerangAnimation.AddRegion(p.GetRegion("kraidboomerang"));
	boomerangAnimation.SetFrameInterval(0.02);
	for (int i = 0; i < 2; i++)
	{
		Sprite * boomerang = new Sprite();
		boomerang->SetRegion(*boomerangAnimation.GetKeyAnimation());
		boomerang->SetSize(16, 14);
		//setup body
		boomerang->collisionType = Dynamic;
		boomerang->setPosition(x, y);
		boomerang->setSize(16, 14);
		//boomerangDef.isSensor = true;
		//boomerang.body = world->CreateBody(boomerangDef);
		boomerang->_CategoryMask = KRAID;
		boomerang->_BitMask = PLAYER;
		//boomerang->SetMass(2);
		//boomerang->SetLinearDrag(0.5, 1);
		//boomerang->SetID("boomerang");
		boomerangs.push_back(boomerang);
	}
}

void Kraid::UpdateVelocity(GameObject * player)
{
}

void Kraid::Update(float dt)
{
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
}

void Kraid::OnHitBomb()
{
}

bool Kraid::IsDead()
{
	return false;
}
