#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXCollision.h>
#include <DirectXPackedVector.h>

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "dxguid")

class CustomColor
{
public:
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};

class NTIndex
{
public:
	union
	{
		struct
		{
			int x;
			int y;
		};

		struct
		{
			int left;
			int right;
		};

		__int64 i64;
	};

public:
	operator __int64()
	{
		return i64;
	}

public:
	NTIndex(int _X, int _Y) : x(_X), y(_Y) {}
	NTIndex(__int64 _Other) : i64(_Other) {}
	~NTIndex() {}
};

class NTVEC2
{
public:
	union
	{
		DirectX::XMFLOAT2 Vec;
		float s[2];
		struct
		{
			float x;
			float y;
		};
	};

public:
	NTVEC2() : Vec(0.0f, 0.0f) {}
	NTVEC2(float _Value) : Vec(_Value, _Value) {}
	NTVEC2(float _x, float _y) : Vec(_x, _y) {}
	NTVEC2(const NTVEC2& _Other) : Vec(_Other.x, _Other.y) {}
	NTVEC2(const DirectX::XMVECTOR& _Other) { DirectX::XMStoreFloat2(&Vec, _Other); }

public:
	void SetFloat2(float _x, float _y)
	{
		x = _x; y = _y;
	}

public:
	NTVEC2& operator=(const NTVEC2& _Other)
	{
		Vec = _Other.Vec;
		return *this;
	}

	NTVEC2& operator=(const DirectX::XMVECTOR& _Value)
	{
		DirectX::XMStoreFloat2(&Vec, _Value);
		return *this;
	}

	NTVEC2 operator*(float _Value) const
	{
		return{ x*_Value, y*_Value };
	}

	NTVEC2 operator/(float _Value) const
	{
		return{ x / _Value, y / _Value };
	}

	NTVEC2 operator+(const NTVEC2& _Other) const
	{
		return{ x + _Other.x , y + _Other.y };
	}

	NTVEC2 operator-(const NTVEC2& _Other) const
	{
		return{ x - _Other.x, y - _Other.y };
	}

	NTVEC2& operator-=(const NTVEC2& _Other)
	{
		x -= _Other.x;
		y -= _Other.y;
		return *this;
	}

	NTVEC2& operator*=(const float _Value)
	{
		x *= _Value;
		y *= _Value;
		return *this;
	}


	float GetLength()
	{
		return sqrtf(x * x + y * y);
	}
};

typedef NTVEC2 NTPOINT;

class NTVEC3
{
public:
	union
	{
		DirectX::XMFLOAT3 Vec;
		float s[3];
		struct
		{
			NTVEC2 Pos;
			float Radius;
		};

		struct
		{
			float x;
			float y;
			float z;
		};
	};

public:
	NTVEC3() : Vec(0, 0, 0) {}
	NTVEC3(float _Value) : Vec(_Value, _Value, _Value) {}
	NTVEC3(float _x, float _y) : Vec(_x, _y, 0.0f) {}
	NTVEC3(float _x, float _y, float _z) : Vec(_x, _y, _z) {}
	NTVEC3(const NTVEC2& _Other) : Vec(_Other.x, _Other.y, 0.0f) {}
	NTVEC3(const NTVEC3& _Other) : Vec(_Other.x, _Other.y, _Other.z) {}
	NTVEC3(const DirectX::XMVECTOR& _Other) { DirectX::XMStoreFloat3(&Vec, _Other); }

public:
	void Set(float _x, float _y, float _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}

	float Diameter() const
	{
		return Radius * 2.0f;
	}

public:
	NTVEC3 operator+(const NTVEC3& _Other) const
	{
		return{ x + _Other.x, y + _Other.y, z + _Other.z };
	}

	NTVEC3& operator=(const NTVEC3& _Other)
	{
		Vec = _Other.Vec;
		return *this;
	}

	NTVEC3& operator=(const DirectX::XMVECTOR& _Other)
	{
		DirectX::XMStoreFloat3(&Vec, _Other);
		return *this;
	}

