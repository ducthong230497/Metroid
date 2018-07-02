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

POINT GameObject::getVelocity() const
{
	return _Velocity;
}

Category GameObject::getBitMask()
{
	return _BitMask;
}

Category GameObject::getCategoryMask()
{
	return _CategoryMask;
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

void GameObject::setVelocity(const float & x, const float & y)
{
	_Velocity.x = x;
	_Velocity.y = y;
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

void GameObject::setBitMask(Category bitmask)
{
	_BitMask = bitmask;
}

void GameObject::setCategoryMask(Category categorymask)
{
	_CategoryMask = categorymask;
}
