#pragma once
#include "Sprite.h"
#include "SpriteBatch.h"
#include "Scene.h"
#include "TileMap.h"
class BreakablePlatform : public Sprite
{
private:
	int health;
	Scene * scene;
	TileMap *tileMap;
public:
	BreakablePlatform();
	~BreakablePlatform();
	void SetScene(Scene *s);
	void SetTilemap(TileMap *map);
	void Update(float dt) override;
	void Render(SpriteBatch* batch);
	void OnHitBomb();
	void OnHitBullet();
	void EraseFromScene();
};