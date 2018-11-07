#include "PreCom.h"
#include "NTMultiRenderTarget.h"
#include "NTWinShortCut.h"


NTMultiRenderTarget::NTMultiRenderTarget() : DepthTexture(nullptr), DepthStencilState(nullptr), DefaultDepth(false)
{
}


NTMultiRenderTarget::~NTMultiRenderTarget()
{
	if (nullptr != DepthStencilState)
	{
		DepthStencilState->Release();
	}
}

void NTMultiRenderTarget::OMSet()
{
	if (nullptr != DepthTexture)
	{
		NTWinShortCut::GetContext()->OMSetRenderTargets(TargetCount(), &RenderTargetView[0], DepthTexture->GetDSV());
		NTWinShortCut::GetContext()->OMSetDepthStencilState(DepthStencilState, 1);
	}
	else
	{
		if (false == DefaultDepth)
		{
			ID3D11DepthStencilView* OldDepth;
			NTWinShortCut::GetContext()->OMGetRenderTargets(0, nullptr, &OldDepth);
			NTWinShortCut::GetContext()->OMSetRenderTargets(TargetCount(), &RenderTargetView[0], OldDepth);
			OldDepth->Release();
		}
		else
		{
			NTWinShortCut::GetContext()->OMSetRenderTargets(TargetCount(), &RenderTargetView[0], NTWinShortCut::GetMainDevice().GetDepth());
		}
		
	}
}

void NTMultiRenderTarget::Clear()
{
	for (size_t i = 0; i < RenderTarget.size(); i++)
	{
		RenderTarget[i]->Clear();
	}

	if (nullptr != DepthTexture)
	{
		NTWinShortCut::GetContext()->ClearDepthStencilView(DepthTexture->GetDSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	}
}

void NTMultiRenderTarget::CreateDepth(unsigned int _W, unsigned int _H)
{
	DepthTexture = new NTTexture();

	if (false == DepthTexture->Create(_W, _H, D3D11_BIND_DEPTH_STENCIL, DXGI_FORMAT_D24_UNORM_S8_UINT))
	{
		tassert(true);
		return;
	}

	DepthState.DepthEnable = TRUE;
	DepthState.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DepthState.DepthFunc = D3D11_COMPARISON_LESS;
	DepthState.StencilEnable = FALSE;
	DepthState.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	DepthState.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	const D3D11_DEPTH_STENCILOP_DESC DefaultStencilOp = { D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS };

	DepthState.FrontFace = DefaultStencilOp;
	DepthState.BackFace = DefaultStencilOp;

	NTWinShortCut::GetDevice()->CreateDepthStencilState(&DepthState, &DepthStencilState);

	if (nullptr == DepthStencilState)
	{
		tassert(true);
		return;
	}

	return;
}


void NTMultiRenderTarget::CreateTarget(UINT _W, UINT _H, UINT _BindFlag, DXGI_FORMAT _Format, D3D11_USAGE _Usage)
{
	Autoptr<NTRenderTarget> Target = new NTRenderTarget();

	if (false == Target->Create(_W, _H, _BindFlag, _Format, _Usage))
	{
		tassert(true);
		return;
	}

	RenderTargetView.push_back(Target->GetTexture()->GetRTV());
	RenderTarget.push_back(Target);

}

Autoptr<NTTexture> NTMultiRenderTarget::GetTargetTexture(unsigned int _Index)
{
	return RenderTarget[_Index]->GetTexture();
}
