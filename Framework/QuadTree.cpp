#include "QuadTree.h"
#include "BreakablePlatform.h"


QuadTree::QuadTree()
{
	minSquareSize =  SCREEN_WIDTH;
	//world = NULL;
}


QuadTree::~QuadTree()
{
	for (std::map<unsigned int, QuadTreeNode*>::iterator it = mapQuadTree.begin(); it != mapQuadTree.end(); ++it)
	{
		if (it->second != NULL)
		{
			delete it->second;
			it->second = NULL;
		}
	}

	for (std::map<unsigned int, Shape::Rectangle*>::iterator it = mapTileRectangle.begin(); it != mapTileRectangle.end(); ++it)
	{
		if (it->second != NULL)
		{
			delete it->second;
			it->second = NULL;
		}
	}

	for (std::map<unsigned int, GameObject*>::iterator it = mapObject.begin(); it != mapObject.end(); ++it)
	{
		if (it->second != NULL)
		{
			delete it->second;
			it->second = NULL;
		}
	}
}

void QuadTree::SetMinSquareSize(float minXY)
{
	minSquareSize = minXY;
}

void QuadTree::StartBuildingTreeNodes(const std::string &outpath, const std::string &tmxFilePath)
{
	this->outpath = outpath;

	//setup out xml file
	rapidxml::xml_node<>* declareNode = doc.allocate_node(rapidxml::node_declaration);
	declareNode->append_attribute(doc.allocate_attribute("version", "1.0"));
	declareNode->append_attribute(doc.allocate_attribute("encoding", "utf-8"));
	doc.append_node(declareNode);

	rootXmlNode = doc.allocate_node(rapidxml::node_element, "spacedivisionmap");


	//load tmx file
	std::string fileContents = "";
	if (LoadFile(tmxFilePath, fileContents) == true)
	{
		// Create new RapidXML document instance to use to parse data
		rapidxml::xml_document<char> currentMap;

		//parse the file
		currentMap.parse<0>((char*)fileContents.c_str());

		//get first node
		rapidxml::xml_node<> *parentNode = currentMap.first_node("map");

		int mapWidth = atoi(parentNode->first_attribute("width")->value());
		int mapHeight = atoi(parentNode->first_attribute("height")->value());
		int tileWidth = atoi(parentNode->first_attribute("tilewidth")->value());
		int tileHeight = atoi(parentNode->first_attribute("tileheight")->value());

		unsigned int sdmwidth = mapWidth * tileWidth;
		unsigned int sdmheight = mapHeight * tileHeight;
		char* value;
		if (sdmwidth > sdmheight)
		{
			//get value to append to the note
			value = doc.allocate_string(std::to_string(sdmwidth).c_str());
		}
		else
		{
			value = doc.allocate_string(std::to_string(sdmheight).c_str());
		}

		//appen width and height
		rootXmlNode->append_attribute(doc.allocate_attribute("width", value));
		rootXmlNode->append_attribute(doc.allocate_attribute("height", value));
		//append to xml doc 
		doc.append_node(rootXmlNode);

		//create a root QNode
		rootQNode = new QuadTreeNode();
		rootQNode->parentID = 0;
		rootQNode->id = 0;
		//set rect
		rootQNode->rect.top = atoi(value);
		rootQNode->rect.left = 0;
		rootQNode->rect.bottom = 0;
		rootQNode->rect.right = atoi(value);

		//get all objects in objects group from tmx file and put to rootQNode
		rapidxml::xml_node<> * objectgroupNode = parentNode->first_node("objectgroup");

		while (objectgroupNode != nullptr)
		{
			rapidxml::xml_node<> *currentTMXNode = objectgroupNode->first_node("object");

			while (currentTMXNode != nullptr)
			{
				unsigned int id = atoi(currentTMXNode->first_attribute("id")->value());
				unsigned int x = atoi(currentTMXNode->first_attribute("x")->value());
				unsigned int y = atoi(currentTMXNode->first_attribute("y")->value());
				unsigned int width = atoi(currentTMXNode->first_attribute("width")->value());
				unsigned int height = atoi(currentTMXNode->first_attribute("height")->value());

				QObject sdqObject;
				sdqObject.isGameObject = true;
				sdqObject.id = id;
				sdqObject.rect.top = sdmheight - y;
				sdqObject.rect.left = x;
				sdqObject.rect.bottom = sdmheight - y - height;
				sdqObject.rect.right = x + width;


				rootQNode->QObjects.push_back(sdqObject);

				currentTMXNode = currentTMXNode->next_sibling();
			}


			objectgroupNode = objectgroupNode->next_sibling();
		}

		//get tile data
		// Create a new node based on the parent node
		rapidxml::xml_node<> *layerNode = parentNode;
		// Move to first layer node
		layerNode = parentNode->first_node("layer");
		TileLayer layer;
		if (layerNode != nullptr)
		{
			//get layer attributes
			unsigned int layerWidth = atoi(layerNode->first_attribute("width")->value());
			unsigned int layerHeight = atoi(layerNode->first_attribute("height")->value());

			//set layer attributes
			layer.SetAttributes("", layerWidth, layerHeight);

			//get layer data
			char* data = layerNode->first_node("data")->value();

			//set layer data
			layer.SetData(data);

		}

		//get tile set 
		// Create a new node based on the parent node
		rapidxml::xml_node<> *currentNode = parentNode;
		//get tileset node
		currentNode = currentNode->first_node("tileset");
		unsigned int tileSetWidth = 0;
		unsigned int tileSetHeight = 0;
		// Check if there is a tileset node
		if (currentNode != nullptr)
		{
			tileSetWidth = atoi(currentNode->first_attribute("tilewidth")->value());
			tileSetHeight = atoi(currentNode->first_attribute("tileheight")->value());
		}


		//load map tile rectangle
		unsigned int **data = layer.GetData();
		unsigned int layerWidth = layer.GetWidth();
		unsigned int layerHeight = layer.GetHeight();

		float x, y;
		float width = tileSetWidth;
		float height = tileSetHeight;
		unsigned int id = -1;
		for (unsigned int row = 0; row < layerHeight; row++)
		{

			for (unsigned int column = 0; column < layerWidth; column++)
			{
				id++;
				if (data[row][column] == 0) continue;

				x = column * width + width / 2;
				y = (layerHeight - 1 - row)*height + height / 2;

				QObject sdqObject;
				sdqObject.isGameObject = false;
				sdqObject.id = id;
				sdqObject.rect.top = y + height / 2;
				sdqObject.rect.left = x - width / 2;
				sdqObject.rect.bottom = y - height / 2;
				sdqObject.rect.right = x + width / 2;

				rootQNode->QObjects.push_back(sdqObject);


			}
		}

	}
}


