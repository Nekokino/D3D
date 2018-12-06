#include "PreCom.h"
#include "NTHullShader.h"
#include "NTWinShortCut.h"

NTHullShader::NTHullShader()
{
	Type = STYPE::ST_HS;
}


NTHullShader::~NTHullShader()
{
	if (Shader != nullptr)
	{
		Shader->Release();
	}
}

bool NTHullShader::Load(const char * _FuncName, UINT _VH, UINT _VL)
{
	UINT iFlag = 0;

#ifdef _DEBUG
	iFlag = D3D10_SHADER_DEBUG;
#endif

	char szBuffer[256] = {};

	sprintf_s(szBuffer, "hs_%d_%d", _VH, _VL);

	if (S_OK != D3DCompileFromFile(GetPath(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, _FuncName, szBuffer, iFlag, 0, &Blob, &ErrBlob))
	{
		std::string ErrorText = (char*)ErrBlob->GetBufferPointer();
		tassert(true);
		return false;
	}

	if (S_OK != NTWinShortCut::GetDevice()->CreateHullShader(Blob->GetBufferPointer(), Blob->GetBufferSize(), nullptr, &Shader))
	{
		return false;
	}

	DefaultInit();

	return true;
}

void NTHullShader::Update()
{
	NTWinShortCut::GetContext()->HSSetShader(Shader, 0, 0);
}

void NTHullShader::UpdateConstBuffer(Autoptr<ConstBuffer> _Buf)
{
	Context->HSSetConstantBuffers(_Buf->Reg, 1, &_Buf->CB);
}