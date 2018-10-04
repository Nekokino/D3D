#include "PreCom.h"
#include "NTSwitcher.h"

NTSwitcher::NTSwitcher() : bDeath(false), bUpdate(true)
{
}


NTSwitcher::~NTSwitcher()
{
}

bool NTSwitcher::Init() { return true; }
void NTSwitcher::PreUpdate() {}
void NTSwitcher::MainUpdate() {}
void NTSwitcher::AfterUpdate() {}
void NTSwitcher::DbgUpdate() {}
void NTSwitcher::DbgRender() {}