#pragma once
#include "Scene.h"
#include "Texture.h"
#include "SpriteBatch.h"
#include "Sprite.h"
#include "CKeyboard.h"
#include "CWindow.h"
#include "TexturePacker.h"
class MenuScene : public Scene
{
private:
	Camera * cam;
	Texture MenuSceneTexture;
	std::vector<TextureRegion> regions;
	SpriteBatch * batch;
	Sprite background;

	CKeyboard * KeyBoard;
	CWindow * Window;
public:
	MenuScene();
	~MenuScene();

	void Init() override;
	//void Update() override;
	eSceneID Render() override;
	void ProcessInput() override;
	void End() override;

	DEFINE_SCENE_UID(MENUSCENE)
};