void QuadTree::BuildTree(QuadTreeNode* QNode, rapidxml::xml_node<>* parentXmlNode)
{
	if (QNode == nullptr) return;

	//allocate a sdq node
	rapidxml::xml_node<>* xmlNode = doc.allocate_node(rapidxml::node_element, "QNode");
	//append attribute id
	char* id = doc.allocate_string(std::to_string(QNode->id).c_str());
	xmlNode->append_attribute(doc.allocate_attribute("id", id));
	//append attribute parent id
	char* parentid = doc.allocate_string(std::to_string(QNode->parentID).c_str());
	xmlNode->append_attribute(doc.allocate_attribute("parentid", parentid));
	//append attribute rect
	char* rectTop = doc.allocate_string(std::to_string(QNode->rect.top).c_str());
	xmlNode->append_attribute(doc.allocate_attribute("recttop", rectTop));
	char* rectLeft = doc.allocate_string(std::to_string(QNode->rect.left).c_str());
	xmlNode->append_attribute(doc.allocate_attribute("rectleft", rectLeft));
	char* rectBottom = doc.allocate_string(std::to_string(QNode->rect.bottom).c_str());
	xmlNode->append_attribute(doc.allocate_attribute("rectbottom", rectBottom));
	char* rectRight = doc.allocate_string(std::to_string(QNode->rect.right).c_str());
	xmlNode->append_attribute(doc.allocate_attribute("rectright", rectRight));

	//append this node to root node
	parentXmlNode->append_node(xmlNode);

	if (QNode->rect.top - QNode->rect.bottom < minSquareSize)
	{
		//append GameObjectid and tileid to this node
		for (std::vector<QObject>::iterator it = QNode->QObjects.begin(); it != QNode->QObjects.end(); ++it)
		{
			char* id = doc.allocate_string(std::to_string(it->id).c_str());
			if (it->isGameObject)
			{
				xmlNode->append_attribute(doc.allocate_attribute("GameObjectid", id));
			}
			else
			{
				xmlNode->append_attribute(doc.allocate_attribute("tileid", id));
			}
		}



		return;
	}

	if (QNode->QObjects.size() == 0)
	{
		return;
	}


	//top left
	QNode->tl = new QuadTreeNode(QNode->id * 10 + 1, QNode->id);
	QNode->tl->rect.top = QNode->rect.top;
	QNode->tl->rect.left = QNode->rect.left;
	QNode->tl->rect.bottom = (QNode->rect.top + QNode->rect.bottom) / 2;
	QNode->tl->rect.right = (QNode->rect.left + QNode->rect.right) / 2;

	//top right
	QNode->tr = new QuadTreeNode(QNode->id * 10 + 2, QNode->id);
	QNode->tr->rect.top = QNode->rect.top;
	QNode->tr->rect.left = (QNode->rect.left + QNode->rect.right) / 2;
	QNode->tr->rect.bottom = (QNode->rect.top + QNode->rect.bottom) / 2;
	QNode->tr->rect.right = QNode->rect.right;


	//bottom left
	QNode->br = new QuadTreeNode(QNode->id * 10 + 3, QNode->id);
	QNode->br->rect.top = (QNode->rect.top + QNode->rect.bottom) / 2;
	QNode->br->rect.left = (QNode->rect.left + QNode->rect.right) / 2;
	QNode->br->rect.bottom = QNode->rect.bottom;
	QNode->br->rect.right = QNode->rect.right;


	//bottom right
	QNode->bl = new QuadTreeNode(QNode->id * 10 + 4, QNode->id);
	QNode->bl->rect.top = (QNode->rect.top + QNode->rect.bottom) / 2;
	QNode->bl->rect.left = QNode->rect.left;
	QNode->bl->rect.bottom = QNode->rect.bottom;
	QNode->bl->rect.right = (QNode->rect.left + QNode->rect.right) / 2;

	Collision collision;

	for (std::vector<QObject>::iterator it = QNode->QObjects.begin(); it != QNode->QObjects.end(); ++it)
	{
		if (collision.IsOverlayingRect(it->rect, QNode->tl->rect))
		{
			QNode->tl->QObjects.push_back(*it);
		}

		if (collision.IsOverlayingRect(it->rect, QNode->tr->rect))
		{
			QNode->tr->QObjects.push_back(*it);
		}

		if (collision.IsOverlayingRect(it->rect, QNode->br->rect))
		{
			QNode->br->QObjects.push_back(*it);
		}

		if (collision.IsOverlayingRect(it->rect, QNode->bl->rect))
		{
			QNode->bl->QObjects.push_back(*it);
		}

	}

	//clear 
	QNode->QObjects.clear();

	//
	BuildTree(QNode->tl, xmlNode);
	BuildTree(QNode->tr, xmlNode);
	BuildTree(QNode->br, xmlNode);
	BuildTree(QNode->bl, xmlNode);
}

