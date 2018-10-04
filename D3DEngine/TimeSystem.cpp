#include "PreCom.h"
#include "TimeSystem.h"

/////////////////////////////////////////////////// Àü¿ª

TimeSystem::Timer TimeSystem::MainTimer = Timer();
std::unordered_map<std::wstring, Autoptr<TimeSystem::Timer>> TimerMap;

////////////////////////////////////////////////// Timer

TimeSystem::Timer::Timer()
{
	Init();
}

TimeSystem::Timer::~Timer()
{

}

void TimeSystem::Timer::Init()
{
	QueryPerformanceFrequency(&SecCount);

	QueryPerformanceCounter(&CurCount);
	FPSTime = 0;
	FPSCount = 0;
}

float TimeSystem::Timer::Update()
{
	QueryPerformanceCounter(&NextCount);

	DeltaTime = ((float)(NextCount.QuadPart - CurCount.QuadPart)) / ((float)SecCount.QuadPart);
	CurCount.QuadPart = NextCount.QuadPart;

	FPSTime += DeltaTime;

	++FPSCount;

	if (1.0f <= FPSTime)
	{
		FPS = ((float)FPSCount / FPSTime);
		FPSTime = 0.0f;
		FPSCount = 0;
	}

	return DeltaTime;
}

TimeSystem::TimeSystem()
{

}


TimeSystem::~TimeSystem()
{
}

void TimeSystem::Init()
{
	MainTimer.Init();
}

void TimeSystem::Update()
{
	MainTimer.Update();
}