#include "PreCom.h"
#include "NTShader.h"


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
