#include "MainScene.h"
#define ANCHOR 55
#define CHANGESCENE 1
MainScene::MainScene()
{
	Time = GameTime::getInstance();
}

MainScene::~MainScene()
{
}
void MainScene::Init()
{
#pragma region set up settings
	cam = Camera::Instance();
	//cam->setPosition(32 * 32, 32 * 89);
	//cam->setPosition(32 * (155), 32 * 145);
	cam->setPosition(32 * (86), 32 * 34);
	batch = SpriteBatch::Instance();
	batch->SetCamera(cam);
	KeyBoard = CKeyboard::getInstance();
	collision = new Collision();
	callback = new CollisionCallback();
#pragma endregion

#pragma region load quadtree
	quadTree = new QuadTree();
	quadTree->Load("Resources/mapQuadTree.xml", "Resources/metroid.tmx");
	mapLoader.AddMap("map1", "Resources/metroid.tmx", 1);
	tileMap = mapLoader.GetMap("map1");
	tileMap->SetSpaceDivisionQuadTree(quadTree);
#pragma endregion

#pragma region Initialize Samus

	samusTexture = Texture("Resources/metroidfullsheet.png");
	samus = new Samus();
	//samus->Init(&samusTexture, 32 * 40, 32 * 80);
	//samus->Init(&samusTexture, 32 * 163, 32 * 136);
	GameObject* Player = quadTree->GetObjectsGroup("Player").front();
	samus->Init(&samusTexture, Player->getPosition().x, Player->getPosition().y);
	samus->SetScene(this);

	cameraOffsetX = samus->getPosition().x - cam->getPosition().x;
	cam->setPosition(samus->getPosition().x - 32 * (8), samus->getPosition().y + 32 * 9);

	explosionEffect.Init(&samusTexture);
	explosionEffect.SetSize(32, 32);

#pragma endregion

#pragma region Initialize Platform

	std::vector<GameObject*> platforms = quadTree->GetObjectsGroup("Platform");
	for (std::vector<GameObject*>::iterator it = platforms.begin(); it != platforms.end(); ++it)
	{
		(*it)->_CategoryMask = PLATFORM;
		(*it)->_BitMask = Category::PLAYER | Category::HEAD | Category::PLAYER_BULLET | Category::PLAYER_ROCKET | Category::RIO | Category::RIPPER | Category::SKREE | Category::ZOOMER | Category::KRAID | Category::CANNON_BULLET;
	}

	std::vector<GameObject*> breakablePlatforms = quadTree->GetObjectsGroup("BreakablePlatform");
	for (std::vector<GameObject*>::iterator it = breakablePlatforms.begin(); it != breakablePlatforms.end(); ++it)
	{
		(*it)->_CategoryMask = BREAKABLE_PLATFORM;
		(*it)->_BitMask = Category::PLAYER | Category::HEAD | Category::PLAYER_BULLET | Category::PLAYER_ROCKET | Category::RIO | Category::RIPPER | Category::SKREE | Category::ZOOMER | Category::BOMB_EXPLOSION;
		((BreakablePlatform*)(*it))->SetScene(this);
		((BreakablePlatform*)(*it))->SetTilemap(tileMap);
	}

#pragma endregion

#pragma region Initialize Door
	doorTexture = Texture("Resources/spriteobjects.PNG");

	doors = quadTree->GetObjectsGroup("Door");
	for (std::vector<GameObject*>::iterator it = doors.begin(); it != doors.end(); ++it)
	{
		((Door*)(*it))->Init(&doorTexture, (*it)->getPosition().x, (*it)->getPosition().y);
		((Door*)(*it))->SetScene(this);
		((Door*)(*it))->SetCam(cam);
		((Door*)(*it))->SetPlayer(samus);
	}

	((Door*)doors[0])->leftDoor->followDirection = DOWN;
	((Door*)doors[0])->rightDoor->followDirection = LEFT;
	((Door*)doors[0])->SetChangeSoundTheme(false);

	((Door*)doors[1])->leftDoor->followDirection = RIGHT;
	((Door*)doors[1])->rightDoor->followDirection = DOWN;
	((Door*)doors[1])->SetChangeSoundTheme(false);

	((Door*)doors[2])->leftDoor->followDirection = UP;
	((Door*)doors[2])->rightDoor->followDirection = LEFT;
	((Door*)doors[2])->SetChangeSoundTheme(false);

	((Door*)doors[3])->leftDoor->followDirection = DOWN;
	((Door*)doors[3])->rightDoor->followDirection = LEFT;
	((Door*)doors[3])->SetChangeSoundTheme(true);
	((Door*)doors[3])->leftFlagSound = Section::Brinstar;
	((Door*)doors[3])->rightFlagSound = Section::KraidTheme;

	((Door*)doors[4])->InitOrangeDoor(&doorTexture, doors[4]->getPosition().x, doors[4]->getPosition().y);
	((Door*)doors[4])->leftDoor->followDirection = RIGHT;
	((Door*)doors[4])->rightDoor->followDirection = UP;
	((Door*)doors[4])->SetChangeSoundTheme(true);
	((Door*)doors[4])->leftFlagSound = Section::MotherBrainTheme;
	((Door*)doors[4])->rightFlagSound = Section::Brinstar;
#pragma endregion

#pragma region Initialize Enemy
	enemiesTexture = Texture("Resources/enemies.png");

	//Zoomer
	std::vector<GameObject*> zoomers = quadTree->GetObjectsGroup("Zoomer");
	for (std::vector<GameObject*>::iterator it = zoomers.begin(); it != zoomers.end(); ++it)
	{
		((Zoomer*)(*it))->Init(&enemiesTexture, (*it)->getPosition().x, (*it)->getPosition().y, 1);
		((Zoomer*)(*it))->SetScene(this);
	}
	//InverseZoomer
	std::vector<GameObject*> inversezoomers = quadTree->GetObjectsGroup("InverseZoomer");
	for (std::vector<GameObject*>::iterator it = inversezoomers.begin(); it != inversezoomers.end(); ++it)
	{
		((Zoomer*)(*it))->Init(&enemiesTexture, (*it)->getPosition().x, (*it)->getPosition().y, 1);
		((Zoomer*)(*it))->SetScene(this);
		((Zoomer*)(*it))->startVelocityX *= -1;
		POINT temp = (*it)->getVelocity();
		temp.x *= -1;
		(*it)->setVelocity(temp.x, temp.y);
	}
	//Skree
	std::vector<GameObject*> skrees = quadTree->GetObjectsGroup("Skree");
	for (std::vector<GameObject*>::iterator it = skrees.begin(); it != skrees.end(); ++it)
	{
		((Skree*)(*it))->Init(&enemiesTexture, (*it)->getPosition().x, (*it)->getPosition().y);
		((Skree*)(*it))->SetScene(this);
	}
	//Ripper
	std::vector<GameObject*> rippers = quadTree->GetObjectsGroup("Ripper");
	for (std::vector<GameObject*>::iterator it = rippers.begin(); it != rippers.end(); ++it)
	{
		((Ripper*)(*it))->Init(&enemiesTexture, (*it)->getPosition().x, (*it)->getPosition().y);
	}
	//Rio
	std::vector<GameObject*> rios = quadTree->GetObjectsGroup("Rio");
	for (std::vector<GameObject*>::iterator it = rios.begin(); it != rios.end(); ++it)
	{
		((Rio*)(*it))->Init(&enemiesTexture, (*it)->getPosition().x, (*it)->getPosition().y);
		((Rio*)(*it))->SetScene(this);
	}
#pragma endregion

#pragma region Initialize Boss
	bossesTexture = Texture("Resources/bosses.png");
	//Kraid
	std::vector<GameObject*> kraids = quadTree->GetObjectsGroup("Kraid");
	GameObject* kraid = kraids.front();
	((Kraid*)kraid)->SetScene(this);
	((Kraid*)kraid)->Init(&bossesTexture, kraid->getPosition().x, kraid->getPosition().y);
	((Kraid*)kraid)->SetPlayer(samus);

	//Mother Brain
	std::vector<GameObject*> motherBrains = quadTree->GetObjectsGroup("MotherBrain");
	GameObject * motherBrain = motherBrains.front();
	((MotherBrain*)motherBrain)->Init(&bossesTexture, motherBrain->getPosition().x, motherBrain->getPosition().y);
	((MotherBrain*)motherBrain)->SetScene(this);


	std::vector<GameObject*> Zeebetites = quadTree->GetObjectsGroup("Zeebetite");
	for (std::vector<GameObject*>::iterator it = Zeebetites.begin(); it != Zeebetites.end(); ++it)
	{
		((Zeebetite*)(*it))->Init(&bossesTexture, (*it)->getPosition().x, (*it)->getPosition().y);
	}

	std::vector<GameObject*> circleBullets = quadTree->GetObjectsGroup("CircleBullet");
	for (std::vector<GameObject*>::iterator it = circleBullets.begin(); it != circleBullets.end(); ++it)
	{
		((Rinka*)(*it))->Init(&enemiesTexture, (*it)->getPosition().x, (*it)->getPosition().y);
		((Rinka*)(*it))->SetScene(this);
		((Rinka*)(*it))->SetPlayer(samus);
	}

	std::vector<GameObject*> lcannons = quadTree->GetObjectsGroup("LeftCannon");
	for (std::vector<GameObject*>::iterator it = lcannons.begin(); it != lcannons.end(); ++it)
	{
		((Cannon*)(*it))->Init(&bossesTexture, Cannon::Type::Left, rand() % 4, (*it)->getPosition().x, (*it)->getPosition().y);
		((Cannon*)(*it))->SetScene(this);
	}

	std::vector<GameObject*> rcannons = quadTree->GetObjectsGroup("RightCannon");
	for (std::vector<GameObject*>::iterator it = rcannons.begin(); it != rcannons.end(); ++it)
	{
		((Cannon*)(*it))->Init(&bossesTexture, Cannon::Type::Right, rand() % 4, (*it)->getPosition().x, (*it)->getPosition().y);
		((Cannon*)(*it))->SetScene(this);
	}
#pragma endregion

#pragma region Initialize Items
	itemsTexture = Texture("Resources/items.png");

	std::vector<GameObject*> marunari = quadTree->GetObjectsGroup("Marunari");
	for (std::vector<GameObject*>::iterator it = marunari.begin(); it != marunari.end(); ++it)
	{
		((Marunari*)(*it))->Init(&itemsTexture, (*it)->getPosition().x, (*it)->getPosition().y);
		((Marunari*)(*it))->SetScene(this);
	}
	bombItem = nullptr;
	bomb = nullptr;
#pragma endregion

#pragma region Initialize UI
	ENTexture = Texture("Resources/EN.png");
	RocketTexture = Texture("Resources/rocketlabel.png");
	font = Font("Arial", 10, 30);
	playerHealthLabel = Label("30", &font, cam->getPosition().x + ANCHOR, cam->getPosition().y - ANCHOR, SCREEN_WIDTH, SCREEN_HEIGHT);
	playerRocketLabel = Label("5", &font, cam->getPosition().x + ANCHOR, cam->getPosition().y - ANCHOR - 30, SCREEN_WIDTH, SCREEN_HEIGHT);
#pragma endregion


#pragma region Load Sounds
	flagsound = Section::Brinstar;
	EatItemSound = Sound::LoadSound("Resources/Audio/ItemAcquisition.wav");
	Appearance = Sound::LoadSound("Resources/Audio/Appearance.wav");
	Brinstar = Sound::LoadSound("Resources/Audio/BrinstarTheme.wav");
	KraidTheme = Sound::LoadSound("Resources/Audio/BossKraid.wav");
	MotherBrainSound = Sound::LoadSound("Resources/Audio/MotherBrain.wav");
#pragma endregion

#pragma region CheckPoints
	checkPoints = quadTree->GetObjectsGroup("CheckPoint");
#pragma endregion





#pragma region collide velocity
	std::vector<GameObject*> vel = quadTree->GetObjectsGroup("Collide");
	GameObject* vels = vel.front();
	vels->setBitMask(Category::PLAYER);
	vels->setCategoryMask(Category::VELOCITY);
#pragma endregion

	nextScene = MAINSCENE;
	Trace::Log("Init MainScene");
}

