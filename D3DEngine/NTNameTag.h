#pragma once
#include <string>

class NTNameTag
{
private:
	std::wstring Name;

public:
	const wchar_t* GetName()
	{
		return Name.c_str();
	}

	unsigned int GetLength()
	{
		return (unsigned int)Name.size() + 1;
	}

	void SetName(const wchar_t* _Name)
	{
		Name = _Name;
	}

public:
	NTNameTag() {}
	NTNameTag(const wchar_t* _Name) : Name(_Name) {}
	~NTNameTag() {}
};