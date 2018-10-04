#include "PreCom.h"
#include "NTWriteStream.h"


NTWriteStream::NTWriteStream(const wchar_t* _Path) : NTFileStream(_Path, L"wb")
{
}

NTWriteStream::~NTWriteStream()
{
}


void NTWriteStream::Write(void * _Buf, int _ByteSize)
{
	NTFileStream::Write(_Buf, _ByteSize);
}

