#include "IntroScene.h"

IntroScene::IntroScene()
{
	Time = GameTime::getInstance();
}

IntroScene::~IntroScene()
{
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

	KeyBoard = CKeyboard::getInstance();
	Window = CWindow::getInstance();

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

	//TitleTheme = Sound::LoadSound("Resources/SoundEffect/TitleTheme.wav");
}

eSceneID IntroScene::Render()
{
	//start drawing
	batch->Begin();
	float deltatime = Time->getDeltaTime();
	float dt = deltatime / 1000;
	background.SetRegion(*introSceneAnimation.Next(dt));
	batch->Draw(background);
	//end drawing
	batch->End();
	//Sound::Loop(TitleTheme);
	if (KeyBoard->IsKeyDown(DIK_RETURN))
	{
		return MENUSCENE;
	}
	return getUID();
}

void IntroScene::End()
{
}
