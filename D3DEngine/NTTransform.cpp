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
			LocalAxis[i].Normalize();
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
		}
	}
	else if (nullptr != GetNTObject()->GetParent()->GetTransform())
	{
		if (true == bWorld || true == GetNTObject()->GetParent()->GetTransform()->bWorld)
		{
			NTMAT tmp = GetNTObject()->GetParent()->GetTransform()->GetWorldMatrix();
			WorldMatrix = ScaleMatrix * RotationMatrix * PositionMatrix * tmp;
		}
	}

	WorldPosition = WorldMatrix.v4;
	WorldScale = NTVEC(WorldMatrix.v1.x, WorldMatrix.v2.y, WorldMatrix.v3.z);

	/*WorldPosition = NTVEC(WorldMatrix.m[3][0], WorldMatrix.m[3][1], WorldMatrix.m[3][2]);
	WorldScale = NTVEC(WorldMatrix.m[0][0], WorldMatrix.m[1][1], WorldMatrix.m[2][2]);

	NTMAT Temp = WorldMatrix;

	WorldAxis[AXIS::AX_FORWARD] = NTVEC::FORWARD;
	WorldAxis[AXIS::AX_RIGHT] = NTVEC::RIGHT;
	WorldAxis[AXIS::AX_UP] = NTVEC::UP;

	for (size_t i = 0; i < AX_MAX; i++)
	{
		WorldAxis[i] = Temp.MulZero(LocalAxis[i]);
		WorldAxis[i].Normalize();
	}*/
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