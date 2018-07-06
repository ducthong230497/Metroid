#pragma once
#include "Sprite.h"
#include "Animation.h"
#include "TexturePacker.h"
#include "SpriteBatch.h"
class Ripper : public Sprite
{
private:
	Animation ripperAnimation;
	bool isHitGround;
	bool isDead;
	//CSound *isShot = NULL;
public:
	Ripper();
	~Ripper();

	void Init(Texture * ripperTexture, float x, float y);
	void Update(float dt) override;
	void Render(SpriteBatch * batch);
	void OnHitGround();
	void OnHitBullet();
};