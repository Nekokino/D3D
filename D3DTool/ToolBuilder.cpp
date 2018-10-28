#include "stdafx.h"
#include "ToolBuilder.h"
#include <ResourceSystem.h>
#include <NTWinShortCut.h>


ToolBuilder::ToolBuilder()
{
}


ToolBuilder::~ToolBuilder()
{
}

void ToolBuilder::Build()
{
	NTWinShortCut::GetMainWindow().InitDevice();
	PathSystem::CreateAllPath(L"Data");
}