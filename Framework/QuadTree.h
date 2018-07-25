#ifndef _QUADTREE_H_
#define _QUADTREE_H_


#include "utils.h"
#include <map>
#include <vector>
#include "GameObject.h"
#include "../rapidxml/rapidxml.hpp"
#include "../rapidxml/rapidxml_print.hpp"
#include "Collision.h"
#include "Windows.h"
#include "Camera.h"
#include "Rectangle.h"
#include "TileLayer.h"
#include "define.h"
#include "Zoomer.h"
#include "Skree.h"
#include "Rio.h"
#include "Ripper.h"
#include "Bomb.h"
#include "Marunari.h"
#include "BombItem.h"
#include "Door.h"

class QObject;
class QuadTreeNode;

class QObject
{
private:
	bool isGameObject; //if true, contain body. if false, contain Rectangle
	unsigned int id;
	RECT rect;

	GameObject* gameObject;
	Shape::Rectangle* tileRectangle;

	friend class QuadTree;

public:
	QObject()
	{
		gameObject = NULL;
		tileRectangle = NULL;
	}

	QObject(unsigned int id, GameObject* gameObject, Shape::Rectangle* tileRectangle)
	{
		this->id = id;
		this->gameObject = gameObject;
		this->tileRectangle = tileRectangle;

	}
	~QObject(){}
	void Release()
	{
		if (tileRectangle != NULL)
		{
			delete tileRectangle;
			tileRectangle = NULL;
		}
	}
};

class QuadTreeNode
{
private:
	RECT rect;
	unsigned int parentID;
	unsigned int id;
	QuadTreeNode *tl, *tr, *bl, *br;
	std::vector<QObject> QObjects;

	friend class QuadTree;

public:

	QuadTreeNode()
	{
		tl = nullptr;
		tr = nullptr;
		bl = nullptr;
		br = nullptr;
	}

	QuadTreeNode(unsigned int id, unsigned int parentid)
	{
		tl = nullptr;
		tr = nullptr;
		bl = nullptr;
		br = nullptr;
		this->id = id;
		this->parentID = parentid;
	}

	void Release()
	{
		for (std::vector<QObject>::iterator it = QObjects.begin(); it != QObjects.end(); ++it)
		{
			it->Release();
		}
	}
	~QuadTreeNode() {}
	//unsigned int GetParentID()
	//{
	//	return parentID;
	//}

	//unsigned int GetID()
	//{
	//	return id;
	//}


};


//Performance issue somehow
//Used for TMX map to reduce the amount of tile have to draw
class QuadTree
{
private:
	//"write to xml" variables
	rapidxml::xml_document<> doc;
	rapidxml::xml_node<>* rootXmlNode;
	std::string outpath;



	//"read from quadtree xml" variables
	std::map<unsigned int, GameObject*> mapObject;
	std::map<unsigned int, Shape::Rectangle*> mapTileRectangle;
	std::map<unsigned int, QuadTreeNode*> mapQuadTree;
	QuadTreeNode *rootQNode;

	//list all bodies within viewport
	std::vector<GameObject*> objectsInViewport;
	//list all tile rectangle within the viewport
	std::vector<Shape::Rectangle*> tileRectsInViewport;

	float minSquareSize;

	bool loadGameObject;
	bool loadTileRect;

	//World *world;

	std::map<std::string, std::vector<GameObject*>> objectGroups;

	friend class MainScene;
private:

	friend class World;
	friend class TileMap;


	void StartBuildingTreeNodes(const std::string &outpath, const std::string &tmxFilePath);
	//
	void BuildTree(QuadTreeNode* sdqNode, rapidxml::xml_node<>* parentXmlNode);
	//
	void EndBuildingTreeNodes();
	//
	void Release(QuadTreeNode* sdqNode);


	void Load(rapidxml::xml_node<>* xmlnode);

	void LoadObjectsInViewport(const RECT &viewport, QuadTreeNode* sdqNode);

	void LinkNodes();

public:
	QuadTree();
	~QuadTree();

	void SetMinSquareSize(float minSquareXY);

	////if world is set, 
	//void SetWorld(World *world);

	//
	void BuildTreeNodesFromTMX(const std::string &outpath, const std::string &tmxFilePath);

	//Load tileRects and bodies from quadtreefile and tmxmapfile
	void Load(const std::string &quadtreeFilePath, const std::string &tmxFilePath);

	GameObject* InitGameObject(std::string str, float id, float x, float y, float w, float h, Shape::Rectangle rect);

	const std::map<unsigned int, QuadTreeNode*>& GetMapQuadTree();

	const std::map<unsigned int, GameObject*>& GetMapObject();

	const std::map<unsigned int, Shape::Rectangle*>& GetMapTileRect();

	//SDQNode* GetRootSDQNode();

	void LoadObjectsInViewport(Camera *camera, bool loadBody, bool loadTileRect);


	const std::vector<GameObject*>& GetObjectsInViewport();

	const std::vector<Shape::Rectangle*>& GetTileRectsInViewport();


	const std::vector<GameObject*>& GetObjectsGroup(const std::string &groupName);

};

#endif // !_QUADTREE_H_