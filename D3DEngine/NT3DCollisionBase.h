#pragma once
#include "NTCollisionComponent.h"

class NTMesh;
class NT3DCollisionBase : public NTCollisionComponent
{
protected:
	Autoptr<NTMesh> DbgMesh;
public:
	bool Init(int _Order = 0);
public:
	NT3DCollisionBase();
	~NT3DCollisionBase();
};

