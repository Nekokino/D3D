#include "PreCom.h"
#include "NTCamera.h"
#include "NTTransform.h"
#include "NTWindow.h"
#include "NTObject.h"
#include "ResourceSystem.h"
#include "NTMesh.h"
#include "NTMaterial.h"
#include "NTWinShortCut.h"

NTCamera::NTCamera() : SMode(CSM_WINDOW), PMode(CPM_ORTHOGRAPHIC), Fov(3.141592f * 0.5f), Near(0.1f), Far(1000.0f), ScreenSize({ 1024.0f, 896.0f }), Order(0)
{
	CamMesh = ResourceSystem<NTMesh>::Find(L"Rect3DMesh");
	CamMaterial = ResourceSystem<NTMaterial>::Find(L"DefferdMergeMat");
	CamScreenMat = ResourceSystem<NTMaterial>::Find(L"ScreenMergeMat");
}


NTCamera::~NTCamera()
{
}

bool NTCamera::Init(int _Order)
{
	if(nullptr == Transform)
	{
		return false;
	}

	Order = _Order;

	Projection.Identify();
	View.Identify();
	

	GetScene()->RenderSystem.PushCamera(this);

	if (nullptr == GetScene()->GetMainCamera())
	{
		GetScene()->MainCamera = this;
	}

	CamTarget = new NTMultiRenderTarget();
	CamTarget->CreateTarget(NTWinShortCut::GetMainWindow().GetWidthU(), NTWinShortCut::GetMainWindow().GetHeightU(), D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET, DXGI_FORMAT_R32G32B32A32_FLOAT);

	return true;
}

void NTCamera::MainUpdate()
{

}

void NTCamera::FinalUpdate()
{
	View.ViewToLH(Transform->GetWorldPosition(), Transform->GetWorldForward(), Transform->GetWorldUp());

	switch (SMode)
	{
	case NTCamera::CSM_WINDOW:
		ScreenSize.x = (float)GetNTWindow()->GetWidth();
		ScreenSize.y = (float)GetNTWindow()->GetHeight();
		break;
	case NTCamera::CSM_CUSTOM:
		break;
	default:
		break;
	}

	switch (PMode)
	{
	case NTCamera::CPM_PERSPECTIVE:
		Projection.PerspectiveLH(Fov, ScreenSize.x, ScreenSize.y, Near, Far);
		break;
	case NTCamera::CPM_ORTHOGRAPHIC:
		Projection.OrthographicLH(ScreenSize.x, ScreenSize.y, Near, Far);
		break;
	default:
		break;
	}

	VP = View * Projection;
	
}

void NTCamera::EndUpdate()
{
	
}

void NTCamera::LightMerge()
{
	if (nullptr == CamMesh || nullptr == CamMaterial)
	{
		tassert(true);
		return;
	}

	CamMaterial->Update();
	CamMaterial->TextureUpdate();
	CamMesh->Update();
	CamMesh->Render();
	CamMaterial->ResetTexture();
}

void NTCamera::ScreenMerge()
{
	if (nullptr == CamMesh || nullptr == CamScreenMat)
	{
		tassert(true);
		return;
	}

	CamTarget->GetTargetTexture(0)->Update(0);
	CamScreenMat->Update();
	CamMesh->Update();
	CamMesh->Render();
	CamTarget->GetTargetTexture(0)->Reset(0);
}