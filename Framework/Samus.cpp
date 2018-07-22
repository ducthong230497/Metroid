#include "Samus.h"
#include "CKeyboard.h"
#include "MainScene.h"

#define BULLET_VELOCITY 400

void Samus::InitSamusAnimation(Texture * samusTexture)
{
	animator = Animator();

	// PARAMETERS [variable]
	animator.parameters.Add("Stand", false);
	animator.parameters.Add("Roll", false);
	animator.parameters.Add("Shoot", false);
	animator.parameters.Add("LookUp", false);
	animator.parameters.Add("Move", false);
	animator.parameters.Add("MoveUp", false);
	animator.parameters.Add("Ground", false);
	animator.parameters.Add("Falling", false);

	// Animation states
	Appear = Animation_("Appear", NULL, NULL, NULL, true);
	Stand = Animation_("Stand", NULL, NULL, NULL, true);
	StandShoot = Animation_("StandShoot", NULL, NULL, NULL, true);
	StandUp = Animation_("StandUp", NULL, NULL, NULL, true);
	Move = Animation_("Move", NULL, NULL, NULL, true);
	MoveShoot = Animation_("MoveShoot", NULL, NULL, NULL, true);
	MoveUp = Animation_("MoveUp", [=] {animator.SetBool("MoveUp", true); }, NULL, NULL, true);
	Jump = Animation_("Jump", [=] { setSize(34, 45);  if (animator.previousAnimation.name != "MoveUp") animator.SetBool("MoveUp", false); }, NULL, [=] {setSize(34, 60); }, true);
	JumpShoot = Animation_("JumpShoot", NULL, NULL, NULL, true);
	JumpUp = Animation_("JumpUp", NULL, NULL, NULL, true);
	JumpRoll = Animation_("JumpRoll", NULL, NULL, NULL, true);
	Roll = Animation_("Roll", [=] { setSize(26, 22); }, NULL, [=] {setSize(34, 60); }, true);
	Hit = Animation_("Hit", NULL, NULL, NULL, false);



	// LOAD IMAGE INTO ANIMATION
	TexturePacker p = TexturePacker(samusTexture, "Resources/samusaran_packer.xml");
	Appear.AddRegion(p.GetRegion("appearing"));
	Appear.SetFrameInterval(0.02);
	Stand.AddRegion(p.GetRegion("standing"));
	Move.AddRegion(p.GetRegion("movewithoutshooting"));
	Move.SetFrameInterval(0.02);
	Jump.AddRegion(p.GetRegion("jumpwithoutshooting"));
	StandUp.AddRegion(p.GetRegion("standandshootup"));
	MoveShoot.AddRegion(p.GetRegion("moveandshoot"));
	MoveUp.AddRegion(p.GetRegion("moveandshootup"));
	JumpShoot.AddRegion(p.GetRegion("jumpandshoot"));
	JumpUp.AddRegion(p.GetRegion("jumpandshootup"));
	Roll.AddRegion(p.GetRegion("rolling"));
	JumpRoll.AddRegion(p.GetRegion("jumpandroll"));
	Hit.AddRegion(p.GetRegion("beinghit"));
	Appear.AddTransition("Stand", { new Condition_("Stand", true) });
	Stand.AddTransition("Roll", { new Condition_("Roll",true) });
	Stand.AddTransition("StandUp", { new Condition_("LookUp",true) });
	Stand.AddTransition("Move", { new Condition_("Move",true) });
	Stand.AddTransition("Jump", { new Condition_("Ground",false) });
	StandUp.AddTransition("JumpUp", { new Condition_("Ground",false),new Condition_("LookUp",true) });
	JumpUp.AddTransition("StandUp", { new Condition_("Ground",true),new Condition_("LookUp",true) });
	JumpUp.AddTransition("Stand", { new Condition_("Ground",true),new Condition_("LookUp",false) });
	Jump.AddTransition("Stand", { new Condition_("Ground",true) });
	Jump.AddTransition("JumpShoot", { new Condition_("Shoot",true),new Condition_("Ground",false) });
	Jump.AddTransition("JumpUp", { new Condition_("LookUp",true),new Condition_("Ground",false),new Condition_("MoveUp",false) });
	JumpShoot.AddTransition("Stand", { new Condition_("Ground",true) });
	Move.AddTransition("Stand", { new Condition_("Move",false),new Condition_("Ground",true) });
	Move.AddTransition("Jump", { new Condition_("Move",true),new Condition_("Falling",true) });
	Move.AddTransition("MoveUp", { new Condition_("LookUp",true), new Condition_("Move",true) });
	MoveUp.AddTransition("Move", { new Condition_("LookUp",false) });
	MoveUp.AddTransition("StandUp", { new Condition_("Move",false),new Condition_("LookUp",true) });
	MoveUp.AddTransition("Jump", { new Condition_("Ground",false) });
	Roll.AddTransition("Stand", { new Condition_("Roll",false) });
	Roll.AddTransition("Move", { new Condition_("Roll",false),new Condition_("Move",true) });
	StandUp.AddTransition("Stand", { new Condition_("LookUp",false) });
	StandUp.AddTransition("MoveUp", { new Condition_("LookUp",true),new Condition_("Move",true) });

	// ADD TO ANIMATOR
	animator.AddAnimation({ &Appear,&Stand,&StandShoot,&StandUp,&Roll,&Move,&MoveShoot,&MoveUp,&Jump,&JumpShoot,&JumpUp });
	//
	animator.currentAnimation = Appear;
	animator.previousAnimation = Appear;
	animator.frameInterval = Appear.duration;
}



