#pragma once
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
	Collision * collision;
	CollisionCallback * callback;
	MapLoader mapLoader;
	TileMap * tileMap;

	std::vector<Bullet*> Bullets;
	int numberOfBullet;
public:
	testScene1();
	~testScene1();

	void Init() override;
	void Update() override;
	eSceneID Render() override;
	void DrawSquare();
	void ProcessInput();
	void End() override;

	void OnCollisionEnter();

	DEFINE_SCENE_UID(TESTSCENE1)
};