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

Autoptr<NTMaterial> NTRenderer::GetMaterial()
{
	if (true == Material->IsOriginal)
	{
		Material = Material->Clone();
	}

	return Material;
}

bool NTRenderer::SetMesh(const wchar_t* _Mesh)
{
	Mesh = ResourceSystem<NTMesh>::Find(_Mesh);
	tassert(nullptr == Mesh);

	if (nullptr == Mesh)
	{
		return false;
	}

	return true;
}

bool NTRenderer::SetMaterial(const wchar_t* _Material)
{
	Material = ResourceSystem<NTMaterial>::Find(_Material);
	tassert(nullptr == Material);

	if (nullptr == Material)
	{
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

	if (nullptr != Material)
	{
		Material->TextureUpdate();
		Material->SamplerUpdate();
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

	TransformConstBufferUpdate();
}

void NTRenderer::TransformConstBufferUpdate()
{
	if (nullptr != Material)
	{
		RndOpt.TexCount = Material->SetTextureData(RndOpt.ArrTex);
	}
	NTWinShortCut::GetMainDevice().SetConstBuffer<MatrixData>(L"MatData", MatData, STYPE::ST_VS);
	NTWinShortCut::GetMainDevice().SetConstBuffer<MatrixData>(L"MatData", MatData, STYPE::ST_PX);
	NTWinShortCut::GetMainDevice().SetConstBuffer<RenderOption>(L"RenderOption", RndOpt, STYPE::ST_VS);
	NTWinShortCut::GetMainDevice().SetConstBuffer<RenderOption>(L"RenderOption", RndOpt, STYPE::ST_PX);
}

void NTRenderer::MeshUpdate()
{
	if (nullptr != Mesh)
	{
		Mesh->Update();
		Mesh->Render();
	}
}

void NTRenderer::MaterialUpdate()
{
	if (nullptr != Material)
	{
		Material->Update();
	}
}