Samus::Samus()
{

	isAppear = true;
}

void Samus::SetScene(Scene * s)
{
	scene = s;
}

void Samus::Init(Texture * texture, float x, float y)
{
	samusTexture = *texture;
	setCategoryMask(Category::PLAYER);
	setBitMask(Category::PLATFORM | Category::SKREE | Category::ZOOMER | Category::RIPPER | Category::RIO | Category::BREAKABLE_PLATFORM | Category::MARUNARI | Category::BOMBITEM | Category::SKREE_BULLET | Category::HEALTHITEM);
	setVelocity(0, -200);
	canRoll = true;
	onGround = false;
	move = false;
	shoot = false;
	roll = false;
	lookUp = false;
	canStand = true;
	jumpTime = 0.0f;
	InitSamusAnimation(&samusTexture);
	SetRegion(*animator.GetKeyAnimation());
	SetSize(34, 60);
	SetPosition(x, y);
}

void Samus::Render(SpriteBatch * batch)
{
	batch->Draw(*this);

	for (std::vector<Bullet*>::iterator it = bullets.begin(); it != bullets.end(); ++it)
	{
		batch->Draw(*it);
	}
}

void Samus::Update(float dt)
{


	if (getVelocity().x > 0)
	{
		this->Flip(false, false);
	}
	else
	{
		if (getVelocity().x < 0)
		{
			this->Flip(true, false);
		}
	}

	animator.CheckCondition(dt);
	SetRegion(*animator.Next(dt));


}

