#pragma once
#include "define.h"
#include "CWindow.h"

class CDevice
{
private:
	LPDIRECT3D9 D3D;
	LPDIRECT3DDEVICE9 D3DDevice;
	LPDIRECT3DSURFACE9 BackBuffer;
	D3DFORMAT BackBufferFormat;

	CDevice();
	static CDevice *Instance;
public:
	UINT BackBufferWidth,
		BackBufferHeight;

	~CDevice();

	LPDIRECT3D9 getD3D() const;
	LPDIRECT3DDEVICE9 getD3DDevice() const;
	LPDIRECT3DSURFACE9 getBackBuffer() const;
	void Init(const CWindow &window, UINT backbufferwidth = 0, UINT backbufferheight = 0);

	static CDevice* getInstance();
};

