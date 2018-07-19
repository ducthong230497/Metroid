#include "testScene_1.h"

testScene1::testScene1()
{
	Time = GameTime::getInstance();
}

testScene1::~testScene1()
{
}

void testScene1::Init()
{
	object1 = new GameObject();
	object1->setPosition(100, 100);
	object1->setVelocity(0, -100);
	object1->setSize(32, 64);
	object1->setCategoryMask(Category::PLAYER);
	object1->setBitMask(Category::PLATFORM | Category::SKREE | Category::ZOOMER | Category::RIPPER | Category::RIO | Category::BREAKABLE_PLATFORM | Category::MARUNARI | Category::BOMBITEM);
	isGrounded = true;
	jumpTime = 100;

	#pragma region TESTREGION
	/*GameObject * object2 = new GameObject();
	object2->setPosition(200, -200);
	object2->setSize(50, 50);
	object2->setCategoryMask(Category::PLATFORM);
	object2->setBitMask(Category::PLAYER | Category::SKREE | Category::ZOOMER | Category::RIPPER);

	GameObject * object3 = new GameObject();
	object3->setPosition(300, -300);
	object3->setSize(50, 50);
	object3->setCategoryMask(Category::SKREE);
	object3->setBitMask(Category::PLAYER | Category::PLATFORM);

	GameObject * object4 = new GameObject();
	object4->setPosition(300, -400);
	object4->setSize(50, 50);
	object4->setCategoryMask(Category::ZOOMER);
	object4->setBitMask(Category::PLAYER | Category::PLATFORM);*/
	#pragma endregion

	enemiesTexture = Texture("Resources/enemies.png");

	ripper = new Ripper();
	ripper->Init(&enemiesTexture, 400, 400);

	skree = new Skree();
	skree->Init(&enemiesTexture, 500, 300);
	skree->SetScene(this);

	rio = new Rio();
	rio->Init(&enemiesTexture, 600, 200);

	zoomer = new Zoomer();
	zoomer->Init(&enemiesTexture, 600, 100, 1);
	zoomer1 = new Zoomer();
	zoomer1->Init(&enemiesTexture, 160, 272, 1);
	zoomer2 = new Zoomer();
	zoomer2->Init(&enemiesTexture, 160, 384, 1);
	zoomer3 = new Zoomer();
	zoomer3->Init(&enemiesTexture, 300, 384, 1);

	itemsTexture = Texture("Resources/items.png");
	marunari = new Marunari();
	marunari->Init(&itemsTexture, 300, 120);
	marunari->SetScene(this);

	bombItem = new BombItem();
	bombItem->Init(&itemsTexture, 350, 120);
	bombItem->SetScene(this);

	/*GameObjects.push_back(object2);
	GameObjects.push_back(object3);
	GameObjects.push_back(object4);*/
	GameObjects.push_back(object1);
	GameObjects.push_back(ripper);
	GameObjects.push_back(skree);
	GameObjects.push_back(rio);
	GameObjects.push_back(zoomer);
	GameObjects.push_back(zoomer1);
	GameObjects.push_back(zoomer2);
	GameObjects.push_back(zoomer3);
	GameObjects.push_back(marunari);
	GameObjects.push_back(bombItem);

	int result = CDevice::getInstance()->getD3DDevice()->CreateOffscreenPlainSurface(
		object1->getSize().x,					// width 				
		object1->getSize().y,					// height
		D3DFMT_X8R8G8B8,		// format
		D3DPOOL_DEFAULT,		// where? (VRAM or RAM)
		&surface,
		NULL);
	CDevice::getInstance()->getD3DDevice()->ColorFill(surface, NULL, D3DCOLOR_XRGB(155, 155, 155));

	cam = Camera::Instance();
	cam->setPosition(0, 32*15);
	batch = SpriteBatch::Instance();
	batch->SetCamera(cam);
	texture = new Texture("Resources/metroidTileMap2.png");
	KeyBoard = CKeyboard::getInstance();
	collision = new Collision();
	callback = new CollisionCallback();

	//load map
	mapLoader.AddMap("map1", "Resources/maptest.tmx", 1);
	tileMap = mapLoader.GetMap("map1");
	//std::vector<GameObject*> Platforms;
	std::vector<Shape::Rectangle> PlatformGroup = tileMap->GetObjectGroup("Platform")->GetRects();
	for (std::vector<Shape::Rectangle>::iterator it = PlatformGroup.begin(); it != PlatformGroup.end(); ++it)
	{
		GameObject * platform = new GameObject();
		platform->setPosition((*it).x, (*it).y);
		platform->setSize((*it).width, (*it).height);
		platform->setCategoryMask(Category::PLATFORM);
		platform->setBitMask(Category::PLAYER | Category::PLAYER_BULLET | Category::RIO | Category::RIPPER | Category::SKREE | Category::ZOOMER);
		GameObjects.push_back(platform);
	}
	std::vector<Shape::Rectangle> BreakablePlatformGroup = tileMap->GetObjectGroup("BreakablePlatform")->GetRects();
	for (std::vector<Shape::Rectangle>::iterator it = BreakablePlatformGroup.begin(); it != BreakablePlatformGroup.end(); ++it)
	{
		BreakablePlatform * breakPlatform = new BreakablePlatform();
		breakPlatform->setPosition((*it).x, (*it).y);
		breakPlatform->setSize((*it).width, (*it).height);
		breakPlatform->setCategoryMask(Category::BREAKABLE_PLATFORM);
		breakPlatform->setBitMask(Category::PLAYER | Category::PLAYER_BULLET | Category::RIO | Category::RIPPER | Category::SKREE | Category::ZOOMER | Category::BOMB_EXPLOSION);
		breakPlatform->SetScene(this);
		breakPlatform->SetTilemap(tileMap);
		GameObjects.push_back(breakPlatform);
	}

	metroidfullsheet = Texture("Resources/metroidfullsheet.png");
	
	bomb = new Bomb(&metroidfullsheet);
	bomb->SetScene(this);
	GameObjects.push_back(bomb);

	nextScene = TESTSCENE1;
	Trace::Log("Init TestScene1");
}
float test = 0;
void testScene1::Update()
{
	//int i = 0, j = 0;
	float dt = Time->getDeltaTime() / 1000.0f;
	test += dt;
	if (test > 1)
	{
		//Trace::Log("x: %f, y: %f, sizeX: %f, sizeY: %f", object1->getPosition().x, object1->getPosition().y, object1->getSize().x, object1->getSize().y);
		test = 0;
	}
	
	//for (std::vector<GameObject*>::iterator it1 = GameObjects.begin(); it1 != GameObjects.end(); it1++, i++) {
	for (int i = 0; i < GameObjects.size(); i++) {
		if (GameObjects.at(i)->_CategoryMask == PLATFORM) continue;
		(GameObjects.at(i))->UpdateVelocity(object1);
		POINT velocity = (GameObjects.at(i))->getVelocity();
		POINT position = (GameObjects.at(i))->getPosition();
		position.x += velocity.x * dt;
		position.y += velocity.y * dt;
		RECT boardphase = collision->GetBroadphaseRect((GameObjects.at(i)), dt);
		RECT box1 = collision->GetRECT((GameObjects.at(i)));

		bool moveX = true, moveY = true, performOverLaying = true;
		bool needMoveX = false, needMoveY = false;
		//for (std::vector<GameObject*>::iterator it2 = GameObjects.begin(); it2 != GameObjects.end(); it2++, j++) {
		for(int j = 0; j < GameObjects.size(); ++j) {
			if (i == j || (GameObjects.at(i)->collisionType == Static && GameObjects.at(j)->collisionType == Static)) continue;
			if (GameObjects[i]->_CategoryMask == BOMB_EXPLOSION && GameObjects[j]->_CategoryMask == BREAKABLE_PLATFORM)
			{
				int a = 2;
			}
			if (collision->CanMaskCollide((GameObjects.at(i)), (GameObjects.at(j))))
			{
				RECT box2 = collision->GetRECT((GameObjects.at(j)));
				if (collision->IsOverlayingRect(boardphase, box2))
				{
					if (collision->Collide((GameObjects.at(i)), (GameObjects.at(j)), dt))
					{
						callback->OnCollisionEnter((GameObjects.at(i)), (GameObjects.at(j)), collision->_CollisionDirection);
						collision->PerformCollision((GameObjects.at(i)), (GameObjects.at(j)), dt, 0, moveX, moveY);

						
					}
					else
					{
						int touching = collision->IsTouching((GameObjects.at(i)), (GameObjects.at(j))); //If istouching, it means in the next frame, two body will not collide anymore
						if (touching == 1 && velocity.y != 0)
						{
							//_Listener->OnCollisionExit(body1, body2, collision._CollisionDirection);
							//Trace::Log("Exit y axis");
						}
						else if (touching == 2 && velocity.x != 0)
						{
							//_Listener->OnCollisionExit(body1, body2, collision._CollisionDirection);
							//Trace::Log("Exit x axis");
						}
					}
				}
				//check overlaying (sometimes two bodies are already overlaying each other 
				if (collision->IsOverlaying((GameObjects.at(i)), (GameObjects.at(j))))
				{
					callback->OnTriggerEnter((GameObjects.at(i)), (GameObjects.at(j)), performOverLaying);
					if (performOverLaying)
					{
						collision->PerformOverlaying((GameObjects.at(i)), (GameObjects.at(j)), moveX, moveY);
					}
					performOverLaying = true;
				}
			}
		}

#pragma region AFTER CHECKING COLLISION
		/*
		POINT actualCollisionPosition(1000.0f, 1000.0f);
		POINT actualCollisionPosition_1(-1000.0f, -1000.0f);
		POINT smallestDistance(1000.0f, 1000.0f);
		//if (collision->_Collided_Objects.size() == 1)
		//{
		//	//collision->PerformCollision((*it1), NULL, 0, collision->_Collided_Objects[0].direction, moveX, moveY);
		//	if (collision->_Collided_Objects[0].direction.x != NOT_COLLIDED)
		//	{
		//		collision->UpdateTargetPosition((*it1), POINT(collision->_Collided_Objects[0].position.x, 0));
		//	}
		//	if (collision->_Collided_Objects[0].direction.y != NOT_COLLIDED)
		//	{
		//		collision->UpdateTargetPosition((*it1), POINT(0, collision->_Collided_Objects[0].position.y));
		//	}
		//}
		if (collision->_Collided_Objects.size() != 0)
		{
			if (collision->_Collided_Objects.size() == 3)
			{
				int a = 2;
			}
			if ((*it1)->_CategoryMask == Category::RIPPER)
			{
				int a = 2;
			}
			actualCollisionPosition = collision->_Collided_Objects[0].position;
			for (int i = 0; i < collision->_Collided_Objects.size(); ++i)
			{
				if (velocity.x > 0 && collision->_Collided_Objects[i].direction.x != NOT_COLLIDED)
				{
					actualCollisionPosition.x = actualCollisionPosition.x < collision->_Collided_Objects[i].position.x ? actualCollisionPosition.x : collision->_Collided_Objects[i].position.x;
					needMoveX = true;
				}
				else if (velocity.x < 0 && collision->_Collided_Objects[i].direction.x != NOT_COLLIDED)
				{
					actualCollisionPosition_1.x = actualCollisionPosition_1.x > collision->_Collided_Objects[i].position.x ? actualCollisionPosition_1.x : collision->_Collided_Objects[i].position.x;
					needMoveX = true;
				}
				if (velocity.y > 0 && collision->_Collided_Objects[i].direction.y != NOT_COLLIDED)
				{
					actualCollisionPosition.y = actualCollisionPosition.y < collision->_Collided_Objects[i].position.y ? actualCollisionPosition.y : collision->_Collided_Objects[i].position.y;
					needMoveY = true;
				}
				else if (velocity.y < 0 && collision->_Collided_Objects[i].direction.y != NOT_COLLIDED)
				{
					actualCollisionPosition_1.y = actualCollisionPosition_1.y > collision->_Collided_Objects[i].position.y ? actualCollisionPosition_1.y : collision->_Collided_Objects[i].position.y;
					needMoveY = true;
				}

				if (collision->_Collided_Objects[i].direction.x != NOT_COLLIDED)
				{
					smallestDistance.x = smallestDistance.x < abs(collision->_Collided_Objects[i].dxEntry) ? smallestDistance.x : collision->_Collided_Objects[i].dxEntry;
					needMoveX = true;
				}
				if (collision->_Collided_Objects[i].direction.y != NOT_COLLIDED)
				{
					smallestDistance.y = smallestDistance.y < abs(collision->_Collided_Objects[i].dyEntry) ? smallestDistance.y : collision->_Collided_Objects[i].dyEntry;
					needMoveY = true;
				}
			}
		}
		if (needMoveX)
		{
			if (velocity.x > 0)
			{
				collision->UpdateTargetPosition((*it1), POINT(actualCollisionPosition.x, 0), moveX, moveY);
			}
			else if (velocity.x < 0)
			{
				collision->UpdateTargetPosition((*it1), POINT(actualCollisionPosition_1.x, 0), moveX, moveY);
			}
		}
		if (needMoveY)
		{
			if (velocity.y > 0)
			{
				collision->UpdateTargetPosition((*it1), POINT(0, actualCollisionPosition.y), moveX, moveY);
			}
			else if (velocity.y < 0)
			{
				collision->UpdateTargetPosition((*it1), POINT(0, actualCollisionPosition_1.y), moveX, moveY);
			}
		}
		collision->Reset();
		*/
#pragma endregion

		(GameObjects.at(i))->Next(dt, moveX, moveY);
		(GameObjects.at(i))->Update(dt);
	}
	for (int i = 0; i < Bullets.size(); ++i)
	{
		Bullet* currentBullet = Bullets[i];
		if (currentBullet->IsDestroyed())
		{
			std::vector<GameObject*>::iterator it = std::find(GameObjects.begin(), GameObjects.end(), currentBullet);
			GameObjects.erase(it);
			delete currentBullet;
			Bullets.erase(Bullets.begin() + i--);
		}
	}
	cam->setPosition(object1->getPosition().x - 200, cam->getPosition().y);
}

