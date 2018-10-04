#pragma once
#include "NTResource.h"
#include "NTTexture.h"
#include "NTSampler.h"

#include <list>

class NTImage : public NTResource
{
private:
	Autoptr<NTTexture> Tex;
	Autoptr<NTSampler> Samp;
	std::vector<NTVEC> UvVec;

	NTVEC2 CutSize;

public:
	const NTVEC& GetUv(size_t _Index = 0)
	{
		return UvVec[_Index];
	}

	size_t GetCutCount()
	{
		return UvVec.size();
	}

	Autoptr<NTTexture> GetTex()
	{
		return Tex;
	}

	Autoptr<NTSampler> GetSamp()
	{
		return Samp;
	}

	NTVEC2 GetCutSize()
	{
		return CutSize;
	}

public:
	void Cut(size_t _X, size_t _Y);
	size_t Cut(NTVEC _Vec);
	
public:
	bool Load(const wchar_t*  _SampName = L"DefaultSamp");
	bool Load(size_t _CutX, size_t _CutY, const wchar_t* _SampName = L"DefaultSamp");
public:
	NTImage();
	~NTImage();
};

