#include "TestGame.h"

TestGame::TestGame():sceneManager(SceneManager::getInstance())
{
}

TestGame::~TestGame()
{
}

void TestGame::Init(HINSTANCE hInstance, LPCWSTR name, UINT width, UINT height, bool isfullscreen, UINT framerate)
{
	Window->Init(hInstance, name, width, height, isfullscreen, framerate);
	Window->OnKillFocus = std::bind(&TestGame::OnKillFocus, this);
	Window->OnSetFocus = std::bind(&TestGame::OnSetFocus, this);

	Device->Init(*Window);
	Keyboard->Init(Window->gethWnd(), Window->gethInstance());
	sceneManager->Init();
}

void TestGame::RenderFrame()
{
	//sceneManager->Update();
	sceneManager->Render();
}

void TestGame::ProcessInput()
{
}

void TestGame::OnKillFocus()
{
	Keyboard->Unacquire();
}

void TestGame::OnSetFocus()
{
	Keyboard->Acquire();
}