	float GetLength()
	{
		sqrtf(x*x + y*y + z*z);
	}
};

typedef NTVEC3 NTCIRCLE;

class NTVEC
{
public:
	static const NTVEC LEFT;
	static const NTVEC RIGHT;
	static const NTVEC UP;
	static const NTVEC DOWN;
	static const NTVEC FORWARD;
	static const NTVEC BACK;
	static const NTVEC ZERO;
	static const NTVEC ONE;
	static const NTVEC RX;
	static const NTVEC RY;
	static const NTVEC RZ;
	static const NTVEC RRX;
	static const NTVEC RRY;
	static const NTVEC RRZ;
public:
	union
	{
		NTVEC2 Vec2;
		NTVEC3 Vec3;
		DirectX::XMFLOAT4 DVec4;
		DirectX::XMFLOAT3 DVec3;
		float s[4];
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};

		struct
		{
			float r;
			float g;
			float b;
			float a;
		};

		struct
		{
			NTVEC2 Pos;
			NTVEC2 Size;
		};
	};

public:
	NTVEC() : DVec4(0.0f, 0.0f, 0.0f, 0.0f) {}
	NTVEC(float _Value) : DVec4(_Value, _Value, _Value, _Value) {}
	NTVEC(float _x, float _y) : DVec4(_x, _y, 0.0f, 0.0f) {}
	NTVEC(float _x, float _y, float _z) : DVec4(_x, _y, _z, 0.0f) {}
	NTVEC(float _x, float _y, float _z, float _w) : DVec4(_x, _y, _z, _w) {}
	NTVEC(const NTVEC2& _Other) : DVec4(_Other.x, _Other.y, 0.0f, 0.0f) {}
	NTVEC(const NTVEC2& _Value1, const NTVEC2& _Value2) : Pos(_Value1), Size(_Value2) {}
	NTVEC(const NTVEC3& _Other) : DVec4(_Other.x, _Other.y, _Other.z, 0.0f) {}
	NTVEC(const NTVEC& _Other) : DVec4(_Other.x, _Other.y, _Other.z, _Other.w) {}
	NTVEC(const DirectX::XMVECTOR& _Other) { DirectX::XMStoreFloat4(&DVec4, _Other); } // DVec에 _Other의 값을 저장.
	
public: // for NTRECT
	float Left() const
	{
		return Pos.x - Size.x * 0.5f;
	}

	float Right() const
	{
		return Pos.x + Size.x * 0.5f;
	}

	float Top() const
	{
		return Pos.y + Size.y * 0.5f;
	}

	float Bottom() const
	{
		return Pos.y - Size.y * 0.5f;
	}

	NTVEC2 LT() const
	{
		return{ Left(), Top() };
	}

	NTVEC2 RT() const
	{
		return{ Right(), Top() };
	}

	NTVEC2 LB() const
	{
		return{ Left(), Bottom() };
	}

	NTVEC2 RB() const
	{
		return{ Right(), Bottom() };
	}


