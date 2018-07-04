#pragma once
#include <string>
#include "Texture.h"
#include "TextureRegion.h"
#include "../rapidxml/rapidxml.hpp"
//Get TextureRegion from a texture packer file
class TexturePacker
{
private:
	std::string _Content;
	Texture *_Texture;

public:
	TexturePacker();
	TexturePacker(Texture* texture, std::string packerfilePath);
	~TexturePacker();

	std::vector<TextureRegion> GetRegion(std::string regionName) const;
	void SetTexture(Texture *texture);
	void SetPacker(std::string packerfilePath);

};