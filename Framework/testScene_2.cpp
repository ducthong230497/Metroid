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

	enemiesTexture = Texture("Resources/enemies.png");

	ripper = new Ripper();
	ripper->Init(&enemiesTexture, 100, 100);

	nextScene = TESTSCENE2;
	Trace::Log("Init testScene2");
}

void testScene2::Update()
{
	quadTree.LoadObjectsInViewport(cam, true, true);
	Trace::Log("x: %f, y: %f", ripper->GetPosition().x, ripper->GetPosition().y);
}

eSceneID testScene2::Render()
{
	batch->Begin();
	map->Render(batch);
	ripper->Render(batch);
	batch->End();
	return nextScene;
}

void testScene2::End()
{
}

