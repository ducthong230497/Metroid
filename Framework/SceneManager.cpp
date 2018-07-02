#include "SceneManager.h"
#define ALA_BLACKSCENE_DELAY 10
SceneManager* SceneManager::Instance = nullptr;

SceneManager::SceneManager() :
	CurrentScene(nullptr),
	CurrentID(UNKNOWNSCENE),
	NextID(UNKNOWNSCENE),
	//BlackScreen(nullptr),
	BlackScreenDelay(ALA_BLACKSCENE_DELAY)
{
}

bool SceneManager::CloseScene()
{
	if (BlackScreenDelay > 0)
	{
		//BlackScreen->Render();
		BlackScreenDelay--;
		return false;
	}
	else
	{
		BlackScreenDelay = ALA_BLACKSCENE_DELAY;
		return true;
	}
}

Scene* SceneManager::get(eSceneID uid)
{
	Scene* scene = nullptr;
	switch (uid)
	{
	case UNKNOWNSCENE: scene = nullptr; break;
	case INTROSCENE: scene = nullptr; break;
	/*case MENUSCENE: scene = new MenuScene(); break;
	case INSTRUCTIONSCENE: scene = new InstructionScene(); break;
	case AGRABAHMARKETSCENE: scene = new AgrabahMarket(); break;
	case JAFARPALACESCENE: scene = new JafarPalace(); break;
	case LEVELCOMPLETESCENE: scene = new LevelCompleteScene(); break;
	case RETIRESCENE: scene = new RetireScene(); break;*/
	case TESTSCENE1: scene = new testScene1(); break;
	case TESTSCENE2: scene = new testScene2(); break;
	}
	if (scene)
		scene->Init();
	return scene;
}


SceneManager::~SceneManager()
{
	SAFE_DELETE(CurrentScene);
	//SAFE_DELETE(BlackScreen);
}

Scene * SceneManager::getCurrentScene() const
{
	return CurrentScene;
}

eSceneID SceneManager::getCurrentID() const
{
	return CurrentID;
}

void SceneManager::Init()
{
	//BlackScreen = new SpriteSheet(L"Sprites/blacksprite.png");
	//BlackScreen->setOpacity(0.4f);

	// First scene change here.
	//CurrentID = NextID = MENUSCENE;
	CurrentID = NextID = TESTSCENE1;
	CurrentScene = get(CurrentID);

#ifdef _DEBUG
	Keyboard = CKeyboard::getInstance();
#endif
}

void SceneManager::Render()
{
#ifdef _DEBUG
#ifdef SHOW_BOUNDINGBOX
	if (Keyboard->IsKeyDown(DIK_Q))
		int a = 2;
		//GameObject::_show_boundingbox = !GameObject::_show_boundingbox;
#endif
	if (Keyboard->IsKeyDown(DIK_F1))
		NextID = AGRABAHMARKETSCENE;
	else if (Keyboard->IsKeyDown(DIK_F2))
		NextID = JAFARPALACESCENE;
	else if (Keyboard->IsKeyDown(DIK_F3))
		NextID = MENUSCENE;
	else if (Keyboard->IsKeyDown(DIK_F4))
		NextID = INSTRUCTIONSCENE;
	else if (Keyboard->IsKeyDown(DIK_F5))
		NextID = LEVELCOMPLETESCENE;
	else if (Keyboard->IsKeyDown(DIK_F6))
		NextID = RETIRESCENE;
	else if (Keyboard->IsKeyDown(DIK_F7))
		NextID = TESTSCENE1;
	else if (Keyboard->IsKeyDown(DIK_F8))
		NextID = TESTSCENE2;
#endif

	if (NextID == CurrentID)
	{
		CurrentScene->Update();
		NextID = CurrentScene->Render();
	}
	else if (CloseScene())
	{
		if (CurrentScene)
		{
			CurrentScene->End();
			SAFE_DELETE(CurrentScene);
		}
		auto nextscene = get(NextID);
		if (nextscene == nullptr)
		{
			Trace::Log("[ERROR] Scene ID (%d) is nullptr.", NextID);
		}
		else
		{
			CurrentID = NextID;
			CurrentScene = nextscene;
			nextscene = nullptr;
		}
	}
	int a = 2;
}

void SceneManager::ProcessInput()
{
	CurrentScene->ProcessInput();
}

SceneManager * SceneManager::getInstance()
{
	if (!Instance)
		Instance = new SceneManager();

	return Instance;
}
