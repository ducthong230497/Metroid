#include "Zeebetite.h"
#define ZEEBETITEHEALTH 4
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

	collisionType = Static;
	_CategoryMask = ZEEBETITE;
	_BitMask = PLAYER | PLAYER_BULLET | PLAYER_ROCKET;
	health = ZEEBETITEHEALTH;
	//isActive = false;
}

void Zeebetite::Update(float dt)
{
	if (health <= 0 && index < regions.size() - 1)
	{
		health = ZEEBETITEHEALTH;
		SetRegion(regions.at(++index));
	}
	else if (health < 0 && index == regions.size() - 1)
	{
		//std::vector<GameObject*>::iterator it = std::find(scene->GameObjects.begin(), scene->GameObjects.end(), this);
		isActive = false;
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

void Zeebetite::OnHitRocket()
{
	health -= ZEEBETITEHEALTH;
}
