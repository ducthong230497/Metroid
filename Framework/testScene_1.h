#pragma once
#include "Scene.h"
#include "CDevice.h"
#include "Texture.h"
#include "SpriteBatch.h"
#include "Camera.h"
#include "CKeyboard.h"
class testScene1 : public Scene
{
private:
	LPDIRECT3DSURFACE9 surface;
	//test
	Texture * texture;
	SpriteBatch * batch;

	GameObject * object1;
public:
	testScene1();
	~testScene1();

	void Init() override;
	void Update() override;
	eSceneID Render() override;
	void ProcessInput();
	void End() override;

	DEFINE_SCENE_UID(TESTSCENE1)
};