void MainScene::Update()
{
	float dt = Time->getDeltaTime() / 1000.0f;
	samus->dt = dt;
	if (samus->IsDead())
	{
		samusDeadTime += dt;
		if (samusDeadTime > CHANGESCENE)
		{
			nextScene = GAMEOVERSCENE;
		}
	}

	quadTree->LoadObjectsInViewport(cam, true, true);

	GameObjects.clear();
	GameObjects.insert(GameObjects.begin(), samus);
	if(samus->animator.currentAnimation.name == "Roll")
		GameObjects.insert(GameObjects.begin(), samus->head);
	 
	if (apprearanceTime < APPEARANCETIME)
	{
		apprearanceTime += dt;
		Sound::Play(Appearance);
		Render();
		samus->Update(dt);
		return;
	}

	if (eatItem && eatItemTime < EATITEMTIME)
	{
		eatItemTime += dt;
		Sound::Stop(Brinstar);
		Sound::Stop(KraidTheme);
		Sound::Play(EatItemSound);
		Render();
		return;
	}
	else
	{
		eatItemTime = 0;
		eatItem = false;
	}

	UpdateCamera();
	UpdateUI();

	if (moveThroughDoor)
	{
		for (std::vector<GameObject*>::iterator it = doors.begin(); it != doors.end(); ++it)
		{
			(*it)->Update(dt);
		}
		UpdateUI();
		Render();

		//samus->Update(dt);
		return;
	}

	PlaySoundTheme();

	GameObjects.insert(GameObjects.end(), quadTree->GetObjectsInViewport().begin(), quadTree->GetObjectsInViewport().end());
	GameObjects.insert(GameObjects.end(), skreeBullet.begin(), skreeBullet.end());
	GameObjects.insert(GameObjects.end(), playerBullets.begin(), playerBullets.end());
	GameObjects.insert(GameObjects.end(), playerRockets.begin(), playerRockets.end());
	GameObjects.insert(GameObjects.end(), healthItems.begin(), healthItems.end());
	GameObjects.insert(GameObjects.end(), rocketItems.begin(), rocketItems.end());
	GameObjects.push_back(&explosionEffect);
	if (bombItem != nullptr) GameObjects.push_back(bombItem);
	if (bomb != nullptr) GameObjects.push_back(bomb);
	//for (std::vector<GameObject*>::iterator it1 = GameObjects.begin(); it1 != GameObjects.end(); it1++, i++) {
	for (int i = 0; i < GameObjects.size(); i++)
	{

		if (GameObjects.at(i)->_CategoryMask == PLATFORM) continue;
		(GameObjects.at(i))->UpdateVelocity(samus);
		POINT velocity = (GameObjects.at(i))->getVelocity();
		POINT position = (GameObjects.at(i))->getPosition();
		position.x += velocity.x * dt;
		position.y += velocity.y * dt;
		RECT boardphase = collision->GetBroadphaseRect((GameObjects.at(i)), dt);
		RECT box1 = collision->GetRECT((GameObjects.at(i)));

		bool moveX = true, moveY = true, performOverLaying = true;
		bool needMoveX = false, needMoveY = false;
		//for (std::vector<GameObject*>::iterator it2 = GameObjects.begin(); it2 != GameObjects.end(); it2++, j++) {
		for (int j = 0; j < GameObjects.size(); ++j)
		{
			if (i == j || (GameObjects.at(i)->collisionType == Static && GameObjects.at(j)->collisionType == Static)) continue;
			/*if (GameObjects[i]->getCategoryMask() == Category::HEAD)
			{
				int a = 2;
			}*/
			if (collision->CanMaskCollide((GameObjects.at(i)), (GameObjects.at(j))))
			{
				if (!GameObjects.at(i)->isTrigger)
				{


					RECT box2 = collision->GetRECT((GameObjects.at(j)));
					if (collision->IsOverlayingRect(boardphase, box2))
					{
						if (collision->Collide((GameObjects.at(i)), (GameObjects.at(j)), dt))
						{
							callback->OnCollisionEnter((GameObjects.at(i)), (GameObjects.at(j)), collision->_CollisionDirection);
							collision->PerformCollision((GameObjects.at(i)), (GameObjects.at(j)), dt, 0, moveX, moveY);


						}
						else
						{
							int touching = collision->IsTouching((GameObjects.at(i)), (GameObjects.at(j))); //If istouching, it means in the next frame, two body will not collide anymore
							if (touching == 1 && velocity.y != 0)
							{
								callback->OnCollisionExit(GameObjects.at(i), GameObjects.at(j), collision->_CollisionDirection);
								//_Listener->OnCollisionExit(body1, body2, collision._CollisionDirection);
								//Trace::Log("Exit y axis");
							}
							else if (touching == 2 && velocity.x != 0)
							{
								callback->OnCollisionExit(GameObjects.at(i), GameObjects.at(j), collision->_CollisionDirection);
								//_Listener->OnCollisionExit(body1, body2, collision._CollisionDirection);
								//Trace::Log("Exit x axis");
							}
						}
					}
					//check overlaying (sometimes two bodies are already overlaying each other 
					if (GameObjects.at(i)->collisionType != Static)
					{
						if (collision->IsOverlaying((GameObjects.at(i)), (GameObjects.at(j))))
						{
							callback->OnTriggerEnter((GameObjects.at(i)), (GameObjects.at(j)), performOverLaying);
							if (performOverLaying)
							{
								collision->PerformOverlaying((GameObjects.at(i)), (GameObjects.at(j)), moveX, moveY);
							}
							performOverLaying = true;
						}
					}
				}
				else {
					if (collision->IsOverlaying((GameObjects.at(i)), (GameObjects.at(j))))
					{
						callback->OnTriggerEnter((GameObjects.at(i)), (GameObjects.at(j)), performOverLaying);
						performOverLaying = true;
					}
				}
			}
		}
		(GameObjects.at(i))->Next(dt, moveX, moveY);

		(GameObjects.at(i))->Update(dt);
	}

	for (int i = 0; i < playerBullets.size(); ++i)
	{
		if (((Bullet*)playerBullets[i])->IsDestroyed())
		{
			std::vector<GameObject*>::iterator it = std::find(GameObjects.begin(), GameObjects.end(), playerBullets[i]);
			delete *it;
			GameObjects.erase(it);
			playerBullets.erase(playerBullets.begin() + i--);
		}
	}
	for (int i = 0; i < playerRockets.size(); ++i)
	{
		if (((Rocket*)playerRockets[i])->IsDestroyed())
		{
			std::vector<GameObject*>::iterator it = std::find(GameObjects.begin(), GameObjects.end(), playerRockets[i]);
			delete *it;
			GameObjects.erase(it);
			playerRockets.erase(playerRockets.begin() + i--);
		}
	}


}

