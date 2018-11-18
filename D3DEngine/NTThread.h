#pragma once
#include <process.h>
#include <Windows.h>
#include <unordered_map>
#include <string>
#include "RefNAutoptr.h"
#include <WinBase.h>
#include <functional>
#include "CustomMacro.h"

class NTThread : public RefCounter
{
private:
	enum FUNC_MODE
	{
		FM_NONE,
		FM_STD,
		FM_MEMBER,
	};

private:
	static std::unordered_map<std::wstring, Autoptr<NTThread>> ThreadMap;

public:
	static Autoptr<NTThread> NTFindThread(const wchar_t* _Name);
	static Autoptr<NTThread> NTCreateThread(const wchar_t* _Name);
	static bool NTPauseThread(const wchar_t* _Name);
	static bool NTResumeThread(const wchar_t* _Name);

	static bool NTStartThread(const wchar_t* _Name, unsigned int(__stdcall *_StdFunc)(void*), void* _Arg = nullptr);

	template<typename T>
	static bool NTStartThread(const wchar_t* _Name, unsigned int(T::*_Member)(void*), T* _Obj, void* _Arg = nullptr)
	{
		Autoptr<NTThread> Thread = NTFindThread(_Name);

		if (nullptr == Thread)
		{
			Thread = NTCreateThread(_Name);
		}

		if (true == Thread->IsWork())
		{
			Thread->Destroy();
		}

		Thread->Start<T>(_Member, _Obj, _Arg);
		return true;
	}

	static unsigned int __stdcall BaseThreadFunc(void* _Arg);

private:
	FUNC_MODE Mode;

	HANDLE Handle;
	bool bPause;
	void* Arg;

	unsigned int(__stdcall *StdFuncPtr)(void*);
	std::function<unsigned int(void*)> MemberPtr;

	unsigned int(__stdcall *EndStdFuncPtr)(void*);
	std::function<unsigned int(void*)> EndMemberPtr;

public:
	bool IsWork()
	{
		return nullptr != Handle;
	}

private:
	bool Start(unsigned int(__stdcall *_StdFuncPtr)(void*), void* _Arg = nullptr);

	template<typename T>
	bool Start(unsigned int(T::*_Member)(void*), T* _Obj, void* _Arg = nullptr)
	{
		if (nullptr == _Obj || nullptr == _Member)
		{
			tassert(true);
			return false;
		}

		Mode = FM_MEMBER;

		StdFuncPtr = nullptr;
		MemberPtr = bind(_Member, _Obj, std::placeholders::_1);

		SetStart(_Arg);

		return true;
	}

private:
	void SetStart(void* _Arg);

public:
	void Pause();
	void Resume();

private:
	void Destroy();
public:
	NTThread();
	~NTThread();
};

