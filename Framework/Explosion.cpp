#include "Explosion.h"



Explosion::Explosion()
{
}


Explosion::~Explosion()
{
}

void Explosion::Init(Texture* explosionTexture)
{
	stateTime = -1;

	TexturePacker p = TexturePacker(explosionTexture, "Resources/effects_packer.xml");

	explosionAnimation.AddRegion(p.GetRegion("explosion"));

	SetRegion(*explosionAnimation.GetKeyAnimation());

}

void Explosion::Render(SpriteBatch* batch)
{
	if (stateTime == -1) return;
	batch->Draw(*this);
}

void Explosion::Update(float dt)
{
	if (stateTime == -1) return;

	if (stateTime < EFFECTLIVETIME)
	{
		SetRegion(*explosionAnimation.Next(dt));
		stateTime += dt;
	}
	else
	{
		stateTime = -1;
	}

}

void Explosion::Play()
{
	stateTime = 0;
}