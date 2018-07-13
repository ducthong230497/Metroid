#pragma once
#include <vector>
#include "define_eSceneID.h"
#include "GameObject.h"
#include "GameTime.h"
#define DEFINE_SCENE_UID(uid) eSceneID getUID() const override { return uid; }
class Scene
{
protected:
	GameTime *Time;
	eSceneID nextScene;
public:
	std::vector<GameObject *> GameObjects;
	Scene();
	virtual ~Scene();

	virtual eSceneID getUID() const = 0;
	virtual void Init();
	virtual void Update();
	virtual eSceneID Render() = 0;
	virtual void ProcessInput();
	virtual void End();
};
