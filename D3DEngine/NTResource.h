#pragma once
#include <string>
#include "NTNameTag.h"
#include "NTTypeParent.h"
#include "RefNAutoptr.h"

class NTResource : public NTNameTag, public NTTypeParent, public RefCounter
{
private:
	std::wstring Pathkey;
	std::wstring Path;
	std::wstring FullFileName;
	std::wstring FileName;
	std::wstring Extension; // »Æ¿Â¿⁄

public:
	const wchar_t* GetPath()
	{
		return Path.c_str();
	}

	const wchar_t* GetFileName()
	{
		return FileName.c_str();
	}

	const wchar_t* GetFullFileName()
	{
		return FullFileName.c_str();
	}

	const wchar_t* GetExtention()
	{
		return Extension.c_str();
	}

	bool IsExtension(const wchar_t* _Extension)
	{
		return Extension == _Extension;
	}

	void SetPath(const wchar_t* _Path);
	void SetPathkey(const wchar_t* _Pathkey)
	{
		Pathkey = _Pathkey;
	}

	const wchar_t* GetPathkey()
	{
		return Pathkey.c_str();
	}

private:
	virtual bool Create();
	virtual bool Load();

	
public:
	NTResource();
	NTResource(const NTResource& _Other);
	~NTResource();
};

