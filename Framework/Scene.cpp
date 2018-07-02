#include "Scene.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::Init()
{
}

void Scene::Update()
{
}

void Scene::ProcessInput()
{
}

void Scene::End()
{
	/*for (auto obj : *GameObject::getGameObjects())
	{
		if (obj->getUID() == ALADDIN)
		{
			obj = nullptr;
			continue;
		}
		SAFE_DELETE(obj);
	}
	GameObject::getGameObjects()->clear();

	for (auto obj : *GameObject::getDeletedGameObjects())
	{
		SAFE_DELETE(obj);
	}
	GameObject::getDeletedGameObjects()->clear();*/
}