eSceneID testScene1::Render()
{
	batch->Begin();
	//ripper->Render(batch);
	//skree->Render(batch);
	//rio->Render(batch);
	//zoomer->Render(batch);
	//zoomer1->Render(batch);
	//zoomer2->Render(batch);
	//zoomer3->Render(batch);
	//bomb->Render(batch);
	tileMap->Render(batch);
	for (int i = 0; i < Bullets.size(); ++i)
	{
		Bullets[i]->Render(batch);
	}
	DrawSquare();
	batch->End();

	return nextScene;
}

void testScene1::DrawSquare()
{
	//batch->Draw(*texture, 100, -100);
	for (std::vector<GameObject*>::iterator it = GameObjects.begin(); it != GameObjects.end(); ++it)
	{
		switch ((*it)->_CategoryMask)
		{
		case RIPPER:
			((Ripper*)(*it))->Render(batch);
			break;
		case ZOOMER:
			((Zoomer*)(*it))->Render(batch);
			break;
		case RIO:
			((Rio*)(*it))->Render(batch);
			break;
		case SKREE:
			((Skree*)(*it))->Render(batch);
			break;
		case NONE: case BOMB_EXPLOSION:
			((Bomb*)(*it))->Render(batch);
			break;
		case MARUNARI:
			((Marunari*)(*it))->Render(batch);
		default:
			break;
		}
		
		batch->DrawSquare((*it)->getPosition().x, (*it)->getPosition().y, (*it)->getSize().x, (*it)->getSize().y, D3DCOLOR_ARGB(255, 0, 128, 0));
	}
}