//
void QuadTree::EndBuildingTreeNodes()
{
	Release(rootQNode);

	// Save to file
	std::ofstream file_stored(outpath);
	file_stored << doc;
	file_stored.close();

	//clear for sure
	doc.clear();
}

void  QuadTree::Release(QuadTreeNode* QNode)
{
	if (QNode == nullptr) return;

	Release(QNode->tl);
	Release(QNode->tr);
	Release(QNode->br);
	Release(QNode->bl);

	if (QNode->tl != nullptr)
	{
		QNode->tl->Release();
		delete QNode->tl;
		QNode->tl = nullptr;
	}

	if (QNode->tr != nullptr)
	{
		QNode->tr->Release();
		delete QNode->tr;
		QNode->tr = nullptr;
	}

	if (QNode->br != nullptr)
	{
		QNode->br->Release();
		delete QNode->br;
		QNode->br = nullptr;
	}

	if (QNode->bl != nullptr)
	{
		QNode->bl->Release();
		delete QNode->bl;
		QNode->bl = nullptr;
	}

}

//
void QuadTree::BuildTreeNodesFromTMX(const std::string &outpath, const std::string &tmxFilePath)
{

	StartBuildingTreeNodes(outpath, tmxFilePath);
	BuildTree(rootQNode, rootXmlNode);
	EndBuildingTreeNodes();

}



