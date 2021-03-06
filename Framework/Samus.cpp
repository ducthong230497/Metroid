#include "Samus.h"
#include "CKeyboard.h"
#include "MainScene.h"

#define BULLET_VELOCITY 400
#define SAMUSSTARTINGHEALTH 30
#define SAMUSSTARTINGROCKET 5
#define GAINHEALTH 6
#define BOMBDELAY 1.5f
#define MAXDEADTIME 0.5
#define PUSHX 200
#define PUSHY 400
#define MAXINVINCIBLETIME 0.3
#define BOMBFORCE 600
#define LOST_HEALTH 2

void Samus::InitSamusAnimation(Texture * samusTexture)
{
	canControl = true;
	animator = Animator();

	CreateParameter();

	CreateAnimation_();

	LoadImageAnimation(samusTexture);

	CreateTransition();


	LoadAnimator();
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
	head = new GameObject(this);
	head->setSize(20, 25);
	head->setPosition(_Position.x, _Position.y + 22);
	head->isTrigger = true;
	head->setCategoryMask(Category::HEAD);
	head->setBitMask(Category::PLATFORM | Category::BREAKABLE_PLATFORM);
	samusTexture = *texture;
	setCategoryMask(Category::PLAYER);
	setBitMask(Category::PLATFORM | Category::SKREE | Category::ZOOMER | Category::RIPPER | Category::RIO |
		Category::BREAKABLE_PLATFORM | Category::MARUNARI | Category::BOMBITEM | Category::SKREE_BULLET |
		Category::HEALTHITEM | Category::KRAID | Category::DOOR | Category::OUTER_DOOR | Category::ROCKET_ITEM |
		Category::ZEEBETITE | Category::BOMB_EXPLOSION | Category::CIRCLEBULLET | Category::CANNON_BULLET | Category::KRAID_BULLET);
	defaultBitMask = _BitMask;
	triggerRoll = true;
	setVelocity(0, -200);
	fall = false;
	activeJump = false;
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

	deadTime = -1;
	_SPEED = 200;
	_JUMPFORCE = 740 / 0.031f;
	_ACCELERATION = 100 / 0.031f;
	_PULLINGFORCE = 520 / 0.031f;
	_PUSHFORCE = 11 / 0.031f;
	health = SAMUSSTARTINGHEALTH;
	rocket = SAMUSSTARTINGROCKET;

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
//	if (animator.currentAnimation.name != "Roll")
		head->setPosition(_Position.x, _Position.y + 22);
	if (dt == 0) {
		//	Trace::Log("dt: = 0");
	}
	//Trace::Log("%f",dt);
	if (hitEnemy)
	{
		invincibleTime += dt;
		if (this->GetTexture()->GetOpacity() == 1)
			this->GetTexture()->SetOpacity(0);
		else
			this->GetTexture()->SetOpacity(1);
		if (invincibleTime > MAXINVINCIBLETIME)
		{
			hitEnemy = false;
			canControl = true;
			_BitMask = defaultBitMask;
			invincibleTime = 0;
			setVelocity(0, -10);
			this->GetTexture()->SetOpacity(1);
		}
	}

	if (hitBullet)
	{
		invincibleTime += dt;
		if (this->GetTexture()->GetOpacity() == 1)
			this->GetTexture()->SetOpacity(0);
		else
			this->GetTexture()->SetOpacity(1);
		if (invincibleTime > MAXINVINCIBLETIME)
		{
			hitBullet = false;
			_BitMask = defaultBitMask;
			invincibleTime = 0;
			this->GetTexture()->SetOpacity(1);
		}
	}

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
#pragma region flip
	if (!hitEnemy) {
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
	}
#pragma endregion

	animator.CheckCondition(dt);
	SetRegion(*animator.Next(dt));


}

