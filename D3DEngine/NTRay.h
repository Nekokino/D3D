#pragma once
#include "NT3DCollisionBase.h"
#include "NTCamera.h"

class NTRay : public NT3DCollisionBase
{
public:
	NTRayData* Ray;
	NTCamera* Cam;
	float Dis;

public:
	virtual bool Init(int _Order = 0);

public:
	void SetCamera(NTCamera* _Camera);
	void SetDistance(float _Dis)
	{
		Dis = _Dis;
	}

public:
	void CollisionFigureUpdate() override;
	void DbgRender() override;
public:
	NTRay();
	~NTRay();
};

