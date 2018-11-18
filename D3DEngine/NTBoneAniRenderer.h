#pragma once
#include "NTRenderer.h"
#include "NTFBX.h"
#include "NTFbxData.h"

class NTBoneAniRenderer : public NTRenderer
{
private:
	Autoptr<NTFbxData> FbxMesh;

	int ClipIndex;
	int FrameCount;
	float UpdateSpeed;
	float CurTime;
	float UpdateTime;
	int StartFrame;
	int EndFrame;

public:
	void Test(const wchar_t* _Path);
	void SetFbx(const wchar_t* _Name);

public:
	std::vector<NTMAT> CurAniMatData;

	Autoptr<NTTexture> BoneTex;

public:
	void EndUpdate();

private:
	void InitMesh();

public:
	void Render(Autoptr<NTCamera> _Camera);
public:
	NTBoneAniRenderer();
	~NTBoneAniRenderer();
};

