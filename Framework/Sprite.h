#pragma once
#include "Texture.h"
#include "GameObject.h"
#include "TextureRegion.h"
//Texture + Texture Region
class Sprite :public GameObject
{
private:
	//Position và Size đã có trong Game Object
	//POINT _Position; //the position of this sprite
	//POINT _Size;
	POINT _ScaleFactor; //scale
	POINT _RotationOrigin; //the position that will be used as origin for rotating
	float _Rotation;
	float _IsCenterOrigin; //
	POINT _RectSize; //the width and height of the rectangle portion in the image
	POINT _RectPosition; //the top left position of portion we want to draw

	Texture *_Texture; //we don't initialize this variable by default, just use this to hold the reference to a texture
					   //if you do want to allocate a new memory for this texture, consider use _CreateNewTexture variable

					   //bool _CreateNewTexture; //if true, allocate a new memory for the texture

	bool _FlipX;
	bool _FlipY;

public:
	Sprite();

	//if create new is true, allocate a new memory for the texture
	//but may affect the performance
	Sprite(Texture *texture);

	//draw a portion of image, stretch it to width and height
	Sprite(Texture *texture, float x, float y, float rectLeft, float rectTop, float rectWidth, float rectHeight);
	~Sprite();

	/*Sprite(const Sprite &sprite);
	Sprite& operator=(const Sprite &texture);*/

	//all get functions
	const POINT& GetRotationOrigin() const;
	const POINT& GetPosition() const;
	const POINT& GetSize() const;
	float GetRotation() const;
	const POINT& GetScale() const;
	Texture* GetTexture() const;
	const POINT& GetRectSize() const;
	const POINT& GetRectPosition() const;

	//all set functions
	void SetRotationOrigin(float centerX, float centerY);
	void SetRotation(float rotation);
	void SetPosition(float x, float y);
	void SetSize(float width, float height);
	void SetTexture(Texture *texture);
	void SetRegion(const TextureRegion &textureRegion);
	void SetRectPosition(float rectX, float rectY);
	void SetRectSize(float rectWidth, float rectHeight);

	//if true, set the origin of this sprite to be always in the center of this texture
	void SetCenterRotationOrigin(bool center);
	bool IsCenterOrigin() const;

	//Flip the texture. This function is done by multiplying the scale x or y with -1. 
	void Flip(bool flipX, bool flipY);
	bool IsFlipX();
	bool IsFlipY();

	//Reset the size of this sprite to the actual size of the texture
	void ResetToWhole();


};