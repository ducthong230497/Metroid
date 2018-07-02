#include "CDevice.h"

CDevice *CDevice::Instance = nullptr;

CDevice::CDevice() :
	D3D(nullptr),
	D3DDevice(nullptr),
	BackBuffer(nullptr),
	BackBufferFormat(D3DFMT_X8R8G8B8)
{
}


CDevice::~CDevice()
{
	if (D3D)
	{
		D3D->Release();
		D3D = nullptr;
	}
	if (D3DDevice)
	{
		D3DDevice->Release();
		D3DDevice = nullptr;
	}
	if (BackBuffer)
	{
		BackBuffer->Release();
		BackBuffer = nullptr;
	}
}

void CDevice::Init(const CWindow &window, UINT backbufferwidth, UINT backbufferheight)
{
	D3D = Direct3DCreate9(D3D_SDK_VERSION);
	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed = window.getIsFullScreen() ? FALSE : TRUE;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	d3dpp.BackBufferFormat = BackBufferFormat;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferHeight = BackBufferHeight = 
		backbufferheight > 0 ? backbufferheight : window.getHeight();
	d3dpp.BackBufferWidth = BackBufferWidth = 
		backbufferwidth > 0 ? backbufferwidth : window.getWidth();

	D3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		window.gethWnd(),
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&D3DDevice);

	if (D3DDevice == NULL)
	{
		Trace::Log("[ERROR] Cannot create device.");
	}

	D3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &BackBuffer);
}

LPDIRECT3D9 CDevice::getD3D() const
{
	return D3D;
}

LPDIRECT3DDEVICE9 CDevice::getD3DDevice() const
{
	return D3DDevice;
}

LPDIRECT3DSURFACE9 CDevice::getBackBuffer() const
{
	return BackBuffer;
}

CDevice * CDevice::getInstance()
{
	if (!Instance)
		Instance = new CDevice();

	return Instance;
}