void MainScene::UpdateCamera()
{
	if (cam->followPlayerX)
		cam->setPosition(samus->getPosition().x - SCREEN_WIDTH / 2, cam->getPosition().y);
	else if (cam->followPlayerY)
	{
		cam->setPosition(cam->getPosition().x, samus->getPosition().y + SCREEN_HEIGHT / 2 - 32);
		/*cam->setPosition(cam->getPosition().x, samus->getPosition().y + 100);
		cam->followPlayerY = false;*/
		/*if (cam->canFollowDown)
		{
			cam->setPosition(cam->getPosition().x, samus->getPosition().y + 100);
		}
		else if(cam->canFollowUp)
		{
			cam->setPosition(cam->getPosition().x, samus->getPosition().y + 100);
		}*/

	}
	else if (cam->canFollowRight)
	{
		if (abs(cam->getPosition().x - samus->getPosition().x) > SCREEN_WIDTH / 2)
			cam->followPlayerX = true;
	}
	else if (cam->canFollowLeft)
	{
		if (abs(cam->getPosition().x + SCREEN_WIDTH - samus->getPosition().x) > SCREEN_WIDTH / 2)
			cam->followPlayerX = true;
	}
	else if (cam->canFollowUp)
	{
		if (abs(cam->getPosition().y - SCREEN_HEIGHT - samus->getPosition().y) > SCREEN_HEIGHT / 2)
			//if (abs(cam->getPosition().y - samus->getPosition().y) < 100)
			cam->followPlayerY = true;
	}
	else if (cam->canFollowDown)
	{
		if (abs(cam->getPosition().y - samus->getPosition().y) > SCREEN_HEIGHT / 2)
			//if (abs(cam->getPosition().y - SCREEN_HEIGHT - samus->getPosition().y) < 100)
			cam->followPlayerY = true;
	}
}

