#include "GameOverScene.h"

GameOverScene::GameOverScene()
{
}

GameOverScene::~GameOverScene()
{
}

void GameOverScene::Init()
{
	//Camera * cam;
	//Texture MenuSceneTexture;
	//std::vector<TextureRegion> regions;
	//SpriteBatch * batch;
	//Sprite background;

	//CKeyboard * KeyBoard;
	//CWindow * Window;
	////thiếu sound

	//eSceneID nextScene;

	KeyBoard = CKeyboard::getInstance();
	Window = CWindow::getInstance();

	cam = Camera::Instance();
	cam->setPosition(0, Window->getHeight() / 2);

	gameOverSceneTexture = Texture("Resources/start.png");
	TexturePacker p = TexturePacker(&gameOverSceneTexture, "Resources/gameoverscene.xml");
	background.SetRegion(p.GetRegion("gameoverscene").at(0));
	background.SetSize(Window->getWidth(), Window->getHeight());
	background.SetPosition(Window->getWidth() / 2, 0);

	batch = SpriteBatch::Instance();
	batch->SetCamera(cam);

	nextScene = GAMEOVERSCENE;
	Trace::Log("Init GameOverScene");
}

eSceneID GameOverScene::Render()
{
	batch->Begin();
	batch->Draw(background);
	batch->End();
	return nextScene;
}

void GameOverScene::ProcessInput()
{
	if (KeyBoard->IsKeyDown(DIK_RETURN))
	{
		nextScene = MENUSCENE;
	}
}

void GameOverScene::End()
{
	Trace::Log("End GameOverScene");
}
