#include "PreCom.h"
#include "NTRay.h"
#include "NTWinShortCut.h"
#include "DebugFunc.h"
#include "InputSystem.h"
#include "DirectValueHeader.h"
#include "NTMesh.h"

NTRay::NTRay() : Dis(1000.0f)
{
	Ray = CreateCollisionFigure<NTRayData>(COLTYPE::CT_3D_RAY);
}

NTRay::~NTRay()
{
	if (nullptr != Ray)
	{
		delete Ray;
	}
}

void NTRay::SetCamera(NTCamera * _Camera)
{
	Cam = _Camera;
}

bool NTRay::Init(int _Order)
{
	NT3DCollisionBase::Init(_Order);

	Cam = GetNTObject()->GetComponent<NTCamera>();

	return true;
}

void NTRay::CollisionFigureUpdate()
{
	if (nullptr == Cam)
	{
		return;
	}

	Ray->Origin = Cam->GetTransform()->GetWorldPosition();

	D3D11_VIEWPORT VData = {};
	UINT ViewIdx = 1;
	NTWinShortCut::GetContext()->RSGetViewports(&ViewIdx, &VData);
	NTVEC2 MousePos = InputSystem::GetMousePos();

	Ray->Direction.x = ((((MousePos.x - VData.TopLeftX) * 2.0f) / VData.Width) - 1.0f);
	Ray->Direction.x -= Cam->GetProjection().Matrix._31;
	Ray->Direction.x /= Cam->GetProjection().Matrix._11;

	Ray->Direction.y = ((((MousePos.y - VData.TopLeftY) * 2.0f) / VData.Height) - 1.0f);
	Ray->Direction.y -= Cam->GetProjection().Matrix._32;
	Ray->Direction.y /= Cam->GetProjection().Matrix._22;
	
	Ray->Direction.z = VData.MaxDepth;

	NTMAT ViewMat = Cam->GetView();
	NTMAT InvViewMat = ViewMat.RVInverse();

	Ray->Direction = InvViewMat.MulZero(Ray->Direction);
	Ray->Direction.Normalize3D();
	Ray->Dis = Dis;
}

void NTRay::DbgRender()
{
//	NTVEC ArrPos[2] = {};
//	DWORD Idx[2] = { 0, 1 };
//
//	ArrPos[0] = Ray->Origin;
//	ArrPos[1] = Ray->Direction * Ray->Dis;
////	ArrPos[1].z *= -1.0f;
//	ArrPos[1] += Ray->Origin;
//
//	ArrPos[0].w = 0.0f;
//	ArrPos[1].w = 0.0f;
//
//	ArrPos[0] = Cam->GetProjection().MulZero(ArrPos[0]);
//	ArrPos[1] = Cam->GetProjection().MulZero(ArrPos[1]);
//
//
//
//
//	Autoptr<NTMesh> Mesh = new NTMesh();
//
//	Mesh->Create(2, (UINT)sizeof(NTVEC), D3D11_USAGE_DYNAMIC, ArrPos, 2, (UINT)IDX32::MemberSize(), D3D11_USAGE_DEFAULT, Idx, IDX32::GetFormat(), D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
//
//	DebugFunc::DrawDbgMesh(Mesh);
}