#pragma once
#include "Outer_Door.h"
#include "Camera.h"
class Door : public GameObject
{
	Scene * scene;
	Camera * cam;
	GameObject * player;
	bool moveCam;
	bool movePlayer;
public:
	OuterDoor * leftDoor;
	OuterDoor * rightDoor;
	Door();
	~Door();
	void SetScene(Scene * s);
	void SetCam(Camera * c);
	void SetPlayer(GameObject * p);
	void Init(Texture * texture, float x, float y);
	void Update(float dt) override;
	void Render(SpriteBatch * batch);
	void OnHitPlayer();
};