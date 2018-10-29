#include "PreCom.h"
#include "NT3DRectRenderer.h"
#include "NTWindow.h"
#include "NTTransform.h"


NT3DRectRenderer::NT3DRectRenderer()
{
}


NT3DRectRenderer::~NT3DRectRenderer()
{
}

void NT3DRectRenderer::Render(const NTMAT & _VP)
{
	tassert(nullptr == Transform);

	if (nullptr == Transform)
	{
		return;
	}

	SubTranformUpdate();

	MatData.World = Transform->GetWorldMatrixConst();
	MatData.WVP = (Transform->GetWorldMatrixConst() * _VP).RTranspose();

	GetNTWindow()->GetDevice().SetCBData<NTMAT>(L"TRANS", MatData.WVP, NTShader::STYPE::ST_VS);

	Material->Update();
	Mesh->Update();
	Mesh->Render();
}

bool NT3DRectRenderer::Init(int _Order)
{
	NTRenderer::Init(_Order);

	if (false == SetMaterial(L"Rect3DMat"))
	{
		return false;
	}

	if (false == SetMesh(L"Rect3DMesh"))
	{
		return false;
	}

	return true;
}