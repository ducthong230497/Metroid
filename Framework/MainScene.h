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
#include "Collision.h"
#include "CollisionCallback.h"

#include "Zoomer.h"
#include "Rio.h"
#include "Skree.h"
#include "Bomb.h"

#define SPEED 2

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

	float test = 0;
	GameObject *object1;

	Collision *collision;
	CollisionCallback *callback;

	Texture enemiesTexture;
	std::vector<Zoomer*> Zoomers;
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