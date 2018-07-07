#include "TileSet.h"

TileSet::TileSet()
{
	_FirstGid = 1;
	_TileWidth = 0;
	_TileHeight = 0;
	_TileCount = 0;
	_Columns = 0;
	_ImageWidth = 0;
	_ImageHeight = 0;
	_Texture = NULL;
}

void TileSet::SetAttributes(const std::string& name, unsigned int firstGid, unsigned int tileWidth, unsigned int tileHeight,
	unsigned int tileCount, unsigned int columns, const std::string& sourcePath, unsigned int imageWidth, unsigned int imageHeight)
{
	_FirstGid = firstGid;
	_Name = name;
	_SourcePath = sourcePath;
	_ImageWidth = imageWidth;
	_ImageHeight = imageHeight;
	_TileWidth = tileWidth;
	_TileHeight = tileHeight;
	_TileCount = tileCount;
	_Columns = columns;

	_Texture = new Texture(_SourcePath);
}

TileSet::~TileSet()
{
	if (_Texture != NULL)
	{
		delete _Texture;
		_Texture = NULL;
	}
}

TileSet::TileSet(const TileSet &TileSet)
{
	_FirstGid = TileSet._FirstGid;
	_Name = TileSet._Name;
	_SourcePath = TileSet._SourcePath;
	_ImageWidth = TileSet._ImageWidth;
	_ImageHeight = TileSet._ImageHeight;
	_TileWidth = TileSet._TileWidth;
	_TileHeight = TileSet._TileHeight;
	_TileCount = TileSet._TileCount;
	_Columns = TileSet._Columns;

	_Texture = new Texture();
	*_Texture = *TileSet._Texture;
}
TileSet& TileSet::operator=(const TileSet &TileSet)
{
	_FirstGid = TileSet._FirstGid;
	_Name = TileSet._Name;
	_SourcePath = TileSet._SourcePath;
	_ImageWidth = TileSet._ImageWidth;
	_ImageHeight = TileSet._ImageHeight;
	_TileWidth = TileSet._TileWidth;
	_TileHeight = TileSet._TileHeight;
	_TileCount = TileSet._TileCount;
	_Columns = TileSet._Columns;

	_Texture = new Texture();
	*_Texture = *TileSet._Texture;

	return *this;

}

Texture* TileSet::GetTexture() const
{
	return _Texture;
}

const std::string& TileSet::GetName() const
{
	return _Name;
}


const std::string& TileSet::GetSourcePath() const
{
	return _SourcePath;
}


unsigned int TileSet::GetFirstGid() const
{
	return _FirstGid;
}

unsigned int TileSet::GetImageWidth() const
{
	return _ImageWidth;
}


unsigned int TileSet::GetImageHeight() const
{
	return _ImageHeight;
}


unsigned int TileSet::GetTileWidth() const
{
	return _TileWidth;
}


unsigned int TileSet::GetTileHeight() const
{
	return _TileHeight;
}


unsigned int TileSet::GetTileCount() const
{
	return _TileCount;
}

unsigned int TileSet::GetColumns() const
{
	return _Columns;
}
