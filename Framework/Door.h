#pragma once
#include "Outer_Door.h"
#include "Camera.h"
class Door : public GameObject
{
	Camera * cam;
	bool moveCam;
public:
	OuterDoor * leftDoor;
	OuterDoor * rightDoor;
	Door();
	~Door();
	void SetScene(Scene * s);
	void SetCam(Camera * c);
	void Init(Texture * texture, float x, float y);
	void Update(float dt) override;
	void Render(SpriteBatch * batch);
	void OnHitPlayer();
};