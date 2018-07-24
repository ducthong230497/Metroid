#include "BreakablePlatform.h"

BreakablePlatform::BreakablePlatform()
{
	collisionType = Static;
	_CategoryMask = BREAKABLE_PLATFORM;
	_BitMask = PLAYER | RIO | ZOOMER | SKREE | RIPPER | BOMB_EXPLOSION | PLAYER_BULLET;
}

BreakablePlatform::~BreakablePlatform()
{
}

void BreakablePlatform::SetScene(Scene *s)
{
	scene = s;
}

void BreakablePlatform::SetTilemap(TileMap *map)
{
	tileMap = map;
}

void BreakablePlatform::Update(float dt)
{
}

void BreakablePlatform::Render(SpriteBatch * batch)
{
	batch->Draw(*this);
}

void BreakablePlatform::OnHitBomb()
{
	EraseFromScene();
}

void BreakablePlatform::OnHitBullet()
{
	//EraseFromScene();
}

void BreakablePlatform::EraseFromScene()
{
	std::vector<GameObject*>::iterator it = std::find(scene->GameObjects.begin(), scene->GameObjects.end(), this);
	if (it != scene->GameObjects.end())
	{
		//delete *it;
		scene->GameObjects.erase(it);
		int column = (int)(GetPosition().x / tileMap->GetTileWidth());
		int row = tileMap->GetHeight() - (int)(GetPosition().y / tileMap->GetTileHeight()) - 1;
		tileMap->SetTileData(row, column, 0);
	}
}