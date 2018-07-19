#include "MainScene.h"

MainScene::MainScene()
{
	Time = GameTime::getInstance();
}

MainScene::~MainScene()
{
}

void MainScene::Init()
{
	cam = Camera::Instance();
	cam->setPosition(0, 32 * 90);
	batch = SpriteBatch::Instance();
	batch->SetCamera(cam);
	KeyBoard = CKeyboard::getInstance();
	collision = new Collision();
	callback = new CollisionCallback();

	//load quadtree
	quadTree = new QuadTree();
	quadTree->Load("Resources/mapQuadTree.xml", "Resources/metroid.tmx");
	mapLoader.AddMap("map1", "Resources/metroid.tmx", 1);
	tileMap = mapLoader.GetMap("map1");
	tileMap->SetSpaceDivisionQuadTree(quadTree);

	object1 = new GameObject();
	object1->setPosition(100, 100);
	object1->setVelocity(0, -100);
	object1->setSize(32, 64);
	object1->setCategoryMask(Category::PLAYER);
	object1->setBitMask(Category::PLATFORM | Category::SKREE | Category::ZOOMER | Category::RIPPER | Category::RIO | Category::BREAKABLE_PLATFORM);

	std::vector<GameObject*> platforms = quadTree->GetObjectsGroup("Platform");
	for (std::vector<GameObject*>::iterator it = platforms.begin(); it != platforms.end(); ++it)
	{
		(*it)->_CategoryMask = PLATFORM;
		(*it)->_BitMask = Category::PLAYER | Category::PLAYER_BULLET | Category::RIO | Category::RIPPER | Category::SKREE | Category::ZOOMER;
	}


	//Initialize Enemy
	enemiesTexture = Texture("Resources/enemies.png");

	//Zoomer
	std::vector<GameObject*> zoomers = quadTree->GetObjectsGroup("Zoomer");
	for (std::vector<GameObject*>::iterator it = zoomers.begin(); it != zoomers.end(); ++it)
	{
		((Zoomer*)(*it))->Init(&enemiesTexture, (*it)->getPosition().x, (*it)->getPosition().y, 1);
	}
	//InverseZoomer
	std::vector<GameObject*> inversezoomers = quadTree->GetObjectsGroup("InverseZoomer");
	for (std::vector<GameObject*>::iterator it = inversezoomers.begin(); it != inversezoomers.end(); ++it)
	{
		((Zoomer*)(*it))->Init(&enemiesTexture, (*it)->getPosition().x, (*it)->getPosition().y, 1);
		POINT temp = (*it)->getVelocity();
		temp.x *= -1;
		(*it)->setVelocity(temp.x, temp.y);
	}
	//Skree
	std::vector<GameObject*> skrees = quadTree->GetObjectsGroup("Skree");
	for (std::vector<GameObject*>::iterator it = skrees.begin(); it != skrees.end(); ++it)
	{
		((Skree*)(*it))->Init(&enemiesTexture, (*it)->getPosition().x, (*it)->getPosition().y);
		((Skree*)(*it))->SetScene(this);
	}
	//Ripper
	std::vector<GameObject*> rippers = quadTree->GetObjectsGroup("Ripper");
	for (std::vector<GameObject*>::iterator it = rippers.begin(); it != rippers.end(); ++it)
	{
		((Ripper*)(*it))->Init(&enemiesTexture, (*it)->getPosition().x, (*it)->getPosition().y);
	}
	//Rio
	std::vector<GameObject*> rios = quadTree->GetObjectsGroup("Rio");
	for (std::vector<GameObject*>::iterator it = rios.begin(); it != rios.end(); ++it)
	{
		((Rio*)(*it))->Init(&enemiesTexture, (*it)->getPosition().x, (*it)->getPosition().y);
	}


	nextScene = MAINSCENE;
	Trace::Log("Init MainScene");
}

void MainScene::Update()
{
	quadTree->LoadObjectsInViewport(cam, true, true);
	int a = 2;

	float dt = Time->getDeltaTime() / 1000.0f;
	test += dt;
	if (test > 1)
	{
		//Trace::Log("x: %f, y: %f, sizeX: %f, sizeY: %f", object1->getPosition().x, object1->getPosition().y, object1->getSize().x, object1->getSize().y);
		test = 0;
	}
	GameObjects.clear();
	GameObjects.insert(GameObjects.end(), quadTree->GetObjectsInViewport().begin(), quadTree->GetObjectsInViewport().end());
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
		for (int j = 0; j < GameObjects.size(); ++j) {
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
	/*for (int i = 0; i < Bullets.size(); ++i)
	{
		Bullet* currentBullet = Bullets[i];
		if (currentBullet->IsDestroyed())
		{
			std::vector<GameObject*>::iterator it = std::find(GameObjects.begin(), GameObjects.end(), currentBullet);
			GameObjects.erase(it);
			delete currentBullet;
			Bullets.erase(Bullets.begin() + i--);
		}
	}*/
	//cam->setPosition(object1->getPosition().x - 200, cam->getPosition().y);
}

eSceneID MainScene::Render()
{
	batch->Begin();
	tileMap->Render(batch);
	DrawSquare();
	batch->End();
	return nextScene;
}

void MainScene::DrawSquare()
{
	/*std::vector<GameObject*> objects;
	objects.insert(objects.end(), quadTree->GetObjectsInViewport().begin(), quadTree->GetObjectsInViewport().end());*/
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
		default:
			break;
		}

		batch->DrawSquare((*it)->getPosition().x, (*it)->getPosition().y, (*it)->getSize().x, (*it)->getSize().y, D3DCOLOR_ARGB(255, 0, 128, 0));
	}
}

void MainScene::ProcessInput()
{
	if (KeyBoard->IsKeyDown(DIK_RIGHT))
	{
		POINT pos = cam->getPosition();
		pos.x += SPEED;
		cam->setPosition(pos);
	}
	if (KeyBoard->IsKeyDown(DIK_LEFT))
	{
		POINT pos = cam->getPosition();
		pos.x -= SPEED;
		cam->setPosition(pos);
	}
	if (KeyBoard->IsKeyDown(DIK_UP))
	{
		POINT pos = cam->getPosition();
		pos.y += SPEED;
		cam->setPosition(pos);
	}
	if (KeyBoard->IsKeyDown(DIK_DOWN))
	{
		POINT pos = cam->getPosition();
		pos.y -= SPEED;
		cam->setPosition(pos);
	}
}

void MainScene::End()
{
}
