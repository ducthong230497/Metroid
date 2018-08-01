#pragma once

#include "Sprite.h"
#include "SpriteBatch.h"
#include "Animation.h"
#include "TexturePacker.h"
#include "Scene.h"
#include "Enemy.h"
class MotherBrain : public Sprite, public Enemy
{
private:
	Animation IdleAnimation;
	Animation OpenDoorAnimation;
	Scene * scene;
public:
	MotherBrain();
	~MotherBrain();

	void SetScene(Scene * s);

	void Init(Texture *itemsTexture, int x, int y);

	void Update(float dt);

	void Render(SpriteBatch * batch);

	void OnHitBullet();

	void OnHitRocket();

	void OnDie();
};