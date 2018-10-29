#include "PreCom.h"
#include "NTCamera.h"
#include "NTTransform.h"
#include "NTWindow.h"
#include "NTObject.h"



NTCamera::NTCamera() : SMode(CSM_WINDOW), PMode(CPM_ORTHOGRAPHIC), Fov(3.141592f * 0.5f), Near(0.1f), Far(1000.0f), ScreenSize({ 1024.0f, 896.0f })
{
}


NTCamera::~NTCamera()
{
}

bool NTCamera::Init()
{
	if(nullptr == Transform)
	{
		return false;
	}

	Projection.Identify();
	View.Identify();
	

	GetScene()->RenderSystem.PushCamera(this);

	if (nullptr == GetScene()->GetMainCamera())
	{
		GetScene()->MainCamera = this;
	}

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