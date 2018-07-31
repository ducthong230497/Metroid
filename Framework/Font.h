#pragma once

#include "..\Directx\Include\d3dx9.h"
#include <string>
#include "utils.h"
#include "CDevice.h"

class Font
{
private:
	ID3DXFont * _pFont;

private:
	friend class Label;

public:
	Font();
	~Font();

	Font(const std::string &fontName, int width, int height);

	void Release();
};