#pragma once
#include "NTRenderer.h"
#include "NTFBX.h"

class NTBoneAniRenderer : public NTRenderer
{
private:
	NTFbxLoader* Loader;

	int ClipIndex;
	int FrameCount;
	float UpdateSpeed;
	float CurTime;
	float UpdateTime;
	int StartFrame;
	int EndFrame;

public:
	void Test(const wchar_t* _Path);

public:
	std::vector<NTMAT> CurAniMatData;

	Autoptr<NTTexture> BoneTex;

public:
	void EndUpdate();

public:
	void Render(Autoptr<NTCamera> _Camera);
public:
	NTBoneAniRenderer();
	~NTBoneAniRenderer();
};

