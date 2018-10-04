#include "PreCom.h"
#include "NTImage.h"
#include "ResourceSystem.h"


NTImage::NTImage() : CutSize(1.0f)
{
	UvVec.push_back({ 0.0f, 0.0f, 1.0f, 1.0f });
}


NTImage::~NTImage()
{
}

bool NTImage::Load(const wchar_t* _SampName)
{
	Tex = ResourceSystem<NTTexture>::Find(GetFileName());

	if (nullptr == Tex)
	{
		Tex = ResourceSystem<NTTexture>::Load(GetPathkey(), GetFullFileName());
		tassert(nullptr == Tex);
	}

	Samp = ResourceSystem<NTSampler>::Find(_SampName);

	if (nullptr == Samp)
	{
		Samp = ResourceSystem<NTSampler>::Create(_SampName);
		tassert(nullptr == Samp);
	}

	return true;
}

bool NTImage::Load(size_t _CutX, size_t _CutY, const wchar_t* _SampName)
{
	Load(_SampName);
	Cut(_CutX, _CutY);
	return true;
}

void NTImage::Cut(size_t _X, size_t _Y)
{
	if (0 >= _X)
	{
		tassert(true);
	}

	if (0 >= _Y)
	{
		tassert(true);
	}

	CutSize.x = (float)_X;
	CutSize.y = (float)_Y;

	UvVec.clear();

	NTVEC CutVec;

	CutVec.z = 1.0f / float(_X);
	CutVec.w = 1.0f / float(_Y);

	for (size_t Y = 0; Y < _Y; ++Y)
	{
		for (size_t X = 0; X < _X; ++X)
		{
			CutVec.x = CutVec.z * X;
			CutVec.y = CutVec.w * Y;
			UvVec.push_back(CutVec);
		}
	}
}

size_t NTImage::Cut(NTVEC _Vec)
{
	UvVec.push_back(_Vec);
	return UvVec.size() - 1;
}