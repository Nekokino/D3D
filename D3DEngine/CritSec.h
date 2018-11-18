#pragma once
#include <Windows.h>
#include <process.h>
#include <unordered_map>

class CritSec
{
private:
	static bool bInit;
	static std::unordered_map<int, CRITICAL_SECTION*> CSMap;
	static CRITICAL_SECTION CB;

public:
	class Creator
	{
	public:
		Creator()
		{
			CritSec::Init();
		}
	};

	class Destroyer
	{
	public:
		~Destroyer()
		{
			CritSec::Release();
		}
	};

	static Creator CreatorInst;
	static Destroyer DestroyerInst;

public:
	int Index;

	CRITICAL_SECTION* GetCB(INT _Index)
	{
		EnterCriticalSection(&CB);

		std::unordered_map<int, CRITICAL_SECTION*>::iterator Iter = CSMap.find(_Index);

		if (Iter == CSMap.end())
		{
			CRITICAL_SECTION* CS = new CRITICAL_SECTION();
			memset(CS, 0, sizeof(CRITICAL_SECTION));
			InitializeCriticalSection(CS);
			CSMap.insert(std::unordered_map<int, CRITICAL_SECTION*>::value_type(_Index, CS));
			Iter = CSMap.find(_Index);
		}

		CRITICAL_SECTION* Return = Iter->second;

		LeaveCriticalSection(&CB);

		return Return;
	}

public:
	static void Init()
	{
		InitializeCriticalSection(&CB);
		bInit = true;
	}

	static void Release()
	{
		std::unordered_map<int, CRITICAL_SECTION*>::iterator StartIter = CSMap.begin();
		std::unordered_map<int, CRITICAL_SECTION*>::iterator EndIter = CSMap.end();

		for (; StartIter != EndIter ; ++StartIter)
		{
			delete StartIter->second;
		}
	}
public:
	CritSec(int _Index)
	{
		Index = _Index;
		EnterCriticalSection(GetCB(_Index));
	}
	~CritSec()
	{
		LeaveCriticalSection(GetCB(Index));
	}
};

