#include "PreCom.h"
#include "NTColorMeshRenderer.h"
#include "NTWindow.h"

NTColorMeshRenderer::NTColorMeshRenderer() : Color({ 1.0f, 1.0f, 1.0f, 0.0f })
{
}


NTColorMeshRenderer::~NTColorMeshRenderer()
{
}

bool NTColorMeshRenderer::Init(int _Order)
{
	NTRenderer::Init(_Order);

	if (false == SetMaterial(L"COLMAT"))
	{
		return false;
	}

	if (false == SetMesh(L"COLORMESH"))
	{
		return false;
	}

	return true;
}

void NTColorMeshRenderer::Render(const NTMAT & _VP)
{
	SubTranformUpdate();

	NTMAT aa = GetSubWorldMat();
	GetNTWindow()->GetDevice().SetData<NTMAT>(L"TRANS", (GetSubWorldMat() * _VP).RTranspose(), NTShader::STYPE::ST_VS);
	GetNTWindow()->GetDevice().SetData<NTVEC>(L"MULCOLOR", Color, NTShader::STYPE::ST_PX);

	Material->Update();
	Mesh->Update();
	Mesh->Render();
}
