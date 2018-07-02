#pragma once
#include "define.h"
#include "define_CollisionType.h"

class GameObject
{
protected:
	POINT _Position;
	POINT _Size;
	CollisionType collisionType;
public:
	GameObject();
	~GameObject();
	POINT getPosition() const;
	POINT getSize() const;
	CollisionType getCollisionType();
	void setPosition(const POINT& p);
	void setSize(const POINT &p);
	void setPosition(const float& x, const float& y);
	void setSize(const float& w, const float& h);
	void setCollisionType(CollisionType ct);
};
