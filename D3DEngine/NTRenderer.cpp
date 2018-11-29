#include "PreCom.h"
#include "NTRenderer.h"
#include "ResourceSystem.h"
#include "NTScene.h"
#include "NTDevice.h"
#include "NTWindow.h"
#include "NTWinShortCut.h"
#include "NTMaterial.h"

NTRenderer::NTRenderer() : Light(0)
{
	if (false == SetMaterial(L"DefaultMat"))
	{
		tassert(true);
	}
}


NTRenderer::~NTRenderer()
{
}

bool NTRenderer::Init(int _Order)
{
	Order = _Order;
	GetScene()->RenderSystem.PushRenderer(this);
	return true;
}

Autoptr<NTMaterial> NTRenderer::GetMaterial(int _Index)
{
	if (MaterialVec[_Index]->IsOriginal)
	{
		MaterialVec[_Index] = MaterialVec[_Index]->Clone();
	}

	return MaterialVec[_Index];
}

void NTRenderer::AddDrawData(UINT _Mesh, UINT _Vtx, UINT _Sub, UINT _Mat)
{
	DrawDataVec.push_back({ _Mesh, _Vtx, _Sub, _Mat });
}

bool NTRenderer::SetMesh(const wchar_t* _Mesh, size_t _Index)
{
	if (MeshVec.size() <= _Index)
	{
		MeshVec.resize(_Index + 1);
	}

	MeshVec[_Index] = ResourceSystem<NTMesh>::Find(_Mesh);

	if (nullptr == MeshVec[_Index])
	{
		tassert(true);
		return false;
	}

	return true;
}

bool NTRenderer::SetMaterial(const wchar_t* _Material, size_t _Index)
{
	if (MaterialVec.size() <= _Index)
	{
		MaterialVec.resize(_Index + 1);
	}

	MaterialVec[_Index] = ResourceSystem<NTMaterial>::Find(_Material);

	if (nullptr == MaterialVec[_Index])
	{
		tassert(true);
		return false;
	}

	return true;
}

bool NTRenderer::SetMesh(Autoptr<NTMesh> _Mesh, size_t _Index)
{
	if (nullptr == _Mesh)
	{
		tassert(true);
		return false;
	}

	if (MeshVec.size() <= _Index)
	{
		MeshVec.resize(_Index + 1);
	}

	MeshVec[_Index] = _Mesh;

	if (nullptr == MeshVec[_Index])
	{
		tassert(true);
		return false;
	}

	return true;
}

void NTRenderer::SetRasterState(const wchar_t * _Name)
{
	RasterState = GetNTWindow()->GetDevice().FindRasterState(_Name);

	if (nullptr == RasterState)
	{
		tassert(true);
		return;
	}
}

void NTRenderer::RenderUpdate()
{
	if (nullptr != RasterState)
	{
		RasterState->Update();
	}
}

void NTRenderer::RenderAfterUpdate()
{
	GetNTWindow()->GetDevice().ResetRasterState();
}

void NTRenderer::TransformUpdate(Autoptr<NTCamera> _Cam)
{
	if (nullptr == Transform)
	{
		tassert(true);
		return;
	}
	MatData.World = Transform->GetWorldMatrixConst().RVTranspose();
	MatData.View = _Cam->GetView().RVTranspose();
	MatData.Projection = _Cam->GetProjection().RVTranspose();
	MatData.WV = (Transform->GetWorldMatrixConst() * _Cam->GetView()).RVTranspose();
	MatData.WVP = (Transform->GetWorldMatrixConst() * _Cam->GetView() * _Cam->GetProjection()).RVTranspose();
}

void NTRenderer::TransformConstBufferUpdate()
{
	NTWinShortCut::GetMainDevice().SetConstBuffer<MatrixData>(L"MatData", MatData, STYPE::ST_VS);
	NTWinShortCut::GetMainDevice().SetConstBuffer<MatrixData>(L"MatData", MatData, STYPE::ST_PX);
}

void NTRenderer::MaterialConstBufferUpdate(int _Index)
{
	if (nullptr != MaterialVec[_Index])
	{
		RndOpt.TexCount = MaterialVec[_Index]->SetTextureData(RndOpt.ArrTex);
	}

	NTWinShortCut::GetMainDevice().SetConstBuffer<RenderOption>(L"RenderOption", RndOpt, STYPE::ST_VS);
	NTWinShortCut::GetMainDevice().SetConstBuffer<RenderOption>(L"RenderOption", RndOpt, STYPE::ST_PX);
}

void NTRenderer::MaterialTextureNSamplerUpdate(int _Index)
{
	if (nullptr != MaterialVec[_Index])
	{
		MaterialVec[_Index]->TextureUpdate();
		MaterialVec[_Index]->SamplerUpdate();
	}
	else
	{
		tassert(true);
		return;
	}
}

void NTRenderer::TargetMeshUpdate(UINT _Mesh, UINT _Vtx, UINT _Idx)
{
	if (nullptr != MeshVec[_Mesh])
	{
		MeshVec[_Mesh]->UpdateAndRender(_Vtx, _Idx);
	}
	else
	{
		tassert(true);
		return;
	}
}

void NTRenderer::MeshUpdate(int _Index)
{
	if (nullptr != MeshVec[_Index])
	{
		MeshVec[_Index]->Update();
		MeshVec[_Index]->Render();
	}
	else
	{
		tassert(true);
		return;
	}
}

void NTRenderer::MaterialUpdate(int _Index)
{
	if (nullptr != MaterialVec[_Index])
	{
		MaterialVec[_Index]->Update();
	}
	else
	{
		tassert(true);
		return;
	}
}