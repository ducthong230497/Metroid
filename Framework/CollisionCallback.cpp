#include "CollisionCallback.h"

CollisionCallback::CollisionCallback()
{
}

CollisionCallback::~CollisionCallback()
{
}

void CollisionCallback::OnCollisionEnter(GameObject * gameObjectA, GameObject * gameObjectB, const POINT & CollisionDirection)
{

	switch (gameObjectA->_CategoryMask | gameObjectB->_CategoryMask)
	{
	case RIPPER | PLATFORM: // PLAYER * RIO:
	{
		if (gameObjectA->_CategoryMask == RIPPER)
		{
			((Ripper*)gameObjectA)->OnHitGround();
		}
		/*else if(gameObjectA->_CategoryMask == PLATFORM)
		{
			((Ripper*)gameObjectB)->OnHitGround();
		}*/
		/*else if(gameObjectA->_CategoryMask == RIO)
		{
			Trace::Log("Rio hit Player");
		}
		else if (gameObjectA->_CategoryMask == PLAYER)
		{
			Trace::Log("Player hit Rio");
		}*/
		break;
	}
	case RIO | PLATFORM:
	{
		if (gameObjectA->_CategoryMask == Category::RIO)
		{
			if (CollisionDirection.x != NOT_COLLIDED)
			{
				((Rio*)gameObjectA)->setVelocity(0, gameObjectA->getVelocity().y);
			}
			if (CollisionDirection.y != NOT_COLLIDED)
			{
				((Rio*)gameObjectA)->setVelocity(gameObjectA->getVelocity().x, 0);
			}
		}
		break;
	}
	case SKREE | PLATFORM:
	{
		if (gameObjectA->_CategoryMask == Category::SKREE)
		{
			if (CollisionDirection.y != NOT_COLLIDED)
			{
				((Skree*)gameObjectA)->setVelocity(0, 0);
			}
		}
		break;
	}
	case PLAYER_BULLET | RIO:
	{
		if (gameObjectA->_CategoryMask == RIO)
		{
			Trace::Log("Bullet hit Rio");
		}
		else
		{

		}
		break;
	}
	case PLAYER_BULLET | RIPPER:
	{
		if (gameObjectA->_CategoryMask == RIPPER)
		{
			Trace::Log("Bullet hit Ripper");
		}
		else
		{

		}
		break;
	}
	case PLAYER_BULLET | SKREE:
	{
		if (gameObjectA->_CategoryMask == SKREE)
		{
			Trace::Log("Bullet hit Skree");
		}
		else if(gameObjectA->_CategoryMask == PLAYER_BULLET)
		{

		}
		break;
	}
	case PLAYER_BULLET | ZOOMER:
	{
		if (gameObjectA->_CategoryMask == ZOOMER)
		{
			Trace::Log("Bullet hit Zoomer");
		}
		else if(gameObjectA->_CategoryMask == PLAYER_BULLET)
		{
			Trace::Log("Bullet hit Zoomer");
		}
		break;
	}
	case PLAYER | RIO :case PLAYER | RIPPER: case PLAYER | ZOOMER: case PLAYER | SKREE:
	{
		if (gameObjectA->_CategoryMask == PLAYER)
		{
			//OnHitEnemy
			Trace::Log("Player hit enemy");
			gameObjectA->setVelocity(CollisionDirection.x * 3, 100);
		}
		else
		{
			if (gameObjectA->_CategoryMask == RIO)
			{
				((Rio*)gameObjectA)->OnHitPlayer();
				Trace::Log("Player hit Rio");
				gameObjectB->setVelocity(CollisionDirection.x * 3, 100);
			}
			else if (gameObjectA->_CategoryMask == RIPPER)
			{
				((Ripper*)gameObjectA)->OnHitPlayer();
				Trace::Log("Player hit Ripper");
			}
		}
		break;
	}
	default:
		break;
	}
}

void CollisionCallback::OnColliding(GameObject * gameObjectA, GameObject * gameObjectB, const POINT & collisionDirection)
{
}

void CollisionCallback::OnCollisionExit(GameObject * gameObjectA, GameObject * gameObjectB, const POINT & collisionDirection)
{
}

void CollisionCallback::OnTriggerEnter(GameObject * gameObjectA, GameObject * gameObjectB)
{
}

void CollisionCallback::OnTriggerStay(GameObject * gameObjectA, GameObject * gameObjectB)
{
}

void CollisionCallback::OnTriggerExit(GameObject * gameObjectA, GameObject * gameObjectB)
{
}


