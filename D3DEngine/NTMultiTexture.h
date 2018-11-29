#pragma once
#include "ResourceSystem.h"
#include "NTTexture.h"

class NTMultiTexture : public NTResource
{
private:
	std::vector<Autoptr<NTTexture>> TexVec;

	std::vector<ID3D11Texture2D*> Tex2DVec;

	D3D11_USAGE Usage;
	DirectX::ScratchImage Image;
	ID3D11Texture2D* MultiTexture;

	ID3D11ShaderResourceView* SRV;

private:
	void CreateMultiTexture(D3D11_USAGE _Usage);

public:
	bool Create();

public:
	template<typename... Rest>
	void CreateMultiTexture(D3D11_USAGE _Usage, Rest... _Arg)
	{
		Usage = _Usage;
		TexVec.reserve(sizeof...(_Arg));
		PushTex(_Arg...);
	}

private:
	template<typename... Rest>
	void PushTex(const wchar_t* _TexName, Rest... _Arg)
	{
		Autoptr<NTTexture> Tex = ResourceSystem<NTTexture>::Find(_TexName);

		if (nullptr == Tex)
		{
			tassert(true);
			return;
		}

		TexVec.push_back(Tex);

		PushTex(_Arg...);
	}

	void PushTex()
	{
		CreateMultiTexture(Usage);
	}

public:
	void Update(unsigned int _Slot);
	void Reset(unsigned int _Slot);
public:
	NTMultiTexture();
	~NTMultiTexture();
};