void Samus::ProcessInput(CKeyboard * KeyBoard)
{
	if (canControl)
	{

#pragma region setting online

		if (KeyBoard->IsFirstKeyDown(DIK_C))
		{
			shootRocket = !shootRocket;
			if (shootRocket) Trace::Log("Active Rocket: %d", shootRocket);
			else Trace::Log("Deactive Rocket: %d", shootRocket);
		}
		if (KeyBoard->IsFirstKeyDown(DIK_U))
		{
			_PULLINGFORCE += 5;
			Trace::Log("Increase _PULLINGFORCE : %d", _PULLINGFORCE);
		}
		if (KeyBoard->IsFirstKeyDown(DIK_I))
		{
			_PULLINGFORCE -= 5;
			Trace::Log("Decrease _PULLINGFORCE : %d", _PULLINGFORCE);
		}

		if (KeyBoard->IsFirstKeyDown(DIK_Q))
		{
			_JUMPFORCE += 5;
			Trace::Log("Increase JUMP FORCE : %d", _JUMPFORCE);
		}
		if (KeyBoard->IsFirstKeyDown(DIK_W))
		{
			_JUMPFORCE -= 5;
			Trace::Log("Decrease JUMP FORCE : %d", _JUMPFORCE);
		}
		if (KeyBoard->IsFirstKeyDown(DIK_A))
		{
			_SPEED += 20;
			Trace::Log("Increase SPEED : %d", _SPEED);
		}
		if (KeyBoard->IsFirstKeyDown(DIK_S))
		{
			_SPEED -= 20;
			Trace::Log("Decrease SPEED : %d", _SPEED);
		}
		if (KeyBoard->IsFirstKeyDown(DIK_E))
		{
			_PUSHFORCE += 1;
			Trace::Log("Increase PUSH FORCE : %d", _PUSHFORCE);
		}
		if (KeyBoard->IsFirstKeyDown(DIK_R))
		{
			_PUSHFORCE -= 1;
			Trace::Log("Decrease PUSH FORCE : %d", _PUSHFORCE);
		}
#pragma endregion

#pragma region movement

		if (KeyBoard->IsKeyDown(DIK_RIGHT))
		{
			setVelocity(_SPEED, getVelocity().y);
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
			setVelocity(-_SPEED, getVelocity().y);
			move = true;
			facingRight = false;
		}
		else if (KeyBoard->IsKeyUp(DIK_LEFT))
		{
			setVelocity(0, getVelocity().y);
			move = false;
		}
#pragma endregion

		if (KeyBoard->IsKeyDown(DIK_DOWN) && canRoll && onGround)
			roll = true;

#pragma region shooting

		if (KeyBoard->IsKeyDown(DIK_Z))
			shoot = true;
		else if (KeyBoard->IsKeyUp(DIK_Z))
			shoot = false;

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
					if (animator.currentAnimation.name != "JumpRoll" && animator.currentAnimation.name != "MoveUp" && animator.currentAnimation.name != "StandUp" && animator.currentAnimation.name != "JumpUp")
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
					else if (animator.currentAnimation.name == "MoveUp" || animator.currentAnimation.name == "StandUp" || animator.currentAnimation.name == "JumpUp")
					{
						if (facingRight)
							b->setPosition(getPosition().x + 5, getPosition().y + 33); // move right
						else
							b->setPosition(getPosition().x - 5, getPosition().y + 33); // move right
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
						if (animator.currentAnimation.name != "MoveUp" && animator.currentAnimation.name != "StandUp" && animator.currentAnimation.name != "JumpUp")
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
						else if (animator.currentAnimation.name == "MoveUp" || animator.currentAnimation.name == "StandUp" || animator.currentAnimation.name == "JumpUp")
						{
							if (facingRight)
								b->setPosition(getPosition().x + 5, getPosition().y + 33); // move right
							else
								b->setPosition(getPosition().x - 5, getPosition().y + 33); // move right
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
#pragma endregion

#pragma region Jump 
		if (KeyBoard->IsFirstKeyDown(DIK_X) && onGround && !roll) {
			setVelocity(getVelocity().x, _JUMPFORCE*dt);
			maxJumpHeight = _Position.y + JUMP_2;
			activeJump = true;
		}

		if (_Position.y >= maxJumpHeight && activeJump) {
			down = true;
			activeJump = false;
		}

		if (KeyBoard->IsKeyDown(DIK_X) && !onGround && !roll && getVelocity().y >= -200 && !down)
			setVelocity(getVelocity().x, getVelocity().y + _PUSHFORCE * dt);
#pragma endregion

#pragma region movement
		if (KeyBoard->IsKeyDown(DIK_X) && roll && triggerRoll)
			roll = false;

		if (KeyBoard->IsKeyDown(DIK_UP)) {
			lookUp = true;
			if (roll && triggerRoll) roll = false;
			if (animator.currentAnimation.name == "Jump" && animator.previousAnimation.name == "MoveUp" && !onGround) lookUp = false;
		}
		else if (KeyBoard->IsKeyUp(DIK_UP)) {
			lookUp = false;
			if (animator.currentAnimation.name == "Jump" && animator.previousAnimation.name == "MoveUp" && !onGround) lookUp = true;
		}
#pragma endregion

		if ((KeyBoard->IsKeyDown(DIK_LEFT) || KeyBoard->IsKeyDown(DIK_RIGHT)) && onGround)
			Sound::Play(moveSound);

		HandleAnimation();


#pragma region CheckPoint
		if (KeyBoard->IsFirstKeyDown(DIK_1))
		{
			_Position = ((MainScene*)scene)->checkPoints[0]->getPosition();
		}
		else if (KeyBoard->IsFirstKeyDown(DIK_2))
		{
			_Position = ((MainScene*)scene)->checkPoints[1]->getPosition();
		}
		else if (KeyBoard->IsFirstKeyDown(DIK_3))
		{
			_Position = ((MainScene*)scene)->checkPoints[2]->getPosition();
		}
		else if (KeyBoard->IsFirstKeyDown(DIK_4))
		{
			_Position = ((MainScene*)scene)->checkPoints[3]->getPosition();
		}
		else if (KeyBoard->IsFirstKeyDown(DIK_5))
		{
			_Position = ((MainScene*)scene)->checkPoints[4]->getPosition();
		}
#pragma endregion

	}
}

void Samus::OnHitGround(POINT direction)
{

	if (direction.y != 10000.0f)
	{
		if (direction.y > 0)
		{
			count = 0;
			onGround = true;
			fall = false;
			activeJump = false;
			down = false;
		}
		else if (direction.y < 0) {
			setVelocity(getVelocity().x, 0);
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

void Samus::OnHitEnemy(GameObject *enemy, POINT CollisionDirection)
{
	health -= LOST_HEALTH;
	hitEnemy = true;
	canControl = false;
	_BitMask = PLATFORM | BREAKABLE_PLATFORM | HEALTHITEM | DOOR | OUTER_DOOR | ZEEBETITE | ROCKET_ITEM;
	onGround = false;


	if (CollisionDirection.x != NOT_COLLIDED)
	{
		if (CollisionDirection.x > 0)
			setVelocity(PUSHX, PUSHY);
		else if (CollisionDirection.x < 0)
			setVelocity(-PUSHX, PUSHY);
	}
	else
	{
		if (enemy->getPosition().x > _Position.x)
			setVelocity(-PUSHX, PUSHY);
		else
			setVelocity(PUSHX, PUSHY);
	}
	move = false;
	if (animator.currentAnimation.name == "MoveUp" || animator.currentAnimation.name == "JumpUp") {
		lookUp = false;
		animator.SetBool("LookUp", false);
	}
	fall = true;
	animator.SetBool("Ground", false);
	animator.SetBool("Falling", true);
	animator.CheckCondition(dt);
}

void Samus::OnHitEnemyBullet()
{
	health -= 1;
	hitBullet = true;
	_BitMask = PLATFORM;
}

void Samus::OnHitBomb()
{
	
	if (triggerRoll) {
		onGround = false;
		_Velocity.y = BOMBFORCE;
	}
}

void Samus::SetCanRoll()
{
	canRoll = true;
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
	count++;
	countRoll++;

	if (countRoll >= 2) { // Not collide with all platform		
		triggerRoll = true;
	}

	if (count >= 2) { // Not collide with all platform
		onGround = false;
		fall = true;

	}

	if (onGround) return;

	if (getVelocity().y >= -200) {
		setVelocity(getVelocity().x, getVelocity().y - _ACCELERATION * dt);
	}
	else if (getVelocity().y < -200) {
		setVelocity(getVelocity().x, -(_PULLINGFORCE+100) * dt  );
	}
}

void Samus::OnTriggerEnter()
{
	triggerRoll = false;
	countRoll = 0;
}

void Samus::HandleAnimation()
{
	if (canStand && move && onGround) {
		animator.SetBool("Stand", true);
		start = true;
	}

	if (start) {
		animator.SetBool("LookUp", lookUp);
		animator.SetBool("Roll", roll);
		animator.SetBool("Move", move);
		animator.SetBool("Ground", onGround);
		animator.SetBool("Shoot", shoot);
		animator.SetBool("Falling", fall);
	}
}



void Samus::CreateParameter()
{
	// PARAMETERS [variable]
	animator.parameters.Add("Stand", false);
	animator.parameters.Add("Roll", false);
	animator.parameters.Add("Shoot", false);
	animator.parameters.Add("LookUp", false);
	animator.parameters.Add("Move", false);
	animator.parameters.Add("MoveUp", false);
	animator.parameters.Add("Ground", false);
	animator.parameters.Add("Falling", false);
}

void Samus::CreateAnimation_()
{
	// Animation states
	Appear = Animation_("Appear", NULL, NULL, NULL, true);
	Stand = Animation_("Stand", NULL, NULL, NULL, true);
	StandShoot = Animation_("StandShoot", NULL, NULL, NULL, true);
	StandUp = Animation_("StandUp", NULL, NULL, NULL, true);
	Move = Animation_("Move", NULL, NULL, NULL, true);
	MoveShoot = Animation_("MoveShoot", NULL, NULL, NULL, true);
	MoveUp = Animation_("MoveUp", NULL, NULL, NULL, true);
	Jump = Animation_("Jump", [=] {setSize(34, 45); }, NULL, [=] {setSize(34, 60); }, true);
	JumpShoot = Animation_("JumpShoot", [=] {setSize(34, 45); }, NULL, [=] {setSize(34, 60); }, true);
	JumpUp = Animation_("JumpUp", [=] { lookUp = false; }, NULL, NULL, true);
	JumpRoll = Animation_("JumpRoll", [=] { setSize(34, 45); }, NULL, [=] {setSize(34, 60); }, true);
	Roll = Animation_("Roll", [=] { head->setPosition(_Position.x, _Position.y + 22); setSize(26, 22); }, NULL, [=] {setSize(34, 60); }, true);
	Hit = Animation_("Hit", NULL, NULL, NULL, false);

}

void Samus::CreateTransition()
{
	Appear.AddTransition("Stand", { new Condition_("Stand", true) });

	Stand.AddTransition("Roll", { new Condition_("Roll",true) });
	Stand.AddTransition("StandUp", { new Condition_("LookUp",true) });
	Stand.AddTransition("Move", { new Condition_("Move",true) });
	Stand.AddTransition("Jump", { new Condition_("Ground",false) });

	StandUp.AddTransition("JumpUp", { new Condition_("Ground",false),new Condition_("LookUp",true) });
	StandUp.AddTransition("Stand", { new Condition_("LookUp",false) });
	StandUp.AddTransition("MoveUp", { new Condition_("LookUp",true),new Condition_("Move",true) });

	JumpUp.AddTransition("StandUp", { new Condition_("Ground",true),new Condition_("LookUp",true) });
	JumpUp.AddTransition("Stand", { new Condition_("Ground",true),new Condition_("LookUp",false) });

	Jump.AddTransition("Stand", { new Condition_("Ground",true) });
	Jump.AddTransition("JumpShoot", { new Condition_("Shoot",true),new Condition_("Ground",false) });
	Jump.AddTransition("JumpUp", { new Condition_("LookUp",true),new Condition_("Ground",false) });

	JumpShoot.AddTransition("Stand", { new Condition_("Ground",true) });

	Move.AddTransition("Stand", { new Condition_("Move",false),new Condition_("Ground",true) });
	Move.AddTransition("Jump", {/* new Condition_("Move",true),*/new Condition_("Ground",false),new Condition_("Falling",true) });
	Move.AddTransition("JumpRoll", { new Condition_("Ground",false),new Condition_("Move",true) });
	Move.AddTransition("MoveUp", { new Condition_("LookUp",true), new Condition_("Move",true) });

	MoveUp.AddTransition("Move", { new Condition_("LookUp",false) });
	MoveUp.AddTransition("StandUp", { new Condition_("Move",false),new Condition_("LookUp",true) });
	MoveUp.AddTransition("Jump", { new Condition_("Ground",false) });

	Roll.AddTransition("Stand", { new Condition_("Roll",false), new Condition_("Falling",false) });
	Roll.AddTransition("Move", { new Condition_("Roll",false),new Condition_("Move",true), new Condition_("Falling",false) });

	JumpRoll.AddTransition("Move", { new Condition_("Ground",true),new Condition_("Move",true) });
	JumpRoll.AddTransition("Stand", { new Condition_("Ground",true),new Condition_("Move",false) });
}

void Samus::LoadImageAnimation(Texture * samusTexture)
{
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

}

void Samus::LoadAnimator()
{
	// ADD TO ANIMATOR
	animator.AddAnimation({ &Appear,&Stand,&StandShoot,&StandUp,&Roll,&Move,&MoveShoot,&MoveUp,&Jump,&JumpShoot,&JumpUp,&JumpRoll });
	//
	animator.currentAnimation = Appear;
	animator.previousAnimation = Appear;
	animator.frameInterval = Appear.duration;

}

