#include "PreCom.h"
#include "NTFileStream.h"


NTFileStream::NTFileStream(const wchar_t* _FileName, const wchar_t* _Mode) : File(nullptr)
{
	_wfopen_s(&File, _FileName, _Mode);
}

NTFileStream::~NTFileStream()
{
	if (nullptr != File)
	{
		fclose(File);
	}
}

void NTFileStream::Seek(long _Size, int _Mode)
{
	if (nullptr != File)
	{
		fseek(File, _Size, _Mode);
	}
}

void NTFileStream::Write(void * _Buf, int _ByteSize)
{
	if (nullptr != File)
	{
		fwrite(_Buf, _ByteSize, 1, File);
	}
}

void NTFileStream::Read(void * _Buf, int _BufByteSize, int _ReadByteSize)
{
	if (nullptr != File)
	{
		if (_BufByteSize < _ReadByteSize)
		{
			_ReadByteSize = _BufByteSize;
		}

		fread_s(_Buf, _BufByteSize, _ReadByteSize, 1, File);
	}
}


