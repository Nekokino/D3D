#include "PreCom.h"
#include "NTRenderTarget.h"

NTRenderTarget::NTRenderTarget()
{
}


NTRenderTarget::~NTRenderTarget()
{
}


bool NTRenderTarget::Create(UINT _W, UINT _H, UINT _BindFlag, DXGI_FORMAT _Format, D3D11_USAGE _Usage)
{
	Texture = new NTTexture();
	if (false == Texture->Create(_W, _H, _BindFlag, _Format, _Usage))
	{
		Texture = nullptr;
		tassert(true);
		return false;
	}

	return true;
}

bool NTRenderTarget::Create(ID3D11Texture2D * _Tex2D, UINT _BindFlag)
{
	Texture = new NTTexture();
	if (false == Texture->Create(_Tex2D, _BindFlag))
	{
		Texture = nullptr;
		tassert(true);
		return false;
	}

	return true;
}