void Samus::ProcessInput(CKeyboard * KeyBoard)
{

	if (KeyBoard->IsKeyDown(DIK_RIGHT))
	{
		setVelocity(200, getVelocity().y);
		move = true;
		facingRight = true;
	}
	else if (KeyBoard->IsKeyUp(DIK_RIGHT))
	{
		setVelocity(0, getVelocity().y);
		move = false;
	}

	if (KeyBoard->IsKeyDown(DIK_LEFT))
	{
		setVelocity(-200, getVelocity().y);
		move = true;
		facingRight = false;
	}
	else if (KeyBoard->IsKeyUp(DIK_LEFT))
	{
		setVelocity(0, getVelocity().y);
		move = false;
	}

	if (KeyBoard->IsKeyDown(DIK_DOWN) && canRoll && onGround) {
		roll = true;
	}

	if (KeyBoard->IsKeyDown(DIK_Z))
	{
		float currentTime = GetTickCount() / 1000.0f;
		if (currentTime > FIRERATE + lastShootTime)
		{
			lastShootTime = currentTime;
			Bullet *b = new Bullet(&samusTexture);
			if (!lookUp)
			{
				if (facingRight)
				{
					b->setPosition(getPosition().x + 20, getPosition().y + 13); // move right
					b->setVelocity(BULLET_VELOCITY, 0);
				}
				else
				{
					b->setPosition(getPosition().x - 20, getPosition().y + 13); // move right
					b->setVelocity(-BULLET_VELOCITY, 0);
				}
			}
			else 
			{
				if (facingRight)
				{
					b->setPosition(getPosition().x + 5, getPosition().y + 33); // move right
				}
				else
				{
					b->setPosition(getPosition().x - 5, getPosition().y + 33); // move right
				}
					b->setVelocity(0, BULLET_VELOCITY);
			}
			bullets.push_back(b);
			((MainScene*)scene)->playerBullets.push_back(b);
		}
	}


#pragma region Jump way 2
	if (KeyBoard->IsFirstKeyDown(DIK_X) && onGround && !roll) {
		setVelocity(getVelocity().x, 400);
		jump1 = _Position.y + JUMP_1;
		jump2 = _Position.y + JUMP_2;
		count = 1;
	}

	if (!onGround && _Position.y >= jump1 && count == 2/* && (jumpTime < 0.7 || jumpTime >= 0.7)*/) {
		count = -1;
		setVelocity(getVelocity().x, -400);
	}

	// set new velocity when it pass jump 1
	if (!onGround && _Position.y >= jump1 && count == 1 && jumpTime >= 0.7)
		setVelocity(getVelocity().x, 350);

	if (_Position.y > jump2 && !onGround) {
		count = -1;
		setVelocity(getVelocity().x, -400);
	}

	if (KeyBoard->IsKeyDown(DIK_X) && !onGround && !roll && count != 2)
		jumpTime += 0.02;
	else if (KeyBoard->IsKeyUp(DIK_X) && !onGround && !roll)
		count = 2;

#pragma endregion

	if (KeyBoard->IsKeyDown(DIK_X) && roll)
		roll = false;


	if (KeyBoard->IsKeyDown(DIK_UP)) {
		lookUp = true;
		if (roll)
			roll = false;
	}
	else if (KeyBoard->IsKeyUp(DIK_UP)) {
		lookUp = false;
	}

	if (KeyBoard->IsKeyDown(DIK_Z)) {
		shoot = true;
	}
	else if (KeyBoard->IsKeyUp(DIK_Z)) {
		shoot = false;
	}

	HandleAnimation();
}

void Samus::OnHitGround(POINT direction)
{
	/*if (direction.y != 10000.0f)
	{
		if (direction.y > 0)
		{
			count = 0;
			onGround = true;
			jumpTime = 0;
			down = false;
		}
	}*/
	if (direction.y != 10000.0f)
	{
		if (direction.y > 0)
		{
			count = 0;
			//setVelocity(getVelocity().x, -450);
			//nextPosition = maxPosition = _Position.y;
			onGround = true;
			jumpTime = 0;
			down = false;
		}
		else if (direction.y < 0) {
			setVelocity(getVelocity().x, 0);
			jump1 = jump2 = _Position.y;
		}

	}
}

void Samus::OnExitGround(POINT direction)
{
	onGround = false;
	Trace::Log("Exit ground");
}

void Samus::HandleAnimation()
{
	if (canStand && move && onGround) {
		animator.SetBool("Stand", true);
		start = true;
	}

	if (start) {
		if (!onGround && animator.currentAnimation.name == "Move") {
			int a = 1;
		}
		//	if(animator.previousAnimation.name != "MoveUp" && animator.currentAnimation.name != "Jump")
		//		animator.SetBool("MoveUp", false);
		animator.SetBool("LookUp", lookUp);
		animator.SetBool("Roll", roll);
		animator.SetBool("Move", move);
		animator.SetBool("Ground", onGround);
		animator.SetBool("Shoot", shoot);
	}
}
