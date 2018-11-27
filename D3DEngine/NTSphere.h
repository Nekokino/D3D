#pragma once
#include "NT3DCollisionBase.h"

class NTSphere : public NT3DCollisionBase
{
public:
	NTSphereData* Sphere;
	
public:
	void CollisionFigureUpdate() override;
	void DbgRender() override;
public:
	NTSphere();
	~NTSphere();
};

