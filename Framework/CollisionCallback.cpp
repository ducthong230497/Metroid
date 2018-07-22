#include "CollisionCallback.h"
#include "Samus.h"
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
			/*if (CollisionDirection.x != NOT_COLLIDED)
			{
				((Rio*)gameObjectA)->setVelocity(0, gameObjectA->getVelocity().y);
			}
			if (CollisionDirection.y != NOT_COLLIDED)
			{
				((Rio*)gameObjectA)->setVelocity(gameObjectA->getVelocity().x, 0);
			}*/
			if (CollisionDirection.y != NOT_COLLIDED)
			{
				if (CollisionDirection.y > 0)
				{
					
					((Rio*)gameObjectA)->OnHitGround();
				}
				else
				{
					/*POINT temp = gameObjectA->getVelocity();
					temp *= -1;
					gameObjectA->setVelocity(temp.x, temp.y);*/
					((Rio*)gameObjectA)->OnHitRoof();
				}
			}
			else if (CollisionDirection.x != NOT_COLLIDED)
			{
				if (CollisionDirection.x > 0)
				{
					/*POINT temp = gameObjectA->getVelocity();
					temp.y *= -1;
					gameObjectA->setVelocity(temp.x, temp.y);*/
					((Rio*)gameObjectA)->OnHitLeft();
				}
				else
				{
					/*POINT temp = gameObjectA->getVelocity();
					temp *= -1;
					gameObjectA->setVelocity(temp.x, temp.y);*/
					((Rio*)gameObjectA)->OnHitRight();
				}
			}
		}
		break;
	}
	case SKREE | PLATFORM:
	{
		if (gameObjectA->_CategoryMask == Category::SKREE)
		{
			if (CollisionDirection.y > 0)
			{
				((Skree*)gameObjectA)->setVelocity(0, 0);
				((Skree*)gameObjectA)->OnHitGround();
			}
		}
		break;
	}
	case ZOOMER | PLATFORM:
	{
		if (gameObjectA->_CategoryMask == ZOOMER)
		{
			((Zoomer*)gameObjectA)->OnHitGround(CollisionDirection);
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
			((Zoomer*)gameObjectA)->OnHitBullet();
			((Bullet*)gameObjectB)->OnHit();
		}
		else if(gameObjectA->_CategoryMask == PLAYER_BULLET)
		{
			Trace::Log("Bullet hit Zoomer");
			((Bullet*)gameObjectA)->OnHit();
			((Zoomer*)gameObjectB)->OnHitBullet();
		}
		break;
	}
	case PLAYER | RIO :case PLAYER | RIPPER: case PLAYER | ZOOMER: case PLAYER | SKREE:
	{
		if (gameObjectA->_CategoryMask == PLAYER)
		{
			//OnHitEnemy
			Trace::Log("Player hit enemy");
			//gameObjectA->setVelocity(-100* 3, 100);
		}
		else
		{
			if (gameObjectA->_CategoryMask == RIO)
			{
				((Rio*)gameObjectA)->OnHitPlayer();
				Trace::Log("Player hit Rio");
				//gameObjectB->setVelocity(-100 * 3, 100);
			}
			else if (gameObjectA->_CategoryMask == RIPPER)
			{
				((Ripper*)gameObjectA)->OnHitPlayer();
				Trace::Log("Player hit Ripper");
			}
		}
		break;
	}
	case PLAYER_BULLET | BREAKABLE_PLATFORM:
	{
		if (gameObjectA->_CategoryMask == BREAKABLE_PLATFORM)
		{
			((BreakablePlatform*)gameObjectA)->OnHitBullet();
		}
		else
		{
			((BreakablePlatform*)gameObjectB)->OnHitBullet();
		}
		break;
	}
	case PLAYER | MARUNARI:
	{
		if (gameObjectA->_CategoryMask == PLAYER)
		{
			Trace::Log("Hit Marunari");
			((Marunari*)gameObjectB)->OnHitPlayer();
		}
	}
	case PLAYER | BOMBITEM:
	{
		if (gameObjectA->_CategoryMask == PLAYER)
		{
			Trace::Log("Hit BombItem");
			((BombItem*)gameObjectB)->OnHitPlayer();
		}
	}
	case PLAYER | PLATFORM:
	{
		if (gameObjectA->_CategoryMask == PLAYER) {
			static_cast<Samus*>(gameObjectA)->OnHitGround(CollisionDirection);
		}
		else {
			static_cast<Samus*>(gameObjectB)->OnHitGround(CollisionDirection);
		}
	}
	default: break;
	}
}

void CollisionCallback::OnColliding(GameObject * gameObjectA, GameObject * gameObjectB, const POINT & collisionDirection)
{
}

void CollisionCallback::OnCollisionExit(GameObject * gameObjectA, GameObject * gameObjectB, const POINT & collisionDirection)
{
	switch (gameObjectA->_CategoryMask | gameObjectB->_CategoryMask)
	{
	case PLAYER | PLATFORM: // PLAYER * RIO:
	{
		if (gameObjectA->_CategoryMask == PLAYER) {
			static_cast<Samus*>(gameObjectA)->OnExitGround();
		}
		else {
			static_cast<Samus*>(gameObjectB)->OnExitGround();
		}
	}
	break;
	}
}

void CollisionCallback::OnTriggerEnter(GameObject * gameObjectA, GameObject * gameObjectB, bool &performOverlaying)
{
	switch (gameObjectA->_CategoryMask | gameObjectB->_CategoryMask)
	{
	case BOMB_EXPLOSION | BREAKABLE_PLATFORM:
	{
		if (gameObjectA->_CategoryMask == BREAKABLE_PLATFORM)
		{
			((BreakablePlatform*)gameObjectA)->OnHitBomb();
		}
		else
		{
			((BreakablePlatform*)gameObjectB)->OnHitBomb();
		}
		performOverlaying = false;
		break;
	}
	default: break;
	}
	
}

void CollisionCallback::OnTriggerStay(GameObject * gameObjectA, GameObject * gameObjectB)
{
}

void CollisionCallback::OnTriggerExit(GameObject * gameObjectA, GameObject * gameObjectB)
{
}


