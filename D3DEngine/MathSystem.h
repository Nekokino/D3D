#pragma once
#include "DirectHeader.h"

class MathSystem
{
public:
	static const float D2R;
	static const float R2D;

	static float GetDistance(const NTVEC& _Left, const NTVEC& _Right);
	static float GetDistance2D(const NTVEC& _Left, const NTVEC& _Right);
	static float GetLength(const NTVEC& _Value);
	static float GetLength2D(const NTVEC& _Value);
	static float GetRadian(const NTVEC& _Left, const NTVEC& _Right);
	static float GetRadian2D(const NTVEC& _Left, const NTVEC& _Right);
	static float GetDegree(const NTVEC& _Left, const NTVEC& _Right);
	static float GetDegree2D(const NTVEC& _Left, const NTVEC& _Right);
	// 진자운동 원점, 추의 초기 위치 (앞의 두 값이 변하면 운동 자체도 변해서 상수값 넣어주는게 좋음), 중력상수, 후보정 각도, 누적시간
	// 이 진자운동의 주기는 2파이 * 루트 (중력상수 / 진자길이)
	static NTVEC Pendulum(const NTVEC& _OriginPoint, const NTVEC& _PenPoint, const float _Gravity, const float _Degree, const float _Time);

	static float GetFABS(const float _Value)
	{
		if (_Value < 0)
			return _Value * -1.0f;
		else
			return _Value;
	}

	static bool R2RCol(const NTRECT& _Left, const NTRECT& _Right)
	{
		if (_Left.Left() > _Right.Right())
		{
			return false;
		}

		if (_Left.Right() < _Right.Left())
		{
			return false;
		}

		if (_Left.Top() < _Right.Bottom())
		{
			return false;
		}

		if (_Left.Bottom() > _Right.Top())
		{
			return false;
		}

		return true;
	}

	static bool R2PCol(const NTRECT& _Left, const NTPOINT& _Right)
	{
		if (_Left.Left() > _Right.x)
		{
			return false;
		}

		if (_Left.Right() < _Right.x)
		{
			return false;
		}

		if (_Left.Top() < _Right.y)
		{
			return false;
		}

		if (_Left.Bottom() > _Right.y)
		{
			return false;
		}

		return true;
	}

	static bool C2CCol(const NTCIRCLE& _Left, const NTCIRCLE& _Right)
	{
		if (_Left.Radius + _Right.Radius > (_Left.Pos - _Right.Pos).GetLength())
		{
			return true;
		}

		return false;
	}

	static bool C2PCol(const NTCIRCLE& _Left, const NTPOINT& _Right)
	{
		if (_Left.Radius > (_Left.Pos - _Right).GetLength())
		{
			return true;
		}

		return false;
	}

	static NTRECT C2RHRECT;
	static NTRECT C2RWRECT;
	static NTCIRCLE C2RCIRCLE;
	static bool C2RCol(const NTCIRCLE& _Left, const NTRECT& _Right)
	{
		C2RHRECT = _Right;
		C2RHRECT.Size.y += _Left.Diameter();
		C2RWRECT = _Right;
		C2RWRECT.Size.x += _Left.Diameter();

		if (true == R2PCol(C2RHRECT, _Left.Pos))
		{
			return true;
		}
		if (true == R2PCol(C2RWRECT, _Left.Pos))
		{
			return true;
		}

		NTVEC2 Tmp = _Right.RT();
		Tmp -= _Right.Pos;

		C2RCIRCLE = _Left;
		C2RCIRCLE.Pos -= _Right.Pos;
		C2RCIRCLE.Pos.x = fabsf(C2RCIRCLE.Pos.x);
		C2RCIRCLE.Pos.y = fabsf(C2RCIRCLE.Pos.y);

		if (true == C2PCol(C2RCIRCLE, Tmp))
		{
			return true;
		}

		return false;
	}

private:
	MathSystem();
	~MathSystem();
};

