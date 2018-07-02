#include "TextureRegion.h"

TextureRegion::TextureRegion()
{
	//set
	_RectPosition.x = 0;
	_RectPosition.y = 0;
	_RectSize.x = 0;
	_RectSize.y = 0;
	_Texture = NULL;
	//_ScaleFactor.Set(1, 1);
}


TextureRegion::TextureRegion(Texture *texture, float rectX, float rectY, float rectWidth, float rectHeight)
{

	_Texture = texture;
	_RectSize = POINT(rectWidth, rectHeight);
	_RectPosition = POINT(rectX, rectY);
	//_ScaleFactor.Set(1, 1);
}

//TextureRegion::TextureRegion(const TextureRegion &textureRegion)
//{
//	_RectPosition = textureRegion._RectPosition;
//	_RectSize = textureRegion._RectSize;
//	_ScaleFactor = textureRegion._ScaleFactor;
//	_Texture = textureRegion._Texture;
//}
//
//TextureRegion& TextureRegion::operator=(const TextureRegion &textureRegion)
//{
//	_RectPosition = textureRegion._RectPosition;
//	_RectSize = textureRegion._RectSize;
//	_ScaleFactor = textureRegion._ScaleFactor;
//	_Texture = textureRegion._Texture;
//	return *this;
//}

//all get functions
const POINT& TextureRegion::GetRectSize() const
{
	return _RectSize;
}

const POINT& TextureRegion::GetRectPosition() const
{
	return _RectPosition;
}

//all set functions
void  TextureRegion::SetRectPosition(float rectX, float rectY)
{
	_RectPosition = POINT(rectX, rectY);
}

void TextureRegion::SetRectSize(float rectWidth, float rectHeight)
{
	_RectSize.x = rectWidth;
	_RectSize.y = rectHeight;
}

void TextureRegion::SetTexture(Texture *Texture)
{
	_Texture = Texture;
}

Texture* TextureRegion::GetTexture() const
{
	return _Texture;
}

////Flip the image 
//void TextureRegion::Flip(bool flipX, bool flipY)
//{
//	if (flipX)
//	{
//		_ScaleFactor.x = abs(_ScaleFactor.x)* -1;
//	}
//	else
//	{
//		_ScaleFactor.x = abs(_ScaleFactor.x);
//	}
//
//	if (flipY)
//	{
//		_ScaleFactor.y = abs(_ScaleFactor.y)* -1;;
//	}
//	else
//	{
//		_ScaleFactor.y = abs(_ScaleFactor.y);
//	}
//}
//
//const POINT& TextureRegion::GetScale() const
//{
//	return _ScaleFactor;
//}

TextureRegion::~TextureRegion()
{
	/*if (_CreateNewTexture && _Texture != NULL)
	{
	delete _Texture;
	_Texture = NULL;
	}*/
}