void QuadTree::Load(const std::string &quadtreeFilePath, const std::string &tmxFilePath)
{
	//load tmx file
	std::string tmxfileContents = "";
	if (LoadFile(tmxFilePath, tmxfileContents) == true)
	{
		// Create new RapidXML document instance to use to parse data
		rapidxml::xml_document<char> currentMap;

		//parse the file
		currentMap.parse<0>((char*)tmxfileContents.c_str());

		//get first node
		rapidxml::xml_node<> *parentNode = currentMap.first_node("map");

		unsigned int mapwidth = atoi(parentNode->first_attribute("width")->value());
		unsigned int mapheight = atoi(parentNode->first_attribute("height")->value());
		unsigned int maptileWidth = atoi(parentNode->first_attribute("tilewidth")->value());
		unsigned int maptileHeight = atoi(parentNode->first_attribute("tileheight")->value());

		// Create a new node based on the parent node
		rapidxml::xml_node<> *currentNode = parentNode;

		// Move to first layer node
		currentNode = currentNode->first_node("objectgroup");

		while (currentNode != nullptr)
		{
			std::string groupName = currentNode->first_attribute("name")->value();
			std::vector<GameObject*> gameObjectsInGroup;

			//get object node
			rapidxml::xml_node<> *childNode = currentNode->first_node("object");
			while (childNode != nullptr)
			{
				float id = atoi(childNode->first_attribute("id")->value());
				float x = atoi(childNode->first_attribute("x")->value());
				float y = atoi(childNode->first_attribute("y")->value());
				float width = atoi(childNode->first_attribute("width")->value());
				float height = atoi(childNode->first_attribute("height")->value());

				Shape::Rectangle rect(x + width / 2, mapheight*maptileHeight - y - height / 2, width, height);
				rect.id = id;

				/*GameObject* gameObject = new GameObject();
				gameObject->setPosition(rect.x, rect.y);
				gameObject->setSize(rect.width, rect.height);
				gameObject->_CategoryMask = 0;
				gameObject->_BitMask = 0;
				gameObject->id = id;*/

				GameObject* gameObject = InitGameObject(groupName, id, x, y, width, height, rect);

				//if (world != NULL)
				//{
				//	//push to world
				//	world->AddSDQGameObject(GameObject);

				//push to vector
				gameObjectsInGroup.push_back(gameObject);

				/*}*/

				//push to map GameObject
				mapObject[rect.id] = gameObject;

				//next
				childNode = childNode->next_sibling("object");
			}

			//push to GameObjectGroups
			objectGroups[groupName] = gameObjectsInGroup;

			// Move to the next objectGroup
			currentNode = currentNode->next_sibling("objectgroup");
		}

		//get tile data
		// Create a new node based on the parent node
		rapidxml::xml_node<> *layerNode = parentNode;
		// Move to first layer node
		layerNode = parentNode->first_node("layer");
		TileLayer layer;
		if (layerNode != nullptr)
		{
			//get layer attributes
			unsigned int layerWidth = atoi(layerNode->first_attribute("width")->value());
			unsigned int layerHeight = atoi(layerNode->first_attribute("height")->value());

			//set layer attributes
			layer.SetAttributes("", layerWidth, layerHeight);

			//get layer data
			char* data = layerNode->first_node("data")->value();

			//set layer data
			layer.SetData(data);

		}

		//get tile set 
		// Create a new node based on the parent node
		currentNode = parentNode;
		//get tileset node
		currentNode = currentNode->first_node("tileset");
		unsigned int tileSetWidth = 0;
		unsigned int tileSetHeight = 0;
		unsigned int columns = 0;
		// Check if there is a tileset node
		if (currentNode != nullptr)
		{
			tileSetWidth = atoi(currentNode->first_attribute("tilewidth")->value());
			tileSetHeight = atoi(currentNode->first_attribute("tileheight")->value());
			columns = atoi(currentNode->first_attribute("columns")->value());
		}


		//load map tile rectangle
		unsigned int **data = layer.GetData();
		unsigned int layerWidth = layer.GetWidth();
		unsigned int layerHeight = layer.GetHeight();

		float x, y, rectImageLeft, rectImageTop;
		float width = tileSetWidth;
		float height = tileSetHeight;
		unsigned int id = -1;
		for (unsigned int row = 0; row < layerHeight; row++)
		{
			for (unsigned int column = 0; column < layerWidth; column++)
			{
				id++;
				if (data[row][column] == 0) continue;

				rectImageLeft = ((data[row][column] - 1) % columns) * width;
				rectImageTop = ((data[row][column] - 1) / columns) * height;

				x = column * width + width / 2;
				y = (layerHeight - 1 - row)*height + height / 2;
				Shape::Rectangle* rect = new Shape::Rectangle(x, y, width, height);
				rect->id = id;
				rect->extraX = rectImageLeft;
				rect->extraY = rectImageTop;
				mapTileRectangle[id] = rect;

			}
		}

	}

	// String to hold file contents
	std::string fileContents = "";
	if (LoadFile(quadtreeFilePath, fileContents) == true)
	{
		// Create new RapidXML document instance to use to parse data
		rapidxml::xml_document<char> currentMap;

		//parse the file
		currentMap.parse<0>((char*)fileContents.c_str());

		//get first node
		rapidxml::xml_node<> *parentNode = currentMap.first_node("spacedivisionmap");


		//get sdq node
		rapidxml::xml_node<>* currentNode = parentNode->first_node("QNode");

		Load(currentNode);

		LinkNodes();
	}



}

