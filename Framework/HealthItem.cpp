#include "HealthItem.h"
#include "MainScene.h"
HealthItem::HealthItem()
{
}

HealthItem::~HealthItem()
{
}

void HealthItem::SetScene(Scene * s)
{
	scene = s;
}

void HealthItem::Init(Texture * itemsTexture, int x, int y)
{
	TexturePacker p = TexturePacker(itemsTexture, "Resources/items_packer.xml");

	animation.AddRegion(p.GetRegion("healthitem"));
	animation.SetFrameInterval(0.2);

	SetRegion(*animation.GetKeyAnimation());

	//setup body
	collisionType = Static;
	setPosition(x, y);
	SetSize(16, 16);
	//bodyDef.isSensor = true;

	_CategoryMask = HEALTHITEM;
	_BitMask = PLAYER;

	isHitPlayer = false;
}

void HealthItem::Update(float dt)
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
	SetRegion(*animation.Next(dt));
}

void HealthItem::Render(SpriteBatch * batch)
{
	if (!isHitPlayer)
		batch->Draw(*this);
}

void HealthItem::OnHitPlayer()
{
	isHitPlayer = true;
}
