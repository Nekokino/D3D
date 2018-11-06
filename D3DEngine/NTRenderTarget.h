#pragma once
#include "NTTexture.h"

// 빛 연산에서의 디퍼드 렌더링은 타겟 횟수를 늘리면서 내부연산을 줄이려는데 있음.

class NTRenderTarget : public NTResource
{
private:
	Autoptr<NTTexture> Texture;

public:
	Autoptr<NTTexture> GetTexture()
	{
		return Texture;
	}

public:
	bool Create(UINT _W, UINT _H, UINT _BindFlag, DXGI_FORMAT _Format, D3D11_USAGE _Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT);
	bool Create(ID3D11Texture2D* _Tex2D, UINT _BindFlag);
public:
	NTRenderTarget();
	~NTRenderTarget();
};

