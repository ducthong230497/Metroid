#include "Zoomer.h"

Zoomer::Zoomer()
{
}

Zoomer::~Zoomer()
{
}

void Zoomer::Init(Texture * zoomerTexture, float x, float y, bool Direction)
{
	TexturePacker p = TexturePacker(zoomerTexture, "Resources/enemies_packer.xml");

	zoomerAnimation.AddRegion(p.GetRegion("zoomer"));
	zoomerAnimation.SetFrameInterval(0.04);

	SetRegion(*zoomerAnimation.GetKeyAnimation());
	SetSize(25, 25);
	SetPosition(x, y);

	_CategoryMask = ZOOMER;
	_BitMask = PLAYER | PLATFORM | PLAYER_BULLET | BOOM_EXPLOSION;

	prevCollisionDirection.x = NOT_COLLIDED;
	prevCollisionDirection.y = -prevVelocity.y * 100;
	curCollisionDirection.x = NOT_COLLIDED;
	curCollisionDirection.y = -prevVelocity.y * 100;

	//set up intial velocity/direction
	if (Direction)
	{
		prevVelocity.x = 0.7f;
		prevVelocity.y = -0.7f;

		setVelocity(0.7f, -0.7f);
		setVelocity(0, 0);
	}
	else
	{
		prevVelocity.x = -0.7f;
		prevVelocity.y = -0.7f;

		setVelocity(-0.7f, -0.7f);
	}
	initalDirection = Direction;
	cooldownAfterCollisionChange = 3;
	health = 2;
	hitBulletTime = -1;
}

void Zoomer::Render(SpriteBatch * batch)
{
	if (isDead)
		return;

	batch->Draw(*this);
}

void Zoomer::Update(float dt, Camera * cam)
{

}
