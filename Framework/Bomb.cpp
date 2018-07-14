#include "Bomb.h"

Bomb::Bomb()
{
}

Bomb::~Bomb()
{
}

Bomb::Bomb(Texture * texture)
{
	isDestroyed = false;
	stateTime = 0;

	//get bullet 
	TexturePacker p = TexturePacker(texture, "Resources/samusaran_packer.xml");
	animation.AddRegion(p.GetRegion("bomb"));

	this->SetRegion(*animation.GetKeyAnimation());
	SetSize(16, 16);
	_Position = POINT(100, -25);
	//body definition
	collisionType = Static;
	//bodyDef.size.Set(16, 16);
	//bodyDef.isSensor = true;

	//create body
	//mainBody = world->CreateBody(bodyDef);
	_CategoryMask = NONE; //not collide with anything		
	_BitMask = SKREE | ZOOMER | PLAYER | PLATFORM | RIO;
	//_Bi = SKREE | ZOOMER_BIT | PLAYER_BIT | BREAKABLEPLATFORM_BIT | HEALTHPILE_BIT | KRAID_BIT;
	//mainBody->PutExtra(this);

	//effects
	explosionEffect.Init(texture);
	explosionEffect.SetSize(32, 32);
}

void Bomb::Render(SpriteBatch * batch)
{
	batch->Draw(*this);

	explosionEffect.Render(batch);
}

void Bomb::Update(float dt)
{
	stateTime += dt;

	if (_CategoryMask == BOMB_EXPLOSION)
	{
		if (stateTime < EXPLOSIONLIVETIME)
		{
			explosionEffect.Update(dt);
			SetSize(GetSize().x + 3, GetSize().y + 3);
		}
		else
		{
			//world->DestroyBody(mainBody);
			//mainBody = NULL;
			//isDestroyed = true;
		}
	}
	else
	{
		//SetPosition(mainBody->GetPosition().x, mainBody->GetPosition().y);

		if (stateTime > BOMBLIVETIME)
		{
			_CategoryMask = BOMB_EXPLOSION;
			SetTexture(NULL);
			explosionEffect.SetPosition(this->GetPosition().x, this->GetPosition().y);
			explosionEffect.Play();
		}
	}
}

bool Bomb::IsDestroyed()
{
	return isDestroyed;
}

void Bomb::OnHitEnemy()
{
	stateTime = 100;
}

void Bomb::Play()
{
}
