#include "Game.h"

void Game::_RenderFrame()
{
	if (Device->getD3DDevice()->BeginScene())
	{
		Device->getD3DDevice()->ColorFill(Device->getBackBuffer(), NULL, D3DCOLOR_XRGB(0, 0, 0));
		RenderFrame();
		Device->getD3DDevice()->EndScene();
	}
	Device->getD3DDevice()->Present(NULL, NULL, NULL, NULL);
}

void Game::RenderFrame()
{
}

void Game::ProcessInput()
{
	if (Keyboard->IsKeyDown(DIK_RIGHTARROW)) 
	{
		POINT pos = object1->getPosition();
		pos.x += v * Time->getDeltaTime();
		object1->setPosition(pos);
	}
	else if(Keyboard->IsKeyDown(DIK_LEFTARROW))
	{
		POINT pos = object1->getPosition();
		pos.x -= v * Time->getDeltaTime();
		object1->setPosition(pos);
	}
}

Game::Game() :
	Device(CDevice::getInstance()),
	Window(CWindow::getInstance()),
	Keyboard(CKeyboard::getInstance()),
	Time(GameTime::getInstance()),
	Paused(false),
#pragma region TEST
	object1(new GameObject()),
	object2(new GameObject())
#pragma endregion
{
	object1->setPosition(100, -100);
	object1->setSize(100, 100);
	object2->setPosition(100, -100);
	object2->setSize(100, 100);
}

Game::~Game()
{
	SAFE_DELETE(Device);
	SAFE_DELETE(Window);
	SAFE_DELETE(Keyboard);
	SAFE_DELETE(Time);
}

CDevice * Game::getDevice() const
{
	return Device;
}

CWindow * Game::getWindow() const
{
	return Window;
}

CKeyboard * Game::getKeyboard() const
{
	return Keyboard;
}

void Game::Init(HINSTANCE hInstance, LPCWSTR name, UINT width, UINT height, bool isfullscreen, UINT framerate)
{
	Window->Init(hInstance, name, width, height, isfullscreen, framerate);
	Device->Init(*Window);
	Keyboard->Init(Window->gethWnd(), Window->gethInstance());
}

void Game::Run()
{
	MSG msg;
	int done = 0;
	DWORD tick_per_frame = 1000 / Window->getFrameRate();
	DWORD DeltaTime = Time->getDeltaTime();

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		Time->Update();

		if ((DeltaTime = Time->getDeltaTime()) >= tick_per_frame)
		{
#ifdef SLOW_FPS
			Sleep(1000 / SLOW_FPS - tick_per_frame);
#endif

			Time->Reset();
			//DeltaTime = tick_per_frame;

			_RenderFrame();
		}
		else
		{
			Sleep(tick_per_frame - DeltaTime);
		}

		Keyboard->Update();

		if (Keyboard->IsKeyDown(DIK_ESCAPE))
			PostMessage(Window->gethWnd(), WM_QUIT, 0, 0);
		else
			ProcessInput();
	}
}
