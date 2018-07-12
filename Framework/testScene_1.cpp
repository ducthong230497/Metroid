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
	object1->setSize(32, 64);
	object1->setCategoryMask(Category::PLAYER);
	object1->setBitMask(Category::PLATFORM | Category::SKREE | Category::ZOOMER | Category::RIPPER | Category::RIO);

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

	rio = new Rio();
	rio->Init(&enemiesTexture, 600, 200);

	zoomer = new Zoomer();
	zoomer->Init(&enemiesTexture, 600, 100, 1);

	/*GameObjects.push_back(object2);
	GameObjects.push_back(object3);
	GameObjects.push_back(object4);*/
	GameObjects.push_back(object1);
	GameObjects.push_back(ripper);
	GameObjects.push_back(skree);
	GameObjects.push_back(rio);
	GameObjects.push_back(zoomer);

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
	std::vector<GameObject*> Platforms;
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

	bulletTexture = Texture("Resources/metroidfullsheet.png");

	/*numberOfBullet = 10;
	for (int i = 0; i < 10; i++)
	{
		Bullet * b = new Bullet(&bulletTexture);
		b->SetPosition(100, 200);
		Bullets.push_back(b);
	}*/
	
	nextScene = TESTSCENE1;
	Trace::Log("Init TestScene1");
}
float test = 0;
void testScene1::Update()
{
	int i = 0, j = 0;
	float dt = Time->getDeltaTime() / 1000.0f;
	test += dt;
	if (test > 1)
	{
		Trace::Log("x: %f, y: %f, sizeX: %f, sizeY: %f", object1->getPosition().x, object1->getPosition().y, object1->getSize().x, object1->getSize().y);
		test = 0;
	}
	
	for (std::vector<GameObject*>::iterator it1 = GameObjects.begin(); it1 != GameObjects.end(); it1++, i++) {
		if ((*it1)->_CategoryMask == PLATFORM) continue;
		(*it1)->UpdateVelocity(object1);
		POINT velocity = (*it1)->getVelocity();
		POINT position = (*it1)->getPosition();
		position.x += velocity.x * dt;
		position.y += velocity.y * dt;
		RECT boardphase = collision->GetBroadphaseRect((*it1), dt);
		RECT box1 = collision->GetRECT((*it1));

		bool moveX = true, moveY = true;
		bool needMoveX = false, needMoveY = false;
		for (std::vector<GameObject*>::iterator it2 = GameObjects.begin(); it2 != GameObjects.end(); it2++, j++) {
			if (collision->CanMaskCollide((*it1), (*it2)))
			{
				RECT box2 = collision->GetRECT((*it2));
				if (collision->IsOverlayingRect(boardphase, box2))
				{
					if (collision->Collide((*it1), (*it2), dt))
					{
						callback->OnCollisionEnter((*it1), (*it2), collision->_CollisionDirection);
						collision->PerformCollision((*it1), (*it2), dt, 0, moveX, moveY);

						
					}
					else
					{
						int touching = collision->IsTouching((*it1), (*it2)); //If istouching, it means in the next frame, two body will not collide anymore
						if (touching == 1 && velocity.y != 0)
						{
							//_Listener->OnCollisionExit(body1, body2, collision._CollisionDirection);
							Trace::Log("Exit y axis");
						}
						else if (touching == 2 && velocity.x != 0)
						{
							//_Listener->OnCollisionExit(body1, body2, collision._CollisionDirection);
							Trace::Log("Exit x axis");
						}
					}
				}
				//check overlaying (sometimes two bodies are already overlaying each other 
				if (collision->IsOverlaying((*it1), (*it2)))
				{
					collision->PerformOverlaying((*it1), (*it2), moveX, moveY);
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

		(*it1)->Next(dt, moveX, moveY);
		(*it1)->Update(dt);
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
}

eSceneID testScene1::Render()
{
	batch->Begin();
	ripper->Render(batch);
	skree->Render(batch);
	rio->Render(batch);
	zoomer->Render(batch);
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

		//// Generate a random area (within back buffer) to draw the surface onto
		//RECT rect;
		///*
		//rect.left = rand() % (SCREEN_WIDTH/2);
		//rect.top = rand() % (SCREEN_HEIGHT/2);
		//rect.right = rect.left + rand() % (SCREEN_WIDTH/2);
		//rect.bottom = rect.left + rand() % (SCREEN_HEIGHT/2);
		//*/

		///*x += v * dt;
		//if (x + 50 > SCREEN_WIDTH || x<0) v = -v;*/

		////
		//// WORLD TO VIEWPORT TRANSFORM USING MATRIX
		////
		//D3DXVECTOR3 position((*it)->getPosition().x, (*it)->getPosition().y, 0);
		//D3DXMATRIX mt;
		//D3DXMatrixIdentity(&mt);
		//mt._22 = -1.0f;
		//mt._41 = -0;
		//mt._42 = 0;
		//D3DXVECTOR4 vp_pos;
		//D3DXVec3Transform(&vp_pos, &position, &mt);

		//D3DXVECTOR3 p(vp_pos.x, vp_pos.y, 0);
		//D3DXVECTOR3 center((float)100 / 2, (float)100 / 2, 0);

		//rect.left = p.x;
		//rect.top = p.y;
		//rect.right = rect.left + (*it)->getSize().x;
		//rect.bottom = rect.top + (*it)->getSize().y;

		////Trace::Log("x : %f, y : %f", p.x, p.y);
		//CDevice::getInstance()->getD3DDevice()->StretchRect(
		//	surface,
		//	NULL,
		//	CDevice::getInstance()->getBackBuffer(),
		//	&rect,
		//	D3DTEXF_NONE
		//);
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
			Bullet *b = new Bullet(&bulletTexture);
			b->setPosition(object1->getPosition().x + 20, object1->getPosition().y + 28);
			Bullets.push_back(b);
			GameObjects.push_back(b);
		}
	}
}

void testScene1::End()
{
	Trace::Log("Init TestScene1");
}
