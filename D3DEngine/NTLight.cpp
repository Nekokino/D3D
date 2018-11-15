#include "PreCom.h"
#include "NTLight.h"
#include "NTScene.h"
#include "ResourceSystem.h"
#include "NTWinShortCut.h"
#include "NTMesh.h"
#include "NTMaterial.h"
#include "DebugFunc.h"


NTLight::NTLight()
{
	Data.Color.Diffuse = NTVEC(1.0f, 1.0f, 1.0f, 1.0f);
	Data.Color.Specular = NTVEC(0.2f, 0.2f, 0.2f, 1.0f);
	Data.Color.Ambient = NTVEC(0.1f, 0.1f, 0.1f, 1.0f);
	SetLightType(LightType::Dir);
}


NTLight::~NTLight()
{
}

bool NTLight::Init()
{
	GetScene()->RenderSystem.PushLight(this);

	return true;
}

void NTLight::EndUpdate()
{
	

}

void NTLight::CalLightData(Autoptr<NTCamera> _Cam)
{
	Data.Dir = GetTransform()->GetWorldForward();
	Data.Pos = GetTransform()->GetWorldPosition();
	Data.Type = Type;
	Data.Range = GetTransform()->GetWorldScale().x;

	Data.Dir = _Cam->GetView().MulZero(Data.Dir);
	Data.Pos = _Cam->GetView().MulOne(Data.Pos);
	Data.InvDir = -Data.Dir;
}


void NTLight::SetLightType(LightType _Type)
{
	Type = _Type;

	switch (_Type)
	{
	case NTLight::Dir:
		LightMesh = ResourceSystem<NTMesh>::Find(L"Rect3DMesh");
		break;
	case NTLight::Point:
		VolumeMesh = ResourceSystem<NTMesh>::Find(L"Sphere");
		LightMesh = ResourceSystem<NTMesh>::Find(L"Rect3DMesh");
		break;
	case NTLight::Spot:
		break;
	default:
		break;
	}

	VolumeMaterial = ResourceSystem<NTMaterial>::Find(L"VolumeMat");
	LightMat = ResourceSystem<NTMaterial>::Find(L"DefferdLightMat");
}

void NTLight::LightRender(Autoptr<NTCamera> _Cam)
{
	NTWinShortCut::GetMainDevice().SetConstBuffer<LightData>(L"DefferdLightData", Data, STYPE::ST_PX);

	if (nullptr == LightMat || nullptr == LightMesh)
	{
		tassert(true);
		return;
	}

	if (Type == LightType::Point)
	{
		NTWinShortCut::GetMainDevice().SetBlendState(L"Volume");
		NTMAT Scale;
		Scale.Scale(NTVEC(Transform->GetWorldScale().x, Transform->GetWorldScale().x, Transform->GetWorldScale().x));

		NTMAT World;
		World.Identify();
		World = Scale * Transform->GetRotationMat() * Transform->GetPositionMat();

		MatData.World = World;
		MatData.View = _Cam->GetView();
		MatData.Projection = _Cam->GetProjection();
		MatData.WV = MatData.World * MatData.View;
		MatData.WVP = MatData.World * MatData.View * MatData.Projection;
		MatData.TransposeAll();

		NTWinShortCut::GetMainDevice().SetConstBuffer<MatrixData>(L"MatData", MatData, STYPE::ST_VS);
		NTWinShortCut::GetMainDevice().SetConstBuffer<MatrixData>(L"MatData", MatData, STYPE::ST_PX);

		NTWinShortCut::GetMainDevice().SetRasterState(L"SFRONT");
		NTWinShortCut::GetMainDevice().SetDepthStencilState(L"BackStencil", 1);

		VolumeMaterial->Update();
		VolumeMesh->Update();
		VolumeMesh->Render();

		NTWinShortCut::GetMainDevice().SetRasterState(L"SBACK");
		NTWinShortCut::GetMainDevice().SetDepthStencilState(L"FrontStencil", 1);

		VolumeMaterial->Update();
		VolumeMesh->Update();
		VolumeMesh->Render();

		NTWinShortCut::GetMainDevice().SetDepthStencilState(L"PassStencil", 1);
	}

	else if (Type == LightType::Spot)
	{

	}
	else
	{
		NTWinShortCut::GetMainDevice().SetDepthStencilState(L"PassStencil", 0);
	}

	NTWinShortCut::GetMainDevice().SetBlendState(L"LightOne");
	LightMat->Update();
	LightMat->TextureUpdate();
	LightMesh->Update();
	LightMesh->Render();
	LightMat->ResetTexture();
}