#include "PreCom.h"
#include "NT3DCollisionBase.h"
#include "NTScene.h"

NT3DCollisionBase::NT3DCollisionBase()
{
}


NT3DCollisionBase::~NT3DCollisionBase()
{
}

bool NT3DCollisionBase::Init(int _Order)
{
	Order = _Order;
	GetScene()->_3DCollisionSystem.Push(this);
	return true;
}