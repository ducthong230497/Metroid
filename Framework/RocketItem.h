#pragma once

#include "Sprite.h"
#include "SpriteBatch.h"
#include "Scene.h"
#include "Animation.h"
#include "TexturePacker.h"
#include "Scene.h"
class RocketItem : public Sprite
{
private:

	bool isHitPlayer;

	Scene * scene;
public:
	RocketItem();
	~RocketItem();

	void SetScene(Scene *s);

	void Init(Texture *itemsTexture, int x, int y);

	void Update(float dt);

	void Render(SpriteBatch * batch);

	void OnHitPlayer();
};