void MainScene::UpdateUI()
{
	if (samus->getHealth() < 0)
	{
		string temp = "0";
		playerHealthLabel.SetText(temp);
	}
	else
	{
		string temp = std::to_string(samus->getHealth());
		playerHealthLabel.SetText(temp);
	}

	if (samus->getNumberRocket() < 0)
	{
		string temp = "0";
		playerRocketLabel.SetText(temp);
	}
	else
	{
		string temp = std::to_string(samus->getNumberRocket());
		playerRocketLabel.SetText(temp);
	}
	playerHealthLabel.SetPosition(cam->getPosition().x + ANCHOR, cam->getPosition().y - ANCHOR);
	playerRocketLabel.SetPosition(cam->getPosition().x + ANCHOR, cam->getPosition().y - ANCHOR - 30);
}

eSceneID MainScene::Render()
{
	batch->Begin();
	DrawSquare();
	tileMap->Render(batch);
	batch->Draw(ENTexture, cam->getPosition().x + 27, cam->getPosition().y - ANCHOR - ENTexture.GetImageSize().y / 2 - 3);
	batch->Draw(RocketTexture, cam->getPosition().x + 27, cam->getPosition().y - ANCHOR - ENTexture.GetImageSize().y / 2 - 35);
	batch->End();
	playerHealthLabel.Draw(cam);
	playerRocketLabel.Draw(cam);
	return nextScene;
}

