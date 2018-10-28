
// UnityBuildMaker.h : UnityBuildMaker ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.

#include <string>
#include <unordered_map>


// CUnityBuildMakerApp:
// �� Ŭ������ ������ ���ؼ��� UnityBuildMaker.cpp�� �����Ͻʽÿ�.
//

class CUnityBuildMakerApp : public CWinApp
{
public:
	CUnityBuildMakerApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

public:
	void CreateCpp(const wchar_t* _Path, const wchar_t* _FileName);

// �����Դϴ�.

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
