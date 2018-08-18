#include "Metroid.h"

Metroid::Metroid():sceneManager(SceneManager::getInstance())
{
}

Metroid::~Metroid()
{
}

void Metroid::Init(HINSTANCE hInstance, LPCWSTR name, UINT width, UINT height, bool isfullscreen, UINT framerate)
{
	Window->Init(hInstance, name, width, height, isfullscreen, framerate);
	Window->OnKillFocus = std::bind(&Metroid::OnKillFocus, this);
	Window->OnSetFocus = std::bind(&Metroid::OnSetFocus, this);

	Device->Init(*Window);
	Keyboard->Init(Window->gethWnd(), Window->gethInstance());
	Sound::Init(Window->gethWnd());
	sceneManager->Init();

	QuadTree quadtree;
	quadtree.BuildTreeNodesFromTMX("Resources/mapQuadTree.xml", "Resources/metroid.tmx");
}

void Metroid::RenderFrame()
{
	//sceneManager->Update();
	sceneManager->Render();
}

void Metroid::ProcessInput()
{
	sceneManager->getCurrentScene()->ProcessInput();
}

void Metroid::OnKillFocus()
{
	Keyboard->Unacquire();
}

void Metroid::OnSetFocus()
{
	Keyboard->Acquire();
}
