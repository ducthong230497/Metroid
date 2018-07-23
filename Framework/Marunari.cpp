#include "Marunari.h"

Marunari::Marunari()
{
}

Marunari::~Marunari()
{
}

void Marunari::SetScene(Scene * s)
{
	scene = s;
}

void Marunari::Init(Texture * itemsTexture, int x, int y)
{
	TexturePacker p = TexturePacker(itemsTexture, "Resources/items_packer.xml");

	animation.AddRegion(p.GetRegion("marumari"));
	animation.SetFrameInterval(0.2);

	SetRegion(*animation.GetKeyAnimation());
	SetSize(25, 32);
	SetPosition(x, y);

	//setup body
	collisionType = Static;
	setPosition(x, y);
	setSize(25, 32);
	//bodyDef.isSensor = true;
	
	_CategoryMask = MARUNARI;
	_BitMask = PLAYER;

	isHitPlayer = false;
}

void Marunari::Update(float dt)
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
		isActive = false;
		//body = NULL;
		return;
	}
	SetRegion(*animation.Next(dt));
}

void Marunari::Render(SpriteBatch * batch)
{
	if (!isHitPlayer)
		batch->Draw(*this);
}

void Marunari::OnHitPlayer()
{
	isHitPlayer = true;
}
