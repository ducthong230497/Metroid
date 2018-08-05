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
#include "MotherBrain.h"
#include "CircleBullet.h"
#include "Cannon.h"
#include "Font.h"
#include "Label.h"
#include <iostream>
#include <string>
#define SPEED 2
#define APPEARANCETIME 3
#define EATITEMTIME 2

enum Section
{
	Brinstar,
	KraidTheme,
	MotherBrainTheme
};

class MainScene : public Scene
{
private:
	float apprearanceTime;
	float eatItemTime;
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

	float samusDeadTime;

	//UI
	Texture ENTexture;
	Texture RocketTexture;
	Font font;
	Label playerHealthLabel;
	Label playerRocketLabel;

public:
	bool moveThroughDoor;
	bool eatItem;
	BombItem * bombItem;
	Explosion explosionEffect;
	Bomb *bomb;
	std::vector<GameObject*> playerBullets;
	std::vector<GameObject*> playerRockets;
	//Enemy
	std::vector<GameObject*> skreeBullet;

	std::vector<GameObject*> kraidBullets;
	//GameObject *object1;
	std::vector<GameObject*> healthItems;
	std::vector<GameObject*> rocketItems;
	
	std::vector<GameObject*> doors;

	int flagsound;
private:
	Collision *collision;
	CollisionCallback *callback;

	Texture samusTexture;
	Texture enemiesTexture;
	Texture bossesTexture;
	Texture itemsTexture;
	Texture doorTexture;

	//Sound
	CSound * EatItemSound;
	CSound * Appearance;
	CSound * Brinstar;
	CSound * KraidTheme;
	CSound * MotherBrainSound;

public:
	MainScene();
	~MainScene();

	std::vector<GameObject*> getSkreeBullet() { return skreeBullet; }
	void Init() override;
	void Update() override;
	void UpdateCamera();
	void UpdateUI();
	eSceneID Render() override;
	void DrawSquare();
	void ProcessInput();
	void End() override;
	void PlaySoundTheme();
	void RemoveObject(GameObject* object);
	DEFINE_SCENE_UID(MAINSCENE)
};