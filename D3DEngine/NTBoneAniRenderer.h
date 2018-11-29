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
	std::vector<NTMAT> CurAniBoneData; // 애니메이션 만들기 전 본행렬
	std::vector<NTMAT> CurAniMatData; // 애니메이션 적용을 위한 행렬

	Autoptr<NTTexture> BoneTex;

public:
	NTMAT GetBoneMat(const wchar_t* _Name);
	NTMAT GetWorldBoneMat(const wchar_t* _Name);

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

