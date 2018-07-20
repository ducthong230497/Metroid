#pragma once
#include <iostream>
#include "Audio.h"

using namespace std;
class Sound
{
private:
	static CSoundManager *dsound;

	Sound()  // Disallow creating an instance of this object
	{

	}

	~Sound()
	{

	}

public:
	static bool Init(HWND hwnd);
	static void Shutdown();
	static CSound *LoadSound(string filename);
	static void Play(CSound *sound);
	static void Loop(CSound *sound);
	static void Stop(CSound *sound);

};

