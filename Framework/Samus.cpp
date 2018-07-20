#include "Samus.h"
#include "CKeyboard.h"

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

	// Animation states
	Appear = Animation_("Appear", NULL, NULL, NULL, true);
	Stand = Animation_("Stand", NULL, NULL, NULL, true);
	StandShoot = Animation_("StandShoot", NULL, NULL, NULL, true);
	StandUp = Animation_("StandUp", NULL, NULL, NULL, true);
	Move = Animation_("Move", NULL, NULL, NULL, true);
	MoveShoot = Animation_("MoveShoot", NULL, NULL, NULL, true);
	MoveUp = Animation_("MoveUp", [=] {animator.SetBool("MoveUp", true); }, NULL, NULL, true);
	Jump = Animation_("Jump", [=] { if (animator.previousAnimation.name != "MoveUp") animator.SetBool("MoveUp", false); }, NULL, NULL, true);
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
	Move.AddTransition("Stand", { new Condition_("Move",false) });
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

void Samus::Init(Texture * samusTexture, float x, float y)
{
	setCategoryMask(Category::PLAYER);
	setBitMask(Category::PLATFORM | Category::SKREE | Category::ZOOMER | Category::RIPPER | Category::RIO | Category::BREAKABLE_PLATFORM | Category::MARUNARI | Category::BOMBITEM);
	setVelocity(0, -200);
	canRoll = true;
	onGround = false;
	move = false;
	shoot = false;
	roll = false;
	lookUp = false;
	canStand = true;
	jumpTime = 0.0f;
	InitSamusAnimation(samusTexture);
	SetRegion(*animator.GetKeyAnimation());
	SetSize(34, 60);
	SetPosition(x, y);
}

void Samus::Render(SpriteBatch * batch)
{
	batch->Draw(*this);
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

	}
	else if (KeyBoard->IsKeyUp(DIK_LEFT))
	{
		setVelocity(0, getVelocity().y);
		move = false;
	}

	if (KeyBoard->IsKeyDown(DIK_DOWN) && canRoll && onGround) {
		roll = true;
	}


	// jump
	if (KeyBoard->IsFirstKeyDown(DIK_X) && onGround && !roll) {
		setVelocity(getVelocity().x, 450);
		nextPosition = _Position.y + JUMP_1;
		maxPosition = _Position.y + JUMP_2;
		jumpTime += 0.02f;
		count = 1;
	}

	if (KeyBoard->IsKeyDown(DIK_X) && !onGround && !roll)
	{
		jumpTime += 0.02f;
		count = 1;
	}

	else if (KeyBoard->IsKeyUp(DIK_X) && !onGround && !roll) {
		count = 2;
		down = true;
		if (jumpTime >= 0.7f) {
			if ((_Position.y >= nextPosition && _Position.y <= maxPosition) || _Position.y >= maxPosition)
				setVelocity(getVelocity().x, -450);
		}
		else {
			if (_Position.y >= nextPosition)
				setVelocity(getVelocity().x, -450);
		}
	}


	if (_Position.y >= maxPosition)
		setVelocity(getVelocity().x, -450);

	//if (count == 0) {
	if (jumpTime < 0.7f && _Position.y >= nextPosition && count != 1)
		setVelocity(getVelocity().x, -450);
	else if (count == 2 && _Position.y >= nextPosition)
		setVelocity(getVelocity().x, -450);
	else if (jumpTime >= 0.7f) {
		if (down && _Position.y >= nextPosition && _Position.y <= maxPosition)
			setVelocity(getVelocity().x, -450);
		/*else if (_Position.y >= maxPosition) {
			if (down) setVelocity(getVelocity().x, -350);
			else down = true;
		}*/

		//	count = 1;
	//	}
	}



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
	if (direction.y != 10000.0f)
	{
		if (direction.y > 0)
		{
			count = 0;
			onGround = true;
			jumpTime = 0;
			down = false;
		}
	}
}

void Samus::OnExitGround()
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
		//	if(animator.previousAnimation.name != "MoveUp" && animator.currentAnimation.name != "Jump")
		//		animator.SetBool("MoveUp", false);
		if (lookUp)
			animator.SetBool("LookUp", true);
		else
			animator.SetBool("LookUp", false);
		if (roll)
			animator.SetBool("Roll", true);
		else
			animator.SetBool("Roll", false);
		if (move) {
			animator.SetBool("Move", true);
		}
		else {
			animator.SetBool("Move", false);
		}
		if (!onGround)
			animator.SetBool("Ground", false);
		else
			animator.SetBool("Ground", true);
		if (shoot)
			animator.SetBool("Shoot", true);
		else
			animator.SetBool("Shoot", false);
	}
}
