#pragma once
#include "NTResource.h"

class NTBlend : public NTResource
{
private:
	ID3D11BlendState* BlendState;
	D3D11_BLEND_DESC Desc;
	NTCOLOR Color;

public:
	bool Create();
	bool Create(D3D11_BLEND_DESC _Desc);
	bool Update();
public:
	NTBlend();
	~NTBlend();
};