GameObject * QuadTree::InitGameObject(std::string str, float id, float x, float y, float w, float h, Shape::Rectangle rect)
{
	if (str._Equal("Player"))
	{
		GameObject* gameObject = new GameObject();
		gameObject->setPosition(rect.x, rect.y);
		gameObject->setSize(rect.width, rect.height);
		gameObject->_CategoryMask = 0;
		gameObject->_BitMask = 0;
		gameObject->id = id;
		gameObject->isActive = false;
		return gameObject;
	}
	else if (str._Equal("Platform"))
	{
		GameObject* gameObject = new GameObject();
		gameObject->setPosition(rect.x, rect.y);
		gameObject->setSize(rect.width, rect.height);
		gameObject->_CategoryMask = 0;
		gameObject->_BitMask = 0;
		gameObject->id = id;
		return gameObject;
	}
	else if (str._Equal("BreakablePlatform"))
	{
		BreakablePlatform * breakablePlatform = new BreakablePlatform();
		breakablePlatform->setPosition(rect.x, rect.y);
		breakablePlatform->setSize(rect.width, rect.height);
		breakablePlatform->_CategoryMask = 0;
		breakablePlatform->_BitMask = 0;
		breakablePlatform->id = id;
		return breakablePlatform;
	}
	else if(str._Equal("Zoomer"))
	{
		Zoomer* zoomer = new Zoomer();
		zoomer->setPosition(rect.x, rect.y);
		zoomer->setSize(rect.width, rect.height);
		zoomer->_CategoryMask = 0;
		zoomer->_BitMask = 0;
		zoomer->id = id;
		return zoomer;
	}
	else if (str._Equal("InverseZoomer"))
	{
		Zoomer* zoomer = new Zoomer();
		zoomer->setPosition(rect.x, rect.y);
		zoomer->setSize(rect.width, rect.height);
		zoomer->_CategoryMask = 0;
		zoomer->_BitMask = 0;
		zoomer->id = id;
		return zoomer;
	}
	else if (str._Equal("Skree"))
	{
		Skree* skree = new Skree();
		skree->setPosition(rect.x, rect.y);
		skree->setSize(rect.width, rect.height);
		skree->_CategoryMask = 0;
		skree->_BitMask = 0;
		skree->id = id;
		return skree;
	}
	else if (str._Equal("Ripper"))
	{
		Ripper* ripper = new Ripper();
		ripper->setPosition(rect.x, rect.y);
		ripper->setSize(rect.width, rect.height);
		ripper->_CategoryMask = 0;
		ripper->_BitMask = 0;
		ripper->id = id;
		return ripper;
	}
	else if (str._Equal("Rio"))
	{
		Rio* rio = new Rio();
		rio->setPosition(rect.x, rect.y);
		rio->setSize(rect.width, rect.height);
		rio->_CategoryMask = 0;
		rio->_BitMask = 0;
		rio->id = id;
		return rio;
	}
	else if (str._Equal("Marunari"))
	{
		Marunari* marunari = new Marunari();
		marunari->setPosition(rect.x, rect.y);
		marunari->setSize(rect.width, rect.height);
		marunari->_CategoryMask = 0;
		marunari->_BitMask = 0;
		marunari->id = id;
		return marunari;
	}
	else if (str._Equal("Door"))
	{
		Door* door = new Door();
		door->setPosition(rect.x, rect.y);
		door->setSize(rect.width, rect.height);
		door->_CategoryMask = 0;
		door->_BitMask = 0;
		door->id = id;
		return door;
	}
	else if(str._Equal("Kraid"))
	{
		Kraid * kraid = new Kraid();
		kraid->setPosition(rect.x, rect.y);
		kraid->setSize(rect.width, rect.height);
		kraid->_CategoryMask = 0;
		kraid->_BitMask = 0;
		kraid->id = id;
		return kraid;
	}
	else if (str._Equal("Zeebetite"))
	{
		Zeebetite * zeebetite = new Zeebetite();
		zeebetite->setPosition(rect.x, rect.y);
		zeebetite->setSize(rect.width, rect.height);
		zeebetite->_CategoryMask = 0;
		zeebetite->_BitMask = 0;
		zeebetite->id = id;
		return zeebetite;
	}
	else if (str._Equal("MotherBrain"))
	{
		MotherBrain * motherBrain = new MotherBrain();
		motherBrain->setPosition(rect.x, rect.y);
		motherBrain->setSize(rect.width, rect.height);
		motherBrain->_CategoryMask = 0;
		motherBrain->_BitMask = 0;
		motherBrain->id = id;
		return motherBrain;
	}
	else if (str._Equal("CircleBullet"))
	{
		Rinka * circleBullet = new Rinka();
		circleBullet->setPosition(rect.x, rect.y);
		circleBullet->setSize(rect.width, rect.height);
		circleBullet->_CategoryMask = 0;
		circleBullet->_BitMask = 0;
		circleBullet->id = id;
		return circleBullet;
	}
	else if (str._Equal("LeftCannon"))
	{
		Cannon * cannon = new Cannon();
		cannon->setPosition(rect.x, rect.y);
		cannon->setSize(rect.width, rect.height);
		cannon->_CategoryMask = 0;
		cannon->_BitMask = 0;
		cannon->id = id;
		return cannon;
	}
	else if (str._Equal("RightCannon"))
	{
		Cannon * cannon = new Cannon();
		cannon->setPosition(rect.x, rect.y);
		cannon->setSize(rect.width, rect.height);
		cannon->_CategoryMask = 0;
		cannon->_BitMask = 0;
		cannon->id = id;
		return cannon;
	}else if (str._Equal("Collide"))
	{
		GameObject * cannon = new GameObject();
		cannon->setPosition(rect.x, rect.y);
		cannon->setSize(rect.width, rect.height);
		cannon->_CategoryMask = 0;
		cannon->_BitMask = 0;
		cannon->id = id;
		return cannon;
	}
	else if (str._Equal("CheckPoint"))
	{
		GameObject * checkPoint = new GameObject();
		checkPoint->setPosition(rect.x, rect.y);
		checkPoint->setSize(rect.width, rect.height);
		checkPoint->_CategoryMask = 0;
		checkPoint->_BitMask = 0;
		checkPoint->id = id;
		return checkPoint;
	}
	return nullptr;
}

