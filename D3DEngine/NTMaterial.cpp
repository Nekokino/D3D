#include "PreCom.h"
#include "NTMaterial.h"
#include "NTPixelShader.h"
#include "NTVertexShader.h"
#include "ResourceSystem.h"


NTMaterial::NTMaterial()
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

bool NTMaterial::Create()
{
	return true;
}