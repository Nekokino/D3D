#pragma once
#include <Windows.h>
#include <crtdbg.h>

#include "RefNAutoptr.h"


class WinCore
{
public:
	class WinCoreBuilder : public RefCounter
	{
	public:
		virtual void Build() = 0;

	protected:
		WinCoreBuilder();
		virtual ~WinCoreBuilder() = 0;
	};

private:
	friend int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
		_In_opt_ HINSTANCE hPrevInstance,
		_In_ LPWSTR lpCmdLine,
		_In_ int nCmdShow);

public:
	template<typename T>
	static int Start(HINSTANCE _Hinst, const wchar_t* _MainWindowName, HWND _hWnd = nullptr)
	{
		new int;
		InitCore(_Hinst, _MainWindowName, _hWnd);

		T Builder = T();
		Builder.Build();

		return 1;
	}

private:
	static void InitCore(HINSTANCE _hInst, const wchar_t* _MainWindowName, HWND _hWnd = nullptr);
	static int Loop();

public:
	static void Progress();

public:
	WinCore();
	~WinCore();
};

#define COREBEGIN(BUILDER, NAME) \
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, \
	_In_opt_ HINSTANCE hPrevInstance, \
	_In_ LPWSTR    lpCmdLine, \
	_In_ int       nCmdShow) \
{ \
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF); \
	WinCore::Start<BUILDER>(hInstance, NAME); \
	WinCore::Loop(); \
	return 0; \
}