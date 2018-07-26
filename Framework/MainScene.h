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
#include "Explosion.h"
#include "HealthItem.h"
#include "Door.h"
#include "Kraid.h"

#define SPEED 2
#define APPEARANCETIME 3

enum Section
{
	Brinstar,
	KraidTheme,
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
	bool moveThroughDoor;
	BombItem * bombItem;
	Explosion explosionEffect;
	std::vector<GameObject*> playerBullets;
	//Enemy
	std::vector<GameObject*> skreeBullet;

	std::vector<GameObject*> kraidBullets;
	//GameObject *object1;
	std::vector<GameObject*> healthItems;
	
	std::vector<GameObject*> doors;
private:
	Collision *collision;
	CollisionCallback *callback;

	Texture enemiesTexture;
	Texture bossesTexture;
	Texture itemsTexture;
	Texture doorTexture;

	//Sound
	CSound * Appearance;
	CSound * Brinstar;
	CSound * KraidTheme;
	CSound * MotherBrain;
	int flagsound;

public:
	MainScene();
	~MainScene();

	std::vector<GameObject*> getSkreeBullet() { return skreeBullet; }

	void Init() override;
	void Update() override;
	void UpdateCamera();
	eSceneID Render() override;
	void DrawSquare();
	void ProcessInput();
	void End() override;
	void PlaySoundTheme();
	void RemoveObject(GameObject* object);
	DEFINE_SCENE_UID(MAINSCENE)
};