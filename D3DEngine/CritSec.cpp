#include "PreCom.h"
#include "CritSec.h"

bool CritSec::bInit = false;
std::unordered_map<int, CRITICAL_SECTION*> CritSec::CSMap;
CRITICAL_SECTION CritSec::CB = CRITICAL_SECTION();

CritSec::Creator CritSec::CreatorInst = CritSec::Creator();
CritSec::Destroyer CritSec::DestroyerInst = CritSec::Destroyer();