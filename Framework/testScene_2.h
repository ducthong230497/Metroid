#pragma once
#include "Scene.h"
#include "CDevice.h"

class testScene2 : public Scene
{
private:
	LPDIRECT3DSURFACE9 surface;
public:
	testScene2();
	~testScene2();

	void Init() override;
	void Update() override;
	eSceneID Render() override;
	//void ProcessInput();
	void End() override;

	DEFINE_SCENE_UID(TESTSCENE2)
};