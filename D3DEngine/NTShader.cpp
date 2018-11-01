#include "PreCom.h"
#include "NTShader.h"
#include "StlAssist.h"
#include "NTWinShortCut.h"



NTShader::NTShader() : Type(ST_NONE)
{
}


NTShader::~NTShader()
{
	if (nullptr != Blob)
	{
		Blob->Release();
	}

	if (nullptr != ErrBlob)
	{
		ErrBlob->Release();
	}
}

void NTShader::DefaultInit()
{
	Context = NTWinShortCut::GetContext();
}

bool NTShader::CreateConstBuffer(ConstBuffer * _Buf)
{
	if (S_OK != NTWinShortCut::GetDevice()->CreateBuffer(&_Buf->Desc, nullptr, &_Buf->CB))
	{
		delete _Buf;
		return false;
	}

	return true;
}

Autoptr<NTShader::ConstBuffer> NTShader::FindConstBuffer(const wchar_t * _Name)
{
	return MapFind<Autoptr<ConstBuffer>>(ConstBufferMap, _Name);
}