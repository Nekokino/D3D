#pragma once
#include "NTShader.h"
class NTGeometryShader : public NTShader
{
private:
	ID3D11GeometryShader* Shader;

public:
	bool Load(const char* _FuncName, UINT _VH = 5, UINT _VL = 0);
	void Update() override;

private:
	void UpdateConstBuffer(Autoptr<ConstBuffer> _Buf);

public:
	NTGeometryShader();
	~NTGeometryShader();
};

