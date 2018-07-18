#include "testScene_2.h"

testScene2::testScene2()
{
}

testScene2::~testScene2()
{
}

void testScene2::Init()
{
	quadTree.Load("Resources/mapQuadTree.xml", "Resources/metroid.tmx");
	//load map
	mapLoader.AddMap("map1", "Resources/metroid.tmx", 1);
	map = mapLoader.GetMap("map1");
	map->SetSpaceDivisionQuadTree(&quadTree);

	cam = Camera::Instance();
	cam->setPosition(0, 32 * 90);
	batch = SpriteBatch::Instance();
	batch->SetCamera(cam);
	KeyBoard = CKeyboard::getInstance();

	enemiesTexture = Texture("Resources/enemies.png");

	ripper = new Ripper();
	ripper->Init(&enemiesTexture, 100, 100);

	nextScene = TESTSCENE2;
	Trace::Log("Init testScene2.");
}

void testScene2::Update()
{
	quadTree.LoadObjectsInViewport(cam, true, true);
}

eSceneID testScene2::Render()
{
	batch->Begin();
	map->Render(batch);
	ripper->Render(batch);
	batch->End();
	return nextScene;
}
int speed = 1;
void testScene2::ProcessInput()
{
	if (KeyBoard->IsKeyDown(DIK_RIGHT))
	{
		POINT pos = cam->getPosition();
		pos.x += speed;
		cam->setPosition(pos);
	}
	if (KeyBoard->IsKeyDown(DIK_LEFT))
	{
		POINT pos = cam->getPosition();
		pos.x -= speed;
		cam->setPosition(pos);
	}
	if (KeyBoard->IsKeyDown(DIK_UP))
	{
		POINT pos = cam->getPosition();
		pos.y += speed;
		cam->setPosition(pos);
	}
	if (KeyBoard->IsKeyDown(DIK_DOWN))
	{
		POINT pos = cam->getPosition();
		pos.y -= speed;
		cam->setPosition(pos);
	}
}

void testScene2::End()
{
}

