#pragma once
#include "NTResource.h"
#include "NTVertexShader.h"
#include "NTPixelShader.h"
#include "NTBlend.h"

enum TEXTYPE
{
	TT_COLOR,
	TT_BUMP,
	TT_TARGET,
};

class TextureData
{
public:
	int Type;
	int Tex_Idx;
	int Tex_Smp;
	int Dummy;

public:
	TextureData() : Tex_Idx(-1), Tex_Smp(-1), Type(-1)
	{

	}
};

class NTTexture;
class NTSampler;
class NTMaterial : public NTResource
{
public:
	friend class NTRenderer;
	friend class NTLight;
	friend class NTCamera;
private:
	Autoptr<NTVertexShader> VertexShader;
	Autoptr<NTPixelShader> PixelShader;
	Autoptr<NTBlend> Blend;

	bool IsOriginal;

public:
	Autoptr<NTVertexShader> GetVertexShader()
	{
		return VertexShader;
	}

	Autoptr<NTPixelShader> GetPixelShader()
	{
		return PixelShader;
	}

public:
	bool SetVertexShader(const wchar_t* _VertexName);
	bool SetPixelShader(const wchar_t* _PixelName);
	bool SetBlend(const wchar_t* _BlendName);

public:
	bool Create();
	void Update();

private:
	std::vector<TextureData> TexData;

public:
	void AddTextureData(TEXTYPE _Type, UINT _TexSlot, const wchar_t* _TexName, UINT _SmpSlot = 0, const wchar_t* _SmpName = L"DefaultSampler");

private:
	std::unordered_map<unsigned int, Autoptr<NTTexture>> TextureMap;
	std::unordered_map<unsigned int, Autoptr<NTTexture>>::iterator TexStartIter;
	std::unordered_map<unsigned int, Autoptr<NTTexture>>::iterator TexEndIter;
	
	std::unordered_map<unsigned int, Autoptr<NTSampler>> SamplerMap;
	std::unordered_map<unsigned int, Autoptr<NTSampler>>::iterator SmpStartIter;
	std::unordered_map<unsigned int, Autoptr<NTSampler>>::iterator SmpEndIter;

public:
	void SetTexture(unsigned int _Slot, const wchar_t* _TexName);
	void SetTargetTexture(unsigned int _Slot, const wchar_t* _TexName);
	void SetSampler(unsigned int _Slot, const wchar_t* _SmpName);
	unsigned int SetTextureData(TextureData* _Data);

private:
	void TextureUpdate();
	void ResetTexture();
	void SamplerUpdate();

public:
	Autoptr<NTMaterial> Clone();

private:
	NTMaterial(const NTMaterial& _Other);

public:
	NTMaterial();
	~NTMaterial();
};

