#include "MainScene.h"

MainScene::MainScene()
{
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


	//load quadtree
	quadTree = new QuadTree();
	quadTree->Load("Resources/mapQuadTree.xml", "Resources/metroid.tmx");
	mapLoader.AddMap("map1", "Resources/metroid.tmx", 1);
	tileMap = mapLoader.GetMap("map1");
	tileMap->SetSpaceDivisionQuadTree(quadTree);

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
	nextScene = MAINSCENE;
	Trace::Log("Init MainScene");
}

void MainScene::Update()
{
	quadTree->LoadObjectsInViewport(cam, true, true);
	int a = 2;
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
	std::vector<GameObject*> objects;
	objects.insert(objects.end(), quadTree->GetObjectsInViewport().begin(), quadTree->GetObjectsInViewport().end());
	for (std::vector<GameObject*>::iterator it = objects.begin(); it != objects.end(); ++it)
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

