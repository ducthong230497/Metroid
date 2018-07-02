#include "testScene_2.h"

testScene2::testScene2()
{
}

testScene2::~testScene2()
{
}

void testScene2::Init()
{
	GameObject * object1 = new GameObject();
	object1->setPosition(200, -200);
	object1->setSize(100, 100);
	GameObjects.push_back(object1);

	int result = CDevice::getInstance()->getD3DDevice()->CreateOffscreenPlainSurface(
		object1->getSize().x,					// width 				
		object1->getSize().y,					// height
		D3DFMT_X8R8G8B8,		// format
		D3DPOOL_DEFAULT,		// where? (VRAM or RAM)
		&surface,
		NULL);
	CDevice::getInstance()->getD3DDevice()->ColorFill(surface, NULL, D3DCOLOR_XRGB(155, 255, 100));
}

void testScene2::Update()
{
}

eSceneID testScene2::Render()
{
	for (std::vector<GameObject*>::iterator it = GameObjects.begin(); it != GameObjects.end(); ++it)
	{
		// Generate a random area (within back buffer) to draw the surface onto
		RECT rect;
		/*
		rect.left = rand() % (SCREEN_WIDTH/2);
		rect.top = rand() % (SCREEN_HEIGHT/2);
		rect.right = rect.left + rand() % (SCREEN_WIDTH/2);
		rect.bottom = rect.left + rand() % (SCREEN_HEIGHT/2);
		*/

		/*x += v * dt;
		if (x + 50 > SCREEN_WIDTH || x<0) v = -v;*/

		//
		// WORLD TO VIEWPORT TRANSFORM USING MATRIX
		//
		D3DXVECTOR3 position((*it)->getPosition().x, (*it)->getPosition().y, 0);
		D3DXMATRIX mt;
		D3DXMatrixIdentity(&mt);
		mt._22 = -1.0f;
		mt._41 = -0;
		mt._42 = 0;
		D3DXVECTOR4 vp_pos;
		D3DXVec3Transform(&vp_pos, &position, &mt);

		D3DXVECTOR3 p(vp_pos.x, vp_pos.y, 0);
		D3DXVECTOR3 center((float)100 / 2, (float)100 / 2, 0);

		rect.left = p.x;
		rect.top = p.y;
		rect.right = rect.left + 50;
		rect.bottom = rect.top + 50;
		Trace::Log("x : %f, y : %f", p.x, p.y);
		CDevice::getInstance()->getD3DDevice()->StretchRect(
			surface,
			NULL,
			CDevice::getInstance()->getBackBuffer(),
			&rect,
			D3DTEXF_NONE
		);
	}
#ifdef _DEBUG
	eSceneID e = getUID();
	return e;
#endif
	return getUID();
}

void testScene2::End()
{
}

