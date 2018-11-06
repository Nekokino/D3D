#pragma once
#include "NTTexture.h"

// �� ���꿡���� ���۵� �������� Ÿ�� Ƚ���� �ø��鼭 ���ο����� ���̷��µ� ����.

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

