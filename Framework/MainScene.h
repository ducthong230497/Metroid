#pragma once
#include "Scene.h"
#include "CDevice.h"
#include "Camera.h"
#include "MapLoader.h"
#include "TileMap.h"
#include "QuadTree.h"
#include "Ripper.h"
#include "Samus.h"
#include "SpriteBatch.h"
#include "Texture.h"
#include "CKeyboard.h"
#include "Collision.h"
#include "CollisionCallback.h"

#include "Zoomer.h"
#include "Rio.h"
#include "Skree.h"
#include "Bomb.h"
#include "Sound.h"

#define SPEED 2
#define APPEARANCETIME 3

enum Section
{
	Brinstar,
	Kraid,
	MotherBrain
};

class MainScene : public Scene
{
private:
	float apprearanceTime;
	Texture samusTexture;
	Samus * samus;
	Camera * cam;
	float cameraOffsetX;
	//load map
	//quadtree
	QuadTree * quadTree;
	MapLoader mapLoader;
	TileMap* tileMap;
	SpriteBatch * batch;
	CKeyboard * KeyBoard;

public:
	std::vector<GameObject*> playerBullets;
	//Enemy
	std::vector<GameObject*> skreeBullet;
	//GameObject *object1;
private:
	Collision *collision;
	CollisionCallback *callback;

	Texture enemiesTexture;
	Texture itemsTexture;

	//Sound
	CSound * Appearance;
	CSound * Brinstar;
	CSound * Kraid;
	CSound * MotherBrain;
	int flagsound;

public:
	MainScene();
	~MainScene();

	std::vector<GameObject*> getSkreeBullet() { return skreeBullet; }

	void Init() override;
	void Update() override;
	eSceneID Render() override;
	void DrawSquare();
	void ProcessInput();
	void End() override;
	void PlaySoundTheme();
	void RemoveObject(GameObject* object);
	DEFINE_SCENE_UID(MAINSCENE)
};