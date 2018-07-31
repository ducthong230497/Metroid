#pragma once
#include "Sprite.h"
#include "SpriteBatch.h"
#include "Animation.h"
#include "TexturePacker.h"
#include "Scene.h"
class Zeebetite : public Sprite
{
private:
	int health;
	//Animation animation;
	std::vector<TextureRegion> regions;
	Scene * scene;
	int index;
public:
	Zeebetite();
	~Zeebetite();

	void SetScene(Scene * s);

	void Init(Texture *itemsTexture, int x, int y);

	void Update(float dt);

	void Render(SpriteBatch * batch);

	void OnHitBullet();
};