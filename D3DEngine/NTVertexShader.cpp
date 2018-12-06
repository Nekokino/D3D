#include "PreCom.h"
#include "NTVertexShader.h"
#include "NTWinShortCut.h"



NTVertexShader::NTVertexShader() : Offset(0)
{
	Type = ST_PS;
}


NTVertexShader::~NTVertexShader()
{
	if (nullptr != Shader)
	{
		Shader->Release();
	}

	if (nullptr != InputLayout)
	{
		InputLayout->Release();
	}
}

bool NTVertexShader::Load(const char* _FuncName, UINT _VH, UINT _VL)
{
	UINT iFlag = 0;

#ifdef _DEBUG
	iFlag = D3D10_SHADER_DEBUG;
#endif

	char szBuffer[256] = {};

	sprintf_s(szBuffer, "vs_%d_%d", _VH, _VL);

	if (S_OK != D3DCompileFromFile(GetPath(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, _FuncName, szBuffer, iFlag, 0, &Blob, &ErrBlob))
	{
		std::string ErrorText = (char*)ErrBlob->GetBufferPointer();
		tassert(true);
		return false;
	}

	if (S_OK != NTWinShortCut::GetDevice()->CreateVertexShader(Blob->GetBufferPointer(), Blob->GetBufferSize(), nullptr, &Shader))
	{
		return false;
	}

	DefaultInit();

	return true;
}

void NTVertexShader::Update()
{
	NTWinShortCut::GetContext()->VSSetShader(Shader, 0, 0);
}

bool NTVertexShader::AddLayout(LPCSTR _SementicName, UINT _SementicIndex, DXGI_FORMAT _Format, UINT _InputSlot, D3D11_INPUT_CLASSIFICATION _InputSlotClass, UINT _InstanceDateStepRate)
{
	D3D11_INPUT_ELEMENT_DESC NewInputLayout = D3D11_INPUT_ELEMENT_DESC();

	if (_InputSlot >= D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT)
	{
		return false;
	}

	NewInputLayout.SemanticName = _SementicName;
	NewInputLayout.SemanticIndex = _SementicIndex;
	NewInputLayout.Format = _Format;
	NewInputLayout.InputSlot = _InputSlot;
	NewInputLayout.AlignedByteOffset = Offset;
	NewInputLayout.InputSlotClass = _InputSlotClass;
	NewInputLayout.InstanceDataStepRate = _InstanceDateStepRate;

	Offset += DirectHelper::GetFormatSize(_Format);
	InputLayoutData.push_back(NewInputLayout);

	return true;
}

bool NTVertexShader::AddLayoutClose(LPCSTR _SementicName, UINT _SementicIndex, DXGI_FORMAT _Format, UINT _InputSlot, D3D11_INPUT_CLASSIFICATION _InputSlotClass, UINT _InstanceDateStepRate)
{
	AddLayout(_SementicName, _SementicIndex, _Format, _InputSlot, _InputSlotClass, _InstanceDateStepRate);
	CreateLayout();

	return true;
}

bool NTVertexShader::CreateLayout()
{
	tassert(nullptr == Shader);

	if (nullptr == Shader)
	{
		return false;
	}

	if (S_OK != NTWinShortCut::GetDevice()->CreateInputLayout(&InputLayoutData[0], (UINT)InputLayoutData.size(), Blob->GetBufferPointer(), Blob->GetBufferSize(), &InputLayout))
	{
		return false;
	}

	return true;
}

void NTVertexShader::SetLayout()
{
	NTWinShortCut::GetContext()->IASetInputLayout(InputLayout);
}

void NTVertexShader::UpdateConstBuffer(Autoptr<ConstBuffer> _Buf)
{
	Context->VSSetConstantBuffers(_Buf->Reg, 1, &_Buf->CB);
}