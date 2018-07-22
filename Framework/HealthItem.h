#pragma once

#include "Sprite.h"
#include "SpriteBatch.h"
#include "Scene.h"
#include "Animation.h"
#include "TexturePacker.h"
#include "Scene.h"
class HealthItem : public Sprite
{
private:

	Animation animation;

	bool isHitPlayer;

	Scene * scene;
public:
	HealthItem();
	~HealthItem();

	void SetScene(Scene *s);

	void Init(Texture *itemsTexture, int x, int y);

	void Update(float dt);

	void Render(SpriteBatch * batch);

	void OnHitPlayer();
};