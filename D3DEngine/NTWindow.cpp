#include "PreCom.h"
#include "NTWindow.h"
#include "StlAssist.h"
#include "NTDevice.h"
#include "ResourceSystem.h"

/////////////////////////////////////////////////////// 전역

HINSTANCE NTWindow::hInst = nullptr;
Autoptr<NTWindow> NTWindow::MainWindow = nullptr;

std::unordered_map<std::wstring, Autoptr<NTWindow>> NTWindow::WinNameMap;
std::unordered_map<HWND, Autoptr<NTWindow>> NTWindow::WinHWNDMap;
std::unordered_map<std::wstring, Autoptr<NTWindow>>::iterator NTWindow::WinStartIter;
std::unordered_map<std::wstring, Autoptr<NTWindow>>::iterator NTWindow::WinEndIter;

void NTWindow::Init(HINSTANCE _hInst)
{
	hInst = _hInst;
}

void NTWindow::SetMainWindow(const wchar_t* _Name)
{
	MainWindow = MapFind<Autoptr<NTWindow>>(WinNameMap, _Name);
	tassert(nullptr == MainWindow);
}

Autoptr<NTWindow> NTWindow::NTCreateWindow(const wchar_t* _Name, HWND _hWnd)
{
	Autoptr<NTWindow> Win = MapFind<Autoptr<NTWindow>>(WinNameMap, _Name);

	if (nullptr != Win)
	{
		return Win;
	}

	NTWindow* NewWindow = nullptr;

	if (nullptr == _hWnd)
	{
		NewWindow = new NTWindow(_Name);
	}
	else
	{
		NewWindow = new NTWindow(_Name, _hWnd);
	}

	if (nullptr == NewWindow->hWnd)
	{
		delete NewWindow;
		return nullptr; // 생성 실패시 없앰.
	}

	NewWindow->SetType();

	WinNameMap.insert(std::unordered_map<std::wstring, Autoptr<NTWindow>>::value_type(_Name, NewWindow));
	WinHWNDMap.insert(std::unordered_map<HWND, Autoptr<NTWindow>>::value_type(NewWindow->hWnd, NewWindow));

	return NewWindow;
}

Autoptr<NTWindow> NTWindow::NTFindWindow(const wchar_t* _Name)
{
	return MapFind<Autoptr<NTWindow>>(WinNameMap, _Name);
}

void NTWindow::NTEraseWindow(const HWND _Handle)
{
	Autoptr<NTWindow> Win = MapFind<Autoptr<NTWindow>>(WinHWNDMap, _Handle);

	tassert(nullptr == Win);

	if (nullptr == Win)
	{
		return;
	}

	std::wstring Name = Win->GetName();
	StlErase(WinNameMap, Name.c_str());
	StlErase(WinHWNDMap, _Handle);
}

void NTWindow::Progress()
{
	WinStartIter = WinNameMap.begin();
	WinEndIter = WinNameMap.end();

	for (; WinStartIter != WinEndIter; ++WinStartIter)
	{
		WinStartIter->second->Update();
	}
}

LRESULT CALLBACK NTWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		NTEraseWindow(hWnd);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

//////////////////////////////////////////////////////////////////// 객체

NTWindow::NTWindow(const wchar_t* _Name) : NTNameTag(_Name), hWnd(nullptr), SceneSystem(this), WindowWidth(0), WindowHeight(0), bWindowed(true), Device(this)
{
	NTRegisterClass();
	if (FALSE == InitInst())
	{
		hWnd = nullptr;
		return;
	}

	hDC = GetDC(hWnd);
}

NTWindow::NTWindow(const wchar_t* _Name, HWND _hWnd) : NTNameTag(_Name), hWnd(nullptr), SceneSystem(this), WindowWidth(0), WindowHeight(0), bWindowed(true), Device(this)
{
	hWnd = _hWnd;

	RECT RC;
	GetClientRect(_hWnd, &RC);

	WindowWidth = RC.right;
	WindowHeight = RC.bottom;

	hDC = GetDC(_hWnd);
}

NTWindow::~NTWindow()
{

}

void NTWindow::Show(int _ShowOption)
{
	ShowWindow(hWnd, _ShowOption);
	UpdateWindow(hWnd);
}

void NTWindow::SetSize(size_t _X, size_t _Y)
{
	WindowWidth = _X;
	WindowHeight = _Y;
	RECT rc = { 0, 0, (int)_X, (int)_Y };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(hWnd, nullptr, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SWP_NOMOVE | SWP_NOZORDER);
}

void NTWindow::SetScreenStatus(bool _Value)
{
	if (true == _Value)
	{
		bWindowed = true;
		return;
	}

	bWindowed = false;
}

void NTWindow::Update()
{
	SceneSystem.Progress();

	if (true == Device.IsInit())
	{
		SceneSystem.Render();
	}

	SceneSystem.PreProgress();
	SceneSystem.Release();
}

ATOM NTWindow::NTRegisterClass()
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInst;
	wcex.hIcon = nullptr;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = GetName();
	wcex.hIconSm = nullptr;

	return RegisterClassExW(&wcex);
}

BOOL NTWindow::InitInst()
{
	hWnd = CreateWindowW(GetName(), L"Bug Maker Ver.2", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInst, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	hDC = GetDC(hWnd);
	return TRUE;
}

bool NTWindow::InitDevice()
{
	bool Return = Device.Init();

	//Device.DefaultInit();
	Device.Default3DInit();

	return Return;
}