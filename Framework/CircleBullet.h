#pragma once

#include "Sprite.h"
#include "SpriteBatch.h"
#include "Animation.h"
#include "TexturePacker.h"
#include "Scene.h"

class Rinka : public Sprite
{
private:
	Scene * scene;
	GameObject * player;

	float lastShootTime;
	float stateTime;
	bool isDestroyed;

	POINT basePosition;
public:
	Rinka();
	~Rinka();

	void SetScene(Scene * s);

	void SetPlayer(GameObject * p);

	void Init(Texture *enemiesTexture, int x, int y);

	void Update(float dt);

	void Render(SpriteBatch * batch);
};