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

#define SPEED 1

class MainScene : public Scene
{
private:
	Camera * cam;
	//load map
	//quadtree
	MapLoader mapLoader;
	TileMap* tileMap;
	QuadTree * quadTree;
	SpriteBatch * batch;
	CKeyboard * KeyBoard;

	Texture enemiesTexture;
	Ripper * ripper;
public:
	MainScene();
	~MainScene();

	void Init() override;
	void Update() override;
	eSceneID Render() override;
	void DrawSquare();
	void ProcessInput();
	void End() override;

	DEFINE_SCENE_UID(MAINSCENE)
};