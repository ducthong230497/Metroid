#include "Door.h"
#include "testScene_1.h"
#define CAMSPEED 300
#define PLAYERSPEED 200
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

void Door::SetPlayer(GameObject * p)
{
	player = p;
}

void Door::Init(Texture * texture, float x, float y)
{
	setPosition(x, y);
	setSize(64, 96);
	_CategoryMask = DOOR;
	_BitMask = PLAYER | PLAYER_BULLET | ZOOMER;
	leftDoor = new OuterDoor();
	leftDoor->Init(texture, x - 32 - 8, y);
	leftDoor->Flip(true, false);
	rightDoor = new OuterDoor();
	rightDoor->Init(texture, x + 32 + 8, y);
}

void Door::Update(float dt)
{
	if (moveCam)
	{
		POINT temp = cam->getPosition();
		if (leftDoor->open)
		{
			temp.x += CAMSPEED * dt;
			if (temp.x > _Position.x)
			{
				temp.x = _Position.x;
				moveCam = false;
				movePlayer = true;
			}
		}
		else
		{
			temp.x -= CAMSPEED * dt;
			if (temp.x + cam->getSize().x < _Position.x)
			{
				temp.x = _Position.x - cam->getSize().x;
				moveCam = false;
				movePlayer = true;
			}
		}
		cam->setPosition(temp);
	}
	else if (!moveCam && movePlayer)
	{
		POINT temp = player->getPosition();
		if (leftDoor->open)
		{
			temp.x += PLAYERSPEED * dt;
			if (temp.x > _Position.x + _Size.x)
			{
				temp.x = _Position.x + _Size.x;
				movePlayer = false;
				testScene1::canMove = true;
				_CategoryMask = DOOR;
				leftDoor->open = false;
				leftDoor->_CategoryMask = OUTER_DOOR;
				rightDoor->open = false;
				//rightDoor->_CategoryMask = OUTER_DOOR;
				cam->canFollow = true;
			}
		}
		else
		{
			temp.x -= PLAYERSPEED * dt;
			if (temp.x < _Position.x - _Size.x)
			{
				temp.x = _Position.x - _Size.x;
				movePlayer = false;
				testScene1::canMove = true;
				_CategoryMask = DOOR;
				rightDoor->open = false;
				//rightDoor->_CategoryMask = OUTER_DOOR;
				leftDoor->open = false;
				leftDoor->_CategoryMask = OUTER_DOOR;
				cam->canFollow = true;
			}
		}
		player->setPosition(temp);
	}
}

void Door::Render(SpriteBatch * batch)
{
	leftDoor->Render(batch);
	rightDoor->Render(batch);
}

void Door::OnHitPlayer()
{
	moveCam = true;
	_CategoryMask = NONE;
	testScene1::canMove = false;
	player->setVelocity(0, 0);
	leftDoor->_CategoryMask = NONE;
	rightDoor->_CategoryMask = NONE;
	cam->followPlayer = false;
	cam->canFollow = false;
}
