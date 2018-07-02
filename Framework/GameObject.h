#pragma once
#include "define.h"
#include "define_CollisionType.h"
#include "define_Category.h"
class GameObject
{
protected:
	POINT _Position;
	POINT _Size;
	POINT _Velocity;
	Category _BitMask;
	Category _CategoryMask;
	CollisionType collisionType;
public:
	GameObject();
	~GameObject();
	POINT getPosition() const;
	POINT getSize() const;
	POINT getVelocity() const;
	Category getBitMask();
	Category getCategoryMask();
	CollisionType getCollisionType();
	void setPosition(const POINT& p);
	void setSize(const POINT &p);
	void setVelocity(const float& x, const float& y);
	void setPosition(const float& x, const float& y);
	void setSize(const float& w, const float& h);
	void setCollisionType(CollisionType ct);
	void set
};
