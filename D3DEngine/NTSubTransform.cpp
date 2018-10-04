#include "PreCom.h"
#include "NTTransform.h"
#include "NTSubTransform.h"


NTSubTransform::NTSubTransform() : Mode(SM_NONE), Scale(1.0f), Rotation(0.0f), Pivot(0.0f)
{
	SubWorldMatrix.Identify();
}


NTSubTransform::~NTSubTransform()
{
}

void NTSubTransform::SubTranformUpdate()
{
	switch (Mode)
	{
	case NTSubTransform::SM_NONE:
		SubWorldMatrix = Transform->GetWorldMatrix();
		break;
	case NTSubTransform::SM_PARENT:
		ScaleMatrix.Scale(Scale);
		XRotMat.RotationX(Rotation.x);
		YRotMat.RotationY(Rotation.y);
		ZRotMat.RotationZ(Rotation.z);
		RotationMatrix = XRotMat * YRotMat * ZRotMat;
		PivotMatrix.Translate(Pivot);
		SubWorldMatrix = ScaleMatrix * RotationMatrix * PivotMatrix * Transform->GetWorldMatrix();
		break;
	case NTSubTransform::SM_SELF:
		ScaleMatrix.Scale(Scale);
		XRotMat.RotationX(Rotation.x);
		YRotMat.RotationY(Rotation.y);
		ZRotMat.RotationZ(Rotation.z);
		RotationMatrix = XRotMat * YRotMat * ZRotMat;
		PivotMatrix.Translate(Pivot + Transform->GetWorldPosition());
		SubWorldMatrix = ScaleMatrix * RotationMatrix * PivotMatrix;
		break;
	case NTSubTransform::SM_INDEPENDENT:
		ScaleMatrix.Scale(Scale);
		XRotMat.RotationX(Rotation.x);
		YRotMat.RotationY(Rotation.y);
		ZRotMat.RotationZ(Rotation.z);
		RotationMatrix = XRotMat * YRotMat * ZRotMat;
		PivotMatrix.Translate(Pivot);
		SubWorldMatrix = ScaleMatrix * RotationMatrix * PivotMatrix;
	default:
		break;
	}
}

void NTSubTransform::InitComponent()
{
	NTStCom::InitComponent();

	tassert(nullptr == Transform);

	if (nullptr == Transform)
	{
		return;
	}
}
