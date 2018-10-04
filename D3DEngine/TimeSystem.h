#pragma once
#include <Windows.h>
#include <unordered_map>
#include <string>

#include "RefNAutoptr.h"

class WinCore;
class TimeSystem
{
private:
	friend WinCore;

public:
	class Timer : public RefCounter
	{
	public:
		friend TimeSystem;

	private:
		LARGE_INTEGER SecCount;
		LARGE_INTEGER CurCount;
		LARGE_INTEGER NextCount;
		float DeltaTime;
		float FPSTime;
		int FPSCount;
		float FPS;

	public:
		void Init();
		float Update();

	public:
		Timer();
		~Timer();
	};

	static Timer MainTimer;
	static std::unordered_map<std::wstring, Autoptr<Timer>> TimerMap;

private:
	static void Init();
	static void Update();

public:
	static float DeltaTime()
	{
		return MainTimer.DeltaTime;
	}

	static float FPS()
	{
		return MainTimer.FPS;
	}
public:
	TimeSystem();
	~TimeSystem();
};