void MainScene::DrawSquare()
{
	/*std::vector<GameObject*> objects;
	objects.insert(objects.end(), quadTree->GetObjectsInViewport().begin(), quadTree->GetObjectsInViewport().end());*/
	for (std::vector<GameObject*>::iterator it = GameObjects.begin(); it != GameObjects.end(); ++it)
	{
		switch ((*it)->_CategoryMask)
		{
		case PLAYER:
			((Samus*)(*it))->Render(batch);
			break;
		case RIPPER:
			((Ripper*)(*it))->Render(batch);
			break;
		case ZOOMER:
			((Zoomer*)(*it))->Render(batch);
			break;
		case RIO:
			((Rio*)(*it))->Render(batch);
			break;
		case SKREE:
			((Skree*)(*it))->Render(batch);
			break;
		case BOMB_EXPLOSION:
			((Bomb*)(*it))->Render(batch);
			break;
		case EXPLOSION_EFFECT:
			((Explosion*)(*it))->Render(batch);
			break;
		case MARUNARI:
			((Marunari*)(*it))->Render(batch);
			break;
		case BOMBITEM:
			((BombItem*)(*it))->Render(batch);
			break;
		case HEALTHITEM:
			((HealthItem*)(*it))->Render(batch);
			break;
		case ROCKET_ITEM:
			((RocketItem*)(*it))->Render(batch);
			break;
		case DOOR:
			((Door*)(*it))->Render(batch);
			break;
		case KRAID:
			((Kraid*)(*it))->Render(batch);
			break;
		case ZEEBETITE:
			((Zeebetite*)(*it))->Render(batch);
			break;
		case MOTHERBRAIN:
			((MotherBrain*)(*it))->Render(batch);
			break;
		case CIRCLEBULLET:
			((Rinka*)(*it))->Render(batch);
			break;
		case CANNON:
			((Cannon*)(*it))->Render(batch);
			break;
		default:
			break;
		}

		batch->DrawSquare((*it)->getPosition().x, (*it)->getPosition().y, (*it)->getSize().x, (*it)->getSize().y, D3DCOLOR_ARGB(255, 0, 128, 0));
	}
}

