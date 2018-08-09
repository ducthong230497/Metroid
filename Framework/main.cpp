#include "define.h"
#include "TestGame.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	time_t _tm = time(NULL);
	tm now;
	localtime_s(&now, &_tm);
	char buff[80];
	asctime_s(buff, &now);
	Trace::Log("|| Application Startup: %s", buff);

	/*auto AGG = AladdinGenesisGame::getInstance();
	AGG->Init(hInstance, L"Aladdin Game", 800, 600, false, 38);
	AGG->Run();*/

	TestGame *g = new TestGame();
	g->Init(hInstance, L"Metroid", SCREEN_WIDTH, SCREEN_HEIGHT, 0, 38);
	g->Run();
	return 0;
}

