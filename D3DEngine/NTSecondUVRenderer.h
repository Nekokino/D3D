#pragma once
#include "NTSpRenderer.h"
class NTSecondUVRenderer : public NTSpRenderer
{
protected:
	float SecondUV;

public:
	void SetSecondUV(float _Value)
	{
		if (_Value > 1.0f / GetImage()->GetCutSize().y * 0.5f)
		{
			SecondUV = 1.0f / GetImage()->GetCutSize().y * 0.5f;
		}

		SecondUV = _Value;
	}

	float GetSecondUV()
	{
		return SecondUV;
	}

	void SetImage(const wchar_t* _ImageName);

public:
	void Render(Autoptr<NTCamera> _Camera) override;

public:
	bool Init(int _Order = 0) override;
	bool Init(const wchar_t* _ImgName, int _Order = 0);
public:
	NTSecondUVRenderer();
	~NTSecondUVRenderer();
};

