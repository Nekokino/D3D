#pragma once
#include <NTLogic.h>
#include <NTCamera.h>

class NTFreeCamera : public NTLogic
{
private:
	Autoptr<NTCamera> Camera;
	NTVEC2 LastMPoint;
	NTVEC2 Rotation;
	float RotSpeed = 1.0f;
	float MoveSpeed;
	float Boost = 100.0f;

public:
	bool Init() override;
	void MainUpdate() override;
	void DbgRender() override;
public:
	NTFreeCamera();
	~NTFreeCamera();
};

