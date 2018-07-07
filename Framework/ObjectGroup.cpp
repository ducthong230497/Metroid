#include "ObjectGroup.h"



ObjectGroup::ObjectGroup()
{
	_Rectangles.clear();
}


ObjectGroup::~ObjectGroup()
{
	/*for (std::vector<Shape::Rectangle>::iterator rect = _Rectangles.begin(); rect != _Rectangles.end(); ++rect)
	{
	delete *rect;
	}*/
}

//ObjectGroup::ObjectGroup(const ObjectGroup &ObjectGroup)
//{
//	_Name = ObjectGroup._Name;
//	for ( std::vector<_Rectangles*>:: const_iterator bo = ObjectGroup._Bodies.begin(); bo != ObjectGroup._Bodies.end(); ++bo)
//	{
//		Body*body = new Body();
//		*body = **bo;
//
//		_Bodies.push_back(body);
//	}
//
//}
//ObjectGroup& ObjectGroup::operator=(const ObjectGroup &ObjectGroup)
//{
//	_Name = ObjectGroup._Name;
//	for (std::vector<Body*>::const_iterator bo = ObjectGroup._Bodies.begin(); bo != ObjectGroup._Bodies.end(); ++bo)
//	{
//		Body*body = new Body();
//		*body = **bo;
//
//		_Bodies.push_back(body);
//	}
//
//	return *this;
//}

void ObjectGroup::SetAttributes(const std::string& name)
{
	_Name = name;
}
void ObjectGroup::AddRect(const Shape::Rectangle &rect)
{
	_Rectangles.push_back(rect);
}

std::string  ObjectGroup::GetName()
{
	return _Name;
}

const std::vector<Shape::Rectangle>& ObjectGroup::GetRects() const
{
	return _Rectangles;
}