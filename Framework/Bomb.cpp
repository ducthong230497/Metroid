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
	_Position = POINT(110, 100);
	//body definition
	collisionType = None;
	//bodyDef.size.Set(16, 16);
	//bodyDef.isSensor = true;

	//create body
	//mainBody = world->CreateBody(bodyDef);
	_CategoryMask = BOMB_EXPLOSION; //not collide with anything		
	_BitMask = NONE;
	default_bitMask = SKREE | ZOOMER | PLAYER | PLATFORM | RIO | BREAKABLE_PLATFORM;
	//_Bi = SKREE | ZOOMER_BIT | PLAYER_BIT | BREAKABLEPLATFORM_BIT | HEALTHPILE_BIT | KRAID_BIT;
	//mainBody->PutExtra(this);

	//effects
	explosionEffect.Init(texture);
	explosionEffect.SetSize(32, 32);
}

void Bomb::SetScene(Scene * s)
{
	scene = s;
}

void Bomb::Render(SpriteBatch * batch)
{
	batch->Draw(*this);

	explosionEffect.Render(batch);
}

void Bomb::Update(float dt)
{
	stateTime += dt;

	if (_BitMask == default_bitMask)
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
			std::vector<GameObject*>::iterator it = std::find(scene->GameObjects.begin(), scene->GameObjects.end(), (this));
			if (it != scene->GameObjects.end())
			{
				//delete *it;
				scene->GameObjects.erase(it);
			}
			isDestroyed = true;
		}
	}
	else
	{
		//SetPosition(mainBody->GetPosition().x, mainBody->GetPosition().y);

		if (stateTime > BOMBLIVETIME)
		{
			_BitMask = default_bitMask;
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