public:
	NTVEC operator+(const NTVEC& _Other) const
	{
		return NTVEC(x + _Other.x, y + _Other.y, z + _Other.z);
	}

	NTVEC operator-(const NTVEC& _Other) const
	{
		return NTVEC(x - _Other.x, y - _Other.y, z - _Other.z);
	}

	NTVEC& operator+=(const NTVEC& _Other)
	{
		x += _Other.x;
		y += _Other.y;
		z += _Other.z;
		return *this;
	}

	NTVEC& operator-=(const NTVEC& _Other)
	{
		x -= _Other.x;
		y -= _Other.y;
		z -= _Other.z;
		return *this;
	}

	NTVEC& operator*=(const float _Value)
	{
		x *= _Value;
		y *= _Value;
		z *= _Value;
		w *= _Value;

		return *this;
	}

	NTVEC operator*(const float _Value) const
	{
		return NTVEC(x*_Value, y*_Value, z*_Value);
	}

	NTVEC operator/(const NTVEC& _Other) const
	{
		NTVEC Return;
		if (_Other.x != 0)
		{
			Return.x = x / _Other.x;
		}
		else
		{
			Return.x = 0;
		}

		if (_Other.y != 0)
		{
			Return.y = y / _Other.y;
		}
		else
		{
			Return.y = 0;
		}

		if (_Other.z != 0)
		{
			Return.z = z / _Other.z;
		}
		else
		{
			Return.z = 0;
		}

		if (_Other.w != 0)
		{
			Return.w = w / _Other.w;
		}
		else
		{
			Return.w = 0;
		}

		return Return;
	}

	NTVEC& operator=(const NTVEC& _Other)
	{
		DVec4 = _Other.DVec4;
		return *this;
	}

	NTVEC operator -()
	{
		return NTVEC(-x, -y, -z, -w);
	}

	NTVEC& operator=(const DirectX::XMVECTOR& _Other)
	{
		DirectX::XMStoreFloat4(&DVec4, _Other);
		return *this;
	}

	bool operator==(const NTVEC& _Other) const
	{
		if (x == _Other.x && y == _Other.y && z == _Other.z && w == _Other.w)
		{
			return true;
		}

		return false;
	}

	operator DirectX::XMVECTOR() const
	{
		return DirectX::XMLoadFloat4(&DVec4); // float4형식을 xmvector형식으로 변환해서 내보내줌
	}

	operator NTVEC2() const
	{
		return{ DVec4.x, DVec4.y };
	}

public:
	static NTVEC Dot(const NTVEC& _Left, const NTVEC& _Right)
	{
		NTVEC Return = DirectX::XMVector4Dot(_Left, _Right);
		return Return.x; // 모든 구성 성분(x,y,z,w)가 같은값으로 채워져있음.
	}

	float Dot(const NTVEC& _Other)
	{
		NTVEC Return = DirectX::XMVector4Dot(*this, _Other);
		return Return.x;
	}

	static NTVEC Cross3D(const NTVEC& _Left, const NTVEC& _Right)
	{
		NTVEC Return = DirectX::XMVector3Cross(_Left, _Right);
		return Return;
	}

	NTVEC Cross3D(const NTVEC& _Other)
	{
		NTVEC Return = DirectX::XMVector3Cross(*this, _Other);
		return Return;
	}

	void Normalize4D()
	{
		*this = DirectX::XMVector4Normalize(*this);
	}

	void Normalize3D()
	{
		*this = DirectX::XMVector3Normalize(*this);
		w = 0.0f;
	}

	void Normalize2D()
	{
		*this = DirectX::XMVector2Normalize(*this);
		z = 0.0f;
		w = 0.0f;
	}

	NTVEC ReturnNormalize()
	{
		return DirectX::XMVector4Normalize(*this);
	}

	float GetLength()
	{
		return sqrtf(x*x + y*y + z*z);
	}

	UINT ColorUINT()
	{
		UINT Color = 0;
		int TempR = (int)r;
		int TempG = (int)g;
		int TempB = (int)b;
		int TempA = (int)a;

		Color |= TempR;
		Color |= (TempG << 8);
		Color |= (TempB << 16);
		Color |= (TempA << 24);

		return Color;
	}
};

typedef NTVEC NTCOLOR;
typedef NTVEC NTRECT;

class NTMAT
{
public:
	union
	{
		float m[4][4];
		DirectX::XMFLOAT4X4 Matrix;

		struct
		{
			NTVEC v1;
			NTVEC v2;
			NTVEC v3;
			NTVEC v4;
		};
	};
public:
	operator DirectX::XMMATRIX() const
	{
		return DirectX::XMLoadFloat4x4(&Matrix);
	}

	NTMAT& operator=(const DirectX::XMMATRIX& _Other)
	{
		DirectX::XMStoreFloat4x4(&Matrix, _Other);
		return *this;
	}

