#include "PreCom.h"
#include "NTRenderTarget.h"
#include "NTWinShortCut.h"

NTRenderTarget::NTRenderTarget() : Color(NTVEC{ 0.768f, 0.866f, 0.945f, 1.0f })
{
}


NTRenderTarget::~NTRenderTarget()
{
}


bool NTRenderTarget::Create(UINT _W, UINT _H, UINT _BindFlag, DXGI_FORMAT _Format, D3D11_USAGE _Usage)
{
	return Create(_W, _H, NTVEC{ 0.768f, 0.866f, 0.945f, 1.0f }, _BindFlag, _Format, _Usage);
}

bool NTRenderTarget::Create(UINT _W, UINT _H, NTVEC _Color, UINT _BindFlag, DXGI_FORMAT _Format, D3D11_USAGE _Usage)
{

	Texture = new NTTexture();
	if (false == Texture->Create(_W, _H, _BindFlag, _Format, _Usage))
	{
		Texture = nullptr;
		tassert(true);
		return false;
	}

	Color = _Color;

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

void NTRenderTarget::Clear()
{
	if (nullptr == Texture || nullptr == Texture->GetRTV())
	{
		tassert(true);
		return;
	}

	NTWinShortCut::GetContext()->ClearRenderTargetView(Texture->GetRTV(), Color.s);
}

