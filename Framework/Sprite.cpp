#include "Sprite.h"

Sprite::Sprite() : GameObject()
{
	_Texture = NULL;
	_ScaleFactor.x = 1;
	_ScaleFactor.y = 1;

	_RotationOrigin.x = 0;
	_RotationOrigin.y = 0;

	_Rotation = 0;
	_IsCenterOrigin = true;
	
	_RectPosition.x = 0;
	_RectPosition.y = 0;
	_RectSize.x = 0;
	_RectSize.y = 0;
	_FlipX = false;
	_FlipY = false;
}

Sprite::Sprite(Texture *texture)
{

	_Texture = texture;
	_ScaleFactor.x = 1;
	_ScaleFactor.y = 1;

	_RotationOrigin.x = 0;
	_RotationOrigin.y = 0;

	_Rotation = 0;
	//_Position.Set(0, 0);
	_IsCenterOrigin = true;

	_RectSize.x = texture->GetImageSize().x;
	_RectSize.y = texture->GetImageSize().y;
	
	//_Size.Set(_RectSize.x, _RectSize.y);
	_RectPosition.x = 0;
	_RectPosition.y = 0;
	_FlipX = false;
	_FlipY = false;
}

Sprite::Sprite(Texture *texture, float x, float y, float rectLeft, float rectTop, float rectWidth, float rectHeight)
{
	_Texture = texture;
	_ScaleFactor.x = 1;
	_ScaleFactor.y = 1;

	_Position.x = x;
	_Position.y = y;

	_Size.x = rectWidth;
	_Size.y = rectHeight;

	_RotationOrigin.x = 0;
	_RotationOrigin.y = 0;

	_Rotation = 0;
	//_Position.Set(0, 0);
	_IsCenterOrigin = true;

	_RectSize.x = rectWidth;
	_RectSize.y = rectHeight;

	//_Size.Set(_RectSize.x, _RectSize.y);
	_RectPosition.x = rectLeft;
	_RectPosition.y = rectTop;
	_FlipX = false;
	_FlipY = false;
}

//Sprite::Sprite(const Sprite &sprite)
//{
//	
//	_Texture = sprite._Texture;
//	_ScaleFactor = sprite._ScaleFactor;
//	_Size = sprite._Size;
//	_RotationOrigin = sprite._RotationOrigin;
//	_IsCenterOrigin = sprite._IsCenterOrigin;
//	_Rotation = sprite._Rotation;
//	_Position = sprite._Position;
//	_RectSize = sprite._RectSize;
//	_RectPosition = sprite._RectPosition;
//}
//
//Sprite& Sprite::operator=(const Sprite &sprite)
//{
//	_Texture = sprite._Texture;
//	_Size = sprite._Size;
//	_ScaleFactor = sprite._ScaleFactor;
//	_RotationOrigin = sprite._RotationOrigin;
//	_IsCenterOrigin = sprite._IsCenterOrigin;
//	_Rotation = sprite._Rotation;
//	_Position = sprite._Position;
//	_RectSize = sprite._RectSize;
//	_RectPosition = sprite._RectPosition;
//
//	return *this;
//}

//all get functions
const POINT& Sprite::GetRotationOrigin() const
{
	return _RotationOrigin;
}
float Sprite::GetRotation() const
{
	return _Rotation;
}
const POINT& Sprite::GetScale() const
{
	return _ScaleFactor;
}

Texture*  Sprite::GetTexture() const
{
	return _Texture;
}

const POINT& Sprite::GetSize() const
{
	return _Size;
}

//all set functions
void Sprite::SetRotationOrigin(float centerX, float centerY)
{
	_RotationOrigin.x = centerX;
	_RotationOrigin.y = centerY;
}

void Sprite::SetRotation(float rotation)
{
	_Rotation = rotation;
}


//chýa có texture region
//void Sprite::SetRegion(const TextureRegion &textureRegion)
//{
//	_Texture = textureRegion.GetTexture();
//	_RectSize = textureRegion.GetRectSize();
//	_RectPosition = textureRegion.GetRectPosition();
//	/*_ScaleFactor.x = abs(_ScaleFactor.x)* textureRegion->GetScale().x;
//	_ScaleFactor.y = abs(_ScaleFactor.y)* textureRegion->GetScale().y;*/
//}

const POINT& Sprite::GetPosition() const
{
	return _Position;
}

const POINT& Sprite::GetRectSize() const
{
	return _RectSize;
}
const POINT& Sprite::GetRectPosition() const
{
	return _RectPosition;
}

void Sprite::SetPosition(float x, float y)
{
	_Position.x = x;
	_Position.y = y;
}

void  Sprite::SetTexture(Texture *texture)
{
	_Texture = texture;
}

void Sprite::SetSize(float width, float height)
{
	_Size.x = width;
	_Size.y = height;
	_ScaleFactor.x  = width / _RectSize.x;
	_ScaleFactor.y = height / _RectSize.y;
}

void Sprite::SetCenterRotationOrigin(bool center)
{
	_IsCenterOrigin = center;
}

bool Sprite::IsCenterOrigin() const
{
	return _IsCenterOrigin;
}

void Sprite::SetRectPosition(float rectX, float rectY)
{
	_RectPosition.x = rectX;
	_RectPosition.y = rectY;
}
void Sprite::SetRectSize(float rectWidth, float rectHeight)
{
	_RectSize.x = rectWidth;
	_RectSize.y = rectHeight;
}

//Flip the image 
void Sprite::Flip(bool flipX, bool flipY)
{
	_FlipX = flipX;
	_FlipY = flipY;

	if (flipX)
	{
		_ScaleFactor.x = abs(_ScaleFactor.x)* -1;
	}
	else
	{
		_ScaleFactor.x = abs(_ScaleFactor.x);
	}

	if (flipY)
	{
		_ScaleFactor.y = abs(_ScaleFactor.y)* -1;;
	}
	else
	{
		_ScaleFactor.y = abs(_ScaleFactor.y);
	}
}

bool Sprite::IsFlipX()
{
	return _FlipX;
}

bool Sprite::IsFlipY()
{
	return _FlipY;
}

void Sprite::ResetToWhole()
{
	_RectSize.x = _Texture->GetImageSize().x;
	_RectSize.y = _Texture->GetImageSize().y;
	_ScaleFactor.x = 1;
	_ScaleFactor.y = 1;
}

Sprite::~Sprite()
{

	/*if (_CreateNewTexture && _Texture!=NULL)
	{
	delete _Texture;
	_Texture = NULL;
	}*/
}
