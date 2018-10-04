#pragma once
#include "DirectHeader.h"
#include "RefNAutoptr.h"
#include "WinCore.h"
#include "NTWindow.h"
#include "NTSceneSystem.h"
#include "NTDevice.h"

class NTWinShortCut
{
public:
	friend WinCore;

private:
	static Autoptr<NTWindow> MainWindow;
	static void SetMainWindow(const wchar_t* _Name);

public:
	static NTWindow& GetMainWindow();
	static NTSceneSystem& GetMainSceneSystem();

	static NTDevice& GetMainDevice();
	static ID3D11Device* GetDevice();
	static ID3D11DeviceContext* GetContext();
public:
	NTWinShortCut();
	~NTWinShortCut();
};

