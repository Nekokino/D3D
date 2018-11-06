#include "PreCom.h"
#include "NTMultiRenderTarget.h"
#include "NTWinShortCut.h"


NTMultiRenderTarget::NTMultiRenderTarget() : DepthTexture(nullptr)
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
		ID3D11DepthStencilView* OldDepth;
		NTWinShortCut::GetContext()->OMGetRenderTargets(0, nullptr, &OldDepth);
		NTWinShortCut::GetContext()->OMSetRenderTargets(TargetCount(), &RenderTargetView[0], OldDepth);
		OldDepth->Release();
	}
}

void NTMultiRenderTarget::CreateDepth(unsigned int _W, unsigned int _H)
{
}