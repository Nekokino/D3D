#pragma once
#include <type_traits>

template<typename BASE, typename CHILD>
class TypeCheck
{
private:
	static short Check(BASE*) {};
	static char Check(...) {};

	static CHILD* p;

public:
	static const bool Value = (bool)(sizeof(Check(p)) - 1);

private:
	TypeCheck() {}
	~TypeCheck() {}
};

class RefCounter
{
public:
	template<typename T>
	friend class Autoptr; // 템플릿 프렌드.

private:
	size_t Counter;

private:
	void AddRef();
	void DecRef();

public:
	RefCounter();
	virtual ~RefCounter() = 0; // 직접 생성할일 없음.
};

template<typename T> // 템플릿 클래스
class Autoptr
{
private:
	T* ptr;

public:
	Autoptr() : ptr(nullptr)
	{

	}

	Autoptr(T* _ptr) : ptr(_ptr)
	{
		AddCheck();
	}

	Autoptr(const Autoptr& _ptr) : ptr(_ptr.ptr)
	{
		AddCheck();
	}

	Autoptr(std::nullptr_t _ptr) : ptr(_ptr) // nullptr을 체크하기위한 형태
	{

	}

	~Autoptr()
	{
		DecCheck();
	}

public:
	bool operator<(const Autoptr& _Other) const
	{
		return ptr < _Other.ptr;
	}

	bool operator>(const Autoptr& _Other) const
	{
		return ptr > _Other.ptr;
	}

	bool operator==(std::nullptr_t _ptr) const
	{
		return ptr == _ptr;
	}

	bool operator==(T* _ptr) const
	{
		return ptr == _ptr;
	}

	bool operator==(const Autoptr& _Other) const
	{
		return ptr == _Other.ptr;
	}

	bool operator!=(std::nullptr_t _ptr) const
	{
		return ptr != _ptr;
	}
	
	operator T*()
	{
		return ptr;
	}

	const T* operator->() const
	{
		return ptr;
	}

	T* operator->()
	{
		return ptr;
	}

	T& operator*()
	{
		return *ptr;
	}

	template<typename Type>
	operator Autoptr<Type>()
	{
		if (nullptr == ptr)
		{
			return nullptr; // 어차피 nullptr일때 형변환 해봐야 똑같아서 예외처리.
		}

		Type* p = dynamic_cast<Type*>(ptr);

		if (true == TypeCheck<T, Type>::Value)
		{
			return (Type*)ptr;
		}

		return p;
	}

	Autoptr<T>& operator=(T* _ptr)
	{
		if (ptr == _ptr)
		{
			return *this; // 대입연산자. 자기자신이면 그냥 그대로.
		}

		DecCheck();
		ptr = _ptr;

		AddCheck();
		return *this;
	}

	Autoptr<T>& operator=(const Autoptr& _ptr)
	{
		if (ptr == _ptr.ptr)
		{
			return *this;
		}

		DecCheck();
		ptr = _ptr.ptr;
		AddCheck();
		return *this;
	}

private:
	void AddCheck()
	{
		if (nullptr != ptr)
		{
			ptr->AddRef();
		}
	}

	void DecCheck()
	{
		if (nullptr != ptr)
		{
			ptr->DecRef();
		}
	}
};

template<typename T>
bool operator==(std::nullptr_t _ptr, const Autoptr<T>& _Autoptr) { return _Autoptr == _ptr; }

template<typename T>
bool operator!=(std::nullptr_t _ptr, const Autoptr<T>& _Autoptr) { return _Autoptr != _ptr; }