void MainScene::ProcessInput()
{
	samus->ProcessInput(KeyBoard);
	/*if (KeyBoard->IsKeyDown(DIK_RIGHT))
	{
		POINT pos = cam->getPosition();
		pos.x += SPEED;
		cam->setPosition(pos);
	}
	if (KeyBoard->IsKeyDown(DIK_LEFT))
	{
		POINT pos = cam->getPosition();
		pos.x -= SPEED;
		cam->setPosition(pos);
	}
	if (KeyBoard->IsKeyDown(DIK_UP))
	{
		POINT pos = cam->getPosition();
		pos.y += SPEED;
		cam->setPosition(pos);
	}
	if (KeyBoard->IsKeyDown(DIK_DOWN))
	{
		POINT pos = cam->getPosition();
		pos.y -= SPEED;
		cam->setPosition(pos);
	}*/
}

void MainScene::End()
{
}

void MainScene::PlaySoundTheme()
{
	if (samus->PlayDeadSound())
	{
		cam->followPlayerX = false;
		cam->followPlayerY = false;
		Sound::Stop(Brinstar);
		Sound::Stop(KraidTheme);
		Sound::Stop(MotherBrainSound);
		return;
	}
	switch (flagsound)
	{
	case Section::Brinstar:
		Sound::Play(Brinstar);
		Sound::Stop(KraidTheme);
		Sound::Stop(MotherBrainSound);
		break;
	case Section::KraidTheme:
		Sound::Stop(Brinstar);
		Sound::Play(KraidTheme);
		Sound::Stop(MotherBrainSound);
		break;
	case Section::MotherBrainTheme:
		Sound::Stop(Brinstar);
		Sound::Stop(KraidTheme);
		Sound::Play(MotherBrainSound);
		break;
	default:
		break;
	}
}

void MainScene::RemoveObject(GameObject * object)
{
	std::vector<GameObject*>::iterator it = std::find(GameObjects.begin(), GameObjects.end(), object);
	if (it != GameObjects.end())
	{
		GameObjects.erase(it);
	}

	if (quadTree != NULL)
	{
		// Attempt to find and return a body using provided name, else return nullptr
		std::map<unsigned int, GameObject*>::iterator it = quadTree->mapObject.find(object->id);

		if (it != quadTree->mapObject.end())
		{
			//delete it->second;
			//it->second = NULL;
			//body = NULL;
			quadTree->mapObject.erase(it);
		}
	}
}

