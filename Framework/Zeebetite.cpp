#include "Zeebetite.h"
#define ZEEBETITEHEALTH 10
Zeebetite::Zeebetite()
{
}

Zeebetite::~Zeebetite()
{
}

void Zeebetite::SetScene(Scene * s)
{
	scene = s;
}

void Zeebetite::Init(Texture * itemsTexture, int x, int y)
{
	TexturePacker p = TexturePacker(itemsTexture, "Resources/bosses_packer.xml");
	index = 0;
	regions = p.GetRegion("zeebetite");
	SetRegion(regions.at(index));
	
	SetPosition(x, y);
	SetSize(16, 64);

	_CategoryMask = ZEEBETITE;
	_BitMask = PLAYER | PLAYER_BULLET;
	health = ZEEBETITEHEALTH;
}

void Zeebetite::Update(float dt)
{
	if (health <= 0 && index < regions.size() - 1)
	{
		health = ZEEBETITEHEALTH;
		SetRegion(regions.at(++index));
	}
	else
	{
		std::vector<GameObject*>::iterator it = std::find(scene->GameObjects.begin(), scene->GameObjects.end(), this);
	}
}

void Zeebetite::Render(SpriteBatch * batch)
{
	batch->Draw(*this);
}

void Zeebetite::OnHitBullet()
{
	health--;
}
