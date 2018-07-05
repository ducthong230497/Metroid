#pragma once
#pragma once
#include "Scene.h"
#include "Texture.h"
#include "SpriteBatch.h"
#include "Sprite.h"
#include "CKeyboard.h"
#include "CWindow.h"
#include "TexturePacker.h"
class GameOverScene : public Scene
{
private:
	Camera * cam;
	SpriteBatch * batch;
	Texture gameOverSceneTexture;
	Sprite background;

	CKeyboard * KeyBoard;
	CWindow * Window;
	//thiếu sound
public:
	GameOverScene();
	~GameOverScene();

	void Init() override;
	//void Update() override;
	eSceneID Render() override;
	void ProcessInput() override;
	void End() override;

	DEFINE_SCENE_UID(MENUSCENE)
};
