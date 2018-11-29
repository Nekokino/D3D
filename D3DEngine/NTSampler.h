#pragma once
#include "NTResource.h"



class NTSampler : public NTResource
{
private:
	D3D11_SAMPLER_DESC Desc;
	ID3D11SamplerState* State;

public:
	ID3D11SamplerState* GetState()
	{
		return State;
	}

	bool Create();

	bool Create(D3D11_FILTER _Filter, 
		D3D11_TEXTURE_ADDRESS_MODE AddressU = D3D11_TEXTURE_ADDRESS_CLAMP,
		D3D11_TEXTURE_ADDRESS_MODE AddressV = D3D11_TEXTURE_ADDRESS_CLAMP, 
		D3D11_TEXTURE_ADDRESS_MODE AddressW = D3D11_TEXTURE_ADDRESS_CLAMP, 
		FLOAT MipLodBias = 1.0f,
		UINT MaxAnisotropy = 0,
		D3D11_COMPARISON_FUNC ComparisonFunc = D3D11_COMPARISON_NEVER,
		NTCOLOR BorderColor = NTVEC::ONE,
		FLOAT MinLOD = -FLT_MAX,
		FLOAT MaxLOD = FLT_MAX);

	bool Set(D3D11_FILTER Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR,
		D3D11_TEXTURE_ADDRESS_MODE AddressU = D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_MODE AddressV = D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_MODE AddressW = D3D11_TEXTURE_ADDRESS_WRAP,
		FLOAT MipLODBias = 1.0f,
		UINT MaxAnisotropy = 0,
		D3D11_COMPARISON_FUNC ComparisonFunc = D3D11_COMPARISON_NEVER,
		NTCOLOR BorderColor = NTVEC::ONE,
		FLOAT MinLOD = -FLT_MAX,
		FLOAT MaxLOD = FLT_MAX);

public:
	void Update(unsigned int _Slot);
public:
	NTSampler();
	~NTSampler();
};

