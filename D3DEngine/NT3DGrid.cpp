#include "PreCom.h"
#include "NT3DGrid.h"
#include "NTTransform.h"
#include "NTWindow.h"


NT3DGrid::NT3DGrid()
{
}


NT3DGrid::~NT3DGrid()
{
}

void NT3DGrid::Render(Autoptr<NTCamera> _Camera)
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

	GetNTWindow()->GetDevice().SetCBData<MatrixData>(L"MatData", MatData, NTShader::STYPE::ST_VS);

	Material->Update();
	Mesh->Update();
	Mesh->Render();
}

bool NT3DGrid::Init(int _Order)
{
	NTRenderer::Init(_Order);

	if (false == SetMaterial(L"Grid3DMat"))
	{
		return false;
	}

	if (false == SetMesh(L"Rect3DMesh"))
	{
		return false;
	}

	return true;
}