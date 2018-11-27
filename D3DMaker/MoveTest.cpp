#include "MoveTest.h"


bool MoveTest::Init()
{
	return true;
}

void MoveTest::EndUpdate()
{
	if (nullptr == AniRenderer)
	{
		return;
	}

	if (BoneName == L"")
	{
		return;
	}

	NTMAT Mat = AniRenderer->GetWorldBoneMat(BoneName.c_str());

	GetTransform()->SetLocalPosition(Mat.v4);
}

MoveTest::MoveTest()
{
}


MoveTest::~MoveTest()
{
}
