#pragma once
#include "NTSceneSystem.h"
#include "NTDevice.h"
#include "NTRenderSystem.h"

#include <Windows.h>
#include <unordered_map>


class WinCore;
class NTWindow : public NTNameTag, public RefCounter, public NTTypeParent
{
public:
	friend WinCore;

	/////////////////////////////////////////////////////////// 전역

private:
	static std::unordered_map<std::wstring, Autoptr<NTWindow>> WinNameMap;
	static std::unordered_map<HWND, Autoptr<NTWindow>> WinHWNDMap;

	static std::unordered_map<std::wstring, Autoptr<NTWindow>>::iterator WinStartIter;
	static std::unordered_map<std::wstring, Autoptr<NTWindow>>::iterator WinEndIter;

private:
	static HINSTANCE hInst;
	static void Init(HINSTANCE _hInst);

	static Autoptr<NTWindow> MainWindow;
	static void SetMainWindow(const wchar_t* _Name);

public:
	static Autoptr<NTWindow> NTCreateWindow(const wchar_t* _Name, HWND _hWnd = nullptr);
	static Autoptr<NTWindow> NTFindWindow(const wchar_t* _Name);
	static void NTEraseWindow(const HWND _Handle);
	static void Progress();

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	static size_t WindowCount()
	{
		return WinNameMap.size();
	}

	////////////////////////////////////////////////////////////// 객체

private:
	HWND hWnd;
	HDC hDC;
	size_t WindowWidth;
	size_t WindowHeight;
	bool bWindowed;

protected:
	NTWindow(const wchar_t* _Name);
	NTWindow(const wchar_t* _Name, HWND _hWnd);
	~NTWindow();

public:
	HWND GetHWND()
	{
		return hWnd;
	}

	HDC GetHDC()
	{
		return hDC;
	}

	size_t GetWidth()
	{
		return WindowWidth;
	}

	size_t GetHeight()
	{
		return WindowHeight;
	}

	float GetWidthf()
	{
		return (float)WindowWidth;
	}

	float GetHeightf()
	{
		return (float)WindowHeight;
	}

	UINT GetWidthU()
	{
		return (UINT)WindowWidth;
	}

	UINT GetHeightU()
	{
		return (UINT)WindowHeight;
	}

	bool IsWindow()
	{
		return bWindowed;
	}

public:
	void Show(int _ShowOption = SW_SHOWDEFAULT);
	void SetSize(size_t _X, size_t _Y);
	void SetScreenStatus(bool _Value);

private:
	void Update();
	ATOM NTRegisterClass();
	BOOL InitInst();

public:
	NTSceneSystem SceneSystem;

	////////////////////////////////////////////////////////////////// 디바이스
private:
	NTDevice Device;

public:
	NTDevice& GetDevice() { return Device; }

public:
	bool InitDevice();

private:
	NTRenderSystem RenderSystem;
};

