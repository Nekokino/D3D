#pragma once
#include "NTStCom.h"

class NTSubTransform : public NTStCom
{
public:
	enum SUBMODE
	{
		SM_NONE, // 나으 위치는 업승
		SM_PARENT, // 모두 영향
		SM_SELF, // 위치만 
		SM_INDEPENDENT, // 완전 별개
	};

private:
	SUBMODE Mode;
	NTVEC Scale;
	NTVEC Rotation;
	NTVEC Pivot;

	NTMAT ScaleMatrix;
	NTMAT RotationMatrix;
	NTMAT PivotMatrix;

	NTMAT XRotMat, YRotMat, ZRotMat;

	NTMAT SubWorldMatrix;

public:
	void SetMode(SUBMODE _Mode)
	{
		Mode = _Mode;
	}

	void SetSubScale(const NTVEC& _Scale)
	{
		Scale = _Scale;
	}

	void SetSubRotation(const NTVEC& _Rotation)
	{
		Rotation = _Rotation;
	}

	void SetSubPivot(const NTVEC& _Pivot)
	{
		Pivot = _Pivot;
	}

	void SetSubMove(const NTVEC& _Pivot)
	{
		Pivot += _Pivot;
	}

	void SetXReverse()
	{
		Scale.x *= -1.0f;
	}

	void SetYReverse()
	{
		Scale.y *= -1.0f;
	}

	NTVEC GetSubPivot()
	{
		return Pivot;
	}

	NTVEC GetSubScale()
	{
		return Scale;
	}

	NTVEC GetWorldSubPivot()
	{
		return SubWorldMatrix.v4;
	}

	const NTMAT& GetSubWorldMat() const
	{
		return SubWorldMatrix;
	}

	const NTVEC GetWorldSubScale()
	{
		NTVEC Return = { SubWorldMatrix.Matrix._11, SubWorldMatrix.Matrix._22, SubWorldMatrix.Matrix._33 };
		return Return;
	}

	void SideReverse()
	{
		Scale.x *= -1.0f;
	}

	
public:
	void SubTranformUpdate();
	void InitComponent() override;

public:
	NTSubTransform();
	~NTSubTransform();
};

