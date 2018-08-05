#include "Samus.h"
#include "CKeyboard.h"
#include "MainScene.h"

#define BULLET_VELOCITY 400
#define SAMUSSTARTINGHEALTH 30
#define SAMUSSTARTINGROCKET 5
#define GAINHEALTH 6
#define BOMBDELAY 1.5f
#define MAXDEADTIME 0.5

void Samus::InitSamusAnimation(Texture * samusTexture)
{
	canControl = true;
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
	JumpRoll = Animation_("JumpRoll", [=] { setSize(34, 45); }, NULL, [=] {setSize(34, 60); }, true);
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
	//	Move.AddTransition("Jump", { new Condition_("Move",true),new Condition_("Falling",true) });
	Move.AddTransition("JumpRoll", { new Condition_("Ground",false),new Condition_("Move",true) });
	Move.AddTransition("MoveUp", { new Condition_("LookUp",true), new Condition_("Move",true) });
	MoveUp.AddTransition("Move", { new Condition_("LookUp",false) });
	MoveUp.AddTransition("StandUp", { new Condition_("Move",false),new Condition_("LookUp",true) });
	MoveUp.AddTransition("Jump", { new Condition_("Ground",false) });
	Roll.AddTransition("Stand", { new Condition_("Roll",false) });
	Roll.AddTransition("Move", { new Condition_("Roll",false),new Condition_("Move",true) });
	StandUp.AddTransition("Stand", { new Condition_("LookUp",false) });
	StandUp.AddTransition("MoveUp", { new Condition_("LookUp",true),new Condition_("Move",true) });
	JumpRoll.AddTransition("Move", { new Condition_("Ground",true),new Condition_("Move",true) });
	JumpRoll.AddTransition("Stand", { new Condition_("Ground",true),new Condition_("Move",false) });
	// ADD TO ANIMATOR
	animator.AddAnimation({ &Appear,&Stand,&StandShoot,&StandUp,&Roll,&Move,&MoveShoot,&MoveUp,&Jump,&JumpShoot,&JumpUp,&JumpRoll });
	//
	animator.currentAnimation = Appear;
	animator.previousAnimation = Appear;
	animator.frameInterval = Appear.duration;

	//Setup Dead effect
	Sprite deadEffectSprite;
	deadEffectSprite.SetRegion(p.GetRegion("deadeffect").front());
	deadEffectSprite.SetSize(8, 8);
	for (int i = 0; i < 8; i++)
	{
		deadEffectSprite.SetRotation(i * 30);
		deadEffect.push_back(deadEffectSprite);
	}

	//sound
	ShootSound = Sound::LoadSound("Resources/Audio/Fire.wav");
	JumpSound = Sound::LoadSound("Resources/Audio/Jump.wav");
	moveSound = Sound::LoadSound("Resources/Audio/Move.wav");
	deathSound = Sound::LoadSound("Resources/Audio/Death.wav");

	deadTime = -1;

	health = SAMUSSTARTINGHEALTH;
	rocket = SAMUSSTARTINGROCKET;
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
	setBitMask(Category::PLATFORM | Category::SKREE | Category::ZOOMER | Category::RIPPER | Category::RIO |
		Category::BREAKABLE_PLATFORM | Category::MARUNARI | Category::BOMBITEM | Category::SKREE_BULLET |
		Category::HEALTHITEM | Category::KRAID | Category::DOOR | Category::OUTER_DOOR | Category::ROCKET_ITEM |
		Category::ZEEBETITE);
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
	if (health > 0)
	{
		batch->Draw(*this);

		for (std::vector<Bullet*>::iterator it = bullets.begin(); it != bullets.end(); ++it)
		{
			batch->Draw(*it);
		}

		for (std::vector<Rocket*>::iterator it = rockets.begin(); it != rockets.end(); ++it)
		{
			batch->Draw(*it);
		}
	}
	else
	{
		if (deadTime < MAXDEADTIME)
		{
			int i = 0;
			for (std::vector<Sprite>::iterator it = deadEffect.begin(); it != deadEffect.end(); ++it)
			{
				it->SetPosition(it->GetPosition().x + 5 * cos(45 * i*Pi / 180), it->GetPosition().y + 5 * sin(45 * i*Pi / 180));
				batch->Draw(*it);
				i++;
			}
		}
	}
}

