#include "PreCom.h"
#include "NTDomainShader.h"
#include "NTWinShortCut.h"


NTDomainShader::NTDomainShader()
{
	Type = STYPE::ST_DS;
}


NTDomainShader::~NTDomainShader()
{
	if (nullptr != Shader)
	{
		Shader->Release();
	}
}

bool NTDomainShader::Load(const char * _FuncName, UINT _VH, UINT _VL)
{
	UINT iFlag = 0;

#ifdef _DEBUG
	iFlag = D3D10_SHADER_DEBUG;
#endif

	char szBuffer[256] = {};

	sprintf_s(szBuffer, "ds_%d_%d", _VH, _VL);

	if (S_OK != D3DCompileFromFile(GetPath(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, _FuncName, szBuffer, iFlag, 0, &Blob, &ErrBlob))
	{
		std::string ErrorText = (char*)ErrBlob->GetBufferPointer();
		tassert(true);
		return false;
	}

	if (S_OK != NTWinShortCut::GetDevice()->CreateDomainShader(Blob->GetBufferPointer(), Blob->GetBufferSize(), nullptr, &Shader))
	{
		return false;
	}

	DefaultInit();

	return true;
}

void NTDomainShader::Update()
{
	NTWinShortCut::GetContext()->DSSetShader(Shader, 0, 0);
}

void NTDomainShader::UpdateConstBuffer(Autoptr<ConstBuffer> _Buf)
{
	Context->DSSetConstantBuffers(_Buf->Reg, 1, &_Buf->CB);
}