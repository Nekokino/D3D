#include "PreCom.h"
#include "MathSystem.h"
#include "DirectHeader.h"
#include "TimeSystem.h"

const float MathSystem::R2D = 180.0f / DirectX::XM_PI;
const float MathSystem::D2R = DirectX::XM_PI / 180.0f;

NTRECT MathSystem::C2RHRECT;
NTRECT MathSystem::C2RWRECT;
NTCIRCLE MathSystem::C2RCIRCLE;

MathSystem::MathSystem()
{
}


MathSystem::~MathSystem()
{
}

float MathSystem::GetDistance2D(const NTVEC& _Left, const NTVEC& _Right)
{
	float X = _Left.x - _Right.x;
	float Y = _Left.y - _Right.y;
	return sqrtf((X*X) + (Y*Y));
}

float MathSystem::GetDistance(const NTVEC & _Left, const NTVEC & _Right)
{
	float X = _Left.x - _Right.x;
	float Y = _Left.y - _Right.y;
	float Z = _Left.z - _Right.z;
	return sqrtf((X*X) + (Y*Y) + (Z*Z));
}

float MathSystem::GetLength2D(const NTVEC& _Value)
{
	return sqrtf((_Value.x*_Value.x) + (_Value.y * _Value.y));
}

float MathSystem::GetLength(const NTVEC & _Value)
{
	return sqrtf((_Value.x*_Value.x) + (_Value.y * _Value.y) + (_Value.z * _Value.z));
}

float MathSystem::GetRadian(const NTVEC & _Left, const NTVEC & _Right) // 좀 손봐야하지 않을까?
{
	float Distance = GetDistance(_Left, _Right);
	float X = 0.0f;

	if (Distance == 0)
	{
		return 0.0f;
	}

	if (_Right.x < _Left.x)
	{
		X *= -1.0f;
	}

	X = GetFABS(_Left.x - _Right.x);

	float Cos = X / Distance;
	float Radian = acosf(Cos);

	if (_Left.y > _Right.y)
	{
		Radian = DirectX::XM_PI * 2.0f - Radian; // 아 이걸 왜 생각이 안났지 ㅋㅋㅋㅋㅋㅋㅋ
	}

	return Radian;
}

float MathSystem::GetRadian2D(const NTVEC & _Left, const NTVEC & _Right)
{
	float Distance = GetDistance2D(_Left, _Right);
	float X = 0.0f;

	if (Distance == 0)
	{
		return 0.0f;
	}

	X = GetFABS(_Left.x - _Right.x);

	if (_Right.x < _Left.x)
	{
		X *= -1.0f;
	}

	float Cos = X / Distance;
	float Radian = acosf(Cos);

	if (_Left.y > _Right.y)
	{
		Radian = DirectX::XM_PI * 2.0f - Radian; // 아 이걸 왜 생각이 안났지 ㅋㅋㅋㅋㅋㅋㅋ
	}

	return Radian;
}

float MathSystem::GetDegree(const NTVEC & _Left, const NTVEC & _Right)
{
	return GetRadian(_Left, _Right) * R2D;
}

float MathSystem::GetDegree2D(const NTVEC & _Left, const NTVEC & _Right)
{
	return GetRadian2D(_Left, _Right) * R2D;
}


NTVEC MathSystem::Pendulum(const NTVEC & _OriginPoint, const NTVEC & _PenPoint, const float _Gravity, const float _Degree, const float _Time)
{
	NTVEC Tmp = _PenPoint - _OriginPoint;
	float InitLength = GetLength2D(Tmp);
	Tmp.Normalize2D();
	float InitRad = GetRadian2D(Tmp, NTVEC::ZERO);
	float PenTime = _Time;

	float PenRad = InitRad * cosf(sqrtf(_Gravity / InitLength) * PenTime);

	PenRad -= _Degree * D2R;

	NTVEC ReturnPos = { cosf(PenRad) * InitLength,sinf(PenRad)*InitLength, _PenPoint.z, _PenPoint.z };

	float Cycle = 2 * DirectX::XM_PI * sqrtf(InitLength / _Gravity); // 주기입니다만, 이걸 표현하려면 함수를 따로 만들어줘야해서 일단 여기에 남겨둡니다.

	return ReturnPos;
}
