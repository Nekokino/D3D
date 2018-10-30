#include "PreCom.h"
#include "NTSecondUVRenderer.h"
#include "ResourceSystem.h"
#include "NTWindow.h"
#include "NTWinShortCut.h"

NTSecondUVRenderer::NTSecondUVRenderer() : SecondUV(0.0f)
{
}


NTSecondUVRenderer::~NTSecondUVRenderer()
{
}

void NTSecondUVRenderer::SetImage(const wchar_t * _ImageName)
{
	Image = ResourceSystem<NTImage>::Find(_ImageName);
	SecondUV = 1.0f / GetImage()->GetCutSize().y * 0.5f;
	tassert(nullptr == Image);
}

void NTSecondUVRenderer::Render(Autoptr<NTCamera> _Camera)
{
	tassert(nullptr == Transform);
	if (nullptr == Transform)
	{
		return;
	}

	if (nullptr == Image)
	{
		return;
	}

	SubTranformUpdate();

	GetNTWindow()->GetDevice().SetCBData<NTMAT>(L"TRANS", (GetSubWorldMat() * _Camera->GetVP()).RTranspose(), NTShader::STYPE::ST_VS);
	GetNTWindow()->GetDevice().SetCBData<NTVEC>(L"MULCOLOR", Color, NTShader::STYPE::ST_PX);
	GetNTWindow()->GetDevice().SetCBData<NTVEC>(L"IMGUV", Image->GetUv(ImgIndex), NTShader::STYPE::ST_PX);
	GetNTWindow()->GetDevice().SetCBData<NTVEC>(L"SECONDUV", SecondUV, NTShader::ST_PX);

	Material->Update();
	Image->GetSamp()->Update();
	Image->GetTex()->Update();
	Mesh->Update();
	Mesh->Render();

	//NTWinShortCut::GetMainDevice().ResetContext();
}

bool NTSecondUVRenderer::Init(int _Order)
{
	NTRenderer::Init(_Order);

	if (false == SetMaterial(L"IMGMAT"))
	{
		return false;
	}

	if (false == SetMesh(L"TEXMESH"))
	{
		return false;
	}

	return true;
}

bool NTSecondUVRenderer::Init(const wchar_t * _ImgName, int _Order)
{
	SetImage(_ImgName);
	SetName(_ImgName);

	if (nullptr == Image)
	{
		return false;
	}

	if (false == Init(_Order))
	{
		return false;
	}

	return true;
}
