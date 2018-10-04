#pragma once
#include "NTShader.h"
#include <vector>

class NTVertexShader : public NTShader
{
private:
	ID3D11VertexShader* Shader;

	std::vector<D3D11_INPUT_ELEMENT_DESC> InputLayoutData;
	UINT Offset;
	ID3D11InputLayout* InputLayout;

public:
	bool Load(const char* _FuncName, UINT _VH = 5, UINT _VL = 0); // VH, VL = HLSL 컴파일러 버전
	void Update() override;

	bool AddLayout(LPCSTR _SementicName, UINT _SementicIndex, DXGI_FORMAT _Format, UINT _InputSlot, D3D11_INPUT_CLASSIFICATION _InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, UINT _InstanceDateStepRate = 0);
	bool CreateLayout();
	void SetLayout();

public:
	NTVertexShader();
	~NTVertexShader();
};

