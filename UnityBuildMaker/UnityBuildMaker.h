
// UnityBuildMaker.h : UnityBuildMaker 응용 프로그램에 대한 주 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"       // 주 기호입니다.

#include <string>
#include <unordered_map>


// CUnityBuildMakerApp:
// 이 클래스의 구현에 대해서는 UnityBuildMaker.cpp을 참조하십시오.
//

class CUnityBuildMakerApp : public CWinApp
{
public:
	CUnityBuildMakerApp();


// 재정의입니다.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

public:
	void CreateCpp(const wchar_t* _Path, const wchar_t* _FileName);

// 구현입니다.

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

class PathSystem
{
public:
	static std::wstring Root;
	static std::wstring BinRoot;
	static void Init()
	{
		wchar_t Arr[1024] = L"";
		GetCurrentDirectoryW(sizeof(wchar_t) * 1024, Arr);
		Root = Arr;
		BinRoot = Arr;

		size_t CutCount = Root.find_last_of('\\', Root.size());
		Root.replace(CutCount, Root.size(), L"\\");
		BinRoot.replace(CutCount, Root.size(), L"\\BIN\\");
	}
};


extern CUnityBuildMakerApp theApp;
