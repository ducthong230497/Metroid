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
	uint16_t _BitMask;
	uint16_t _CategoryMask;
	CollisionType collisionType;
public:
	GameObject();
	~GameObject();
	POINT getPosition() const;
	POINT getSize() const;
	POINT getVelocity() const;
	uint16_t getBitMask();
	uint16_t getCategoryMask();
	CollisionType getCollisionType();
	void setPosition(const POINT& p);
	void setSize(const POINT &p);
	void setVelocity(const float& x, const float& y);
	void setPosition(const float& x, const float& y);
	void setSize(const float& w, const float& h);
	void setCollisionType(CollisionType ct);
	void setBitMask(uint16_t bitmask);
	void setCategoryMask(uint16_t categorymask);
	void Next(float DeltaTime, bool moveX, bool moveY);
	virtual void Update();
};
