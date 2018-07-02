#pragma once
#include "define.h"
#include "CKeyboard.h"
#include "CWindow.h"
#include "CDevice.h"
#include "GameTime.h"
#include "GameObject.h"
class Game
{
private:
	void _RenderFrame();
protected:
	CDevice * Device;
	CWindow *Window;
	CKeyboard *Keyboard;
	GameTime *Time;
	bool Paused;

#pragma region Test
	// Random color for the surface
	int r = 255;
	int g = 255;
	int b = 255;
	GameObject * object1;
	GameObject * object2;
	float x = 100;
	float y = -100;
	float v = 0.06f;
	LPDIRECT3DSURFACE9 surface;
#pragma endregion


	virtual void RenderFrame();
	virtual void ProcessInput();
public:
	Game();
	virtual ~Game();

	CDevice* getDevice() const;
	CWindow* getWindow() const;
	CKeyboard* getKeyboard() const;

	void Init(HINSTANCE hInstance, LPCWSTR name, UINT width, UINT height, bool isfullscreen, UINT framerate);
	void Run();
};

#pragma once
