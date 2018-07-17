#include "testScene_2.h"

testScene2::testScene2()
{
}

testScene2::~testScene2()
{
}

void testScene2::Init()
{
	
}

void testScene2::Update()
{
}

eSceneID testScene2::Render()
{
	
#ifdef _DEBUG
	eSceneID e = getUID();
	return e;
#endif
	return getUID();
}

void testScene2::End()
{
}

