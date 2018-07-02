#include "CWindow.h"

CWindow *CWindow::Instance = nullptr;


CWindow::CWindow()
{
}

CWindow::~CWindow()
{
}

LPCWSTR CWindow::getName() const
{
	return Name;
}

UINT CWindow::getWidth() const
{
	return Width;
}

UINT CWindow::getHeight() const
{
	return Height;
}

UINT CWindow::getFrameRate() const
{
	return FrameRate;
}

bool CWindow::getIsFullScreen() const
{
	return IsFullScreen;
}

HWND CWindow::gethWnd() const
{
	return hWnd;
}

HINSTANCE CWindow::gethInstance() const
{
	return hInstance;
}

void CWindow::Init(HINSTANCE _hInstance, LPCWSTR name, UINT width, UINT height, bool isfullscreen, UINT framerate)
{
	hInstance = _hInstance;
	Name = name;
	Width = width;
	Height = height;
	IsFullScreen = isfullscreen;
	FrameRate = framerate;

	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = Name;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	DWORD style;
	if (IsFullScreen)
		style = WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP;
	else
		style = WS_OVERLAPPEDWINDOW;

	hWnd =
		CreateWindow(
			Name,
			Name,
			style,
			234, //CW_USEDEFAULT,
			84, //CW_USEDEFAULT,
			Width,
			Height,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd)
	{
		DWORD ErrCode = GetLastError();
		Trace::Log("[ERROR] Unable to Create Window, error code: %d", ErrCode);
	}

	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);
}

CWindow * CWindow::getInstance()
{
	if (!Instance)
		Instance = new CWindow();

	return Instance;
}

LRESULT CWindow::WinProc(HWND _hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KILLFOCUS:
		if (Instance->OnKillFocus != nullptr)
			Instance->OnKillFocus();
		break;
	case WM_SETFOCUS:
		if (Instance->OnSetFocus != nullptr)
			Instance->OnSetFocus();
		break;
	default:
		return DefWindowProc(_hWnd, message, wParam, lParam);
	}

	return 0;
}
