#pragma once
#include "define.h"
#include "define_CollisionType.h"

class GameObject
{
protected:
	POINT _Position;
	POINT _Size;
public:
	GameObject();
	~GameObject();
	static CollisionType collisionType;
	POINT getPosition() const;
	POINT getSize() const;
	void setPosition(const POINT& p);
	void setSize(const POINT &p);
	void setPosition(const float& x, const float& y);
	void setSize(const float& w, const float& h);
};
