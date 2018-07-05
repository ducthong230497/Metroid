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
	object1->setPosition(100, -100);
	object1->setSize(50, 50);
	object1->setCategoryMask(Category::PLAYER);
	object1->setBitMask(Category::PLATFORM | Category::SKREE | Category::ZOOMER);

	GameObject * object2 = new GameObject();
	object2->setPosition(200, -200);
	object2->setSize(50, 50);
	object2->setCategoryMask(Category::PLATFORM);
	object2->setBitMask(Category::PLAYER | Category::SKREE | Category::ZOOMER);

	GameObject * object3 = new GameObject();
	object3->setPosition(300, -300);
	object3->setSize(50, 50);
	object3->setCategoryMask(Category::SKREE);
	object3->setBitMask(Category::PLAYER | Category::PLATFORM);

	GameObject * object4 = new GameObject();
	object4->setPosition(300, -400);
	object4->setSize(50, 50);
	object4->setCategoryMask(Category::ZOOMER);
	object4->setBitMask(Category::PLAYER | Category::PLATFORM);

	ripper = new Ripper();
	enemiesTexture = Texture("Resources/enemies.png");
	ripper->Init(&enemiesTexture, 200, -200);

	GameObjects.push_back(object1);
	GameObjects.push_back(object2);
	GameObjects.push_back(object3);
	GameObjects.push_back(object4);
	GameObjects.push_back(ripper);

	int result = CDevice::getInstance()->getD3DDevice()->CreateOffscreenPlainSurface(
		object1->getSize().x,					// width 				
		object1->getSize().y,					// height
		D3DFMT_X8R8G8B8,		// format
		D3DPOOL_DEFAULT,		// where? (VRAM or RAM)
		&surface,
		NULL);
	CDevice::getInstance()->getD3DDevice()->ColorFill(surface, NULL, D3DCOLOR_XRGB(155, 155, 155));

	cam = Camera::Instance();
	cam->setPosition(0, 0);
	batch = SpriteBatch::Instance();
	batch->SetCamera(cam);
	texture = new Texture("Resources/metroidTileMap2.png");
	KeyBoard = CKeyboard::getInstance();
	collision = new Collision();

	nextScene = TESTSCENE1;
	Trace::Log("Init TestScene1");
}

void testScene1::Update()
{
	int i = 0, j = 0;
	bool collide = false;
	//Trace::Log("Update Scene");
	float dt = Time->getDeltaTime();
	for (std::vector<GameObject*>::iterator it1 = GameObjects.begin(); it1 != GameObjects.end(); it1++, i++) {
		POINT velocity = (*it1)->getVelocity();
		POINT position = (*it1)->getPosition();
		position.x += velocity.x * dt;
		position.y += velocity.y * dt;
		RECT boardphase = collision->GetBroadphaseRect((*it1), dt);
		RECT box1 = collision->GetRECT((*it1));

		bool moveX = true, moveY = true;

		for (std::vector<GameObject*>::iterator it2 = GameObjects.begin(); it2 != GameObjects.end(); it2++, j++) {
			if (collision->CanMaskCollide((*it1), (*it2)))
			{
				RECT box2 = collision->GetRECT((*it2));
				if (collision->IsOverlayingRect(boardphase, box2))
				{
					if (collision->Collide((*it1), (*it2), dt))
					{
						Trace::Log("Collide");
						collide = true;
						collision->PerformCollision((*it1), (*it2), dt, 0, moveX, moveY);
					}
				}
			}
		}
		j = 0;
		(*it1)->Next(dt, moveX, moveY);
	}
}

eSceneID testScene1::Render()
{
	batch->Begin();
	ripper->Render(batch);
	batch->Draw(*texture, 100, -100);
	//for (std::vector<GameObject*>::iterator it = GameObjects.begin(); it != GameObjects.end(); ++it)
	//{
	//	// Generate a random area (within back buffer) to draw the surface onto
	//	RECT rect;
	//	/*
	//	rect.left = rand() % (SCREEN_WIDTH/2);
	//	rect.top = rand() % (SCREEN_HEIGHT/2);
	//	rect.right = rect.left + rand() % (SCREEN_WIDTH/2);
	//	rect.bottom = rect.left + rand() % (SCREEN_HEIGHT/2);
	//	*/

	//	/*x += v * dt;
	//	if (x + 50 > SCREEN_WIDTH || x<0) v = -v;*/

	//	//
	//	// WORLD TO VIEWPORT TRANSFORM USING MATRIX
	//	//
	//	D3DXVECTOR3 position((*it)->getPosition().x, (*it)->getPosition().y, 0);
	//	D3DXMATRIX mt;
	//	D3DXMatrixIdentity(&mt);
	//	mt._22 = -1.0f;
	//	mt._41 = -0;
	//	mt._42 = 0;
	//	D3DXVECTOR4 vp_pos;
	//	D3DXVec3Transform(&vp_pos, &position, &mt);

	//	D3DXVECTOR3 p(vp_pos.x, vp_pos.y, 0);
	//	D3DXVECTOR3 center((float)100 / 2, (float)100 / 2, 0);

	//	rect.left = p.x;
	//	rect.top = p.y;
	//	rect.right = rect.left + 50;
	//	rect.bottom = rect.top + 50;

	//	//Trace::Log("x : %f, y : %f", p.x, p.y);
	//	CDevice::getInstance()->getD3DDevice()->StretchRect(
	//		surface,
	//		NULL,
	//		CDevice::getInstance()->getBackBuffer(),
	//		&rect,
	//		D3DTEXF_NONE
	//	);
	//	batch->DrawSquare((*it)->getPosition().x + (*it)->getSize().x / 2, (*it)->getPosition().y - (*it)->getSize().y / 2, (*it)->getSize().x, (*it)->getSize().y, D3DCOLOR_ARGB(255, 0, 128, 0));
	//}
	batch->End();

	return nextScene;
}

void testScene1::ProcessInput()
{
	
	if (KeyBoard->IsKeyDown(DIK_RIGHT))
	{
		//object1->setPosition(object1->getPosition().x+1, object1->getPosition().y);
		object1->setVelocity(0.5, object1->getVelocity().y);
	}
	else if(KeyBoard->IsKeyUp(DIK_RIGHT))
	{
		object1->setVelocity(0, object1->getVelocity().y);
	}
	if (KeyBoard->IsKeyDown(DIK_LEFT))
	{
		//object1->setPosition(object1->getPosition().x -1, object1->getPosition().y);
		object1->setVelocity(-0.5, object1->getVelocity().y);
	}
	else if (KeyBoard->IsKeyUp(DIK_LEFT))
	{
		object1->setVelocity(0, object1->getVelocity().y);
	}
	if (KeyBoard->IsKeyDown(DIK_UP))
	{
		//object1->setPosition(object1->getPosition().x, object1->getPosition().y + 1);
		object1->setVelocity(object1->getVelocity().x, 0.5);
	}
	else if (KeyBoard->IsKeyUp(DIK_UP))
	{
		object1->setVelocity(object1->getVelocity().x, 0);
	}
	if (KeyBoard->IsKeyDown(DIK_DOWN))
	{
		//object1->setPosition(object1->getPosition().x, object1->getPosition().y - 1);
		object1->setVelocity(object1->getVelocity().x, -0.5);
	}
	else if (KeyBoard->IsKeyUp(DIK_DOWN))
	{
		object1->setVelocity(object1->getVelocity().x, 0);
	}
	if (KeyBoard->IsKeyDown(DIK_RETURN))
	{
		nextScene = GAMEOVERSCENE;
	}
}

void testScene1::End()
{
	Trace::Log("Init TestScene1");
}
