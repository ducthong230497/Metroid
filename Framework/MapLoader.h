#pragma onceMapMapLoader

#include <unordered_map>
#include "..\rapidxml\rapidxml.hpp"
#include "TileMap.h"
#include "TileLayer.h"
#include "TileSet.h"
#include "utils.h"


class MapLoader
{
private:

	//use unordered_map for fast access purpose
	std::unordered_map<std::string, TileMap*> _MapContainer;


	//private functions
	void LoadMapSettings(TileMap* map, rapidxml::xml_node<> *parentNode);
	void LoadTileSets(TileMap* map, rapidxml::xml_node<> *parentNode, const std::string& folderPath);
	void LoadLayers(TileMap* map, rapidxml::xml_node<> *parentNode);
	void LoadObjectGroups(TileMap* map, rapidxml::xml_node<> *parentNode);

	float _ScaleFactor;

public:
	MapLoader();
	~MapLoader();

	void AddMap(const std::string& mapName, const std::string& filePath, float scale);
	TileMap* GetMap(const std::string& mapName) const;

};