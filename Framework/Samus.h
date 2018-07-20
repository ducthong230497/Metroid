#ifndef _SAMUS_H
#define _SAMUS_H
#include "SpriteBatch.h"
#include "Sprite.h"
#include "TexturePacker.h"
#include "Animator.h"

#define JUMP_1 90;
#define JUMP_2 200;
#define IME 0.4f;

class CKeyboard;
class Samus : public Sprite
{
private:
	float nextPosition = 0;
	float maxPosition = 0;
	int count = 0;
	bool isAppear;
	void InitSamusAnimation(Texture * samusTexture);
	void HandleAnimation();
public:
	Animator  animator;
	Animation_
		Appear,
		Stand, StandShoot, StandUp,
		Move, MoveShoot, MoveUp,
		Jump, JumpShoot, JumpUp, JumpRoll,
		Roll,
		Hit;

	bool canRoll, onGround, move, roll, lookUp, canStand, start,shoot,down;
	float jumpTime = 0.f;
	Samus();
	void Init(Texture * samusTexture, float x, float y);
	void Render(SpriteBatch *batch);
	void Update(float dt) override;
	void ProcessInput(CKeyboard * KeyBoard);
	void OnHitGround(POINT direction);
	void OnExitGround();
	
};

#endif