#pragma once

class TabDlgBase : public CDialogEx
{
public:
	CString SceneName;
	bool SceneChange;
public:
	TabDlgBase(int _ID, CWnd* _Wnd);
	~TabDlgBase();
};

