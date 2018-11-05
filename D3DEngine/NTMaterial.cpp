#include "PreCom.h"
#include "NTMaterial.h"
#include "NTPixelShader.h"
#include "NTVertexShader.h"
#include "ResourceSystem.h"
#include "NTSampler.h"
#include "NTTexture.h"


NTMaterial::NTMaterial() : VertexShader(nullptr), PixelShader(nullptr), IsOriginal(false)
{
}


NTMaterial::~NTMaterial()
{
}

bool NTMaterial::SetVertexShader(const wchar_t* _VertexName)
{
	VertexShader = ResourceSystem<NTVertexShader>::Find(_VertexName);
	tassert(nullptr == VertexShader);

	if (nullptr == VertexShader)
	{
		return false;
	}

	return true;
}

bool NTMaterial::SetPixelShader(const wchar_t* _PixelName)
{
	PixelShader = ResourceSystem<NTPixelShader>::Find(_PixelName);
	tassert(nullptr == VertexShader);

	if (nullptr == PixelShader)
	{
		return false;
	}

	return true;
}

bool NTMaterial::SetBlend(const wchar_t* _BlendName)
{
	Blend = ResourceSystem<NTBlend>::Find(_BlendName);
	tassert(nullptr == Blend);

	if (nullptr == Blend)
	{
		return false;
	}

	return true;
}

void NTMaterial::Update()
{
	if (nullptr != VertexShader)
	{
		VertexShader->Update();
		VertexShader->SetLayout();
	}

	if (nullptr != PixelShader)
	{
		PixelShader->Update();
	}

	if (nullptr != Blend)
	{
		Blend->Update();
	}
}

void NTMaterial::AddTextureData(UINT _TexSlot, const wchar_t * _TexName, UINT _SmpSlot, const wchar_t * _SmpName)
{
	TextureData TD;
	TD.Tex_Idx = _TexSlot;
	TexData.push_back(TD);
	SetTexture(_TexSlot, _TexName);
}

bool NTMaterial::Create()
{
	IsOriginal = true;
	return true;
}

void NTMaterial::SetTexture(unsigned int _Slot, const wchar_t * _TexName)
{
	Autoptr<NTTexture> Tex = ResourceSystem<NTTexture>::Find(_TexName);

	if (nullptr == Tex)
	{
		tassert(true);
		return;
	}

	std::unordered_map<unsigned int, Autoptr<NTTexture>>::iterator FindIter = TextureMap.find(_Slot);

	if (FindIter == TextureMap.end())
	{
		TextureMap.insert(std::unordered_map<unsigned int, Autoptr<NTTexture>>::value_type(_Slot, Tex));
	}
	else
	{
		FindIter->second = Tex;
	}
}

void NTMaterial::SetSampler(unsigned int _Slot, const wchar_t * _SmpName)
{
	Autoptr<NTSampler> Smp = ResourceSystem<NTSampler>::Find(_SmpName);

	if (nullptr == Smp)
	{
		tassert(true);
		return;
	}

	std::unordered_map<unsigned int, Autoptr<NTSampler>>::iterator FindIter = SamplerMap.find(_Slot);

	if (FindIter == SamplerMap.end())
	{
		TextureMap.insert(std::unordered_map<unsigned int, Autoptr<NTSampler>>::value_type(_Slot, Smp));
	}
	else
	{
		FindIter->second = Smp;
	}
}

unsigned int NTMaterial::SetTextureData(TextureData * _Data)
{
	for (size_t i = 0; i < TexData.size(); i++)
	{
		_Data[i] = TexData[i];
	}

	return (unsigned int)TexData.size();
}

void NTMaterial::TextureUpdate()
{
	TexStartIter = TextureMap.begin();
	TexEndIter = TextureMap.end();
	for (; TexStartIter != TexEndIter; ++TexStartIter)
	{
		TexStartIter->second->Update(TexStartIter->first);
	}
}

void NTMaterial::SamplerUpdate()
{
	SmpStartIter = SamplerMap.begin();
	SmpEndIter = SamplerMap.end();
	for (; SmpStartIter != SmpEndIter; ++SmpStartIter)
	{
		SmpStartIter->second->Update(SmpStartIter->first);
	}
}