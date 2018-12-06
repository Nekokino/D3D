#include "PreCom.h"
#include "NTPixelShader.h"
#include "NTWinShortCut.h"


NTPixelShader::NTPixelShader()
{
	Type = ST_PS;
}


NTPixelShader::~NTPixelShader()
{
	if (nullptr != Shader)
	{
		Shader->Release();
	}
}

bool NTPixelShader::Load(const char* _FuncName, UINT _VH, UINT _VL)
{
	UINT iFlag = 0;
#ifdef _DEBUG
	iFlag = D3D10_SHADER_DEBUG;
#endif

	char szBuffer[256] = {};
	sprintf_s(szBuffer, "ps_%d_%d", _VH, _VL);

	if (S_OK != D3DCompileFromFile(GetPath(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, _FuncName, szBuffer, iFlag, 0, &Blob, &ErrBlob))
	{

		std::string ErrorText = (char*)ErrBlob->GetBufferPointer();
		tassert(true);
		return false;
	}

	if (S_OK != NTWinShortCut::GetDevice()->CreatePixelShader(Blob->GetBufferPointer(), Blob->GetBufferSize(), nullptr, &Shader))
	{
		return false;
	}

	DefaultInit();

	return true;
}

void NTPixelShader::Update()
{
	NTWinShortCut::GetContext()->PSSetShader(Shader, 0, 0);
}

void NTPixelShader::UpdateConstBuffer(Autoptr<ConstBuffer> _Buf)
{
	Context->PSSetConstantBuffers(_Buf->Reg, 1, &_Buf->CB);
}
