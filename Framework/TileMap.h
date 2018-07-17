#pragma once

#include <vector>
#include "TileSet.h"
#include "TileLayer.h"
#include "ObjectGroup.h"
#include "SpriteBatch.h"
#include <unordered_map>
#include "QuadTree.h"

class TileMap
{
private:
	unsigned int _Width;
	unsigned int _Height;
	unsigned int _TileWidth;
	unsigned int _TileHeight;

	TileSet *_TileSet;
	std::unordered_map<std::string, TileLayer*> _Layers;
	std::unordered_map<std::string, ObjectGroup*> _ObjectGroups;

	//Camera* _Cam; //hold camera reference

	float _ScaleFactor;

	QuadTree* _SDQuadTree;

	void SetAttributes(unsigned int width, unsigned int height, unsigned int tileWidth, unsigned int tileHeight);
	void SetTileSet(const TileSet &tileSet);
	void AddLayer(const std::string &layerName, const TileLayer &layer);
	void AddObjectGroup(const std::string &name, const ObjectGroup &objectGroup);

	friend class MapLoader;

public:
	TileMap();
	~TileMap();

	////set camera to reduce the amount of tiles that have to be loaded 
	//void SetCamera(Camera* camera);


	unsigned int GetWidth() const;
	unsigned int GetHeight() const;
	unsigned int GetTileWidth() const;
	unsigned int GetTileHeight() const;

	TileSet *GetTileSet() const;
	const std::unordered_map<std::string, TileLayer*>& GetLayers() const;
	const std::unordered_map<std::string, ObjectGroup*>& GetObjectGroups() const;

	ObjectGroup* GetObjectGroup(const std::string &groupName) const;
	TileLayer* GetTileLayer(const std::string &tileLayerName) const;

	//Render this map
	void Render(SpriteBatch *batch);

	void SetScale(float scale);
	float GetScale();

	//set this to reduce the amount of tile have to render
	//void SetSpaceDivisionQuadTree(SpaceDivisionQuadTree *sdquadtree);

	void SetTileData(int row, int column, int value);



};