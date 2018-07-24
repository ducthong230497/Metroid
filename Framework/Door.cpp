#include "Door.h"
#define CAMSPEED 200
Door::Door()
{
}

Door::~Door()
{
}

void Door::SetScene(Scene * s)
{
	leftDoor->SetScene(s);
	rightDoor->SetScene(s);
}

void Door::SetCam(Camera * c)
{
	cam = c;
}

void Door::Init(Texture * texture, float x, float y)
{
	setPosition(x, y);
	setSize(64, 96);
	_CategoryMask = DOOR;
	_BitMask = PLAYER | PLAYER_BULLET | ZOOMER;
	leftDoor = new OuterDoor();
	leftDoor->Init(texture, x - 32 - 4, y);
	leftDoor->Flip(true, false);
	rightDoor = new OuterDoor();
	rightDoor->Init(texture, x + 32 + 4, y);
}

void Door::Update(float dt)
{
	if (moveCam)
	{
		POINT temp = cam->getPosition();
		temp.x += CAMSPEED * dt;
		if (temp.x > _Position.x)
		{
			temp.x = _Position.x;
			moveCam = false;
		}
		cam->setPosition(temp);
	}
}

void Door::Render(SpriteBatch * batch)
{
	leftDoor->Render(batch);
	rightDoor->Render(batch);
}

void Door::OnHitPlayer()
{
	//if(leftDoor->)
	moveCam = true;
}
