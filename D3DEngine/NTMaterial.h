#pragma once
#include "NTResource.h"
#include "NTVertexShader.h"
#include "NTPixelShader.h"
#include "NTBlend.h"

class NTMaterial : public NTResource
{
private:
	Autoptr<NTVertexShader> VertexShader;
	Autoptr<NTPixelShader> PixelShader;
	Autoptr<NTBlend> Blend;

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

public:
	NTMaterial();
	~NTMaterial();
};

