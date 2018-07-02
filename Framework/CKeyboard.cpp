#include "CKeyboard.h"

CKeyboard* CKeyboard::Instance = nullptr;

CKeyboard::CKeyboard() :
	DInput(nullptr),
	Keyboard(nullptr)
{
}


CKeyboard::~CKeyboard()
{
	if (DInput)
	{
		DInput->Release();
		DInput = nullptr;
	}
	if (Keyboard)
	{
		Keyboard->Release();
		Keyboard = nullptr;
	}
}

bool CKeyboard::IsFirstKeyDown(int KeyCode)
{
	return (CurrentKeyStates[KeyCode] & 0x80) > 0 &&
		(PreviousKeyStates[KeyCode] & 0x80) <= 0;
}

bool CKeyboard::IsKeyDown(int KeyCode)
{
	return (CurrentKeyStates[KeyCode] & 0x80) > 0;
}

bool CKeyboard::IsKeyUp(int KeyCode)
{
	return (CurrentKeyStates[KeyCode] & 0x80) <= 0 &&
		(PreviousKeyStates[KeyCode] & 0x80) > 0;
}

void CKeyboard::Init(HWND hWnd, HINSTANCE hInstance)
{
	HRESULT
		hr = DirectInput8Create
		(
			hInstance,
			DIRECTINPUT_VERSION,
			IID_IDirectInput8, (VOID**)&DInput, NULL
		);

	if (hr != DI_OK)
	{
		Trace::Log("[ERROR] Unable to create DirectInput.");
		return;
	}

	hr = DInput->CreateDevice(GUID_SysKeyboard, &Keyboard, NULL);

	if (hr != DI_OK)
	{
		Trace::Log("[ERROR] Unable to create Device Keyboard.");
		return;
	}

	// Set the data format to "keyboard format" - a predefined data format 
	//
	// A data format specifies which controls on a device we
	// are interested in, and how they should be reported.
	//
	// This tells DirectInput that we will be passing an array
	// of 256 bytes to IDirectInputDevice::GetDeviceState.
	hr = Keyboard->SetDataFormat(&c_dfDIKeyboard);
	hr = Keyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	Acquire();
}

void CKeyboard::Update()
{
	// Current -> Previous
	for (int i = 0; i < 256; i++)
		PreviousKeyStates[i] = CurrentKeyStates[i];

	// Collect new key states
	Keyboard->GetDeviceState(sizeof(CurrentKeyStates), CurrentKeyStates);
}

void CKeyboard::Unacquire()
{
	HRESULT hr = Keyboard->Unacquire();
	if (hr == S_FALSE)
	{
		Trace::Log("Keyboard unacquired.");
	}
}

void CKeyboard::Acquire()
{
	HRESULT hr = Keyboard->Acquire();
	if (hr != DI_OK)
	{
		Trace::Log("[ERROR] Unable to acquire keyboard.");
	}
	else
	{
		Trace::Log("Keyboard acquired.");
	}
}

CKeyboard * CKeyboard::getInstance()
{
	if (!Instance)
		Instance = new CKeyboard();

	return Instance;
}
