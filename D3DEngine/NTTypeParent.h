#pragma once
#include <typeinfo.h>

class NTTypeParent
{
private:
	const type_info* TypeInfo;
	bool DbgFlag;

public:
	void SetType()
	{
		TypeInfo = &typeid(*this); // 내 타입을 기억해주는 거시다.
	}

	bool FlagCheck()
	{
		return DbgFlag;
	}

	void SetDbgFlag(bool _Value)
	{
		DbgFlag = _Value;
	}

	template<typename T>
	bool IsEqual()
	{
		return (*TypeInfo) == typeid(T); // T랑 같은 타입이냐?
	}

	template<typename T>
	bool IsParent()
	{
		return dynamic_cast<T*>(this) != nullptr;
	}

	const type_info* GetType()
	{
		return TypeInfo;
	}

public:
	NTTypeParent() : DbgFlag(false) {}
	virtual ~NTTypeParent() = 0 {}
};
