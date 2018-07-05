#include "MenuScene.h"

MenuScene::MenuScene()
{
	Time = GameTime::getInstance();
}

MenuScene::~MenuScene()
{
}

void MenuScene::Init()
{
	KeyBoard = CKeyboard::getInstance();
	Window = CWindow::getInstance();

	cam = Camera::Instance();
	cam->setPosition(0, Window->getHeight() / 2);
	MenuSceneTexture = Texture("Resources/start.png");
	TexturePacker p = TexturePacker(&MenuSceneTexture, "Resources/startscene.xml");
	regions = p.GetRegion("startscene");

	background.SetRegion(p.GetRegion("startscene").at(0));
	background.SetSize(Window->getWidth(), Window->getHeight());
	background.SetPosition(Window->getWidth() / 2, 0);

	batch = SpriteBatch::Instance();
	batch->SetCamera(cam);

	nextScene = MENUSCENE;
	Trace::Log("Init MenuScene");
}

eSceneID MenuScene::Render()
{
	batch->Begin();
	batch->Draw(background);
	batch->End();
	return nextScene;
}

void MenuScene::ProcessInput()
{
	if (KeyBoard->IsKeyDown(DIK_DOWN))
	{
		background.SetRegion(regions.at(1));
	}
	if (KeyBoard->IsKeyDown(DIK_UP))
	{
		background.SetRegion(regions.at(0));
	}
	if (KeyBoard->IsKeyDown(DIK_RETURN))
	{
		nextScene = TESTSCENE1;
	}
}

void MenuScene::End()
{
	Trace::Log("End MenuScene");
}
