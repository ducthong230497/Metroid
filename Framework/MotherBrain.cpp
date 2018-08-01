#include "MotherBrain.h"
#include "MainScene.h"
MotherBrain::MotherBrain()
{
}

MotherBrain::~MotherBrain()
{
}

void MotherBrain::SetScene(Scene * s)
{
	scene = s;
}

void MotherBrain::Init(Texture * bossessTexture, int x, int y)
{
	TexturePacker p = TexturePacker(bossessTexture, "Resources/bosses_packer.xml");

	IdleAnimation.AddRegion(p.GetRegion("motherbrain"));
	IdleAnimation.SetFrameInterval(0.2);

	OpenDoorAnimation.AddRegion(p.GetRegion("weakmotherbrain"));
	OpenDoorAnimation.SetFrameInterval(0.2);

	SetRegion(*IdleAnimation.GetKeyAnimation());

	SetPosition(x, y);
	SetSize(160, 128);
	Flip(true, false);

	//setup body
	collisionType = Dynamic;

	_CategoryMask = MOTHERBRAIN;
	_BitMask = PLAYER | PLAYER_BULLET | PLAYER_ROCKET;


}

void MotherBrain::Update(float dt)
{
	if (health <= 0)
	{
		isActive = false;
		OnDie();
	}

	if (health > 80)
	{
		SetRegion(*IdleAnimation.Next(dt));
	}
	else
	{
		SetRegion(*OpenDoorAnimation.Next(dt));
	}
}

void MotherBrain::Render(SpriteBatch * batch)
{
	batch->Draw(*this);
}

void MotherBrain::OnHitBullet()
{
	health -= 2;
}

void MotherBrain::OnHitRocket()
{
	health -= 5;
}

void MotherBrain::OnDie()
{
	((MainScene*)scene)->explosionEffect.setPosition(_Position);
	((MainScene*)scene)->explosionEffect.SetSize(_Size.x / 2, _Size.y / 2);
	((MainScene*)scene)->explosionEffect.Play();
}


