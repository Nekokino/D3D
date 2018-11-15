#include "NTFBX.h"

NTFBX::NTFBX()
{
}

NTFBX::~NTFBX()
{
}

void NTFBX::Load(const wchar_t * _Path)
{
	FbxManager* Manager = FbxManager::Create();

	Manager->Destroy();
}