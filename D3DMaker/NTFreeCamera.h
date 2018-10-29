#pragma once
#include <NTLogic.h>
#include <NTCamera.h>

class NTFreeCamera : public NTLogic
{
private:
	Autoptr<NTCamera> Camera;
	NTVEC2 LastMPoint;
	NTVEC2 Rotation;
	float Speed;

public:
	bool Init() override;
	void MainUpdate() override;
	void DbgRender() override;
public:
	NTFreeCamera();
	~NTFreeCamera();
};

