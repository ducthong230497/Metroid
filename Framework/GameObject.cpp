#include "GameObject.h"

GameObject::GameObject()
{
	_Position.x = _Position.y = 0;
	_Size.x = _Size.y = 0;
}

GameObject::~GameObject()
{
}

POINT GameObject::getPosition() const
{
	return _Position;
}

POINT GameObject::getSize() const
{
	return _Size;
}

CollisionType GameObject::getCollisionType()
{
	return collisionType;
}

void GameObject::setPosition(const POINT & p)
{
	_Position = p;
}

void GameObject::setSize(const POINT & p)
{
	_Size = p;
}

void GameObject::setPosition(const float& x, const float& y)
{
	_Position.x = x;
	_Position.y = y;
}

void GameObject::setSize(const float& w, const float& h)
{
	_Size.x = w;
	_Size.y = h;
}

void GameObject::setCollisionType(CollisionType ct)
{
	collisionType = ct;
}
