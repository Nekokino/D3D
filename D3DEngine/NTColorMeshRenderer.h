#pragma once
#include "NTRenderer.h"
class NTColorMeshRenderer : public NTRenderer
{
private:
	NTCOLOR Color;

public:
	void SetColor(NTCOLOR _Color)
	{
		Color = _Color;
	}

	float GetAlpha()
	{
		return Color.a;
	}

	NTCOLOR GetColor()
	{
		return Color;
	}
public:
	bool Init(int _Order = 0) override;
	void Render(const NTMAT& _VP) override;
public:
	NTColorMeshRenderer();
	~NTColorMeshRenderer();
};

