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
	bool changeSoundTheme;
public:
	OuterDoor * leftDoor;
	OuterDoor * rightDoor;
	int leftFlagSound;
	int rightFlagSound;
	Door();
	~Door();
	void SetScene(Scene * s);
	void SetCam(Camera * c);
	void SetPlayer(GameObject * p);
	void SetCamDirection(FollowDirection fd);
	void SetChangeSoundTheme(bool b);
	void Init(Texture * texture, float x, float y);
	void Update(float dt) override;
	void Render(SpriteBatch * batch);
	void OnHitPlayer();
	void ChangeFlagLeftDoor();
	void ChangFlagRightDoor();
};