#include "Ripper.h"

Ripper::Ripper()
{
}

Ripper::~Ripper()
{
}

void Ripper::Init(Texture *ripperTexture, float x, float y)
{
	TexturePacker p = TexturePacker(ripperTexture, "Resources/enemies_packer.xml");

	ripperAnimation.AddRegion(p.GetRegion("ripper"));
	//zoomerAnimation.SetFrameInterval(0.04);

	SetRegion(*ripperAnimation.GetKeyAnimation());
	SetSize(30, 15);
	SetPosition(x, y);

	//setup body
	_CategoryMask = RIPPER;
	_BitMask = PLAYER | PLATFORM | PLAYER_BULLET;
	collisionType = CollisionType::Dynamic;

	_Velocity.x = -90;
	_Velocity.y = 0;

	//sound when Ripper is shot
	//isShot = Sound::LoadSound("Resources/SoundEffect/ShootRipper.wav");
}

void Ripper::Update(float dt)
{
}

void Ripper::Render(SpriteBatch *batch)
{
	if (isDead)
		return;

	batch->Draw(*this);
}

void Ripper::OnHitGround()
{
	Flip(!IsFlipX(), IsFlipY());
	setVelocity(-_Velocity.x, 0);
}

void Ripper::OnHitBullet()
{
}

void Ripper::OnHitPlayer()
{
	Trace::Log("Ripper hit Player");
}
