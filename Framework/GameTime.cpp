#include "GameTime.h"

GameTime* GameTime::Instance = nullptr;

GameTime::GameTime() :
	DeltaTime(0)
{
	InitTime = CurrentTime = LastTime = GetTickCount();
}

GameTime::~GameTime()
{
}

DWORD GameTime::getDeltaTime() const
{
	return DeltaTime;
}

DWORD GameTime::getElapsedTime() const
{
	return CurrentTime - InitTime;
}

void GameTime::Update()
{
	CurrentTime = GetTickCount();
	DeltaTime = CurrentTime - LastTime;
}

void GameTime::Reset()
{
	LastTime = CurrentTime;
}

GameTime * GameTime::getInstance()
{
	if (!Instance)
		Instance = new GameTime();

	return Instance;
}
