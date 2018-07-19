#pragma once

#include "Sprite.h"
#include "SpriteBatch.h"
#include "Scene.h"
#include "Animation.h"
#include "TexturePacker.h"
#include "Scene.h"
class Marunari : public Sprite
{
private:

	Animation animation;

	bool isHitPlayer;

	Scene * scene;
public:
	Marunari();
	~Marunari();

	void SetScene(Scene *s);

	void Init(Texture *itemsTexture, int x, int y);

	void Update(float dt);

	void Render(SpriteBatch * batch);

	void OnHitPlayer();
};
