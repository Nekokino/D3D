#pragma once

class TabDlgBase : public CDialogEx
{
public:
	CString SceneName;
public:
	TabDlgBase(int _ID, CWnd* _Wnd);
	~TabDlgBase();
};

