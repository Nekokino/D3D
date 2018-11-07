#pragma once
#include "NTRenderTarget.h"
#include "NTRenderSystem.h"
#include "ResourceSystem.h"

class NTMultiRenderTarget : public NTResource
{
public:
	friend NTRenderSystem;

private:
	std::vector<Autoptr<NTRenderTarget>> RenderTarget;
	std::vector<ID3D11RenderTargetView*> RenderTargetView;

	bool DefaultDepth;
	Autoptr<NTTexture> DepthTexture;
	D3D11_DEPTH_STENCIL_DESC DepthState;
	ID3D11DepthStencilState* DepthStencilState;

public:
	unsigned int TargetCount()
	{
		return (unsigned int)RenderTarget.size();
	}

public:
	void OMSet();
	void Clear();
	void CreateDepth(unsigned int _W, unsigned int _H);

public:
	template<typename ...Rest>
	bool Create(Rest ..._Arg)
	{
		RenderTarget.reserve(sizeof...(_Arg));
		InsertTarget(_Arg...);
		return true;
	}

private:
	template<typename ...Rest>
	void InsertTarget(const wchar_t* _TargetName, Rest ..._Arg)
	{
		Autoptr<NTRenderTarget> Target = ResourceSystem<NTRenderTarget>::Find(_TargetName);

		if (nullptr == Target)
		{
			tassert(true);
			return;
		}

		RenderTarget.push_back(Target);
		RenderTargetView.push_back(Target->GetTexture()->GetRTV());
		InsertTarget(_Arg...);
	}

	void InsertTarget() {}

public:
	std::vector<Autoptr<NTRenderTarget>> GetTargetTextureList()
	{
		return RenderTarget;
	}

	void CreateTarget(UINT _W, UINT _H, UINT _BindFlag, DXGI_FORMAT _Format, D3D11_USAGE _Usage = D3D11_USAGE_DEFAULT);

	Autoptr<NTTexture> GetTargetTexture(unsigned int _Index);
public:
	NTMultiRenderTarget();
	~NTMultiRenderTarget();
};

