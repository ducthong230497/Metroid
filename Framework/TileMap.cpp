#include "TileMap.h"


TileMap::TileMap()
{
	_Width = 0;
	_Height = 0;
	_TileWidth = 0;
	_TileHeight = 0;

	_TileSet = NULL;
	_Layers.clear();
	_ObjectGroups.clear();
	//_Cam = NULL;

	_ScaleFactor = 1;

	//_SDQuadTree = NULL;
}

TileMap::~TileMap()
{
	for (std::unordered_map<std::string, TileLayer*>::iterator it = _Layers.begin(); it != _Layers.end(); it++)
	{
		delete it->second;
	}

	if (_TileSet != NULL)
	{
		delete _TileSet;
	}

	for (std::unordered_map<std::string, ObjectGroup*>::iterator it = _ObjectGroups.begin(); it != _ObjectGroups.end(); it++)
	{
		delete it->second;
	}

}

//void TileMap::SetCamera(Camera* camera)
//{
//	_Cam = camera;
//}

void TileMap::SetAttributes(unsigned int width, unsigned int height, unsigned int tileWidth, unsigned int tileHeight)
{
	_Width = width;
	_Height = height;
	_TileWidth = tileWidth;
	_TileHeight = tileHeight;
}

void TileMap::SetTileSet(const TileSet &tileSet)
{
	_TileSet = new TileSet();
	*_TileSet = tileSet;
}

void TileMap::AddLayer(const std::string &layerName, const TileLayer &layer)
{
	TileLayer* Layer = new TileLayer();
	*Layer = layer;
	_Layers[layerName] = Layer;
}

void TileMap::AddObjectGroup(const std::string &groupName, const ObjectGroup &objectGroup)
{
	ObjectGroup* group = new ObjectGroup;
	*group = objectGroup;
	_ObjectGroups[groupName] = group;
}

ObjectGroup* TileMap::GetObjectGroup(const std::string &groupName) const
{
	// Attempt to find and return a TileMap using provided name, else return nullptr
	std::unordered_map<std::string, ObjectGroup*>::const_iterator it = _ObjectGroups.find(groupName);

	if (it == _ObjectGroups.end())
	{
		return NULL;
	}
	else
	{
		//first means key, which is TileMapName
		//second means value, which is TileMap
		return it->second;
	}
}

TileSet* TileMap::GetTileSet() const
{
	return _TileSet;
}

unsigned int TileMap::GetWidth() const
{
	return _Width;
}


unsigned int TileMap::GetHeight() const
{
	return _Height;
}


unsigned int TileMap::GetTileWidth() const
{
	return _TileWidth;
}


unsigned int TileMap::GetTileHeight() const
{
	return _TileHeight;
}

const std::unordered_map<std::string, TileLayer*>& TileMap::GetLayers() const
{
	return _Layers;
}

TileLayer* TileMap::GetTileLayer(const std::string &tileLayerName) const
{
	// Attempt to find and return a TileMap using provided name, else return nullptr
	std::unordered_map<std::string, TileLayer*>::const_iterator it = _Layers.find(tileLayerName);

	if (it == _Layers.end())
	{
		return NULL;
	}
	else
	{
		//first means key, which is TileMapName
		//second means value, which is TileMap
		return it->second;
	}
}

const std::unordered_map<std::string, ObjectGroup*>& TileMap::GetObjectGroups() const
{
	return _ObjectGroups;
}

void TileMap::SetSpaceDivisionQuadTree(QuadTree *sdquadtree)
{
	_SDQuadTree = sdquadtree;

}

void TileMap::SetTileData(int row, int column, int value)
{
	_Layers.begin()->second->GetData()[row][column] = 0;
	if (_SDQuadTree != NULL)
	{
		Shape::Rectangle* tileRect = _SDQuadTree->GetMapTileRect().at(row*_Width + column);
		tileRect->width = 0;
		tileRect->height = 0;
	}
}

//Render this TileMap
void TileMap::Render(SpriteBatch *batch)
{
	//if (true)
	if (_SDQuadTree == NULL)
	{

		//get necessary variables
		TileLayer* layer = _Layers.begin()->second;
		unsigned int **data = layer->GetData();
		unsigned int layerWidth = layer->GetWidth();
		unsigned int layerHeight = layer->GetHeight();

		Texture* texture = _TileSet->GetTexture();
		unsigned int columns = _TileSet->GetColumns();
		unsigned int tileSetWidth = _TileSet->GetTileWidth();
		unsigned int tileSetHeight = _TileSet->GetTileHeight();

		float x, y, rectLeft, rectTop, rectWidth, rectHeight;
		rectWidth = tileSetWidth;
		rectHeight = tileSetHeight;

		float width = tileSetWidth * _ScaleFactor;
		float height = tileSetHeight * _ScaleFactor;

		for (unsigned int row = 0; row < layerHeight; row++)
		{
			int a = 2;
			for (unsigned int column = 0; column < layerWidth; column++)
			{
				if (data[row][column] == 0) continue;

				rectLeft = ((data[row][column] - 1) % columns) * rectWidth;
				rectTop = ((data[row][column] - 1) / columns) * rectHeight;

				x = column * width + width / 2;
				y = (layerHeight - 1 - row)*height + height / 2;

				////check to see if this tile is out of the scope of the camera
				//if (_Cam != NULL)
				//{

				//	if (x + width / 2 < camPostion.x - screenWidth / 2 ||
				//		x - width / 2 > camPostion.x + screenWidth / 2 ||
				//		y + height / 2 < camPostion.y - screenHeight / 2 ||
				//		y - height / 2 > camPostion.y + screenHeight / 2)
				//		continue;

				//}

				batch->Draw(*texture, x, y, rectLeft, rectTop, rectWidth, rectHeight, width, height);
			}
		}

	}
	else
	{

		Texture* texture = _TileSet->GetTexture();
		/*std::vector<Shape::Rectangle*> tileRectsInViewport = _SDQuadTree->GetTileRectsInViewport();*/
		for (std::vector<Shape::Rectangle*>::const_iterator it = _SDQuadTree->tileRectsInViewport.begin(); it != _SDQuadTree->tileRectsInViewport.end(); ++it)
		{
			float x = (*it)->x;
			float y = (*it)->y;
			float width = (*it)->width;
			float height = (*it)->height;
			float rectImageLeft = (*it)->extraX;
			float rectImageTop = (*it)->extraY;

			////check to see if this tile is out of the scope of the camera
			//if (x + width / 2 < camPostion.x - screenWidth / 2 ||
			//	x - width / 2 > camPostion.x + screenWidth / 2 ||
			//	y + height / 2 < camPostion.y - screenHeight / 2 ||
			//	y - height / 2 > camPostion.y + screenHeight / 2)
			//	continue;


			batch->Draw(*texture, x, y, rectImageLeft, rectImageTop, width, height, width, height);
		}
	}
}

void TileMap::SetScale(float scale)
{
	_ScaleFactor = scale;

}

float TileMap::GetScale()
{
	return _ScaleFactor;
}