#include "PreCom.h"
#include "NTBlend.h"
#include "NTWinShortCut.h"


NTBlend::NTBlend() : BlendState(nullptr), Color(0.0f)
{
}


NTBlend::~NTBlend()
{
	if (nullptr != BlendState)
	{
		BlendState->Release();
	}
}

bool NTBlend::Create()
{
	Desc.AlphaToCoverageEnable = false;
	Desc.IndependentBlendEnable = false;

	Desc.RenderTarget[0].BlendEnable = true;
	Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	Desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

	Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

	if (S_OK != NTWinShortCut::GetDevice()->CreateBlendState(&Desc, &BlendState))
	{
		tassert(true);
		return false;
	}

	return true;
}

bool NTBlend::Update()
{
	NTWinShortCut::GetContext()->OMSetBlendState(BlendState, Color.s, 0xffffffff);

	return true;
}