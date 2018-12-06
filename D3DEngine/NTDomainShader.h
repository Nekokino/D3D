#pragma once
#include "NTShader.h"
class NTDomainShader : public NTShader
{
private:
	ID3D11DomainShader* Shader;

public:
	bool Load(const char* _FuncName, UINT _VH = 5, UINT _VL = 0);
	void Update() override;

private:
	void UpdateConstBuffer(Autoptr<ConstBuffer> _Buf);
public:
	NTDomainShader();
	~NTDomainShader();
};

