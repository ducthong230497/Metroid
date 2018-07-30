#include "Door.h"
#include "testScene_1.h"
#include "MainScene.h"
#include "Samus.h"
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
	scene = s;
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

void Door::SetCamDirection(FollowDirection fd)
{
	switch (fd)
	{
	case LEFT:
		cam->canFollowLeft = true;
		break;
	case RIGHT:
		cam->canFollowRight = true;
		break;
	case UP:
		cam->canFollowUp = true;
		break;
	case DOWN:
		cam->canFollowDown = true;
		break;
	default:
		break;
	}
}

void Door::SetChangeSoundTheme(bool b)
{
	changeSoundTheme = b;
}

void Door::Init(Texture * texture, float x, float y)
{
	setPosition(x, y);
	setSize(64, 96);
	_CategoryMask = DOOR;
	_BitMask = PLAYER | PLAYER_BULLET | PLAYER_ROCKET | ZOOMER;
	leftDoor = new OuterDoor();
	leftDoor->Init(texture, x - 32 - 8, y);
	leftDoor->Flip(true, false);
	rightDoor = new OuterDoor();
	rightDoor->Init(texture, x + 32 + 8, y);
}

void Door::InitOrangeDoor(Texture * texture, float x, float y)
{
	setPosition(x, y);
	setSize(64, 96);
	_CategoryMask = DOOR;
	_BitMask = PLAYER | PLAYER_BULLET | PLAYER_ROCKET | ZOOMER;
	leftDoor = new OuterDoor();
	leftDoor->InitOrangeDoor(texture, x - 32 - 8, y);
	leftDoor->Flip(true, false);
	rightDoor = new OuterDoor();
	rightDoor->InitOrangeDoor(texture, x + 32 + 8, y);
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
				((Samus*)player)->canControl = true;
				((Samus*)player)->moveThroughDoor = false;
				((MainScene*)scene)->moveThroughDoor = false;
				_CategoryMask = DOOR;
				leftDoor->open = false;
				leftDoor->_CategoryMask = OUTER_DOOR;
				rightDoor->open = false;
				rightDoor->_CategoryMask = OUTER_DOOR;
				SetCamDirection(leftDoor->followDirection);
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
				((Samus*)player)->canControl = true;
				((Samus*)player)->moveThroughDoor = false;
				((MainScene*)scene)->moveThroughDoor = false;
				_CategoryMask = DOOR;
				rightDoor->open = false;
				rightDoor->_CategoryMask = OUTER_DOOR;
				leftDoor->open = false;
				leftDoor->_CategoryMask = OUTER_DOOR;
				SetCamDirection(rightDoor->followDirection);
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
	player->setVelocity(0, player->getVelocity().y);
	((Samus*)player)->move = false;
	((Samus*)player)->canControl = false;
	((Samus*)player)->moveThroughDoor = true;
	((MainScene*)scene)->moveThroughDoor = true;
	leftDoor->_CategoryMask = NONE;
	rightDoor->_CategoryMask = NONE;
	cam->followPlayerX = false;
	cam->followPlayerY = false;
	cam->canFollowLeft = false;
	cam->canFollowRight = false;
	cam->canFollowUp = false;
	cam->canFollowDown = false;
	if (changeSoundTheme)
	{
		if (leftDoor->open)
		{
			ChangeFlagLeftDoor();
		}
		else
		{
			ChangFlagRightDoor();
		}
	}
}

void Door::ChangeFlagLeftDoor()
{
	((MainScene*)scene)->flagsound = leftFlagSound;
}

void Door::ChangFlagRightDoor()
{
	((MainScene*)scene)->flagsound = rightFlagSound;
}
