#include "CoreStarter.h"
#include <InputSystem.h>
#include <NTWinShortCut.h>
#include <ResourceSystem.h>
CoreStarter::CoreStarter()
{
}


CoreStarter::~CoreStarter()
{
}

void CoreStarter::Build()
{
	NTWinShortCut::GetMainWindow().SetSize(1280, 800);
	NTWinShortCut::GetMainWindow().Show();
	NTWinShortCut::GetMainWindow().InitDevice();

	PathSystem::CreateAllPath(L"Texture");

}