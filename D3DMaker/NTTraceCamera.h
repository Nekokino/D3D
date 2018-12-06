#pragma once
#include <NTLogic.h>
#include <NTCamera.h>

class NT3DTerrainRenderer;
class NTTraceCamera : public NTLogic
{
public:
	Autoptr<NT3DTerrainRenderer> Terrain;

private:
	NTVEC Pivot;
	Autoptr<NTTransform> TraceTransform;
	Autoptr<NTCamera> Camera;
	NTVEC2 PrevMouse;
	NTVEC2 RotPos;
	float RotSpeed;
	float Speed;
	float Boost;

public:
	void SetTrace(Autoptr<NTTransform> _TraceTransform);

public:
	bool Init() override;
	void MainUpdate() override;
	void DbgRender() override;
public:
	NTTraceCamera();
	~NTTraceCamera();
};

