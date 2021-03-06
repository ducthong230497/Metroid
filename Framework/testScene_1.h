﻿#pragma once
#include "Scene.h"
#include "CDevice.h"
#include "Texture.h"
#include "SpriteBatch.h"
#include "Camera.h"
#include "CKeyboard.h"
#include "Collision.h"
#include "CollisionCallback.h"
#include "Ripper.h"
#include "Skree.h"
#include "Rio.h"
#include "Zoomer.h"
#include "MapLoader.h"
#include "Rectangle.h"
#include "Bullet.h"
#include "Bomb.h"
#include "BreakablePlatform.h"
#include "Marunari.h"
#include "BombItem.h"
#include "Samus.h"
#include "Kraid.h"
#include "Door.h"
#include "Zeebetite.h"
#define MAXJUMPTIME 0.4f

class testScene1 : public Scene
{
private:
	LPDIRECT3DSURFACE9 surface;
	//test
	Texture * texture;
	SpriteBatch * batch;
	Camera * cam;
	GameObject * object1;
	CKeyboard * KeyBoard;
	Texture enemiesTexture;
	Ripper * ripper;
	Skree * skree;
	Rio * rio;
	Zoomer * zoomer;
	Zoomer * zoomer1;
	Zoomer * zoomer2;
	Zoomer * zoomer3;
	Bomb * bomb;
	Texture itemsTexture;
	Marunari * marunari;
	BombItem * bombItem;
	Samus* samus;
	Zeebetite * zeebetite;
	Texture bossesTexture;
	Texture doorTexture;
	Kraid * kraid;
	Collision * collision;
	CollisionCallback * callback;
	MapLoader mapLoader;
	TileMap * tileMap;
	//phần này bỏ trong player
	//shoot
	Texture metroidfullsheet, samusTexture;
	std::vector<Bullet*> Bullets;
	std::vector<GameObject*> kraidBullets;
	float lastShootTime = 0;
	//jump
	float g;
	float jumpTime;
	bool isGrounded;
	
public:
	static bool canMove;
	testScene1();
	~testScene1();

	void Init() override;
	void Update() override;
	void UpdateCamera();
	eSceneID Render() override;
	void DrawSquare();
	void ProcessInput();
	void End() override;

	DEFINE_SCENE_UID(TESTSCENE1)
};