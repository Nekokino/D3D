#pragma once
#include "NTResource.h"


class NTShader : public NTResource
{
public:
	enum STYPE
	{
		ST_NONE,
		ST_VS,
		ST_PX,
	};
protected:
	STYPE Type;
	ID3DBlob* Blob; // 쉐이더 코드를 컴파일했을때 나오는 바이트 배열
	ID3DBlob* ErrBlob; // 에러 바이트 배열

public:
	STYPE GetSType()
	{
		return Type;
	}
private:
	virtual void Update() = 0;
public:
	NTShader();
	~NTShader();
};