void Samus::Update(float dt)
{
	if (health <= 0)
	{
		if (deadTime == -1)
		{
			playDeadSound = true;
			isDead = true;
			Sound::Play(deathSound);
			//setup for deadEffect
			for (std::vector<Sprite>::iterator it = deadEffect.begin(); it != deadEffect.end(); ++it)
			{
				it->SetPosition(getPosition().x, getPosition().y);
			}
			deadTime = 0;
		}
		else
		{
			deadTime += dt;
		}
	}

	if (moveThroughDoor) return;

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
	if (canControl)
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

		if (KeyBoard->IsKeyDown(DIK_Z) && !roll)
		{
			float currentTime = GetTickCount() / 1000.0f;
			if (!shootRocket)
			{
				if (currentTime > FIRERATE + lastShootTime)
				{
					Sound::Play(ShootSound);
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
			else
			{
				if (rocket > 0)
				{
					if (currentTime > FIRERATEROCKET + lastShootTime)
					{
						Sound::Play(ShootSound);
						lastShootTime = currentTime;
						Rocket *b = new Rocket(&samusTexture);
						if (!lookUp)
						{
							if (facingRight)
							{
								b->setPosition(getPosition().x + 20, getPosition().y + 13); // move right
								b->setVelocity(BULLET_VELOCITY, 0);
								b->SetRotation(0);
							}
							else
							{
								b->setPosition(getPosition().x - 20, getPosition().y + 13); // move right
								b->setVelocity(-BULLET_VELOCITY, 0);
								b->SetRotation(180);
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
							b->SetRotation(-90);
						}
						rockets.push_back(b);
						((MainScene*)scene)->playerRockets.push_back(b);
						rocket--;
					}
				}
			}
		}
		else if (KeyBoard->IsKeyDown(DIK_Z) && roll)
		{
			float currentTime = GetTickCount() / 1000.0f;
			if (currentTime > lastBombTime + BOMBDELAY)
			{
				lastBombTime = currentTime;
				Bomb * _bomb = new Bomb(&samusTexture);
				_bomb->SetScene(scene);
				_bomb->setPosition(_Position);
				((MainScene*)scene)->bomb = _bomb;
			}
		}


#pragma region Jump way 2
		if (KeyBoard->IsFirstKeyDown(DIK_X) && onGround && !roll) {
			setVelocity(getVelocity().x, 720);
			/*jump1 = _Position.y + JUMP_1;
			jump2 = _Position.y + JUMP_2;
			count = 1;*/
		}

		if (KeyBoard->IsKeyDown(DIK_X) && !onGround && !roll)
			setVelocity(getVelocity().x, getVelocity().y + 6.1);


		/*if (KeyBoard->IsFirstKeyDown(DIK_X) && onGround && !roll) {
			setVelocity(getVelocity().x, 400);
			jump1 = _Position.y + JUMP_1;
			jump2 = _Position.y + JUMP_2;
			count = 1;
		}

		if (!onGround && _Position.y >= jump1 && count == 2) {
			count = -1;
			setVelocity(getVelocity().x, -400);
		}



		if (_Position.y > jump2 && !onGround) {
			count = -1;
			setVelocity(getVelocity().x, -400);
		}

		if (KeyBoard->IsKeyDown(DIK_X) && !onGround && !roll && count != 2)
			jumpTime += 0.02;
		else if (KeyBoard->IsKeyUp(DIK_X) && !onGround && !roll)
			count = 2;*/

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

		if (KeyBoard->IsFirstKeyDown(DIK_S))
		{
			shootRocket = !shootRocket;
			Trace::Log("shootRocket: %d", shootRocket);
		}
		HandleAnimation();

		if ((KeyBoard->IsKeyDown(DIK_LEFT) || KeyBoard->IsKeyDown(DIK_RIGHT)) && onGround)
		{
			Sound::Play(moveSound);
		}
	}
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
		//	setVelocity(getVelocity().x, -1);
			jumpTime = 0;
			down = false;
		}
		else if (direction.y < 0) {
			setVelocity(getVelocity().x, 0);
			//		jump1 = jump2 = _Position.y;
		}

	}
}

void Samus::OnExitGround(POINT direction)
{
	onGround = false;
	Trace::Log("Exit ground");
}

void Samus::OnShoot()
{
	Sound::Play(ShootSound);
}

void Samus::OnJump()
{
}

void Samus::OnHitItem()
{
	((MainScene*)scene)->eatItem = true;
}

void Samus::OnHitHealthItem()
{
	health += GAINHEALTH;
}

void Samus::OnHitRocketItem()
{
	rocket++;
}

void Samus::OnHitEnemy()
{
	health -= 30;
}

int Samus::getHealth()
{
	return health;
}

int Samus::getNumberRocket()
{
	return rocket;
}

bool Samus::IsDead()
{
	return isDead;
}

bool Samus::PlayDeadSound()
{
	return playDeadSound;
}

void Samus::UpdateVelocity(GameObject * obj)
{
	if (onGround) return;
	//float currentTime = GetTickCount() / 1000.0f;
	setVelocity(getVelocity().x, getVelocity().y - 100);
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
