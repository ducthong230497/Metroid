#pragma once
#include "Scene.h"
#include "CDevice.h"
#include "Camera.h"
#include "MapLoader.h"
#include "TileMap.h"
#include "QuadTree.h"
#include "Ripper.h"
#include "SpriteBatch.h"
#include "Texture.h"
#include "CKeyboard.h"
class testScene2 : public Scene
{
private:
	Camera *cam;
	//load map
	//quadtree
	MapLoader mapLoader;
	TileMap* map;
	QuadTree quadTree;
	SpriteBatch *batch;

	Texture enemiesTexture;
	Ripper * ripper;

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