#pragma once
#include "define.h"

class CWindow
{
private:
	LPCWSTR Name;
	UINT Width, Height,
		FrameRate;
	bool IsFullScreen;	
	HWND hWnd;
	HINSTANCE hInstance;

	CWindow();
	static CWindow *Instance;
public:	
	std::function<void()> OnKillFocus;
	std::function<void()> OnSetFocus;

	~CWindow();

	LPCWSTR getName() const;
	UINT getWidth() const;
	UINT getHeight() const;
	UINT getFrameRate() const;
	bool getIsFullScreen() const;
	HWND gethWnd() const;
	HINSTANCE gethInstance() const;
	void Init(HINSTANCE hInstance, LPCWSTR name, UINT width, UINT height, bool isfullscreen, UINT framerate);

	static CWindow* getInstance();
	static LRESULT CALLBACK WinProc(HWND _hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

