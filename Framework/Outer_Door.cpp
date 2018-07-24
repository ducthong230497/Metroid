#include "Outer_Door.h"
#define CLOSEDOORTIME 1
OuterDoor::OuterDoor()
{
}

OuterDoor::~OuterDoor()
{
}

void OuterDoor::SetScene(Scene * s)
{
	scene = s;
}

void OuterDoor::Init(Texture * doorTexture, float x, float y)
{
	this->doorTexture = doorTexture;
	TexturePacker p = TexturePacker(doorTexture, "Resources/door.xml");
	SetRegion(p.GetRegion("outer_door").at(0));
	SetPosition(x, y);
	SetSize(16, 96);
	collisionType = CollisionType::Static;
	_CategoryMask = OUTER_DOOR;
	_BitMask = PLAYER | PLAYER_BULLET | ZOOMER | RIO;

	hitBulletTime = -1;
}

void OuterDoor::Update(float dt)
{
	if (hitBulletTime == -1) return;
	
}

void OuterDoor::Render(SpriteBatch * batch)
{
	batch->Draw(*this);
}

void OuterDoor::OnHitBullet()
{
	hitBulletTime = 0;
	_CategoryMask = 0;
	SetTexture(NULL);
}