	NTMAT& operator=(const NTMAT& _Other)
	{
		memcpy_s(this, sizeof(NTMAT), &_Other, sizeof(NTMAT));
		return *this;
	}

	NTMAT operator*(const DirectX::XMMATRIX& _Other) const
	{
		return DirectX::XMMatrixMultiply(*this, _Other);
	}

public:
	NTMAT& Identify()
	{
		*this = DirectX::XMMatrixIdentity();
		return *this;
	}

	void RotationX(float _Radian)
	{
		*this = DirectX::XMMatrixRotationX(_Radian);
	}

	void RotationY(float _Radian)
	{
		*this = DirectX::XMMatrixRotationY(_Radian);
	}

	void RotationZ(float _Radian)
	{
		*this = DirectX::XMMatrixRotationZ(_Radian);
	}

	void Scale(const NTVEC& _Scale)
	{
		*this = DirectX::XMMatrixScaling(_Scale.x, _Scale.y, _Scale.z);
	}

	void Translate(const NTVEC& _Translation)
	{
		*this = DirectX::XMMatrixTranslation(_Translation.x, _Translation.y, _Translation.z);
	}

	void Transpose() // 전치행렬로 만들어준다.
	{
		*this = DirectX::XMMatrixTranspose(*this);
	}

	const NTMAT& RTranspose()
	{
		*this = DirectX::XMMatrixTranspose(*this);
		return *this;
	}

	const NTMAT RVTranspose() const
	{
		NTMAT Mat = *this;
		Mat = DirectX::XMMatrixTranspose(Mat);
		return Mat;
	}

	NTMAT& Inverse()
	{
		*this = DirectX::XMMatrixInverse(nullptr, *this);
		return *this;
	}

	NTMAT RVInverse()
	{
		NTMAT Return = *this;
		Return.Inverse();
		return Return;
	}

	void ViewAtLH(const NTVEC& _Pos, const NTVEC& _LookAt, const NTVEC& _Up)
	{
		*this = DirectX::XMMatrixLookAtLH(_Pos, _LookAt, _Up);
	}

	void ViewToLH(const NTVEC& _Pos, const NTVEC& _LookDir, const NTVEC& _Up)
	{
		*this = DirectX::XMMatrixLookToLH(_Pos, _LookDir, _Up);
	}

	void PerspectiveLH(float _Fov, float _Width, float _Height, float _Near, float _Far)
	{
		*this = DirectX::XMMatrixPerspectiveFovLH(_Fov, _Width / _Height, _Near, _Far);
	}

	void OrthographicLH(float _Width, float _Height, float _Near, float _Far)
	{
		*this = DirectX::XMMatrixOrthographicLH(_Width, _Height, _Near, _Far);
	}

	NTVEC MulOne(const NTVEC& _Vec) const
	{
		NTMAT Return = *this;
		return DirectX::XMVector3TransformCoord(_Vec, Return);
	}

	NTVEC MulZero(const NTVEC& _Vec) const
	{
		NTMAT Return;
		memcpy_s(&Return, sizeof(NTMAT), this, sizeof(NTMAT));
		return DirectX::XMVector3TransformNormal(_Vec, Return);
	}

public:
	NTMAT() {}
	NTMAT(const NTMAT& _Other)
	{
		memcpy_s(this, sizeof(NTMAT), &_Other, sizeof(NTMAT));
	}
	NTMAT(const DirectX::XMMATRIX& _Other)
	{
		DirectX::XMStoreFloat4x4(&Matrix, _Other);
	}
};

class DirectHelper
{
public:
	static UINT GetFormatSize(DXGI_FORMAT _Format);
};

class MatrixData
{
public:
	NTMAT World;
	NTMAT View;
	NTMAT Projection;
	NTMAT WV;
	NTMAT WVP;

public:
	void TransposeAll()
	{
		World.Transpose();
		View.Transpose();
		Projection.Transpose();
		WV.Transpose();
		WVP.Transpose();
	}
};

enum STYPE
{
	ST_NONE,
	ST_VS,
	ST_PX,
};