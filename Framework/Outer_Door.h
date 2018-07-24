#pragma once
#include "Scene.h"
#include "Sprite.h"
#include "SpriteBatch.h"
#include "Animation.h"
#include "TexturePacker.h"
#include "Scene.h"

class OuterDoor : public Sprite
{
private: 
	Scene * scene;
	Texture * doorTexture;
	float hitBulletTime;
public: 
	OuterDoor();
	~OuterDoor();
	void SetScene(Scene * s);
	void Init(Texture * texture, float x, float y);
	void Update(float dt) override;
	void Render(SpriteBatch * batch);
	void OnHitBullet();
};
