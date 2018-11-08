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
	Data.Dir = GetTransform()->GetWorldForward();
	Data.Pos = GetTransform()->GetWorldPosition();
	Data.Type = Type;

	wchar_t Arr[256] = {};
	swprintf_s(Arr, L"LightDir : %f %f %f", Data.Dir.x, Data.Dir.y, Data.Dir.z);
	DebugFunc::DrawLog(Arr);
	swprintf_s(Arr, L"LightPos : %f %f %f", Data.Pos.x, Data.Pos.y, Data.Pos.z);
	DebugFunc::DrawLog(Arr);
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
	LightMat = ResourceSystem<NTMaterial>::Find(L"DefferdDirLightMat");
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
		NTWinShortCut::GetMainDevice().SetDepthStencilState(L"PassStencil", 1);

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
	}

	else if (Type == LightType::Spot)
	{

	}
	else
	{
		NTWinShortCut::GetMainDevice().SetDepthStencilState(L"PassStencil", 1);
	}

	LightMat->Update();
	LightMat->TextureUpdate();
	LightMesh->Update();
	LightMesh->Render();
	LightMat->ResetTexture();
}