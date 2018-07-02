#pragma once
#include "define.h"
class GameTime
{
private:
	static GameTime* Instance;
	DWORD
		InitTime,	 // The time when first initialized
		LastTime,	 // The time of previous frame
		CurrentTime, // The time of the current frame
		DeltaTime;	 // = CurrentTime - LastTime
	GameTime();
public:
	~GameTime();

	DWORD getDeltaTime() const;
	DWORD getElapsedTime() const;
	// Must be called every game loop
	void Update();
	// Reset the time so that LastTime = CurrentTime
	void Reset();

	static GameTime* getInstance();
};

#define ALA_DELTATIME GameTime::getInstance()->getDeltaTime()
