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

void NT3DRectRenderer::Render(Autoptr<NTCamera> _Camera)
{
	tassert(nullptr == Transform);

	if (nullptr == Transform)
	{
		return;
	}

	SubTranformUpdate();

	MatData.World = Transform->GetWorldMatrixConst();
	MatData.View = _Camera->GetView();
	MatData.Projection = _Camera->GetProjection();
	MatData.WVP = (MatData.World * MatData.View * MatData.Projection).RTranspose();

	MaterialVec[0]->GetVertexShader()->SetConstBuffer<MatrixData>(L"MatData", MatData);

	MaterialVec[0]->Update();
	MeshVec[0]->Update();
	MeshVec[0]->Render();
}

bool NT3DRectRenderer::Init(int _Order)
{
	NTRenderer::Init(_Order);

	if (false == SetMaterial(L"Rect3DMat"))
	{
		return false;
	}

	if (false == SetMesh(L"Sphere"))
	{
		return false;
	}

	SetRasterState(L"SBACK");
	return true;
}