#pragma once
#include <NTLogic.h>
#include <NTBoneAniRenderer.h>
class MoveTest : public NTLogic
{
private:
	std::wstring BoneName;
	Autoptr<NTBoneAniRenderer> AniRenderer;

public:
	void SetAniRenderer(Autoptr<NTBoneAniRenderer> _AniRenderer)
	{
		AniRenderer = _AniRenderer;
	}

	void SetBoneName(const wchar_t* _BoneName)
	{
		BoneName = _BoneName;
	}

	bool Init() override;

	void EndUpdate() override;
public:
	MoveTest();
	~MoveTest();
};

