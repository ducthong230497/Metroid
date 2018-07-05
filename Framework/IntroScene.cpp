#include "IntroScene.h"

IntroScene::IntroScene()
{
	Time = GameTime::getInstance();
}

IntroScene::~IntroScene()
{
	cam = nullptr;
	batch = nullptr;
	KeyBoard = nullptr;
	Window = nullptr;
}

void IntroScene::Init()
{
	/*
	Camera * cam;
	Texture introSceneTexture;
	Animation introSceneAnimation;
	SpriteBatch * batch;
	Sprite background;
	
	CKeyboard * KeyBoard;
	CWindow * Window;
	*/

	Window = CWindow::getInstance();
	KeyBoard = CKeyboard::getInstance();
	//KeyBoard->Init(Window->gethWnd(), Window->gethInstance());

	cam = Camera::Instance();
	cam->setPosition(0, Window->getHeight() / 2);
	introSceneTexture = Texture("Resources/intro.png");
	TexturePacker p = TexturePacker(&introSceneTexture, "Resources/introscene.xml");

	introSceneAnimation.AddRegion(p.GetRegion("introscene"));
	introSceneAnimation.SetFrameInterval(0.03);
	//	background.SetRegion(&p.GetRegion("introscene").at(0));
	background.SetRegion(*introSceneAnimation.GetKeyAnimation());
	/*background = Sprite(&introSceneTexture);
	background.SetTexture(&introSceneTexture);*/
	background.SetSize(Window->getWidth(), Window->getHeight());
	background.SetPosition(Window->getWidth() / 2, 0);

	batch = SpriteBatch::Instance();
	batch->SetCamera(cam);

	nextScene = INTROSCENE;
	//TitleTheme = Sound::LoadSound("Resources/SoundEffect/TitleTheme.wav");

	Trace::Log("Init IntroScene");
}

eSceneID IntroScene::Render()
{
	//start drawing
	batch->Begin();
	background.SetRegion(*introSceneAnimation.Next(Time->getDeltaTime() / 1000.0f));
	batch->Draw(background);
	//end drawing
	batch->End();
	//Sound::Loop(TitleTheme);
	return nextScene;
}

void IntroScene::ProcessInput()
{
	if (KeyBoard->IsFirstKeyDown(DIK_RETURN))
	{
		nextScene = MENUSCENE;
	}
}

void IntroScene::End()
{
	Trace::Log("End IntroScene");
}
