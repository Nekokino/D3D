#include "PreCom.h"
#include "WinCore.h"
#include "NTWindow.h"
#include "TimeSystem.h"
#include "InputSystem.h"
#include "NTWinShortCut.h"
#include "ResourceSystem.h"
#include "NTCollisionComponent.h"
#include "NTSoundDevice.h"

/////// ºô´õ ////////


WinCore::WinCoreBuilder::WinCoreBuilder()
{

}

WinCore::WinCoreBuilder::~WinCoreBuilder()
{

}


//////// ÄÚ¾î /////////

WinCore::WinCore()
{

}


WinCore::~WinCore()
{

}


int WinCore::Loop()
{
	MSG msg;
	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Progress();
		}

		if (0 == NTWindow::WindowCount())
		{
			break;
		}
	}

	return 1;
}

void WinCore::Progress()
{
	TimeSystem::Update();
	InputSystem::Update();
	NTSoundDevice::Update();
	NTWindow::Progress();
}

void WinCore::InitCore(HINSTANCE _hInst, const wchar_t* _MainWindowName, HWND _hWnd)
{
	if (nullptr == _hWnd)
	{
		if (nullptr == NTWindow::NTCreateWindow(_MainWindowName))
		{
			return;
		}
	}
	else
	{
		if (nullptr == NTWindow::NTCreateWindow(_MainWindowName, _hWnd))
		{
			return;
		}
	}

	NTWinShortCut::SetMainWindow(_MainWindowName);

	NTSoundDevice::Init();
	PathSystem::Init();
	PathSystem::CreatePath(L"Shader");
	PathSystem::CreatePath(L"Texture");
	PathSystem::CreatePath(L"Data");
	PathSystem::CreatePath(L"Sound");
	TimeSystem::Init();
	NTCollisionComponent::CollsionInit();
}