void QuadTree::Load(rapidxml::xml_node<>* xmlnode)
{
	if (xmlnode == nullptr) return;

	QuadTreeNode* QNode = new QuadTreeNode();

	//get id of the node
	QNode->id = atoi(xmlnode->first_attribute("id")->value());

	//get parentid of the node
	QNode->parentID = atoi(xmlnode->first_attribute("parentid")->value());

	//get rect
	QNode->rect.top = atoi(xmlnode->first_attribute("recttop")->value());
	QNode->rect.left = atoi(xmlnode->first_attribute("rectleft")->value());
	QNode->rect.bottom = atoi(xmlnode->first_attribute("rectbottom")->value());
	QNode->rect.right = atoi(xmlnode->first_attribute("rectright")->value());


	//get all GameObject ids
	rapidxml::xml_attribute<>* nodeGameObjectID = xmlnode->first_attribute("GameObjectid");

	while (nodeGameObjectID != nullptr)
	{
		unsigned int GameObjectID = atoi(nodeGameObjectID->value());
		QNode->QObjects.push_back(QObject(GameObjectID, mapObject[GameObjectID], NULL));

		//next attribute
		nodeGameObjectID = nodeGameObjectID->next_attribute("GameObjectid");
	}

	//get all tile rectangle ids
	rapidxml::xml_attribute<>* nodeTileID = xmlnode->first_attribute("tileid");

	while (nodeTileID != nullptr)
	{
		unsigned int tileID = atoi(nodeTileID->value());
		QNode->QObjects.push_back(QObject(tileID, NULL, mapTileRectangle[tileID]));

		//next attribute
		nodeTileID = nodeTileID->next_attribute("tileid");
	}

	//put this node into mapQuadtree
	mapQuadTree[QNode->id] = QNode;

	//Load for child node
	Load(xmlnode->first_node("QNode"));

	//Load for next sibling
	Load(xmlnode->next_sibling());


}

