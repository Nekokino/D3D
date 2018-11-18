#include "PreCom.h"
#include "NTThread.h"
#include "StlAssist.h"

std::unordered_map<std::wstring, Autoptr<NTThread>> NTThread::ThreadMap;

NTThread::NTThread() : Mode(FM_NONE), Handle(nullptr), bPause(false), Arg(nullptr), StdFuncPtr(nullptr), MemberPtr(nullptr), EndMemberPtr(nullptr), EndStdFuncPtr(nullptr)
{
}

NTThread::~NTThread()
{
	// 쓰레드 종료시 실행할 것을 넣어주자.

	if (nullptr != EndStdFuncPtr)
	{
		EndStdFuncPtr(this);
	}

	if (nullptr != EndMemberPtr)
	{
		EndMemberPtr(this);
	}

	Destroy();
}

Autoptr<NTThread> NTThread::NTFindThread(const wchar_t * _Name)
{
	return MapFind<Autoptr<NTThread>>(ThreadMap, _Name);
}

Autoptr<NTThread> NTThread::NTCreateThread(const wchar_t * _Name)
{
	NTThread* NewThread = new NTThread();

	ThreadMap.insert(std::unordered_map<std::wstring, Autoptr<NTThread>>::value_type(_Name, NewThread));

	return NewThread;
}

bool NTThread::NTPauseThread(const wchar_t * _Name)
{
	Autoptr<NTThread> Thread = NTFindThread(_Name);

	if (nullptr == Thread)
	{
		tassert(true);
		return false;
	}

	if (true == Thread->bPause)
	{
		return true;
	}

	Thread->Pause();

	return true;
}

bool NTThread::NTResumeThread(const wchar_t * _Name)
{
	Autoptr<NTThread> Thread = NTFindThread(_Name);

	if (nullptr == Thread)
	{
		tassert(true);
		return false;
	}

	if (false == Thread->bPause)
	{
		return true;
	}

	Thread->Resume();

	return true;
}

bool NTThread::NTStartThread(const wchar_t * _Name, unsigned int(__stdcall *_StdFunc)(void *), void * _Arg)
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

	Thread->Start(_StdFunc, _Arg);

	return true;
}

unsigned int __stdcall NTThread::BaseThreadFunc(void * _Arg)
{
	NTThread* Thread = (NTThread*)_Arg;

	switch (Thread->Mode)
	{
	case FM_STD:
		return Thread->StdFuncPtr(Thread->Arg);
		break;
	case FM_MEMBER:
		return Thread->MemberPtr(Thread->Arg);
		break;
	default:
		break;
	}

	return 0;
}

void NTThread::SetStart(void * _Arg)
{
	Arg = _Arg;
	Handle = (HANDLE)_beginthreadex(nullptr, 0, &NTThread::BaseThreadFunc, this, 0, nullptr);
}

void NTThread::Pause()
{
	if ((DWORD)-1 != SuspendThread(Handle))
	{
		bPause = true;
		return;
	}
}

void NTThread::Resume()
{
	if ((DWORD)-1 != ResumeThread(Handle))
	{
		bPause = false;
		return;
	}
}

void NTThread::Destroy()
{
	if (nullptr != Handle)
	{
		TerminateThread(Handle, 0);
		Handle = nullptr;
	}
}

bool NTThread::Start(unsigned int(__stdcall *_StdFuncPtr)(void *), void * _Arg)
{
	if (nullptr == _StdFuncPtr)
	{
		tassert(true);
		return false;
	}

	Mode = FM_STD;

	StdFuncPtr = _StdFuncPtr;
	MemberPtr = nullptr;
	SetStart(_Arg);

	return true;
}

