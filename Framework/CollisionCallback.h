#pragma once
#include "Ripper.h"
#include "Skree.h"
#include "Rio.h"
#include "Zoomer.h"
#include "BreakablePlatform.h"
#include "Marunari.h"
#include "BombItem.h"
#include "Bullet.h"
#include "HealthItem.h"
#include "Kraid.h"
#include "MotherBrain.h"
#include "Door.h"
#include "Zeebetite.h"
#include "Rocket.h"
class CollisionCallback
{
public:
	CollisionCallback();
	~CollisionCallback();

	//Call everytime two object collide with each other. 
	//Occur at the same moment of update world function
	//Note that if it's possible, don't try to perform any of the physic things here because it may lead to wrong physic results later. 
	void OnCollisionEnter(GameObject *gameObjectA, GameObject *gameObjectB, const POINT &CollisionDirection);

	void OnColliding(GameObject *gameObjectA, GameObject * gameObjectB, const POINT &collisionDirection);

	void OnCollisionExit(GameObject * gameObjectA, GameObject * gameObjectB, const POINT &collisionDirection);

	void OnTriggerEnter(GameObject *gameObjectA, GameObject *gameObjectB, bool &performOverLaying);

	void OnTriggerStay(GameObject *gameObjectA, GameObject *gameObjectB);

	void OnTriggerExit(GameObject *gameObjectA, GameObject * gameObjectB);

};