void QuadTree::LinkNodes()
{
	for (std::map<unsigned int, QuadTreeNode*>::iterator it = mapQuadTree.begin(); it != mapQuadTree.end(); ++it)
	{
		if (it->first == 0)
		{
			rootQNode = it->second;
		}
		else
		{
			//get parentID
			unsigned int parentID = it->second->parentID;

			//Get parent node from ID
			QuadTreeNode* parentNode = mapQuadTree[parentID];

			//get childID, which is current node
			unsigned int childID = it->first;
			switch (childID % 10)
			{
			case 1:
				parentNode->tl = it->second;
				break;
			case 2:
				parentNode->tr = it->second;
				break;
			case 3:
				parentNode->bl = it->second;
				break;
			case 4:
				parentNode->br = it->second;
				break;
			}
		}

	}

}

const std::map<unsigned int, QuadTreeNode*>& QuadTree::GetMapQuadTree()
{
	return mapQuadTree;
}

const std::map<unsigned int, GameObject*>& QuadTree::GetMapObject()
{
	return mapObject;
}

//QNode* QuadTree::GetRootQNode()
//{
//	return rootQNode;
//}

void QuadTree::LoadObjectsInViewport(Camera* camera, bool loadGameObject, bool loadTileRect)
{
	RECT viewportRect;
	//viewportRect.top = camera->getPosition().y + SCREEN_HEIGHT / 2;
	//viewportRect.left = camera->getPosition().x - SCREEN_WIDTH / 2;
	//viewportRect.bottom = camera->getPosition().y - SCREEN_HEIGHT / 2;
	//viewportRect.right = camera->getPosition().x + SCREEN_WIDTH / 2;

	viewportRect.top = camera->getPosition().y;
	viewportRect.left = camera->getPosition().x;
	viewportRect.bottom = camera->getPosition().y - SCREEN_HEIGHT;
	viewportRect.right = camera->getPosition().x + SCREEN_WIDTH;

	this->loadGameObject = loadGameObject;
	this->loadTileRect = loadTileRect;

	LoadObjectsInViewport(viewportRect, rootQNode);

}

