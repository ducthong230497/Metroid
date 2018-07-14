#pragma once
#include "SpriteBatch.h"
class IRenderable
{
public:
	IRenderable();
	~IRenderable();
	virtual void Render(SpriteBatch *sp);
};
