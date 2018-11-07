#include "PreCom.h"
#include "NTLight.h"
#include "NTScene.h"
#include "ResourceSystem.h"
#include "NTWinShortCut.h"
#include "NTMesh.h"
#include "NTMaterial.h"


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
}

void NTLight::SetLightType(LightType _Type)
{
	switch (_Type)
	{
	case NTLight::Dir:
		LightMesh = ResourceSystem<NTMesh>::Find(L"Rect3DMesh");
		LightMat = ResourceSystem<NTMaterial>::Find(L"DefferdDirLightMat");
		break;
	case NTLight::Point:
		LightMesh = ResourceSystem<NTMesh>::Find(L"Sphere");
		break;
	case NTLight::Spot:
		break;
	default:
		break;
	}
}

void NTLight::LightRender()
{
	NTWinShortCut::GetMainDevice().SetConstBuffer<LightData>(L"DefferdLightData", Data, STYPE::ST_PX);

	if (nullptr == LightMat || nullptr == LightMesh)
	{
		tassert(true);
		return;
	}

	LightMat->Update();
	LightMat->TextureUpdate();
	LightMesh->Update();
	LightMesh->Render();
	LightMat->ResetTexture();
}