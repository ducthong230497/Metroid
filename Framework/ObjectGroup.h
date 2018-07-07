#pragma once
#include <string>
#include <vector>
#include "Rectangle.h"


class ObjectGroup
{
private:
	std::string _Name;
	std::vector<Shape::Rectangle> _Rectangles;


	void SetAttributes(const std::string& name);
	void AddRect(const Shape::Rectangle &rectangle);


	friend class MapLoader;

public:
	ObjectGroup();
	~ObjectGroup();
	//ObjectGroup(const ObjectGroup &ObjectGroup);
	//ObjectGroup& operator=(const ObjectGroup &ObjectGroup);


	std::string GetName();
	const std::vector<Shape::Rectangle>& GetRects() const;

};