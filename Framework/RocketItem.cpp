#include "RocketItem.h"
#include "MainScene.h"
RocketItem::RocketItem()
{
}

RocketItem::~RocketItem()
{
}

void RocketItem::SetScene(Scene * s)
{
	scene = s;
}

void RocketItem::Init(Texture * itemsTexture, int x, int y)
{
	TexturePacker p = TexturePacker(itemsTexture, "Resources/items_packer.xml");

	SetRegion(p.GetRegion("rocketitem").front());

	//setup body
	collisionType = Static;
	setPosition(x, y);
	SetSize(16, 24);
	SetRotation(-90);
	//bodyDef.isSensor = true;

	_CategoryMask = ROCKET_ITEM;
	_BitMask = PLAYER;

	isHitPlayer = false;
}

void RocketItem::Update(float dt)
{
	if (isHitPlayer)
	{
		//world->DestroyBody(body);
		std::vector<GameObject*>::iterator it = std::find(scene->GameObjects.begin(), scene->GameObjects.end(), (this));
		if (it != scene->GameObjects.end())
		{
			//delete *it;
			scene->GameObjects.erase(it);
		}
		std::vector<GameObject*>::iterator it1 = std::find(((MainScene*)scene)->healthItems.begin(), ((MainScene*)scene)->healthItems.end(), (this));
		if (it1 != ((MainScene*)scene)->healthItems.end())
		{
			//delete *it;
			((MainScene*)scene)->healthItems.erase(it1);
		}
		//body = NULL;
		return;
	}
}

void RocketItem::Render(SpriteBatch * batch)
{
	if (!isHitPlayer)
		batch->Draw(*this);
}

void RocketItem::OnHitPlayer()
{
	isHitPlayer = true;
}
