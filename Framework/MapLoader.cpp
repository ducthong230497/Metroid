#include "MapLoader.h"


MapLoader::MapLoader()
{
}


MapLoader::~MapLoader()
{
	for (std::unordered_map<std::string, TileMap*>::iterator it = _MapContainer.begin(); it != _MapContainer.end(); it++)
	{
		delete it->second;
	}
}


void MapLoader::AddMap(const std::string& mapName, const std::string& filePath, float scale)
{
	_ScaleFactor = scale;

	// String to hold file contents
	std::string fileContents = "";

	// Attempt to load file using provided file path
	if (LoadFile(filePath, fileContents) == true)
	{
		// Create new RapidXML document instance to use to parse map data
		rapidxml::xml_document<char> currentMap;

		//parse the file
		currentMap.parse<0>((char*)fileContents.c_str());

		//get first node
		rapidxml::xml_node<> *parentNode = currentMap.first_node("map");

		// Add new TileMap to m_mapContainer
		_MapContainer[mapName] = new TileMap();

		// Load the map settings, tilesets and layers
		LoadMapSettings(_MapContainer[mapName], parentNode);

		//get folder path
		std::size_t found = filePath.find_last_of("/");
		LoadTileSets(_MapContainer[mapName], parentNode, filePath.substr(0, found));

		LoadLayers(_MapContainer[mapName], parentNode);

		LoadObjectGroups(_MapContainer[mapName], parentNode);
	}
}


TileMap* MapLoader::GetMap(const std::string& mapName) const
{
	// Attempt to find and return a map using provided name, else return nullptr
	std::unordered_map<std::string, TileMap*>::const_iterator it = _MapContainer.find(mapName);

	if (it == _MapContainer.end())
	{
		return NULL;
	}
	else
	{
		//first means key, which is mapName
		//second means value, which is TileMap
		return it->second;
	}
}

void MapLoader::LoadMapSettings(TileMap* map, rapidxml::xml_node<> *parentNode)
{
	unsigned int width = atoi(parentNode->first_attribute("width")->value());
	unsigned int height = atoi(parentNode->first_attribute("height")->value());
	unsigned int tileWidth = atoi(parentNode->first_attribute("tilewidth")->value());
	unsigned int tileHeight = atoi(parentNode->first_attribute("tileheight")->value());

	map->SetAttributes(width, height, tileWidth, tileHeight);
	map->SetScale(_ScaleFactor);
}


void MapLoader::LoadTileSets(TileMap* map, rapidxml::xml_node<> *parentNode, const std::string& folderPath)
{
	// Create a new node based on the parent node
	rapidxml::xml_node<> *currentNode = parentNode;

	//get tileset node
	currentNode = currentNode->first_node("tileset");

	// Check if there is a tileset node
	if (currentNode != nullptr)
	{
		unsigned int firstGid = atoi(currentNode->first_attribute("firstgid")->value());
		std::string name = currentNode->first_attribute("name")->value();
		unsigned int tileWidth = atoi(currentNode->first_attribute("tilewidth")->value());
		unsigned int tileHeight = atoi(currentNode->first_attribute("tileheight")->value());
		unsigned int tileCount = atoi(currentNode->first_attribute("tilecount")->value());
		unsigned int columns = atoi(currentNode->first_attribute("columns")->value());

		currentNode = currentNode->first_node("image");
		std::string sourcePath;
		unsigned int imageWidth;
		unsigned int imageHeight;
		if (currentNode != nullptr)
		{
			sourcePath = folderPath + "/" + currentNode->first_attribute("source")->value();
			imageWidth = atoi(currentNode->first_attribute("width")->value());
			imageHeight = atoi(currentNode->first_attribute("height")->value());
		}

		TileSet tileSet;
		tileSet.SetAttributes(name, firstGid, tileWidth, tileHeight, tileCount, columns, sourcePath, imageWidth, imageHeight);

		map->SetTileSet(tileSet);

	}
}


void MapLoader::LoadLayers(TileMap* map, rapidxml::xml_node<> *parentNode)
{
	// Create a new node based on the parent node
	rapidxml::xml_node<> *currentNode = parentNode;

	// Move to first layer node
	currentNode = currentNode->first_node("layer");

	while (currentNode != nullptr)
	{
		TileLayer layer;

		//get layer attributes
		std::string layerName = currentNode->first_attribute("name")->value();
		unsigned int layerWidth = atoi(currentNode->first_attribute("width")->value());
		unsigned int layerHeight = atoi(currentNode->first_attribute("height")->value());

		//set layer attributes
		layer.SetAttributes(layerName, layerWidth, layerHeight);

		//get layer data
		char* data = currentNode->first_node("data")->value();

		//set layer data
		layer.SetData(data);

		//Add layer to map
		map->AddLayer(layerName, layer);

		// Move to the next layer
		currentNode = currentNode->parent()->next_sibling("layer");
	}
}

void MapLoader::LoadObjectGroups(TileMap* map, rapidxml::xml_node<> *parentNode)
{
	// Create a new node based on the parent node
	rapidxml::xml_node<> *currentNode = parentNode;

	// Move to first layer node
	currentNode = currentNode->first_node("objectgroup");

	while (currentNode != nullptr)
	{
		ObjectGroup objectGroup;

		//get objectGroup attributes
		std::string objectGroupName = currentNode->first_attribute("name")->value();

		//set layer attributes
		objectGroup.SetAttributes(objectGroupName);

		//get object node
		rapidxml::xml_node<> *childNode = currentNode->first_node("object");
		while (childNode != nullptr)
		{
			float id = atoi(childNode->first_attribute("id")->value());
			float x = atoi(childNode->first_attribute("x")->value())*_ScaleFactor;
			float y = atoi(childNode->first_attribute("y")->value())*_ScaleFactor;
			float width = atoi(childNode->first_attribute("width")->value())*_ScaleFactor;
			float height = atoi(childNode->first_attribute("height")->value())*_ScaleFactor;

			Shape::Rectangle rect(x + width / 2, map->GetHeight()*map->GetTileHeight()*_ScaleFactor - y - height / 2, width, height);
			rect.id = id;
			objectGroup.AddRect(rect);

			//next
			childNode = childNode->next_sibling("object");
		}

		//Add layer to map
		map->AddObjectGroup(objectGroupName, objectGroup);

		// Move to the next objectGroup
		currentNode = currentNode->next_sibling("objectgroup");
	}
}