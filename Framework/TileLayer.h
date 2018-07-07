#pragma once

#include <string>
#include <iterator>

class TileLayer
{
private:
	std::string _LayerName;
	unsigned int _Width;
	unsigned int _Height;
	unsigned int** _Data;

	void SetAttributes(const std::string& name, unsigned int width, unsigned int height);
	void SetData(char* matrixCSV);


	friend class MapLoader;
	friend class SpaceDivisionQuadTree;

public:
	TileLayer();
	~TileLayer();
	TileLayer(const TileLayer &TileLayer);
	TileLayer& operator=(const TileLayer &TileLayer);


	const std::string& GetName() const;
	unsigned int GetWidth() const;
	unsigned int GetHeight() const;
	unsigned int** GetData() const;


};