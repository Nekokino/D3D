#include "PreCom.h"
#include "NTTransform.h"
#include "NTObject.h"


NTTransform::NTTransform() : LocalScale(1.0f), bLocalScale(true), bLocalPosition(true), bLocalRotation(true), bWorld(true), ForceUpdate(false)
{
	LocalAxis[AXIS::AX_FORWARD] = NTVEC::FORWARD;
	LocalAxis[AXIS::AX_RIGHT] = NTVEC::RIGHT;
	LocalAxis[AXIS::AX_UP] = NTVEC::UP;

	WorldAxis[AXIS::AX_FORWARD] = NTVEC::FORWARD;
	WorldAxis[AXIS::AX_RIGHT] = NTVEC::RIGHT;
	WorldAxis[AXIS::AX_UP] = NTVEC::UP;
}


NTTransform::~NTTransform()
{
}

bool NTTransform::IsMulti(NTObject* _Obj)
{
	if (nullptr != _Obj->GetComponent<NTTransform>())
	{
		return false;
	}

	return true;
}

void NTTransform::FinalUpdate()
{
	if (true == bLocalScale)
	{
		ScaleMatrix.Scale(LocalScale);
		bLocalScale = false;
	}

	if (true == bLocalRotation)
	{
		RotationXMatrix.RotationX(LocalRotation.x);
		RotationYMatrix.RotationY(LocalRotation.y);
		RotationZMatrix.RotationZ(LocalRotation.z);

		RotationMatrix = RotationXMatrix * RotationYMatrix * RotationZMatrix;

		LocalAxis[AXIS::AX_FORWARD] = NTVEC::FORWARD;
		LocalAxis[AXIS::AX_RIGHT] = NTVEC::RIGHT;
		LocalAxis[AXIS::AX_UP] = NTVEC::UP;

		for (size_t i = 0; i < AX_MAX; i++)
		{
			LocalAxis[i] = RotationMatrix.MulZero(LocalAxis[i]);
			LocalAxis[i].Normalize3D();
		}

		if (nullptr != GetNTObject()->GetParent())
		{
			WorldRotation = GetNTObject()->GetTransform()->GetWorldRotation() + LocalRotation;

			NTMAT WorldRotationMat;
			NTMAT TempRotationX;
			NTMAT TempRotationY;
			NTMAT TempRotationZ;

			TempRotationX.RotationX(WorldRotation.x);
			TempRotationY.RotationY(WorldRotation.y);
			TempRotationZ.RotationZ(WorldRotation.z);

			WorldRotationMat = TempRotationX * TempRotationY * TempRotationZ;

			WorldAxis[AXIS::AX_FORWARD] = NTVEC::FORWARD;
			WorldAxis[AXIS::AX_UP] = NTVEC::UP;
			WorldAxis[AXIS::AX_RIGHT] = NTVEC::RIGHT;

			for (size_t i = 0; i < AXIS::AX_MAX; i++)
			{
				WorldAxis[i] = WorldRotationMat.MulZero(WorldAxis[i]);
				WorldAxis[i].Normalize3D();
			}
		}
		else
		{
			for (size_t i = 0; i < AXIS::AX_MAX; i++)
			{
				WorldAxis[i] = LocalAxis[i];
				WorldAxis[i].Normalize3D();
			}
		}

		bLocalRotation = false;
	}

	if (true == bLocalPosition)
	{
		PositionMatrix.Translate(LocalPosition);
		bLocalPosition = false;
	}


	if (nullptr == GetNTObject()->GetParent())
	{
		if (true == bWorld)
		{
			WorldMatrix = ScaleMatrix * RotationMatrix * PositionMatrix;

			WorldPosition = WorldMatrix.v4;
			WorldScale = NTVEC(WorldMatrix.v1.x, WorldMatrix.v2.y, WorldMatrix.v3.z);
		}
	}
	else if (nullptr != GetNTObject()->GetParent()->GetTransform())
	{
		if (true == bWorld || true == GetNTObject()->GetParent()->GetTransform()->bWorld)
		{
			NTMAT tmp = GetNTObject()->GetParent()->GetTransform()->GetWorldMatrix();
			WorldMatrix = ScaleMatrix * RotationMatrix * PositionMatrix * tmp;

			WorldPosition = WorldMatrix.v4;
			WorldScale = NTVEC(WorldMatrix.v1.x, WorldMatrix.v2.y, WorldMatrix.v3.z);
		}
	}
}

void NTTransform::EndUpdate()
{
	bWorld = false;
}

bool NTTransform::Init()
{
	GetNTObject()->SetTransform(this);
	return true;
}

void NTTransform::MainUpdate()
{
	if (ForceUpdate == true)
	{
		bWorld = true;
		ForceUpdate = false;
	}
}

void NTTransform::Reset()
{
	LocalPosition = 0.0f;
	LocalRotation = 0.0f;
	LocalScale = 0.0f;
	bLocalScale, bLocalPosition, bLocalRotation = true;

	LocalAxis[AXIS::AX_FORWARD] = NTVEC::FORWARD;
	LocalAxis[AXIS::AX_UP] = NTVEC::UP;
	LocalAxis[AXIS::AX_RIGHT] = NTVEC::RIGHT;

	WorldAxis[AXIS::AX_FORWARD] = NTVEC::FORWARD;
	WorldAxis[AXIS::AX_UP] = NTVEC::UP;
	WorldAxis[AXIS::AX_RIGHT] = NTVEC::RIGHT;
}