void QuadTree::LoadObjectsInViewport(const RECT &viewport, QuadTreeNode* QNode)
{
	if (QNode == nullptr) return;

	if (QNode == rootQNode)
	{
		objectsInViewport.clear();
		tileRectsInViewport.clear();
	}

	Collision collision;
	if (QNode->tl == nullptr) //leaf node
	{
		if (collision.IsOverlayingRect(QNode->rect, viewport))
		{
			int a = 2;
			for (std::vector<QObject>::iterator it = QNode->QObjects.begin(); it != QNode->QObjects.end(); ++it)
			{
				GameObject *gameObject = it->gameObject;
				if (it->gameObject != NULL && /*it->gameObject->_CategoryMask != 0 &&*/ it->gameObject->isActive)
				{
					if (!loadGameObject) continue;

					//find this GameObject in bodiesInViewport
					std::vector<GameObject*>::iterator itGameObject = std::find(objectsInViewport.begin(), objectsInViewport.end(), gameObject);
					if (itGameObject != objectsInViewport.end()) //if this vector contains current GameObject
					{
						continue;
					}
					else
					{
						objectsInViewport.push_back(gameObject);
						if (gameObject->_CategoryMask == DOOR)
						{
							objectsInViewport.push_back(((Door*)gameObject)->leftDoor);
							objectsInViewport.push_back(((Door*)gameObject)->rightDoor);
						}
						else if (gameObject->_CategoryMask == KRAID)
						{
							objectsInViewport.insert(objectsInViewport.end(), ((Kraid*)gameObject)->bullets.begin(), ((Kraid*)gameObject)->bullets.end());
							objectsInViewport.insert(objectsInViewport.end(), ((Kraid*)gameObject)->boomerangs.begin(), ((Kraid*)gameObject)->boomerangs.end());
						}
						else if (gameObject->_CategoryMask == CANNON)
						{
							objectsInViewport.push_back((((Cannon*)gameObject)->cannonBullet));
						}
					}
				}
				else
				{
					if (it->tileRectangle != NULL)
					{
						if (!loadTileRect) continue;

						//find this GameObject in bodiesInViewport
						std::vector<Shape::Rectangle*>::iterator itRect = std::find(tileRectsInViewport.begin(), tileRectsInViewport.end(), it->tileRectangle);
						if (itRect != tileRectsInViewport.end()) //if this vector contains current GameObject
						{
							continue;
						}
						else
						{
							tileRectsInViewport.push_back(it->tileRectangle);
						}
					}
				}

			}
		}
	}
	else
	{
		if (collision.IsOverlayingRect(QNode->tl->rect, viewport))
		{
			LoadObjectsInViewport(viewport, QNode->tl);
		}
		if (collision.IsOverlayingRect(QNode->tr->rect, viewport))
		{
			LoadObjectsInViewport(viewport, QNode->tr);
		}
		if (collision.IsOverlayingRect(QNode->br->rect, viewport))
		{
			LoadObjectsInViewport(viewport, QNode->br);
		}
		if (collision.IsOverlayingRect(QNode->bl->rect, viewport))
		{
			LoadObjectsInViewport(viewport, QNode->bl);
		}
	}

}

const std::vector<GameObject*>&  QuadTree::GetObjectsInViewport()
{
	return objectsInViewport;
}

const std::vector<Shape::Rectangle*>&  QuadTree::GetTileRectsInViewport()
{
	return tileRectsInViewport;
}

const std::map<unsigned int, Shape::Rectangle*>& QuadTree::GetMapTileRect()
{
	return mapTileRectangle;
}

//void QuadTree::SetWorld(World *world)
//{
//	this->world = world;
//}

const std::vector<GameObject*>& QuadTree::GetObjectsGroup(const std::string &groupName)
{
	// Attempt to find and return a vector using provided name, else return nullptr
	std::map<std::string, std::vector<GameObject*>>::const_iterator it = objectGroups.find(groupName);

	if (it == objectGroups.end())
	{
		std::vector<GameObject*> nullVector;
		return nullVector;
	}
	else
	{
		//first means key, which is mapName
		//second means value, which is TMXMap
		return it->second;
	}
}