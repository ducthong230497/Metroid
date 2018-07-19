#include "BombItem.h"

BombItem::BombItem()
{
}

BombItem::~BombItem()
{
}

void BombItem::SetScene(Scene * s)
{
}

void BombItem::Init(Texture * itemsTexture, int x, int y)
{
	TexturePacker p = TexturePacker(itemsTexture, "Resources/items_packer.xml");

	animation.AddRegion(p.GetRegion("bombitem"));
	animation.SetFrameInterval(0.2);

	SetRegion(*animation.GetKeyAnimation());
	SetSize(25, 32);
	SetPosition(x, y);

	//setup body
	collisionType = Static;
	setPosition(x, y);
	setSize(25, 32);
	//bodyDef.isSensor = true;

	_CategoryMask = BOMBITEM;
	_BitMask = PLAYER;

	isHitPlayer = false;
}

void BombItem::Update(float dt)
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
		SetTexture(NULL);
		//body = NULL;
		return;
	}
	SetRegion(*animation.Next(dt));
}

void BombItem::Render(SpriteBatch * batch)
{
	if (!isHitPlayer)
		batch->Draw(*this);
}

void BombItem::OnHitPlayer()
{
}
