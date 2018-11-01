#include "PreCom.h"
#include "NT3DGrid.h"
#include "NTTransform.h"
#include "NTWindow.h"
#include "DebugFunc.h"


NT3DGrid::NT3DGrid() : InterBlockSize(10.0f)
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

	CalData(_Camera);

	MatData.World = Transform->GetWorldMatrixConst();
	MatData.View = _Camera->GetView();
	MatData.Projection = _Camera->GetProjection();
	MatData.WVP = (MatData.World * MatData.View * MatData.Projection).RTranspose();

	GetNTWindow()->GetDevice().SetCBData<MatrixData>(L"MatData", MatData, NTShader::STYPE::ST_VS);
	GetNTWindow()->GetDevice().SetCBData<NTVEC>(L"GridData", GridData, NTShader::STYPE::ST_PX);

	Material->Update();
	Mesh->Update();
	Mesh->Render();
}

void NT3DGrid::DbgRender()
{
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

	if (nullptr == GetNTWindow()->GetDevice().FindConstBuffer(L"GridData"))
	{
		GetNTWindow()->GetDevice().CreateConstBuffer<NTVEC>(L"GridData", D3D11_USAGE_DYNAMIC, 0);
	}

	SetRasterState(L"SNONE");
	return true;
}

void NT3DGrid::CalData(Autoptr<NTCamera> _Camera)
{
	float y = fabsf(_Camera->GetNTObject()->GetTransform()->GetWorldPosition().y);

	float CheckUpper = InterBlockSize;
	float Check = InterBlockSize;

	while (Check <= y)
	{
		Check *= InterBlockSize;
	}

	Check *= 0.1f;

	GridData.x = Check;

	if (InterBlockSize >= GridData.x)
	{
		Check = InterBlockSize;
		GridData.x = InterBlockSize;
	}

	CheckUpper = Check * InterBlockSize;

	GridData.y = y * 0.01f;

	GridData.z = fabsf((y - CheckUpper) / (CheckUpper - Check));
}
