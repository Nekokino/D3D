#include "PreCom.h"
#include "NTGeometryShader.h"
#include "NTWinShortCut.h"

NTGeometryShader::NTGeometryShader()
{
	Type = STYPE::ST_GS;
}


NTGeometryShader::~NTGeometryShader()
{
	if (Shader != nullptr)
	{
		Shader->Release();
	}
}

bool NTGeometryShader::Load(const char * _FuncName, UINT _VH, UINT _VL)
{
	UINT iFlag = 0;

#ifdef _DEBUG
	iFlag = D3D10_SHADER_DEBUG;
#endif

	char szBuffer[256] = {};

	sprintf_s(szBuffer, "gs_%d_%d", _VH, _VL);

	if (S_OK != D3DCompileFromFile(GetPath(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, _FuncName, szBuffer, iFlag, 0, &Blob, &ErrBlob))
	{
		std::string ErrorText = (char*)ErrBlob->GetBufferPointer();
		tassert(true);
		return false;
	}

	if (S_OK != NTWinShortCut::GetDevice()->CreateGeometryShader(Blob->GetBufferPointer(), Blob->GetBufferSize(), nullptr, &Shader))
	{
		return false;
	}

	DefaultInit();

	return true;
}

void NTGeometryShader::Update()
{
	NTWinShortCut::GetContext()->GSSetShader(Shader, 0, 0);
}

void NTGeometryShader::UpdateConstBuffer(Autoptr<ConstBuffer> _Buf)
{
	Context->GSSetConstantBuffers(_Buf->Reg, 1, &_Buf->CB);
}