void testScene1::ProcessInput()
{
	if (canMove)
	{
		if (KeyBoard->IsKeyDown(DIK_RIGHT))
		{
			//object1->setPosition(object1->getPosition().x+1, object1->getPosition().y);
			object1->setVelocity(200, object1->getVelocity().y);
		}
		else if (KeyBoard->IsKeyUp(DIK_RIGHT))
		{
			object1->setVelocity(0, object1->getVelocity().y);
		}
		if (KeyBoard->IsKeyDown(DIK_LEFT))
		{
			//object1->setPosition(object1->getPosition().x -1, object1->getPosition().y);
			object1->setVelocity(-200, object1->getVelocity().y);
		}
		else if (KeyBoard->IsKeyUp(DIK_LEFT))
		{
			object1->setVelocity(0, object1->getVelocity().y);
		}
		if (KeyBoard->IsKeyDown(DIK_UP))
		{
			//object1->setPosition(object1->getPosition().x, object1->getPosition().y + 1);
			object1->setVelocity(object1->getVelocity().x, 200);
		}
		else if (KeyBoard->IsKeyUp(DIK_UP))
		{
			object1->setVelocity(object1->getVelocity().x, 0);
		}
		if (KeyBoard->IsKeyDown(DIK_DOWN))
		{
			//object1->setPosition(object1->getPosition().x, object1->getPosition().y - 1);
			object1->setVelocity(object1->getVelocity().x, -200);
		}
		else if (KeyBoard->IsKeyUp(DIK_DOWN))
		{
			object1->setVelocity(object1->getVelocity().x, 0);
		}
	}
	if (KeyBoard->IsKeyDown(DIK_RETURN))
	{
		nextScene = GAMEOVERSCENE;
	}
	if (KeyBoard->IsKeyDown(DIK_X))
	{
		//if (Bullets.size() != 0)
		//{
		//	GameObjects.push_back(Bullets.front());
		//	std::vector<Bullet*>::iterator it = std::find(Bullets.begin(), Bullets.end(), Bullets.front());
		//	(*it)->isActive = true;
		//	//Bullets.erase(it);
		//}
		float currentTime = GetTickCount() / 1000.0f;
		if (currentTime > FIRERATE +  lastShootTime)
		{
			lastShootTime = currentTime;
			Bullet *b = new Bullet(&metroidfullsheet);
			b->setPosition(object1->getPosition().x + 20, object1->getPosition().y + 28);
			Bullets.push_back(b);
			GameObjects.push_back(b);
		}
	}
	//hold-jump 
	if (KeyBoard->IsKeyDown(DIK_Z))
	{
		if (jumpTime < MAXJUMPTIME) //continue jumping if there is still jumptime
		{
			object1->setVelocity(object1->getVelocity().x, object1->getVelocity().y + 1.0f);
			jumpTime += 0.02f;
		}
		else
		{
			jumpTime = 100;  //don't jump more
		}
	}
	else
	{
		jumpTime = 100; //don't jump more
	}

	//jump only if grounded
	if (KeyBoard->IsFirstKeyDown(DIK_Z) && isGrounded /*&& !isRolling*/)
	{
		//Sound::Play(jump);
		object1->setVelocity(object1->getVelocity().x, 50);
		isGrounded = false;
		jumpTime = 0;
	}
}

void testScene1::End()
{
	Trace::Log("Init TestScene1");
}
