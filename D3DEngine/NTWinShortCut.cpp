#include "PreCom.h"
#include "NTWinShortCut.h"
#include "NTWindow.h"
#include "NTDevice.h"

Autoptr<NTWindow> NTWinShortCut::MainWindow = nullptr;


NTWinShortCut::NTWinShortCut()
{
}


NTWinShortCut::~NTWinShortCut()
{
}

void NTWinShortCut::SetMainWindow(const wchar_t* _Name)
{
	MainWindow = NTWindow::NTFindWindow(_Name);
	tassert(nullptr == MainWindow);
}

NTWindow& NTWinShortCut::GetMainWindow()
{
	return *MainWindow;
}

NTSceneSystem& NTWinShortCut::GetMainSceneSystem()
{
	return MainWindow->SceneSystem;
}

NTDevice& NTWinShortCut::GetMainDevice()
{
	return MainWindow->GetDevice();
}

ID3D11Device* NTWinShortCut::GetDevice()
{
	return MainWindow->GetDevice().GetDevice();
}

ID3D11DeviceContext* NTWinShortCut::GetContext()
{
	return MainWindow->GetDevice().GetContext();
}