#include "PreCom.h"
#include "NTLight.h"
#include "NTScene.h"

NTLight::NTLight()
{
	Data.Color.Diffuse = NTVEC(1.0f, 1.0f, 1.0f, 1.0f);
	Data.Color.Specular = NTVEC(0.2f, 0.2f, 0.2f, 1.0f);
	Data.Color.Ambient = NTVEC(0.1f, 0.1f, 0.1f, 1.0f);
}


NTLight::~NTLight()
{
}

bool NTLight::Init()
{
	GetScene()->RenderSystem.PushLight(this);

	return true;
}

void NTLight::MainUpdate()
{
	Data.Dir = GetTransform()->GetWorldForward();
	Data.Pos = GetTransform()->GetWorldPosition();
}